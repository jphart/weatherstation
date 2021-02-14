Weather Station
===============

A project comprising of (multiple) weather sensors based on a Arduino and central monitoring database and dashboard viewable through a web browser hosted on a Raspberry Pi Zero.

Components
----------

Sensor

* 1x Arduino Nano
* BME-280 (BMP-280) Temperature, pressure and humidity module
* HC-12 wireless transceiver
* USB battery bank

Base station

* Raspberry Pi Zero W
* HC-12 wireless transceiver

Software
--------

Sensor

* Custom code to read and transmit the sensor data - see Sensor directory

Base station

* Raspbian linux 
* Grafana docker image for dashboard
* InfluxDB docker image for storing sensor data
* Custom receiver code - See Receiver 

Setup

* Requires arduino development tools to build sensor code
* Ansible to configure and setup the base station



How to
======
 
For a newly flashed SD card with raspbian:

```shell

ansible-galaxy collection install community.grafana
ansible-playbook --ask-pass -i hosts.ini weatherstation_setup.yml
```

or 

1. Copy you id_rsa.pub key to ~/.ssh/authorized_keys on the pi
2. ansible-playbook -i hosts.ini weatherstation_setup.yml



Licence
=======

Creative Commons Attribution CC BY - https://creativecommons.org/licenses/by/4.0/