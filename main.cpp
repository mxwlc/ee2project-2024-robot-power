#include <Arduino.h>
//const
const int print_interval = 500;
const float batteryVoltage = 7.2;        
const float initialCapacity = 2.0;    
const int sensorPin = 26;  
const float totalEnergy = (initialCapacity * batteryVoltage)*2;
//variables              
int digitalValue = 0; 
float analogVoltage = 0.00;
float current = 0.00;
float consumedPower = 0.00;
float consumedEnergy = 0.0;              // Consumed energy in watt-hour
unsigned long previousTime = 0;
void setup() {
  Serial.begin(115200);
  previousTime = millis(); //time track
}

void loop() {
  static unsigned long printTimer = 0;
  if (millis() > printTimer){
    printTimer += print_interval;
    digitalValue = analogRead(sensorPin);
    Serial.print("digital value = ");
    Serial.println(digitalValue);       

    //convert digital value to analog voltage
    analogVoltage = -(((digitalValue * 3.30)/(4095.00))-2.47);
    Serial.print("analog voltage = ");
    Serial.println(analogVoltage);

    //current with 0.01 shunt 
    current=analogVoltage/(0.033*100);
    Serial.print("current = ");
    Serial.println(current);

    // Calculate the elapsed time in hours
    unsigned long currentTime = millis();
    float elapsedTime = (currentTime - previousTime) / 3600000.0; // Time in hours
    previousTime = currentTime;
    //cosumed power
    consumedPower=current*analogVoltage;
    Serial.print("Consumed Power = ");
    Serial.println(consumedPower);

    //consumed energy
    consumedEnergy += consumedPower * elapsedTime;
    Serial.print("Consumed energy = ");
    Serial.println(consumedEnergy);
    
    //remaining energy
    float remainingEnergy = totalEnergy - consumedEnergy;
    if (remainingEnergy < 0) remainingEnergy = 0;
    Serial.print("Remaining Energy = ");
    Serial.print(remainingEnergy);
    Serial.println(" Wh");
  }

  delay(1000);
}