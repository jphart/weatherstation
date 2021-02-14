#!/usr/bin/env python3

import serial
import json
from influxdb import InfluxDBClient
from datetime import datetime

LOCATION_ID = 'l'
TEMPERATURE_ID = 't'
HUMIDITY_ID = 'h'
DEW_POINT_ID = 'd'
PRESSURE_ID = 'p'


def buildInfluxDBMessage(sensor_message):
    """
    Builds a influxDB json object from the message from a sensor. 
    """
    
    json_body = [{
        "measurement" : "weather_reading",
        "time" : datetime.now().isoformat(),
        "tags" : 
            {
                "location" : sensor_message[LOCATION_ID],
            },
        "fields" : 
            {
                "humidity" : sensor_message[HUMIDITY_ID],
                "temperature" : sensor_message[TEMPERATURE_ID],
                "pressure" : sensor_message[PRESSURE_ID],
                "dew_point" : sensor_message[DEW_POINT_ID]
            }
    }]

    return json_body



ser = serial.Serial(
	port='/dev/ttyS0',
	baudrate=1200,
        parity=serial.PARITY_NONE,
	stopbits=serial.STOPBITS_ONE,
	bytesize=serial.EIGHTBITS,
	timeout=30
    )


client = InfluxDBClient(host='localhost', port=8086)
client.switch_database('weather')


while True:
    message = ser.readline()
    print(message)
    print("\n\n")
    
    try:
        jsonmsg = json.loads(message)
        
        json_body = buildInfluxDBMessage(jsonmsg)


        print(json.dumps(json_body,indent=2))

        print("Write points: {0}".format(json_body))
        client.write_points(json_body)

    except json.decoder.JSONDecodeError as ex:
        print(ex.msg)
