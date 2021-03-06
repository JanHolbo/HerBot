#
# http://www.kaddu.dk/index.php/en/projects/herbot
# GitHub Repository: https://github.com/JanHolbo/HerBot
# (C) 2014 by Jan Rasmussen & Kaddu.dk jan at kaddu dot dk
# This file and others in this pack is - where not otherwise stated 
# covered by the GPL v2 license
# Please feel free to fork this project and please submit patches and 
# feature requests to the above email 
#
# File: HerBot/tests/Time_test/timetest.py
# Version: 
#
# This is a test-bed python script to determine the accuracy 
# of a non-RTC based clock on Arduino
# See also the Arduino sketch file HerBot/tests/Time_test/Time_test.ino
#

import serial
import time
import glob
import sys


# Open the serial port. Check which port is being used by the serial link
#  to the Arduino (ttyUSBx)


def serial_ports():
    """Lists serial ports

    :raises EnvironmentError:
        On unsupported or unknown platforms
    :returns:
        A list of available serial ports
    """
    if sys.platform.startswith('win'):
        ports = ['COM' + str(i + 1) for i in range(256)]

    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this is to exclude your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')

    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')

    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result




# 
# main function (begin)
#

if __name__ == '__main__':
    serPorts = serial_ports()		# get a list of available serial ports 
    port = serial.Serial(serPorts[0], baudrate=9600, timeout = 2.0)
					# select the first available

    print ("HerBot V 0.1-alpha timetest.py script\n")
    print ("Please enter probing frequency (times per hour) : ")
    freq = int ( raw_input())
    print ("Please enter number of probes :")
    probes = int ( raw_input())


    print ("Test will last approx. " + str (probes / freq) + " hours.\n")

    print ("T" + str(int(time.time())) + "\n")
    port.write ("T" + str(int(time.time())) + "\n")

    count = 0;

    while count<probes:

        time.sleep((60/freq)*60)

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

# 
# main function (end)
#

