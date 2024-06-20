# Tuning Guide
Making this so hopefully anyone can tune and test the controller. The latest version of the controller can be found in the control_system branch, under the directory microcontroller code/ZC Code/src 
and it's called new.cpp. It's also reccomended that you download all the scripts in the Python RPI folder, as this will help with tuning 
by allowing you to change variables wirelessly.

## How it works
The controller has two main bits, the inner loop and the outer loop, but before I get onto those, there is a few things I'd like to point out first.
The first thing, is right at the top of the code, there is instructions on how to use the RPI to change certain variables wirelessly. The variables that can
be chagned and defined in the definition section, and they also have descriptions on what they do, as well as their default values below. If you cannot
for whatever reaosn use the RPI, you must change the default value of these variables in order to change their value, and re-upload the code.

Also, if you are not using the RPI, there are two lines in the setup function that must be commented out (shown in the code).

### Outer Loop
This loop takes the average of the speed we want, and the average of the speed we have, in order to find the error in our current speed. This is then turned into a setpoint
value via PID controller, which is then passed on to the inner loop. There is also some logic in place in this section in order to prevent the setpoint from getting too large, by limiting it's value;
and another piece of logic to automatically adjust the idle setpoint, if the robot isn't moving much (not very effective, but correct setting of the setpoint
produces a stable enough system, only bother trying to adjust/fix this if all other issues are fixed first, potential ways of fixing this will be explained in detail later on).

### Inner Loop
This loop takes the current angle, derrived from the MPU via a complementary filter (look in Ed's project github for more detail on this), and subtracts it from the 
setpoint angle, to find the tilt error. This is then put through another PID to get the variable PIDout, which is used primarily to drive the acceleration of the motors.
The speed of the motors is kept at a set value, however this is multiplied by the sign of PIDout (in order to change direction depending on tilt
angle) (one motor will also be multiplied by -1, due to the motors facing opposite directions).

The rotation code is also found in this loop, and is responsible for turning the robot. This code is the one with the issue, and should be focused on the most. Through a few
different methods, I eventually ended up using another PID controller to implemnt this, by getting the rotation of the robot by integrating the gyroscope (simillar to the complementary filter),
and using this with the rotation we actually want, to create a rotation error. Once again, this goes through a PID to create a rotation offset, which is used alongside
the PIDout,to control the speed of the motors. The idea here is that one motor will be going faster than the other, in order to turn the robot. It successfully does this, however the robot will 
not stop turning, and also can only turn in one direction. 

## How to find the setpoint
The idle setpoint can change easily, and it's important to be set correctly in order for the robot to balance properly. This is done by turning it on
and allowing the motors to run. When you hold it, it's likely that they will spin at full speed, just simply place it down on the floor and it should stablise 
itself (however keep your hands nearby in case it doesn't). If the robot balances, but has a tendancy towards the back (the side with the Arduino), then the setpoint is too low, and will
need to be higher. If it moves in the opposite direction, the setpoint is too high, and must be lowered. On average, the setpoint will typically be anywhere between 0.01 to 0.06.

## Things to try
- In regards to the turning, I'd reccomend trying to tune the PID values a little bit more, especially the I and D terms, as I have not had much time to properly investigate their effects, and 
from talking with another group, the D term specifically seemed to have a large effect. 
- Alternatively, you could alter how the rotation effects the actual accelration/speed of the motors, to see how this will effect
the system. 
- You could also just implement something different entirely, just be careful not to break it.
- If you can get it to turn, then you can try getting it more stable, by either tuning the PID values for the inner tilt angle loop, or 
by adjusting the speed proportional value, in order to get a more precise version of the setpoint correction. 
