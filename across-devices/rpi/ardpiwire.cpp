#include "ardpiwire.hpp"

void ArdPiWire::init() {
    wiringPiSetupGpio();

    pinMode(CLK_PIN, OUTPUT);
    pinMode(ACK_PIN, INPUT);
    pinMode(DATA_PIN, OUTPUT);

    digitalWrite(CLK_PIN, 0);
    digitalWrite(DATA_PIN, 0);

    sendByte(0xa5);
}

char ArdPiWire::recvByte(bool do_pre_delay) {
    char out = 0;
    if (do_pre_delay) std::this_thread::sleep_for(std::chrono::microseconds(PRE_SEND_DELAY_US));
    for (int i = 0; i < 8; ++i) {
        digitalWrite(CLK_PIN, 1);
        std::this_thread::sleep_for(std::chrono::microseconds(SEND_DELAY_H_US));
        out >>= 1;
        if (digitalRead(RECV_PIN)) out += 0x80;
        digitalWrite(CLK_PIN, 0);
        std::this_thread::sleep_for(std::chrono::microseconds(SEND_DELAY_L_US));
    }
    return out;
}

float ArdPiWire::recvFloat(bool do_pre_delay) {
    float out;
    char* out_bytes = (char*) &out;
    out_bytes[0] = recvByte(do_pre_delay);
    out_bytes[1] = recvByte();
    out_bytes[2] = recvByte();
    out_bytes[3] = recvByte();

    return out;
}

void ArdPiWire::sendByte(char data) {
    for (int mask = 1; mask < 0x100; mask <<= 1) {
        
        // Wait for ACK = 0
        while (digitalRead(ACK_PIN)) { }

        // Send signal to send bit
        digitalWrite(CLK_PIN, 1);

        // Await acknowledgement for bit
        while (digitalRead(ACK_PIN) != 1) {
            std::this_thread::sleep_for(std::chrono::microseconds(WAIT_DELAY_US));
        }

        // Send bit
        digitalWrite(DATA_PIN, (data & mask) != 0);
        //std::this_thread::sleep_for(std::chrono::microseconds(1));
        digitalWrite(CLK_PIN, 0);

        // Give Arduino time. In this, it sets ACK back to 0, and reads DATA_PIN.
        //std::this_thread::sleep_for(std::chrono::microseconds(1'000'000 / FREQ));
    }
}

void ArdPiWire::sendFloat(float data) {
    char* data_bytes = (char*) &data;
    sendByte(data_bytes[0]);
    sendByte(data_bytes[1]);
    sendByte(data_bytes[2]);
    sendByte(data_bytes[3]);
}