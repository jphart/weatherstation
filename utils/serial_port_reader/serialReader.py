#!/usr/bin/env python3

import serial

ser = serial.Serial(

	port='/dev/ttyS0',
	baudrate=1200,
        parity=serial.PARITY_NONE,
	stopbits=serial.STOPBITS_ONE,
	bytesize=serial.EIGHTBITS,
	timeout=30
    )


while 1:
    x=ser.readline()
    print(x)
    print("\n\n")