from sys import argv
from os import curdir, sep
from urllib.parse import urlparse, parse_qs
from board import SCL, SDA
import busio
import math
import time

# Import the PCA9685 module.
from adafruit_pca9685 import PCA9685

# Create the I2C bus interface.
i2c_bus = busio.I2C(SCL, SDA)

# Create a simple PCA9685 class instance.
pca = PCA9685(i2c_bus)

# Set the PWM frequency to 1000hz.
pca.frequency = 60

R_CHANNEL = 0
G_CHANNEL = 1
B_CHANNEL = 3
AMP = 0xffff

angle = 0
PI_2 = math.pi * 2


def clamp(v, minn, maxn):
    return max(min(v, maxn), minn)


def set_color(r, g, b):
    pca.channels[R_CHANNEL].duty_cycle = clamp(AMP - r, 0, AMP)
    pca.channels[G_CHANNEL].duty_cycle = clamp(AMP - g, 0, AMP)
    pca.channels[B_CHANNEL].duty_cycle = clamp(AMP - b, 0, AMP)


while True:
    if angle > PI_2:
        angle = 0
    else:
        angle += 0.02

    r = int(math.sin(angle) * AMP)
    g = int(math.sin(angle + 1 / 3 * PI_2) * AMP)
    b = int(math.sin(angle + 2 / 3 * PI_2) * AMP)

    set_color(r, g, b)

    time.sleep(0.01)
