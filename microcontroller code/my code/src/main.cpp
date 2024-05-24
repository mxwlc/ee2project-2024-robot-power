//Sorry code is very messy, use guide in log.md to tune
#include <Arduino.h>
#include <TimerInterrupt_Generic.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <step.h>

// The Stepper pins
#define STEPPER1_DIR_PIN 16   //Arduino D9
#define STEPPER1_STEP_PIN 17  //Arduino D8
#define STEPPER2_DIR_PIN 4    //Arduino D11
#define STEPPER2_STEP_PIN 14  //Arduino D10
#define STEPPER_EN 15         //Arduino D12

// Diagnostic pin for oscilloscope
#define TOGGLE_PIN  32        //Arduino A4

const int PRINT_INTERVAL = 500;
const int LOOP_INTERVAL = 10;
const int  STEPPER_INTERVAL_US = 20;
const float C = 0.7;


const float kp = 1000.0;
const float ki = 0;//should be 0 if static setpoint is set correctly
float kd = 50;//Should come directly from gyroscope value

float error = 0;
float previous_error;

float a_angle;
float g_angle;
float current_angle = 0;
float previous_angle = 0;

float p_term;
float i_term;
float d_term;

float PIDout;


//Global objects
ESP32Timer ITimer(3);
Adafruit_MPU6050 mpu;         //Default pins for I2C are SCL: IO22/Arduino D3, SDA: IO21/Arduino D4

step step1(STEPPER_INTERVAL_US,STEPPER1_STEP_PIN,STEPPER1_DIR_PIN );
step step2(STEPPER_INTERVAL_US,STEPPER2_STEP_PIN,STEPPER2_DIR_PIN );


//Interrupt Service Routine for motor update
//Note: ESP32 doesn't support floating point calculations in an ISR
bool TimerHandler(void * timerNo)
{
  static bool toggle = false;

  //Update the stepper motors
  step1.runStepper();
  step2.runStepper();

  //Indicate that the ISR is running
  digitalWrite(TOGGLE_PIN,toggle);  
  toggle = !toggle;
	return true;
}

void setup()
{
  Serial.begin(115200);
  pinMode(TOGGLE_PIN,OUTPUT);

  // Try to initialize Accelerometer/Gyroscope
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);

  //Attach motor update ISR to timer to run every STEPPER_INTERVAL_US μs
  if (!ITimer.attachInterruptInterval(STEPPER_INTERVAL_US, TimerHandler)) {
    Serial.println("Failed to start stepper interrupt");
    while (1) delay(10);
    }
  Serial.println("Initialised Interrupt for Stepper");

  //Set motor acceleration values
  step1.setAccelerationRad(10.0);
  step2.setAccelerationRad(10.0);

  //Enable the stepper motor drivers
  pinMode(STEPPER_EN,OUTPUT);
  digitalWrite(STEPPER_EN, false);

}

void loop()
{
  //Static variables are initialised once and then the value is remembered betweeen subsequent calls to this function
  static unsigned long printTimer = 0;  //time of the next print
  static unsigned long loopTimer = 0;   //time of the next control update
  static float setpoint_angle = 0.002;
  static float tolerance = 0.002;
  
  //Run the control loop every LOOP_INTERVAL ms
  if (millis() > loopTimer) {
    loopTimer += LOOP_INTERVAL;

    // Fetch data from MPU6050
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    //Calculate Tilt using accelerometer and sin x = x approximation for a small tilt angle
    a_angle = a.acceleration.z/9.67;//Assuming that's gravitational constant
    g_angle = g.gyro.pitch;
    //kd = g.gyro.pitch;
    previous_angle = current_angle;
    current_angle = (1-C)*a_angle + C*(g_angle*LOOP_INTERVAL*0.001+previous_angle);
    
    previous_error = error;
    error = setpoint_angle - current_angle;
    
    if (abs(error) > tolerance){
      p_term = kp*error;
      i_term = ki*(error*LOOP_INTERVAL*0.001); //0.001 for ms
      d_term = kd*((error-previous_error)/LOOP_INTERVAL*0.001);
    
      PIDout = p_term + i_term + d_term;
    }
    else PIDout = 0;
    step1.setTargetSpeedRad(PIDout);//Include direction here
    step2.setTargetSpeedRad(-PIDout);

    //Set target motor speed proportional to tilt angle
    //Note: this is for demonstrating accelerometer and motors - it won't work as a balance controller
    //step1.setTargetSpeedRad(tiltx*kp);
    //step2.setTargetSpeedRad(-tiltx*kx);
  }
  
  //Print updates every PRINT_INTERVAL ms
  if (millis() > printTimer) {
    printTimer += PRINT_INTERVAL;
    Serial.print("PIDout : ");
    Serial.print(PIDout);
    Serial.print(" Current angle");
    Serial.print(current_angle);
    Serial.print(" Error ");
    Serial.print(error);
    Serial.println();
  }
}
