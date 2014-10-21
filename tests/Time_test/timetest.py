#
# http://www.kaddu.dk/index.php/en/projects/herbot
# GitHub Repository: https://github.com/JanHolbo/HerBot
# (C) 2014 by Jan Rasmussen & Kaddu.dk jan at kaddu dot dk
# This file and others in this pack is - where not otherwise stated - covered by the GPL v2 license
# Please feel free to fork this project and please submit patches and feature requests to the above email 
#
# File: HerBot/tests/Time_test/timetest.py
# Version: 
#
# This is a test-bed python script to determine the accuracy of a non-RTC based clock on Arduino
# See also the Arduino sketch file HerBot/tests/Time_test/Time_test.ino
#

import serial
import time

port = serial.Serial("/dev/ttyUSB1", baudrate=9600, timeout = 3.0)

print ("T" + str(int(time.time())) + "\n")
port.write ("T" + str(int(time.time())) + "\n")

count = 0;
while count<8*12:
    time.sleep(5*60)

    temp = port.readline()
    port.write (chr(7))
    pctime = int(time.time())
    head = port.read()
    if (head=='T'):
        ardu = port.readline()
        arduTime = int(ardu)
        print (str(pctime) + "," + str(arduTime) + ",") 
    else:
        temp = port.readline()
    count = count + 1

