#include <Arduino.h>
#include <TimerInterrupt_Generic.h>

// Diagnostic pin for oscilloscope
#define TOGGLE_PIN  32      //Arduino A4
#define TRIG_PIN  18        //Arduino D6
#define ECHO_PIN  5         //Arduino D7

const int PRINT_INTERVAL = 500;

long duration;
int distance;

//Global objects
ESP32Timer ITimer(3);


//Interrupt Service Routine for motor update
//Note: ESP32 doesn't support floating point calculations in an ISR
bool TimerHandler(void * timerNo)
{
  static bool toggle = false;

  //Indicate that the ISR is running
  digitalWrite(TOGGLE_PIN,toggle);  
  toggle = !toggle;
	return true;
}

void setup()
{
  Serial.begin(115200);
  pinMode(TOGGLE_PIN,OUTPUT);
  pinMode(TRIG_PIN,OUTPUT);
  pinMode(ECHO_PIN,INPUT);
  Serial.println("I'm Fucking alive!");
}

void loop()
{
  //Static variables are initialised once and then the value is remembered betweeen subsequent calls to this function
  static unsigned long printTimer = 0;  //time of the next print
  
  //Print updates every PRINT_INTERVAL ms
  if (millis() > printTimer) {
    printTimer += PRINT_INTERVAL;
    // Clears the trigPin
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10); //ACCORDING TO DATASHEET TO RELIABLY TRIGGER ULTRASONIC BURST
    digitalWrite(TRIG_PIN, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(ECHO_PIN, HIGH);
      // If duration is zero, there might be an issue with reading the pulse
    if (duration == 0) {
      Serial.println("No pulse detected");
      return;
    }
    // Calculating the distance, 0.034=SPEED OF SOUND
    distance = duration * 0.034 / 2;
    // Prints the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.println(distance);
  }
}