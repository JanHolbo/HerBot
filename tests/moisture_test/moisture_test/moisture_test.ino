/* 
 * http://www.kaddu.dk/index.php/en/projects/herbot
 * GitHub Repository: https://github.com/JanHolbo/HerBot
 * (C) 2014 by Jan Rasmussen & Kaddu.dk jan at kaddu dot dk
 * This file and others in this pack is - where not otherwise stated - covered by the GPL v2 license
 * Please feel free to fork this project and please submit patches and feature requests to the above email 
 *
 * File: HerBot/tests/moisture_test/moisture_test.ino
 * Version: 
 *
 * This is a test-bed sketch to determine how to read the YL-100 moisture probe
 *
 */ 
 
void setup()  
{
  Serial.begin(9600);
  Serial.println("moisture_test - reading moisture probe");
}

void loop()
{    
  long moisture = analogRead (A0);
  Serial.println("probe 1 : " + String (moisture) + " probe 1 >> 2 : " + String ((moisture >> 2)) + " (probe 1 >> 2)) << 2 : " + String ((moisture >> 2) << 2));

  delay(1000);
}

