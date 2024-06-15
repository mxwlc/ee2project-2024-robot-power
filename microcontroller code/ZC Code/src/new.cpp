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

/*Conventions:
When I refer to forward, I mean the side with the edge of the triangle, and backwards
is the side with the flat face (and the ESP on it).

When moving forwards, step1 is negative, and step2 is positive.

Facing the ESP, step1 is to the right, and step2 is to the left
*/

//Libraries
#include <Arduino.h>
#include <TimerInterrupt_Generic.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <step.h>
#include "ardpiwire.hpp"

//Definitions
#pragma region 
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
#define CONF_ACCEL_TOLERANCE 10
#define CONF_SPEED1 11
#define CONF_SPEED2 12
#define CONF_ACCEL_KP 13
#pragma endregion

volatile float configurable_params[] = {
  /*set_speed*/       20.0f,
  /*idle_setpoint*/   0.03f,
  /*kp*/              1500.0f,
  /*ki*/              0.0f,
  /*kd*/              8000.0f,
  /*speed_kp*/        0.00035f,
  /*speed_ki*/        0.0f,
  /*speed_kd*/        0.0f,
  /*speed_tolerance*/ 2.0f,
  /*angle_tolerance*/ 0.05f,
  /*accel_tolerance*/ 0.1f,
  /*speed1*/          0.0f,
  /*speed2*/          0.0f,
  /*accel_kp*/        0.01f
};

//Variables
#pragma region 
volatile float& setSpeed = configurable_params[CONF_SET_SPEED];

//Angle Tuning
const float C = 0.95;
volatile float& idle_setpoint = configurable_params[CONF_IDLE_SETPOINT];
float setpoint_angle = idle_setpoint;
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

volatile float& accel_kp = configurable_params[CONF_ACCEL_KP];

//This is how large the error can be for setpoint to change
volatile float& speed_tolerance = configurable_params[CONF_SPEED_TOLERANCE];
volatile float& angle_tolerance = configurable_params[CONF_ANGLE_TOLERANCE];
volatile float& accel_tolerance = configurable_params[CONF_ACCEL_TOLERANCE];
float lowest_speed = 100;

//Speeds
volatile float& speed1 = configurable_params[CONF_SPEED1];
volatile float& speed2 = configurable_params[CONF_SPEED2];
float current_speed = 0;

float speed_error = 0;
float previous_speed_error = 0;

float speed_p_term;
float speed_i_term;
float speed_d_term;

float PIDout;

//Static variables are initialised once and then the value is remembered betweeen subsequent calls to this function
static unsigned long printTimer = 0;  //time of the next print
static unsigned long loopTimer = 0;   //time of the next control update
static unsigned long speedTimer = 0;


//Global objects
ESP32Timer ITimer(3);
Adafruit_MPU6050 mpu;         //Default pins for I2C are SCL: IO22/Arduino D3, SDA: IO21/Arduino D4

step step1(STEPPER_INTERVAL_US,STEPPER1_STEP_PIN,STEPPER1_DIR_PIN );
step step2(STEPPER_INTERVAL_US,STEPPER2_STEP_PIN,STEPPER2_DIR_PIN );
#pragma endregion

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
  mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);//44

  //Attach motor update ISR to timer to run every STEPPER_INTERVAL_US μs
  if (!ITimer.attachInterruptInterval(STEPPER_INTERVAL_US, TimerHandler)) {
    Serial.println("Failed to start stepper interrupt");
    while (1) delay(10);
    }
  Serial.println("Initialised Interrupt for Stepper");

  //Enable the stepper motor drivers
  pinMode(STEPPER_EN,OUTPUT);
  digitalWrite(STEPPER_EN, false);

  // Enable the background listening process
  Serial.println("Initialising Pi wire");
  //ArdPiWire::init();
  Serial.println("Initialised Pi wire");
  //ArdPiWire::values = configurable_params;
  Serial.println((int) configurable_params);
  //Serial.println((int) ArdPiWire::values);
  BaseType_t cur_core = xPortGetCoreID(), targ_core;
  if (cur_core == 0) targ_core = 1;
  else targ_core = 0;
  //xTaskCreatePinnedToCore(ArdPiWire::receiverThreadMain, "Recv", 1000, (void*) configurable_params, 1, &ArdPiWire::receiver_thread, targ_core);
  speedTimer = millis();
  printTimer = millis();
  loopTimer = millis();
}


void loop()
{
  if (millis() > 500 & millis() < 1000) lowest_speed = 100; //Speed is always 0 to begin with, so this resets it
  
  //Outer Loop
  if (millis() > speedTimer){
    speedTimer += SPEED_INTERVAL;

    //Calculate setpoint angle
    //float speed_avg = (speed1);//finding average may cause issue, try again later
    float speed_avg = (speed1 + speed2)/2; //subtracted as one will need to be negative

    //current_speed = -step1.getSpeedRad();
    current_speed = -(step1.getSpeedRad() - step2.getSpeedRad())/2;
    previous_speed_error = speed_error;
    speed_error = speed_avg - current_speed;
    
    speed_p_term = speed_kp*speed_error;
    speed_i_term = speed_ki*speed_error*SPEED_INTERVAL*0.001;
    speed_d_term = speed_kd*((speed_error - previous_speed_error)/SPEED_INTERVAL*0.001);
    setpoint_angle = speed_p_term + speed_i_term + speed_d_term + idle_setpoint;
    
    //Limit the setpoint angle
    if (setpoint_angle > idle_setpoint+angle_tolerance) setpoint_angle = idle_setpoint+angle_tolerance;
    else if (setpoint_angle < idle_setpoint-angle_tolerance) setpoint_angle = idle_setpoint-angle_tolerance;
    
    //Setpoint correction (Lowest speed version)
    // if (abs(current_speed) < speed_tolerance & abs(current_speed) < lowest_speed){
    //   idle_setpoint = setpoint_angle;
    //   lowest_speed = abs(current_speed);
    // } 
    
    //Setpoint correction (angular acceleration version)
    if (abs(current_speed) < speed_tolerance){//If you are close to the desired speed
      if (g_angle < accel_tolerance){//If you are not accelerating much (e.g not in the middle of a large oscillation)
        idle_setpoint -= g_angle*accel_kp;//adjust setpoint accordingly
      }
    }

  }
  
  //Inner Loop
  if (millis() > loopTimer) {
    loopTimer += LOOP_INTERVAL;
    // Fetch data from MPU6050
    sensors_event_t a, g, temp;
    if (!mpu.getEvent(&a, &g, &temp)) return;
    
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
    //step2.setAccelerationRad(PIDout);

    step1.setTargetSpeedRad(setSpeed*sign(PIDout)*sign(speed1));//Should work fine here for movement
    //step2.setTargetSpeedRad(-setSpeed*sign(PIDout)*sign(speed2));

    //Below is old code, kept only for potential future use
    // step1.setTargetSpeedRad(PIDout + (1-speed_ratio)*speed1);
    // step2.setTargetSpeedRad(-PIDout + (1-speed_ratio)*speed2);
  }
  
  //Print Loop
  if (millis() > printTimer) {
    printTimer += PRINT_INTERVAL;
    Serial.print("Speed Error = ");
    Serial.println(speed_error);
    Serial.print("Step1 Speed = ");
    Serial.println(step1.getSpeedRad());
    Serial.print("Setpoint = ");
    Serial.println(setpoint_angle*100);
    Serial.print("Current Angle = ");
    Serial.println(current_angle*100);
    Serial.println();
  }
  
  //Speed Changes
  if (millis() > 5000){
    speed1 = 60;
    speed2 = -60;
  }
  // if (millis() > 7000){
  //   speed1 = 0;
  //   speed2 = 0;
  // }
  vTaskDelay(1);
}