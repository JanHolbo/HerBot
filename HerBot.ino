/* 
 * HerBot
 * http://www.kaddu.dk/index.php/en/projects/herbot
 * GitHub Repository: https://github.com/JanHolbo/HerBot
 * (C) 2014 by Jan Rasmussen & Kaddu.dk jan at kaddu dot dk
 * This file and others in this pack is - where not otherwise stated - covered by the GPL v2 license
 * Please feel free to fork this project and please submit patches and feature requests to the above email 
 *
 * File: HerBot/HerBot.ino
 * Version: 
 *
 * This is the main HerBot file
 *
 */

 
// Use the Time Library (http://www.pjrc.com/teensy/td_libs_Time.html)
#include <Time.h>

// Number of pots to survey
const int pots = 3;

// Pin assignments
//rtc =
const int tempSensor = 8;
const int lightSensor = 9;
const int humiditySensor = 10;
const int moistureProbe[] = {4, 5, 6};



int logEntry = 0;     // index to log structure

const int logEntries = 1500;

// log data structure
time_t logTime[logEntries];      // 4 bytes
byte temp[logEntries];     // 1 byte signed
byte light[logEntries];     // 1 byte
byte humid[logEntries];     // 1 byte
byte moist[logEntries][pots];     // pots * 1 bytes

void setup()
{
}

void loop()
{
   logTime[logEntry] = now();      // use current time (in seconds since the Epoch) as time of log entry
   temp[logEntry] = readTemp();     // read the current ambient Temperature
   light[logEntry] = readLight();     // read the current level of light
   humid[logEntry] = readHumidity();     // read the relative humidity
   for (int i = 0; i <= pots; i++)     // for each pot
   {
      moist[logEntry][i] = (byte) readMoisture(moistureProbe[i]);     // read the moisture level
   }
   logEntry++;

                    // We want to log data every 5 minutes.
   delay(15*60*1000);     // as we cannot rely on having an RTC, we have to delay 5 minutes minus the time spent in logging the data.
}

int readTemp()          // Read the temperature probe
{
  
  return (0);
}

int readLight()          // Read the light probe
{
  
  return (0);
}

int readHumidity()       // Read the humidity probe
{
  
  return (0);
}

int readMoisture(int pin)          // Read a moisture probe
{
  
  return (0);
}



