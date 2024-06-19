#pragma once

#define TRIG_PIN  18        //Arduino D6
#define ECHO_PIN  5         //Arduino D7

namespace UltrasoundSensor {

    void init();
    float getReading();

}