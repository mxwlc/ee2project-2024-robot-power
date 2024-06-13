#pragma once

#include "wiringPi.h"
#include <thread>
#include <chrono>

namespace ArdPiWire {

    const char CLK_PIN = 26;
    const char ACK_PIN = 16;
    const char DATA_PIN = 6;
    static const char& RECV_PIN = ACK_PIN;

    const int WAIT_DELAY_US = 100;
    const int SEND_DELAY_H_US = 1;
    const int SEND_DELAY_L_US = 1;
    const int PRE_SEND_DELAY_US = 1;

    void init();

    char recvByte(const bool do_pre_delay = false);
    float recvFloat(const bool do_pre_delay = false);
    void sendByte(const char data);
    void sendFloat(const float data);

}