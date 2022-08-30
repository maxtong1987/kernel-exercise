from sys import argv
from os import curdir, sep
from urllib.parse import urlparse, parse_qs
import logging
from http.server import BaseHTTPRequestHandler, HTTPServer
from board import SCL, SDA
import busio

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
B_CHANNEL = 2


def set_color2(num):
    print("num: ", num)
    r = (num & 0xff0000) >> 16
    g = (num & 0x00ff00) >> 8
    b = num & 0xff
    set_color(r, g, b)


def set_color(r, g, b):
    print("RGB:", r, g, b)
    pca.channels[R_CHANNEL].duty_cycle = 0xffff - (r << 8)
    pca.channels[G_CHANNEL].duty_cycle = 0xffff - (g << 8)
    pca.channels[B_CHANNEL].duty_cycle = 0xffff - (b << 8)


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
