#!/usr/bin/env python3

import serial
import time

ser = serial.Serial(

	port='/dev/ttyS0',
	baudrate=1200,
        parity=serial.PARITY_NONE,
	stopbits=serial.STOPBITS_ONE,
	bytesize=serial.EIGHTBITS,
	timeout=30
    )


while 1:
    ser.write(b'H')
    ser.close()
    time.sleep(3)
    print("ping \n")