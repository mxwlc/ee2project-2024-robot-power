/*How to Connect to RPI, to tune wirelessly

Connect both the RPI and arduino to the power PCB, and turn it on (not the motors yet)

Wait for the wifi network Robot_Power_Bot to show up on your laptop, then connect to it
(you won't have internet while you do this)

run the python scripts var_setter (to set variables, see list below), or var_getter (to see variables)

var_setter:
Type the index of the variable you wish to change (see definitions list to find 
the indexes)
Enter the new value
The programme will then close, so you'll need to run it again to change any other variables

var_getter:
Enter the index of the variable you wish to view, and press enter repeatedly to view the 
value of this variable as it changes 
(CAN BE TEMPREMENTAL, BETTER TO USE SERIAL MONITOR)

IF THE ARDUINO IS DISCONNECTED AT ANY POINT, EITHER TO UPLOAD NEW CODE, OR IF THE RESET BUTTON
IS PRESSED, YOU WILL NEED TO RESTART BOTH THE RPI AND ARDUINO (FLICK GREEN SWITCH ON
PCB)
*/

/*Conventions:
When I refer to forward, I mean the side with the edge of the triangle, and backwards
is the side with the flat face (and the ESP on it).

When moving forwards, step1 is negative, and step2 is positive.

Facing the ESP, step1 is to the right, and step2 is to the left

To turn left, step1 must be negative, and step2 must be negative, but speed1 must
be positive, and speed 2 must be negative (weird I know).

To turn right, the opposite must be true.
*/

//Libraries
#include <Arduino.h>
#include <TimerInterrupt_Generic.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <step.h>
#include "ardpiwire.hpp"


#pragma region Definitions
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

#define CONF_SET_SPEED 0 //Speed of the motors
#define CONF_IDLE_SETPOINT 1 //Setpoint when the robot is balanced
#define CONF_KP 2 //Angle Proportional term
#define CONF_KI 3 //Angle Integral term
#define CONF_KD 4 //Angle Differential term
#define CONF_SPEED_KP 5 //Speed Proportional term
#define CONF_SPEED_KI 6 //Speed Integral term
#define CONF_SPEED_KD 7 //Speed Differential term
#define CONF_SPEED_TOLERANCE 8 //Maximum speed motors can be moving in order for setpoint correction to occur
#define CONF_ANGLE_TOLERANCE 9 //Maximum distance away from the idle setpoint that the setpoint can become
#define CONF_ACCEL_TOLERANCE 10 //Not used
#define CONF_SPEED1 11 //Speed of motor 1
#define CONF_SPEED2 12 //Speed of motor 2
#define CONF_ACCEL_KP 13 //Not used
#define CONF_TURN 14 //Not used
#define CONF_ACCEL_ATTEN 15 //Not used
#define CONF_CURRENT_ROTATION 16 //Current rotation of the robot 
#define CONF_SPEED_OFFSET 17 //Not used
#define CONF_ROTATION_ANGLE 18//Desired rotation of the robot (rotation setpoint)
#define CONF_ROTATION_KP 19 //Rotation Proportional Term
#define CONF_ROTATION_KI 20 //Rotation Integral Term
#define CONF_ROTATION_KD 21 //Rotation Differential Term
#define CONF_DISTANCE 21 //Ultrasonic distance
#define CONF_POWER_CONSUMPTION 22 //Power Consumption
#define CONF_POWER_REMAINING 23 //Remaining Power

#pragma endregion

volatile float configurable_params[] = {//Default values of above
  /*set_speed*/       20.0f,
  /*idle_setpoint*/   0.01f,
  /*kp*/              2250.0f,
  /*ki*/              0.0f,
  /*kd*/              18000.0f,
  /*speed_kp*/        0.001f,
  /*speed_ki*/        0.0f,
  /*speed_kd*/        0.0f,
  /*speed_tolerance*/ 0.005f,
  /*angle_tolerance*/ 0.05f,
  /*accel_tolerance*/ 0.01f,
  /*speed1*/          0.0f,
  /*speed2*/          0.0f,
  /*accel_kp*/        0.01f, //Look more closely at this, could solve setpoint issue
  /*turn*/            0.0f,
  /*accel_atten*/     0.2f,
  /*current_rotation*/0.0f,
  /*speed_offset*/    0.5f,
  /*rotation_angle*/  0.0f,
  /*rotation_kp*/     0.75f,
  /*rotation_ki*/     0.0f,
  /*rotation_kd*/     0.0f,
  /*distance*/        0.0f,
  /*power_consumption*/ 0.0f,
  /*power_remaining*/   0.0f
};


#pragma region Variables
const float C = 0.95;
volatile float& idle_setpoint = configurable_params[CONF_IDLE_SETPOINT];
float setpoint_angle = idle_setpoint;
volatile float& kp = configurable_params[CONF_KP];
volatile float& ki = configurable_params[CONF_KI]; //should be 0 if static setpoint is set correctly
volatile float& kd = configurable_params[CONF_KD];

//Errors
float error = 0;
float previous_error;

//Angles
float a_angle; //Accelerometer reading
float g_angle; //Gyroscope reading
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
volatile float& accel_atten = configurable_params[CONF_ACCEL_ATTEN];

//This is how large the error can be for setpoint to change
volatile float& speed_tolerance = configurable_params[CONF_SPEED_TOLERANCE];
volatile float& angle_tolerance = configurable_params[CONF_ANGLE_TOLERANCE];
volatile float& accel_tolerance = configurable_params[CONF_ACCEL_TOLERANCE];
float lowest_speed = 100; //Lowest speed achieved thus far by the motors


//Speeds
volatile float& setSpeed = configurable_params[CONF_SET_SPEED];
volatile float& speed1 = configurable_params[CONF_SPEED1];
volatile float& speed2 = configurable_params[CONF_SPEED2];
volatile float& turn = configurable_params[CONF_TURN];
volatile float& speed_offset = configurable_params[CONF_SPEED_OFFSET];
float current_speed = 0;

float speed_error = 0;
float previous_speed_error = 0;

float speed_p_term;
float speed_i_term;
float speed_d_term;

float PIDout;

//Rotation
bool rotating = false;
volatile float& rotation_angle = configurable_params[CONF_ROTATION_ANGLE];
volatile float& rotation_kp = configurable_params[CONF_ROTATION_KP];
volatile float& rotation_ki = configurable_params[CONF_ROTATION_KI];
volatile float& rotation_kd = configurable_params[CONF_ROTATION_KD];
volatile float& current_rotation = configurable_params[CONF_CURRENT_ROTATION];
float prev_rotation = 0;

float previous_rotation_error;
float rotation_error = 0;

float rotation_p_term;
float rotation_i_term;
float rotation_d_term;

float rotation_offset;

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

//Gives 1 if positive, and -1 if negative
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
  ArdPiWire::init();  //COMMENT THIS LINE OUT IF TUNING WITH ARDUINO, AND NOT RPI
  Serial.println("Initialised Pi wire");
  //ArdPiWire::values = configurable_params;
  Serial.println((int) configurable_params);
  //Serial.println((int) ArdPiWire::values);
  BaseType_t cur_core = xPortGetCoreID(), targ_core;
  if (cur_core == 0) targ_core = 1;
  else targ_core = 0;
  //COMMENT THE LINE BELOW OUT IF TUNING WITH ARDUINO, AND NOT RPI
  xTaskCreatePinnedToCore(ArdPiWire::receiverThreadMain, "Recv", 1000, (void*) configurable_params, 1, &ArdPiWire::receiver_thread, targ_core);
  speedTimer = millis();
  printTimer = millis();
  loopTimer = millis();
}


void loop()
{
  if (rotation_angle != 0) rotating = true;
  else rotating = false;//This sets rotationg to true, if the rotation
  //setpoint is not 0 (AKA only if we want to turn)

  //Speed is always 0 to begin with, so this resets it, so the true minimum can be found
  if (millis() > 500 & millis() < 1000) lowest_speed = 100; 
  
  //Outer Loop
  if (millis() > speedTimer){
    speedTimer += SPEED_INTERVAL;

    //Calculate setpoint angle
    float speed_avg = (speed1 + speed2)/2;

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
    
    //Setpoint correction (Lowest speed version) (Small effect, mainly on static balancing)
    if (abs(current_speed) < speed_tolerance & abs(current_speed) < lowest_speed){
      idle_setpoint = setpoint_angle;
      lowest_speed = abs(current_speed);
    } 
    
  }
  
  //Inner Loop
  if (millis() > loopTimer) {
    loopTimer += LOOP_INTERVAL;

    // Fetch data from MPU6050 (get sensor data)
    sensors_event_t a, g, temp;
    if (!mpu.getEvent(&a, &g, &temp)) return;
    
    //Calculate Tilt using MPU, code given in starter code, don't need to worry, works correctly
    a_angle = a.acceleration.z/9.67;
    g_angle = g.gyro.pitch;
    previous_angle = current_angle;
    current_angle = ((1-C)*a_angle + C*(g_angle*LOOP_INTERVAL*0.001+previous_angle));//Complementary filter
    
    previous_error = error;
    error = setpoint_angle - current_angle;

    angle_p_term = kp*error;
    angle_i_term = ki*(error*LOOP_INTERVAL*0.001); //0.001 for ms
    angle_d_term = kd*((error-previous_error)/LOOP_INTERVAL*0.001);
  
    PIDout = angle_p_term + angle_i_term + angle_d_term;

    //Rotation
    prev_rotation = current_rotation;
    //Calculate the rotation of the robot (via a simillar method to the complementary filter above)
    if (abs(g.gyro.roll) >= 1) current_rotation = (-g.gyro.roll - 1.005) *LOOP_INTERVAL*0.001 + prev_rotation;
  

    previous_rotation_error = rotation_error;
    rotation_error = rotation_angle - current_rotation;
    if (rotation_error < 0.075) rotation_error = 0;//If there is a small error, ignore it
    
    rotation_p_term = rotation_kp*rotation_error;
    rotation_i_term = rotation_ki*(rotation_error+previous_rotation_error*LOOP_INTERVAL*0.001);
    rotation_d_term = rotation_kd*(rotation_error-previous_rotation_error)/(LOOP_INTERVAL*0.001);

    //We want to change acceleration, not speed
    step1.setAccelerationRad(PIDout-rotation_offset);//Introduce offset between accelerations
    step2.setAccelerationRad(-PIDout-rotation_offset);
    step1.setTargetSpeedRad(setSpeed*sign(PIDout));//Sign of PID used to control direction
    step2.setTargetSpeedRad(-setSpeed*sign(PIDout));

    //This is an alternative method, with just a fixed offset, it works worse, but 
    //included here in case you find it useful.
    // if(sign(speed1) == sign(speed2)){
    //   step1.setAccelerationRad(PIDout);
    //   step2.setAccelerationRad(PIDout);
    //   step1.setTargetSpeedRad(setSpeed*sign(PIDout));//Should work fine here for movement
    //   step2.setTargetSpeedRad(-setSpeed*sign(PIDout));
    // }
    // else if (sign(speed1) == 1 && sign(speed2) == -1){//Left turn
    //     step1.setAccelerationRad(PIDout + speed1*speed_offset);
    //     step2.setAccelerationRad(PIDout);
    //     step1.setTargetSpeedRad(setSpeed*sign(PIDout));
    //     step2.setTargetSpeedRad(-setSpeed*sign(PIDout));
    //   }
    // else if (sign(speed1) == -1 && sign(speed2) == 1){//Right turn)
    //   step1.setAccelerationRad(PIDout);
    //   step2.setAccelerationRad(PIDout + speed2*speed_offset);
    //   step1.setTargetSpeedRad(setSpeed*sign(PIDout));
    //   step2.setTargetSpeedRad(-setSpeed*sign(PIDout));
    // }
  }
  
  //Print Loop, put any print statements here
  if (millis() > printTimer) {
    printTimer += PRINT_INTERVAL;
    sensors_event_t a, g, temp;
    if (!mpu.getEvent(&a, &g, &temp)) return;
    Serial.print(current_rotation);
    Serial.print(" | ");
    Serial.print(g.gyro.roll);
    Serial.println();
  }
  
  //Changes - IF TUNING VIA ARDUINO, AND NOT RPI, THEN USE THIS TO CREATE SPEED 
  //AND/OR ROTATIONAL CHANGES, AS IT ALLOWS THE BOT 5s TO BALANCE BEFORE CHANGING AGAIN
  //AFTER 2s.
  // if (millis() > 5000){
  //   rotation_angle = 0.25;
  // }
  // if (millis() > 7000){
  //   rotation_angle = -0.25;
  // }

  vTaskDelay(1);
}