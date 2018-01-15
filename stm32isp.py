#!/usr/bin/env python

import serial
import time

s = serial.Serial('/dev/tty.usbserial',
                  baudrate=57600,
                  bytesize=8,
                  parity='O',
                  stopbits=1,
                  timeout=3)

def stm32_sync():
    s.flushInput()
    while True:
        s.write(0x7f)
        time.sleep(0.01)
        if s.inWaiting() != 0:
            if s.read(1) == 0x79:
                return True
            else:
                return False

print(stm32_sync())

s.close()
        
