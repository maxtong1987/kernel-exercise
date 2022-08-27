# Simple demo of of the PCA9685 PWM servo/LED controller library.
# This will move channel 0 from min to max position repeatedly.
# Author: Tony DiCola
# License: Public Domain
from __future__ import division
import math
from http.server import BaseHTTPRequestHandler, HTTPServer
import logging
from urllib.parse import urlparse, parse_qs
from os import curdir, sep
from sys import argv

# Import the PCA9685 module.
import Adafruit_PCA9685

# Initialise the PCA9685 using the default address (0x40).
pwm = Adafruit_PCA9685.PCA9685()

pwm.set_pwm_freq(1000)

R_CHANNEL = 0
G_CHANNEL = 1
B_CHANNEL = 2
END_TICK = 4095
SCALE = 4096.0 / 256


def set_color2(num):
    print("num: ", num)
    r = (num & 0xff0000) >> 16
    g = (num & 0x00ff00) >> 8
    b = num & 0xff
    set_color(r, g, b)


def to_brightness(color):
    return max(math.floor(SCALE * color), 0)


def set_color(r, g, b):
    print("RGB:", r, g, b)
    pwm.set_pwm(R_CHANNEL, to_brightness(r), END_TICK)
    pwm.set_pwm(G_CHANNEL, to_brightness(g), END_TICK)
    pwm.set_pwm(B_CHANNEL, to_brightness(b), END_TICK)


class MyServer(BaseHTTPRequestHandler):

    def do_setColor(self, query_components):
        color = query_components['color'][0]
        set_color2(int(color, 0))

    def do_GET(self):
        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.end_headers()
        result = urlparse(self.path)
        path = result.path
        query_components = parse_qs(result.query)
        print(result.path)
        if result.path == '/color':
            self.do_setColor(query_components)
        else:
            print(curdir + path)
            f = open(curdir + path, 'rb')
            self.wfile.write(f.read())
            f.close()


def run(server_class=HTTPServer, handler_class=MyServer, port=8080):
    logging.basicConfig(level=logging.INFO)
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    logging.info('Starting httpd...\n')
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        pass
    httpd.server_close()
    logging.info('Stopping httpd...\n')


if __name__ == '__main__':
    run()
