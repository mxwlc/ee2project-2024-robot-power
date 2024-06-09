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

//Loops
const int PRINT_INTERVAL = 500;
const int LOOP_INTERVAL = 10;
const int SPEED_INTERVAL = 250;
const int  STEPPER_INTERVAL_US = 20;

//Motor acceleration in rad/s
const float acceleration = 15;

//Angle Tuning
const float C = 0.95;
const float idle_setpoint = 0.0325;
float setpoint_angle = idle_setpoint;//0.032 - Roughly upright
const float kp = 1000.0;
const float ki = 0;//should be 0 if static setpoint is set correctly
float kd = 2500;

//Errors
float error = 0;
float previous_error;

//Angles
float a_angle;
float g_angle;
float current_angle = 0;
float previous_angle = 0;

float angle_p_term;
float angle_i_term;
float angle_d_term;

//Speed Tuning
const float speed_kp = 0.0075;
const float speed_ki = 0;
const float speed_kd = 100;

//This is how large the error can be for setpoint to change
const float change_tollerance = 0.01;

//Speeds
float speed1 = 0;
float speed2 = 0;
float current_speed = 0;
float accel = 0;
float previous_accel = 0;

float speed_error = 0;
float previous_speed_error = 0;

float speed_p_term;
float speed_i_term;
float speed_d_term;

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
  step1.setAccelerationRad(acceleration);
  step2.setAccelerationRad(acceleration);

  //Enable the stepper motor drivers
  pinMode(STEPPER_EN,OUTPUT);
  digitalWrite(STEPPER_EN, false);

}

void loop()
{
  //Static variables are initialised once and then the value is remembered betweeen subsequent calls to this function
  static unsigned long printTimer = 0;  //time of the next print
  static unsigned long loopTimer = 0;   //time of the next control update
  static unsigned long speedTimer = 0;

  //Outer Loop
  if (millis() > speedTimer){
    speedTimer += SPEED_INTERVAL;

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    //Calculate setpoint angle
    float speed_avg = (speed1 + speed1)/2;
    previous_accel = accel;
    accel = a.acceleration.z;
    current_speed = ((step1.getSpeedRad() + step2.getSpeedRad())/2);//If they have the same sign, this will be 0
    previous_speed_error = speed_error;
    speed_error = speed_avg - current_speed;
    
    speed_p_term = speed_kp*speed_error;
    speed_i_term = speed_ki*speed_error*LOOP_INTERVAL*0.01;
    speed_d_term = speed_kd*((speed_error - previous_speed_error)/LOOP_INTERVAL*0.01);
    setpoint_angle = speed_p_term + speed_i_term + speed_d_term + idle_setpoint;
  }
  
  //Inner Loop
  if (millis() > loopTimer) {
    loopTimer += LOOP_INTERVAL;

    // Fetch data from MPU6050
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    //Calculate Tilt using accelerometer and sin x = x approximation for a small tilt angle
    a_angle = a.acceleration.z/9.67;//Assuming that's gravitational constant
    g_angle = g.gyro.pitch;
    previous_angle = current_angle;
    current_angle = ((1-C)*a_angle + C*(g_angle*LOOP_INTERVAL*0.001+previous_angle));
    
    previous_error = error;
    error = setpoint_angle - current_angle;
  
    angle_p_term = kp*error;
    angle_i_term = ki*(error*LOOP_INTERVAL*0.001); //0.001 for ms
    angle_d_term = kd*((error-previous_error)/LOOP_INTERVAL*0.001);
  
    PIDout = angle_p_term + angle_i_term + angle_d_term;
    // step1.setTargetSpeedRad(PIDout);
    // step2.setTargetSpeedRad(-PIDout);

    //Assuming max error to be around 0.5
    float speed_ratio = error/0.5;
    step1.setTargetSpeedRad(PIDout + (1-speed_ratio)*speed1);
    step2.setTargetSpeedRad(-PIDout + (1-speed_ratio)*speed2);
  }
  
  //Print Loop
  if (millis() > printTimer) {
    printTimer += PRINT_INTERVAL;
    Serial.print("Current_Angle = ");
    Serial.print(current_angle);
    Serial.print(" | Setpoint_Angle = ");
    Serial.print(setpoint_angle);
    Serial.print(" | Error = ");
    Serial.print(error);
    Serial.print(" | Current_Speed = ");
    Serial.print(current_speed);
    Serial.println();
  }
  
  //Speed Changes
  // if (millis() > 5000){
  //   speed1 = 1.5;
  //   speed2 = -1.5;
  // }
  // if (millis() > 7000){
  //   speed1 = 0;
  //   speed2 = 0;
  // }
}