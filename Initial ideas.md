# Power:
## Initial ideas
-ADC - Use comparator
-j3 connect straight away to motor
-j2 lower voltage then connect to adc and to microcontroller
-opamp design, differential opamp
- rail to rail input
resistor on rail?
-Amplify voltage difference --> skewed value?
## Consultation
- ADC on the microcontroller module
- Differential amplifier better as less power loss compared to voltage divider (high current means high power loss over resistor)
- motor wires connect directly to (microcontroller/rasberry pi?)
- gyroscope module near wheels
- power management from J2 where adc output shows current power usage
- single supply rail opamp
