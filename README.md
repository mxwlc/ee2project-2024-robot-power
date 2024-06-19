
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
We will need to amplify the difference between 5V and I5 as the voltage drop between the 2 nodes represents the power consumed.

Code will need to be written to calculate the power consumed and remaining energy. This code will go on the ESP32.

The sensor feature can be completed using either an Infrared sensor or an Ultrasonic sensor.
Ultrasonic sensors tend to be more accurate compared to Infrared sensors.

Code will need to be written to calculate the distance to nearby objects.
This will be done by multiplying the speed of a soundwave by the time the wave takes to travel from the emitter to the object.


## Differential amplifier Prototype 1

![image](https://github.com/mxwlc/ee2project-2024-robot-power/assets/54540123/3457c768-0957-4cc9-a32b-755d2e7c83f7)

To choose the gain we use the following calculation:
3.3/4096 = 0.8 V/A
As 10 mV/A is across the op-amp inputs, we require a minimum of x80 gain to meet the resolution. This will represent a value 1 as 0.8 mV within the code and 4096 as 4.096 amps

Following Nyquist's formula, the corner frequency of this circuit should be less than half the sampling frequency (100Hz)
1/2*pi*R*f = C
Where R = 800k, f = 50Hz
So, we choose 2.2nF

We chose the LT1677 as it is a rail-to-rail, precision amplifier.
This is necessary as the 5V and I5 inputs are very close to the supply rail (5V).

## Power consumption code

% #include <Arduino.h>
% //const
% const int print_interval = 500;
% const float batteryVoltage = 7.2;        
% const float initialCapacity = 2.0;    
% const int sensorPin = 27;  
% const float totalEnergy = (initialCapacity * batteryVoltage)*2;
% //variables              
% int digitalValue = 0; 
% float analogVoltage = 0.00;
% float current = 0.00;
% float consumedPower = 0.00;
% float consumedEnergy = 0.0;              // Consumed energy in watt-hour
% unsigned long previousTime = 0;
% void setup() {
%   Serial.begin(115200);
%   previousTime = millis(); //time track
% }

% void loop() {
%   static unsigned long printTimer = 0;
%   if (millis() > printTimer){
%     printTimer += print_interval;
%     digitalValue = analogRead(sensorPin);
%     Serial.print("digital value = ");
%     Serial.println(digitalValue);       

%     //convert digital value to analog voltage
%     analogVoltage = -((digitalValue * 3.30)/(4095.00)-2.47);
%     Serial.print("analog voltage = ");
%     Serial.println(analogVoltage);

%     //current with 0.01 shunt 
%     current=analogVoltage/(0.033*100);
%     Serial.print("current = ");
%     Serial.println(current);

%     // Calculate the elapsed time in hours
%     unsigned long currentTime = millis();
%     float elapsedTime = (currentTime - previousTime) / 3600000.0; // Time in hours
%     previousTime = currentTime;
%     //cosumed power
%     consumedPower=current*analogVoltage;
%     Serial.print("Consumed Power = ");
%     Serial.println(consumedPower);

%     //consumed energy
%     consumedEnergy += consumedPower * elapsedTime;
%     Serial.print("Consumed energy = ");
%     Serial.println(consumedEnergy);
    
%     //remaining energy
%     float remainingEnergy = totalEnergy - consumedEnergy;
%     if (remainingEnergy < 0) remainingEnergy = 0;
%     Serial.print("Remaining Energy = ");
%     Serial.print(remainingEnergy);
%     Serial.println(" Wh");
%   }

%   delay(1000);
% }

The output of the amplifier circuit is fed into the ADC pin of the ESP32.
This converts the analogue voltage into a digital value.
Energy is calculated using E = P*T
The opamp transmits the measured voltage over a shunt resistor with a value of 0.01 Ohms.
Current is calculated, which can be used to find power consumed: P = I*V.
Multiplying this value with elapsed time will update the consumed energy in every iteration of the loop. Then, after subtracting from the total capacity, we are left with the remaining capacity.

***
Shunt resistor is measured and found as 0.033 Ohms.



## Differential amplifier Prototype 2

After testing the code with the 1st amplifier circuit, we saw that the digital value is not accurate.
This is likely because the amplified differential voltage is very close to the 0V supply rail.
Although we are using a rail-to-rail, precision amplifier, it is not perfect and has a 110 mV output swing.

We then use a 2.5V DC offset to counter this.
The gain is also changed to 100x to have a larger signal.

![image](https://github.com/mxwlc/ee2project-2024-robot-power/assets/54540123/8ccbfa18-2f42-481c-90bb-c247a58dbc6f)

With new resistor values, the capacitor values have been updated accordingly to continue following Nyquist's formula.

Testing with the new circuit, the digital value is much better.


## Sensor

# snena
