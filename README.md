# Progress Log
## Initial Idea
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


# Friday 24th May
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
  - I've introduced a tolerance variable that can be adjusted to allow for the system to ignore some small changes in tilt, however this should be set to 0, or something rather small, (since the system shouldn't experience large pertubations in angle, so you shouldn't ignore small changes)
-The speed of motors can also be changed in the setup function, by default they are 10, but I haven't changed these at all, and and not sure what values are acceptable, so change with cuation.

In order for this to work, you need Visual Studio Code installed, and you want to install the PlatformIO extention. To connect the microcontroller, you'll also need some drivers, these should automatically install, but if it doesn't, use the link below:
- https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers?tab=downloads
- Right click the system information file and press install
- To upload the code from PlatformIO, click on the PlatformIO alien at the bottom of the left toolbar in visual studio, and press upload or upload and monitor to view the Serial Monitor.

Any questions let me know :)
  

# Tuesday 28th May
Today I corrected an error in the angle calculation, which meant the PID parameters could be tuned properly. The setpoint has also been adjusted accordingly, and now the bot can balance, with some small oscillations, which will always occur. It also has some good robustness to purtubations from external sources (I poked it with my finger and it didn't fall, and corrected relatively quickly). It cannot overcome large pertubations in tilt angle, however this is not possible to rectify within the scope of our project (don't poke it too hard). Some finer tuning to the PID values can be attempted, however the values as they stand do work correctly. 

Now the task from here is to design the outer-loop controller. This is also to be done via a PID controller, and I have a one main idea on how this will work:
Firstly, setting the setpoint to be something slightly off centre, may achieve the effect of cuasing the bot to move forwwards and backwards. I noticed while tuning the setpoint today, that if it is set too high/low, the robot will have a tendancy to drift towrards one side (the side it's tilting).
- This will be easy to test
- Doesn't account for turning
- However, the speed for this method is likely to be slow, so some offset of the motor speed may need to be experimented with (this would also solve the turning issue)

Experimenting with this tomorrow will likely adjust the course of future development, but this is a good starting point. Derriving the speed input from the two motors can be done relatively easily, under the following condition:
- The magnitude of the speed of each motors should always be the same (the direction/sign can differ)

This will allow me to take an average of the speeds, to find a singular one. For example, if you are travelling forwards at 10m/s (10 on both motors), then the output will be 10, but if you are turning at the same speed (10 and -10) then the average gives 0. This allows me to then adjust the setpoint based of the speed (since you want a tilt if going forwards or back, but no tilt whilst turning). 

The obvious arguement here is then that if you get a average of 0 when turning, will the bot even move, which is where the offset comes in. If you use the average to find angle setpoint, and the origninal speeds as offsets for the motors speeds (so the input to the motors will be PID_correction + Speed_offset)

I will begin tomorrow by testing the above theory, and seeing if it works. If it does, adding some code to derrive setpoint from speed won't be difficult, then it's just a case of adjusting PID values again. I'll also likely need to discuss how the speed will show up in my script, however I imagine it's just through a variable. 

# Sunday 9th June 
Haven't updated this over the week, but there hasn't been as much progress as I was hoping for. The theory outlined above sort of works, but there are still some major issues that prevent it from working consistently. 
- At random intervals, the motors will stutter and cause the bot to collapse.
  - No clue why this happens, and I can't seem to find an effective way to prevent it. It could be that the PID output suddenly becomes too high, but this isn't supported by any evidence (the angle error is never that big when this happens, and monitoring the angle never shows any glitches from the MPU)
  - Could also be due to battery isssues, as this issue becomes more prevelant when the battery is low.
  - Could also be a faulty connection.
- The speed of movement seems to vary depending on the angle at which the speed is inputed
  - For example, if it is told to go forward, but the current error is already high, then the bot will likely lose control.
  - The obvious solution to this is to tune the PID values more accurately to ensure the bot is always stable, however due to the other issue above, and the batteries dying today, this is difficult.
  - I have already implemented some changes to help correct this issue (speed/setpoint calulation placed in an outer loop, to ensure the inner loop has sufficient time to correct any error before the speed changes)
- Using the speed as an offset didn't exactly work, as it caused the bot to become more unstable. Instead, I used it as an offset, but it with a term infront that will reduce it's effect if the error is too big, meaning error correction will take a greater priority.
  - Need to investigate this further, as I'm not sure if it's effective.
 
I also realised the angle system I have is very arbitrary and doesn't have any physical basis (not degrees or radians). I attempted to create an entirely new controller with a radians system, however this was a waste of time, as it required me tuning the PID values again, and I didn't see the point in that. I also realised a couple of hours ago, that the angle I have is likely in degreses, but probably a factor of 1000 out as demonstrated in the starter code. Baboon. 

I will attempt to fix the issues above over the next week, and also begin on the report. I will likely require some assistance with this over the week, as it is a lot to do, in such a small amount of time. 

# Wednesday 12th June
So over the past 3 days I have decided to do a relatively large overhaul on the controller. The main changes are now the PID output of the angle controller controlls the acceleration instead of the target speed. This means a constant force is applied to prevent the bot from falling, instead of a speed, which once reached, will remove the counter-active force. This has meant the PID values needed to be retuned, however the static system is much more stable, and can overcome larger pertubations. 

The outerloop has also been heavily modified. I still have more work to do on it, but in essence, it will work the same as before, but now it can correctly measure current speed. It also has some sort of self correcting in terms of the setpoint, and it will aim to correct the speed properly in static conditions. I have tuned the values to the point where it will oscillate slightly while stationairy.

I have also tested movement, the results of which can be seen below:

With a speed of 35 on both motors:
| Test No  | Outcome | Description  |
| ------------- | ------------- | -------- | 
| Test 1  | Success |            | 
| Test 2  | Success |            | 
| Test 3  | Failure | Unstable before speed change | 
| Test 4  | Success |            | 
| Test 5  | Success |            | 
| Test 6  | Success |     Slow       | 
| Test 7  | Success |     Slow       | 
| Test 8  | Success |     Slow       | 
| Test 9  | Success |            | 
| Test 10  | Success |           | 

With any negative speed on the motors however, the setpoint correction logic appears to cause the motors to accelerate too quickly, and therefore movement in the other direction isn't possible with this current configuration. 

From here, the plan is as follows:
- Correct logic issue preventing negative direction
- Test negative direction movement
- Test turning movement
- Attempt to smooth out moving (forward to negative tilt)
- Tune system more accurately to remove oscillations

# Saturday 15th June
Not much progress has been made since the last update, due to other subsystems requiring the bot, and a lot of annoying issues preventing me form mvoing forward. We can now tune the bot wirelessly, and my plan was to finish all the tasks above over this weekend, however the batteries died when I came in today, which means I have only Monday now to finish it all.

I have tuned the bot slightly better, and added an alternative method of setpoint correction (untested), as well as potentially fixed the issue with negative directions (again, untested). The smoothing of movement with a backwards tilt at the end of moving can be done relatively easily via chaning the speeds. The plan on Monday as it stands is as follows:
- Test new setpoint correction
- Tune better
- Test movement
- Test turning
- Try smoothing of movement
Knowing how these things usually go, there will likely be a small issue that takes a long time to fix, and as a result, I may not finish the points above, in which case the tasks will be handed on to another member of my team to complete. I have also included instrucitons on approximate setpoint tuning for whoever requires it, at the top of the code. 
