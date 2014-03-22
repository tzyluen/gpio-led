#!/usr/bin/python
#
# Description: Test the GPIO pins

import RPi.GPIO as GPIO
import time

out_pins_map = {'P0':11, 'P1':12, 'P2':13, 'P3':15, 'P4':16,
                'P5':18, 'P6':22, 'P7':7}

def blink(pin):
    print pin
    GPIO.output(pin, GPIO.HIGH)
    time.sleep(1)
    GPIO.output(pin, GPIO.LOW)
    time.sleep(1)
    return

def set_all():
    for k, p in out_pins_map.iteritems():
        GPIO.output(p, GPIO.HIGH)
    return

def unset_all():
    for k, p in out_pins_map.iteritems():
        GPIO.output(p, GPIO.LOW)
    return

# set to use Raspberry Pi board pin numbers
GPIO.setmode(GPIO.BOARD)
for k, p in out_pins_map.iteritems():
    # set up GPIO pins output channel
    GPIO.setup(p, GPIO.OUT)
    blink(p);

#set_all()
#time.sleep(3)
#unset_all()

GPIO.cleanup()
