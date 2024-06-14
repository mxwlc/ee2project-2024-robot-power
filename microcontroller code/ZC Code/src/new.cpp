#include <Arduino.h>
#include <TimerInterrupt_Generic.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <step.h>
#include "ardpiwire.hpp"

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

#define CONF_SET_SPEED 0
#define CONF_IDLE_SETPOINT 1
#define CONF_KP 2
#define CONF_KI 3
#define CONF_KD 4
#define CONF_SPEED_KP 5
#define CONF_SPEED_KI 6
#define CONF_SPEED_KD 7
#define CONF_SPEED_TOLERANCE 8
#define CONF_ANGLE_TOLERANCE 9
#define CONF_ERROR_TOLERANCE 10
#define CONF_SPEED1 11
#define CONF_SPEED2 12
volatile float configurable_params[] = {
  /*set_speed*/       20.0f,
  /*idle_setpoint*/   0.0375f,
  /*kp*/              1500.0f,
  /*ki*/              0.0f,
  /*kd*/              12000.0f,
  /*speed_kp*/        0.00035f,
  /*speed_ki*/        0.0f,
  /*speed_kd*/        0.0f,
  /*speed_tolerance*/ 2.0f,
  /*angle_tolerance*/ 0.5f,
  /*error_tolerance*/ 0.1f,
  /*speed1*/          0.0f,
  /*speed2*/          0.0f
};

//Motor acceleration in rad/s
const float acceleration = 15;
volatile float& setSpeed = configurable_params[CONF_SET_SPEED];

//Angle Tuning
const float C = 0.95;
volatile float& idle_setpoint = configurable_params[CONF_IDLE_SETPOINT];
float setpoint_angle = idle_setpoint;//0.032 - Roughly upright
volatile float& kp = configurable_params[CONF_KP];
volatile float& ki = configurable_params[CONF_KI];//should be 0 if static setpoint is set correctly
volatile float& kd = configurable_params[CONF_KD];

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
volatile float& speed_kp = configurable_params[CONF_SPEED_KP];
volatile float& speed_ki = configurable_params[CONF_SPEED_KI];
volatile float& speed_kd = configurable_params[CONF_SPEED_KD];

//This is how large the error can be for setpoint to change
volatile float& speed_tolerance = configurable_params[CONF_SPEED_TOLERANCE];
volatile float& angle_tolerance = configurable_params[CONF_ANGLE_TOLERANCE];
volatile float& error_tolerance = configurable_params[CONF_ERROR_TOLERANCE];
float lowest_speed = 100;
float speed_errors[10];

//Speeds
volatile float& speed1 = configurable_params[CONF_SPEED1];
volatile float& speed2 = configurable_params[CONF_SPEED2];
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


int sign(float input){
  if (input >= 0)
    return 1;
  else return -1;
}

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
  // step1.setAccelerationRad(acceleration);
  // step2.setAccelerationRad(acceleration);

  // step1.setTargetSpeedRad(10);
  // step2.setTargetSpeedRad(-10);

  //Enable the stepper motor drivers
  pinMode(STEPPER_EN,OUTPUT);
  digitalWrite(STEPPER_EN, false);

  // Enable the background listening process
  Serial.println("Initialising Pi wire");
  ArdPiWire::init();
  Serial.println("Initialised Pi wire");
  //ArdPiWire::values = configurable_params;
  Serial.println((int) configurable_params);
  //Serial.println((int) ArdPiWire::values);
  BaseType_t cur_core = xPortGetCoreID(), targ_core;
  if (cur_core == 0) targ_core = 1;
  else targ_core = 0;
  xTaskCreatePinnedToCore(ArdPiWire::receiverThreadMain, "Recv", 1000, (void*) configurable_params, 1, &ArdPiWire::receiver_thread, targ_core);
}

/*

TO START RPI STUFF:

TURN ON RPI
CONNECT TO Robot_Power_Bot WiFi NETWORK
TURN ON PUTTY
DOUBLE CLICK "BalanceBot"
USER NAME: "bal_bot"
PASSWORD: "RobotPower"
./var_setter

*/


void loop()
{
  //Static variables are initialised once and then the value is remembered betweeen subsequent calls to this function
  static unsigned long printTimer = 0;  //time of the next print
  static unsigned long loopTimer = 0;   //time of the next control update
  static unsigned long speedTimer = 0;

  if (millis() > 500 & millis() < 1000) lowest_speed = 100;
  //Outer Loop
  if (millis() > speedTimer){
    speedTimer += SPEED_INTERVAL;

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    //Calculate setpoint angle
    float speed_avg = (speed1);

    current_speed = -step1.getSpeedRad() * 0;//If they have the same sign, this will be 0
    previous_speed_error = speed_error;
    speed_error = speed_avg - current_speed;
    speed_errors[0] = speed_error;
    
    speed_p_term = speed_kp*speed_error;
    speed_i_term = speed_ki*speed_error*SPEED_INTERVAL*0.01;
    speed_d_term = speed_kd*((speed_error - previous_speed_error)/SPEED_INTERVAL*0.01);
    setpoint_angle = speed_p_term + speed_i_term + speed_d_term + idle_setpoint;
    
    //Limit the setpoint angle
    if (setpoint_angle > idle_setpoint+angle_tolerance) setpoint_angle = idle_setpoint+angle_tolerance;
    else if (setpoint_angle < idle_setpoint-angle_tolerance) setpoint_angle = idle_setpoint-angle_tolerance;
    
    //Pick a new idle setpoint if we find a point with very little movement
    // if (abs(current_speed) < speed_tolerance & abs(current_speed) < lowest_speed){
    //   idle_setpoint = setpoint_angle;
    //   lowest_speed = abs(current_speed);
    // } 
    // float total = 0;
    // for (float value : speed_errors){
    //   total += value;
    // }
    // float avg = total/10;
    // if (avg > error_tolerance) lowest_speed = 100;
    // //Move down stack
    // for (int i = 0; i < 9; i++){
    //   speed_errors[i+1] = speed_errors[i];
    // }
  }
  
  //Inner Loop
  if (millis() > loopTimer) {
    loopTimer += LOOP_INTERVAL;

    // Fetch data from MPU6050
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    previous_accel = accel;
    accel = a.acceleration.z;
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

    //Assuming max error to be around 0.5
    float speed_ratio = error/0.5;

    step1.setAccelerationRad(PIDout);
    step2.setAccelerationRad(PIDout);

    step1.setTargetSpeedRad(setSpeed*sign(PIDout)*sign(speed1));
    step2.setTargetSpeedRad(-setSpeed*sign(PIDout)*sign(speed2));

    // step1.setTargetSpeedRad(PIDout + (1-speed_ratio)*speed1);
    // step2.setTargetSpeedRad(-PIDout + (1-speed_ratio)*speed2);
  }
  
  //Print Loop
  if (false) {//(millis() > printTimer) {
    printTimer += PRINT_INTERVAL;
    Serial.print(current_angle*100);
    Serial.print(" | ");
    Serial.print(setpoint_angle*100);
    Serial.print(" | ");
    Serial.print(idle_setpoint*100);
    Serial.println();
  }
  
  //Speed Changes
  // if (millis() > 5000){
  //   speed1 = 35;
  //   speed2 = 35;
  // }
  // if (millis() > 7000){
  //   speed1 = 0;
  //   speed2 = 0;
  // }
}