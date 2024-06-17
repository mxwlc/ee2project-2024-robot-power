#include "Arduino.h"

#pragma once

namespace ArdPiWire {

    const char CLK_PIN = 35;//red
    const char ACK_PIN = 27;//orange
    const char DATA_PIN = 33;//yellow
    const char SEND_PIN = ACK_PIN;

    //static float* values;

    enum Instructions: char {
        SET_VAR,
        GET_VAR
    };

    void init();
    bool recvIsPending();

    char recvByte();
    float recvFloat();
    void sendByte(const char data);
    void sendFloat(const float data);


    static TaskHandle_t receiver_thread;
    void receiverThreadMain(void* arg);

}