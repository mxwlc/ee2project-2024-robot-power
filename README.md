
# Power_Sensors

  

## Overview

  

This branch is dedicated to the power consumption and sensor features of this project. This involves:

  

- Calculating power consumed and energy remaining
- Use sensors to find distance to nearby objects

  


## Initial ideas

The power consumption feature requires the PCB voltage (5V) to be stepped down (3.3V) for the ADC pin on the ESP32.
This can be done using a potential divider or a differential amplifier to amplify the difference.
A potential divider is likely to have less accuracy and more noise.
A differential amplifier will have more complex circuitry but it should improve accuracy.

The sensor feature can be completed using either an Infrared sensor or an Ultrasonic sensor.
Ultrasonic sensors tend to be more accurate compared to Infrared sensors.


## Differential amplifier Prototype 1

![image](https://github.com/mxwlc/ee2project-2024-robot-power/assets/54540123/3457c768-0957-4cc9-a32b-755d2e7c83f7)

