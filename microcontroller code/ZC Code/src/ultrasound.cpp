#include "ultrasound.hpp"
#include <Arduino.h>

void UltrasoundSensor::init()
{
  pinMode(TRIG_PIN,OUTPUT);
  pinMode(ECHO_PIN,INPUT);
}

float UltrasoundSensor::getReading()
{
  
    long duration;

    // Clears the trigPin
    //digitalWrite(TRIG_PIN, LOW);
    //delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10); //ACCORDING TO DATASHEET TO RELIABLY TRIGGER ULTRASONIC BURST
    digitalWrite(TRIG_PIN, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(ECHO_PIN, HIGH);
      // If duration is zero, there might be an issue with reading the pulse
    /*if (duration == 0) {
      //Serial.println("No pulse detected");
      return 0.0f;
    }*/
    // Calculating the distance, 0.034=SPEED OF SOUND
    return duration * 0.034 / 2;
    // Prints the distance on the Serial Monitor
    //Serial.print("Distance: ");
    //Serial.println(distance);
}