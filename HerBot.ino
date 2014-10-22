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
#define LOG_PULL_REQUEST 'L'  // Header tag for requesting log contents 

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

bool timeSynced = false;

int logEntryNo = 0;     // index to log structure
int logMultiplexer = 0;          // as we are listening for requests on the 
const int logThreshold = 5*60;     // serial line, we need to check this more 
                                 // often than we log. Thus we can only delay() 
                                 // for a short while (1000 ms = 1 s). Logging is 
                                 // then done every nth time that the loop() 
                                 // iteration runs.

// log data structure
struct logEntryType {
  time_t logTime;      // 4 bytes
  byte temperature;    // 1 byte signed
  byte light;          // 1 byte
  byte humidity;       // 1 byte
  byte moisture[pots]; // pots * 1 bytes
};
const int logEntries = 125;
logEntryType logEntry[logEntries];

void setup()
{
  Serial.begin(9600);
  Serial.println(versionHeader);
  Serial.println(F("Waiting for TimeSync ..."));

  pinMode(warningLogFullLED, OUTPUT);

  dht.begin();
}

int readLight()          // Read the light probe
{
  
  return (0);
}

int readMoisture(int pin)          // Read a moisture probe
{
  
  return (0);
}


void timeSync()
{
  char c;
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
}


void sendLog ()
{
  Serial.println ("E" + String(logEntryNo));
  Serial.println ("T" + String(now()));
    
  for (int i=0; i < logEntryNo; i++)
  {
    Serial.print (String(logEntry[i].logTime) + ",");
    Serial.print (String(logEntry[i].temperature) + ",");
    Serial.print (String(logEntry[i].light) + ",");
    Serial.print (String(logEntry[i].humidity) + ",");
    for (int j = 0; j < pots; j++) Serial.print (String(logEntry[i].moisture[j]) + ",");
  }
}

void handleInput()
{
  char c = Serial.read(); 

  switch (c)
  {
    case 'T':
      timeSync();
      break;
    case 'L':
      sendLog();
      break;
    default:
      String inputString = "";
      while (Serial.available() && c != '\n')
      {
        inputString = inputString + c;
        c = Serial.read();
      }
      inputString = inputString + c;
      Serial.println("!Unknown input: " + inputString);
  }
}


boolean logStorageSpace()
{
  if (logEntryNo >= logEntries)
  {
    Serial.println(F("!Warning! Log storage is FULL!!!!!"));
    digitalWrite (warningLogFullLED, HIGH);
    return false;
  } else if (logEntryNo >= (logEntries*9/10))
  {
    Serial.println(F("!Warning! Log storage is more than 90% full"));
    if (logMultiplexer % 2)
    {
      digitalWrite (warningLogFullLED, LOW);

    } else
    {
      digitalWrite (warningLogFullLED, HIGH);
    }
  } else if (logEntryNo >= (logEntries*3/4))
  {
    if (logMultiplexer % 4)
    {
      digitalWrite (warningLogFullLED, LOW);

    } else
    {
      Serial.println(F("!Warning! Log storage is more than 75% full"));
      digitalWrite (warningLogFullLED, HIGH);
    }
  }
  return true;
}


void logData()
{
  logEntry[logEntryNo].logTime = now();
  logEntry[logEntryNo].temperature = int (dht.readTemperature());
  logEntry[logEntryNo].humidity = int (dht.readHumidity());
}


void loop()
{
  if (Serial.available())
  {
    handleInput();
  }

  if (logStorageSpace())
  {
    if (logMultiplexer >= logThreshold)
    {
      logData();
      logMultiplexer = 0;
    }
  
  }

  logMultiplexer++;
  delay(1000);
}


