# Initial idea
So the requiremnts say we need to use a cascaded controller, with the outerloop as 'speed' and the inner loop as the tit angle. There is a few things to consider here:
- How do you get a 'speed' value given that we have to control two motors, which may have opposing directions, also consider turning, what effect should that have on tilt angle, and how will the
  system currently respond to that? (consider using velocity instead, and finding a way to combine the two speeds of the motors? Or have system run once for each motor?)
- How can you correlate between a speed and an angle? What is a suitable way to determine the required angle?
- What exactly are the inputs and outputs of the system?
- Can I make a simplified model, and then find transfer functions of the system from this model? (May not actually need this in terms of implementation, but would be useful for the final report)

## Plan from here:
- Create inner loop PID controller (look through examples and code, don't worry about modelling yet), and tune parameters
- Create outer loop controller (link the motors together, and use the 'speed' and a PID controller to create a angle setpoint)
  - Should look into this more, not super sure why the tilt is necessary whilst moving?
- Model the system and create transfer functions and graphs from this
- Potentially look into generating PID values via loop-shaping and synthesis?

#Friday 24th July
Created a basic PID controller, implemented tilt angle calculation, wired up robot and begain trial and error for PID values.

Biggest unresolved issue thus far has been tuning. It has prooven very difficult to obtain suitable values of gain to stablise the robot. More time needs to be invested into either manually tuning, or calculating PID values through loop shaping (potentially long and difficult). If anyone reading this wants to try manually tuning, use the guide below to do so (since the code I have uploaded right now is very messy)

- PID gains are at the top of the script:
  - they (according to 3-4 other groups and my own trial and error) should be something in the range of hundereds for kp, 0 for ki (if the setpoint is roughly correct and around the centre of mass), and kd should be 10-500 (haven't varied this too much yet, so feel free to).
  - If the robot keeps running away and falling, try adjusting the proportional gain
  - If it oscillates a lot, try adjusting the differential gain
  - No clue what the integral one does
- Chaning the value of C is a constant between 0 and 1:
  - It will vary the ratio between the accelerometer and the gyroscope (acceloremter measures changes in angle over a longer period than the gyroscope, and the gyroscope will be more sensitive to rapid changes. If C is high, then the gyrscope is more favoured, and vice versa.
  - According to the project github, you want C to be close to 1.
- The setpoint is the idle angle, and can be found by monitoring the robot while it's connected your laptop via PlatformIO, and holding it upright and viewing the angle (typically around -0.01 to 0.03)
  - I've introduced a tolerance variable that can be adjusted to allow for the system to ignore some small changes in tilt, however this should be adjusted, and potentially even eliminated, as it may cause some issues with the response.
-The speed of motors can also be changed in the setup function, by default they are 10, but I haven't changed these at all, and and not sure what values are acceptable, so change with cuation.

In order for this to work, you need Visual Studio Code installed, and you want to install the PlatformIO extention. To connect the microcontroller, you'll also need some drivers, these should automatically install, but if it doesn't, use the link below:
- https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers?tab=downloads
- Right click the system information file and press install
- To upload the code from PlatformIO, click on the PlatformIO alien at the bottom of the left toolbar in visual studio, and press upload or upload and monitor to view the Serial Monitor.

Any questions let me know :)
  

