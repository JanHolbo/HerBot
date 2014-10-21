/* 
 * http://www.kaddu.dk/index.php/en/projects/herbot
 * GitHub Repository: https://github.com/JanHolbo/HerBot
 * (C) 2014 by Jan Rasmussen & Kaddu.dk jan at kaddu dot dk
 * This file and others in this pack is - where not otherwise stated - covered by the GPL v2 license
 * Please feel free to fork this project and please submit patches and feature requests to the above email 
 *
 * File: HerBot/tests/Time_test/Time_test.ino
 * Version: 
 *
 * This is a test-bed sketch to determine the accuracy of a non-RTC based clock on Arduino
 *
 * Based upon: TimeSerial.pde
 * example code illustrating Time library set through serial port messages.
 *
 * Messages consist of the letter T followed by ten digit time (as seconds since Jan 1 1970)
 * you can send the text on the next line using Serial Monitor to set the clock to noon Jan 1 2010
 T1262347200  
 *
 * A Processing example sketch to automatically send the messages is inclided in the download
 */ 
 
#include <Time.h>  

#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by unix time_t as ten ascii digits
#define TIME_HEADER  'T'   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 

void setup()  {
  Serial.begin(9600);
  Serial.println("Waiting for sync message");
}

void loop(){    


  if(Serial.available() ) 
  {
    char c = Serial.read() ; 

    if (c == TIME_REQUEST)
    {
      Serial.println ("T" + String(now()));
    } else if (c == TIME_HEADER)
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
    }  
  }


  delay(1000);
}

