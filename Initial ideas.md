# Power:
## Initial ideas
- ADC - Use comparator
- j3 connect straight away to motor
- j2 lower voltage then connect to adc and to microcontroller
- opamp design, differential opamp
- rail to rail input
- resistor on rail?
- amplify voltage difference --> skewed value?
## Consultation
- ADC on the microcontroller module
- Differential amplifier better as less power loss compared to voltage divider (high current means high power loss over resistor)
- motor wires connect directly to (microcontroller/rasberry pi?)
- gyroscope module near wheels
- power management from J2 where adc output shows current power usage
- single supply rail opamp
- Coulomb counting
- DC offset on opamp to avoid clipping
##
- Check if our differential amplifier would work
- Motor V * Motor I , Battery V,  Battery I --> Why would this not work?
## Ed help
- rail to rail, precision opamp --> small AC signal at rail voltage
- capacitor in feedback loop
- Rails at 5,0 V
- R = 10mOhms
  ![image](https://github.com/mxwlc/ee2project-2024-robot-power/assets/54540123/99bce9d5-42c1-4a13-b07d-823b29d5a97d)
- Current going straight into USB ports --> differential of voltages at I5 and 5 V will give power consumed

  ##Mehwish
  ![image](https://github.com/mxwlc/ee2project-2024-robot-power/assets/170433436/f9e0758c-59cd-4b77-87ce-9506f7811fb7)

