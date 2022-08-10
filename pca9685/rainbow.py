# Simple demo of of the PCA9685 PWM servo/LED controller library.
# This will move channel 0 from min to max position repeatedly.
# Author: Tony DiCola
# License: Public Domain
from __future__ import division
import time
import math

# Import the PCA9685 module.
import Adafruit_PCA9685


# Uncomment to enable debug output.
#import logging
#logging.basicConfig(level=logging.DEBUG)

# Initialise the PCA9685 using the default address (0x40).
pwm = Adafruit_PCA9685.PCA9685()

pwm.set_pwm_freq(1000)

R_CHANNEL = 0
G_CHANNEL = 1
B_CHANNEL = 2

angle = 0
AMP = 4095

PI_2 = math.pi * 2

while True:
    if angle > PI_2:
        angle = 0
    else:
        angle += 0.02

    R = int(math.sin(angle) * AMP)
    G = int(math.sin(angle + 1 / 3 * PI_2) * AMP)
    B = int(math.sin(angle + 2 / 3 * PI_2) * AMP)

    pwm.set_pwm(R_CHANNEL, max(0, R), 4095)
    pwm.set_pwm(G_CHANNEL, max(0, G), 4095)
    pwm.set_pwm(B_CHANNEL, max(0, B), 4095)
    
    time.sleep(0.01)