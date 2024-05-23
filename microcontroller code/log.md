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

