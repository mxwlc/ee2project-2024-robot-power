#include "ardpiwire.hpp"

void ArdPiWire::init() {
    pinMode(CLK_PIN, INPUT);
    pinMode(ACK_PIN, OUTPUT);
    pinMode(DATA_PIN, INPUT);

    digitalWrite(ACK_PIN, 0);

    char bit_stream = 0;
    // Assuming noise is long strings of continuous 1's and 0's, as observed.
    while (bit_stream != 0xa5) {
        // Wait for CLK = 1
        while (!digitalRead(CLK_PIN)) { }

        // Send ACK
        digitalWrite(ACK_PIN, 1);

        // Wait for CLK = 0
        while (digitalRead(CLK_PIN)) { }

        // Read bit and clear ACK
        bit_stream >>= 1;
        if (digitalRead(DATA_PIN)) bit_stream += 0x80;
        digitalWrite(ACK_PIN, 0);
    }
}

bool ArdPiWire::recvIsPending() {
    return digitalRead(CLK_PIN);
}

char ArdPiWire::recvByte() {
    char out = 0;
    for (char i = 0; i < 8; ++i) {

        // Wait for CLK = 1
        while (!digitalRead(CLK_PIN)) { }

        // Send ACK
        digitalWrite(ACK_PIN, 1);

        // Wait for CLK = 0
        while (digitalRead(CLK_PIN)) { }

        // Read bit and clear ACK
        out >>= 1;
        if (digitalRead(DATA_PIN)) out += 0x80;
        digitalWrite(ACK_PIN, 0);
    }
    return out;
}

float ArdPiWire::recvFloat() {
    float out;
    char* out_bytes = (char*) &out;
    out_bytes[0] = recvByte();
    out_bytes[1] = recvByte();
    out_bytes[2] = recvByte();
    out_bytes[3] = recvByte();
    return out;
}

void ArdPiWire::sendByte(char data) {
    for (int mask = 1; mask < 0x100; mask <<= 1) {

        // Wait for CLK = 1
        while (!digitalRead(CLK_PIN)) { }

        // Send bit
        digitalWrite(SEND_PIN, (data & mask) != 0);

        // Wait for CLK = 0
        while (digitalRead(CLK_PIN)) { }
    }
    if constexpr (SEND_PIN == ACK_PIN) {
        digitalWrite(SEND_PIN, 0);
    }
}

void ArdPiWire::sendFloat(float data) {
    const char* data_bytes = (char*) &data;
    sendByte(data_bytes[0]);
    sendByte(data_bytes[1]);
    sendByte(data_bytes[2]);
    sendByte(data_bytes[3]);
}

void ArdPiWire::receiverThreadMain(void* arg) {
    float *values = (float*) arg;
    while (true) {
        if (!recvIsPending()) {
            // Perform something else while waiting?
            continue;
        }
        char ins = recvByte();
        char ind;
        switch (ins) {
        case SET_VAR:
            ind = recvByte();
            values[ind] = recvFloat();
            break;
        case GET_VAR:
            ind = recvByte();
            sendFloat(values[ind]);
            break;
        default:
            break;
        }
    }
}