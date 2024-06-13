# NOTE: THIS IS JUST A DRAFT, AND HASN't BEEN FINISHED/FULLY TESTED AS IT SEEMS UNNECESSARY

import RPi.GPIO as GPIO
from time import sleep

__CLK_PIN = 23
__ACK_PIN = 22
__DATA_PIN = 21

__FREQ = 10_000

def init():

    GPIO.setmode(GPIO.BCM)

    GPIO.setmode(__CLK_PIN, GPIO.OUT, initial = GPIO.LOW)
    GPIO.setmode(__ACK_PIN, GPIO.IN)
    GPIO.setmode(__DATA_PIN, GPIO.OUT, initial = GPIO.LOW)

def sendByte(data):
    mask = 1
    for i in range(8):
        
        # Send signal to send bit
        GPIO.output(__CLK_PIN, 1)

        # Await acknowledgement for bit
        while (GPIO.input(__ACK_PIN) != 1):
            sleep(0.000_01)
        
        # Send bit
        GPIO.output(__DATA_PIN, (data & mask) != 1)
        # sleep(0.000_01)
        GPIO.output(__CLK_PIN, 0)

        # Give Arduino time. In this, it sets ACK back to 0, and reads DATA_PIN.
        if mask != 0x80: sleep(1 / __FREQ)

        mask <<= 1
