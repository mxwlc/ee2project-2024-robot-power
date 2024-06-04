
const int trigPin = X;
const int echopin = Y; // defines variables
long duration;
//Extended size variables for number storage
int distance;
void setup()
{
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT);  // Sets the echopin as an Input
    Serial.begin(9600);       // Starts the serial communication, data rate in bits per second
}

void loop()
{
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10); //ACCORDING TO DATASHEET TO RELIABLY TRIGGER ULTRASONIC BURST
    digitalWrite(trigPin, LOW);
    // Reads the echopin, returns the sound wave travel time in microseconds
    duration = pulseIn(echopin, HIGH);
    // Calculating the distance, 0.034-SPEED OF SOUND
    distance duration * 0.034 / 2;
    // Prints the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.println(distance);
    // Short delay before the next measurement
    delay(50); // 50 milliseconds delay
}
