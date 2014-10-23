#
# http://www.kaddu.dk/index.php/en/projects/herbot
# GitHub Repository: https://github.com/JanHolbo/HerBot
# (C) 2014 by Jan Rasmussen & Kaddu.dk jan at kaddu dot dk
# This file and others in this pack is - where not otherwise stated 
# covered by the GPL v2 license
# Please feel free to fork this project and please submit patches and 
# feature requests to the above email 
#
# File: HerBot/herbot.py
# Version: 0.2-beta
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
# dump the log contents into a file (appending it)
#

def dump_log():
    """Reads the log present on the HerBot Arduino through the serial port

    :raises EnvironmentError:
        On unsupported or unknown platforms
    :returns:
    """

    print ("Requesting the log ...")
    port.write ('L')		# request the log

    logfile = open('herbot.log', 'a')	# please append to the file

    response = 1
    while (response):
        head = port.read()	# first character should be an E (Entries)

        if (head == 'E'):
            entries=int(port.readline())
            print (str(entries) + " log entries to read:")
            count = 0

            while (count<entries):
                temp = port.readline()
                logfile.write (temp) 
                count = count + 1
            if (entries == 0):
                response = 0
        elif (head == 'T'):
            herbot_time=int(port.readline())
            print (str(herbot_time) + " seconds since epoch")
            response = 0
        else:
            temp = port.readline()
            print ("unrecognised response: " + head + temp)

    port.write ("T" + str (int (time.time())) + "\n")


# 
# main function (begin)
#

if __name__ == '__main__':
    serPorts = serial_ports()	# get a list of available serial ports 
    port = serial.Serial(serPorts[0], baudrate=9600, timeout = 2.0)
				# select the first available

    print ("HerBot v0.2-beta herbot.py script\n")

    while (port.inWaiting()):
        head = port.read()	# first character should be an E (Entries)

        if (head == '!'):	# error (info?) message
            temp = port.readline()
            print ("Error/info mesage: " + head + temp)
        elif (head == '?'):	# message requesting response
            temp = port.readline()
            print ("Requesting action: " + head + temp)
            print ("HerBot not synced so providing it with time")
            port.write ("T" + str (int (time.time())) + "\n") # not synced so provide time
            temp = port.readline()
        elif (head == 'T'):
            herbot_time=int(port.readline())
            print (str(herbot_time) + " seconds since epoch")
        else:
            temp = port.readline()
            print ("unrecognised response: " + head + temp)

    dump_log()

# 
# main function (end)
#

