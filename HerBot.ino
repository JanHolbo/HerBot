/* 
 * HerBot
 * http://www.kaddu.dk/index.php/en/projects/herbot
 * GitHub Repository: https://github.com/JanHolbo/HerBot
 * (C) 2014 by Jan Rasmussen & Kaddu.dk jan at kaddu dot dk
 * This file and others in this pack is - where not otherwise stated - covered by the GPL v2 license
 * Please feel free to fork this project and please submit patches and feature requests to the above email 
 *
 * File: HerBot/HerBot.ino
 * Version: 0.1-alpha
 *
 * This is the main HerBot file
 *
 */

char versionHeader[] = "HerBot v 0.1-alpha";

// Use the Time Library (http://www.pjrc.com/teensy/td_libs_Time.html)
#include <Time.h>
#include <DHT.h>


#define TIME_MSG_LEN  11    // time sync to PC is HEADER followed by unix time_t as ten ascii digits
#define TIME_HEADER  'T'    // Header tag for serial time sync message
#define LOG_PULL_REQUEST  7 // ASCII bell character requests the log data 

// Number of pots to survey
const int pots = 3;

// Pin assignments
const int warningLogFullLED = 13;
//rtc =
const int tempSensor = 8;
const int lightSensor = 9;
const int humiditySensor = 10;
const int moistureProbe[] = {4, 5, 6};

#define DHTPIN 2 
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);


int logEntry = 0;     // index to log structure
int logMultiplexer = 0;          // as we are listening for requests on the 
const int logThreshold = 5*60;     // serial line, we need to check this more 
                                 // often than we log. Thus we can only delay() 
                                 // for a short while (1000 ms = 1 s). Logging is 
                                 // then done every nth time that the loop() 
                                 // iteration runs.

const int logEntries = 125;

// log data structure
time_t logTime[logEntries];      // 4 bytes
byte temp[logEntries];           // 1 byte signed
byte light[logEntries];          // 1 byte
byte humid[logEntries];          // 1 byte
byte moist[logEntries][pots];    // pots * 1 bytes

void setup()
{
  Serial.begin(9600);
  Serial.println(versionHeader);
  Serial.println(F("Waiting for TimeSync ..."));

  pinMode(warningLogFullLED, OUTPUT);

  dht.begin();
}

void loop()
{
  if (Serial.available())
  {
    char c = Serial.read() ; 

    if (c == TIME_HEADER)
    {
      time_t pctime = 0;
      for(int i=0; i < TIME_MSG_LEN -1; i++)
      {   
        c = Serial.read();          
        if( c >= '0' && c <= '9')
        {   
            pctime = (10 * pctime) + (c - '0') ; // convert digits to a number    
        }
      }
      setTime(pctime);   // Sync Arduino clock to the time received on the serial port
      Serial.println ("T" + String(now()) + " (Synced)");
    } else if (c == LOG_PULL_REQUEST)
    {
      Serial.println ("E" + String(logEntry));
      
      for (int i=0; i < logEntry; i++)
      {
        Serial.print (String(logTime[i]) + "," + String(temp[i]) + ",");
        Serial.print (String(light[i]) + "," + String(humid[i]));
        for (int j=0; j < pots; j++)
        {
          Serial.print ("," + String(moist[i][j]));
        }
        Serial.println (",");
      }
    } 

  }

  if (logEntry >= logEntries)
  {
    Serial.println("Warning! Log storage is FULL!!!!!");
    digitalWrite (warningLogFullLED, HIGH);
  } else if (logEntry >= (logEntries*9/10))
  {
    Serial.println("Warning! Log storage is more than 90% full");
    if (logMultiplexer % 2)
    {
      digitalWrite (warningLogFullLED, LOW);

    } else
    {
      digitalWrite (warningLogFullLED, HIGH);
    }
  } else if (logEntry >= (logEntries*3/4))
  {
    if (logMultiplexer % 4)
    {
      digitalWrite (warningLogFullLED, LOW);

    } else
    {
      Serial.println("Warning! Log storage is more than 75% full");
      digitalWrite (warningLogFullLED, HIGH);
    }
  }


  
  if ((logMultiplexer >= logThreshold) && (logEntry <= logEntries))
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
    logMultiplexer = 0;
  }
  
  logMultiplexer++;
  
  delay(1000);
}

int readTemp()          // Read the temperature probe
{
  float t = dht.readTemperature();

  return (int(t));
}

int readLight()          // Read the light probe
{
  
  return (0);
}

int readHumidity()       // Read the humidity probe
{
  float h = dht.readHumidity();
  
  return (int(h));
}

int readMoisture(int pin)          // Read a moisture probe
{
  
  return (0);
}



