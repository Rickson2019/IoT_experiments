
#include <ESP8266WiFi.h>

const int sensorPin = 2; // D4
const int sensorAnaPin = A0; //A0
int sensorState = 0;
int lastState = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
}

void loop()
{ 


  sensorState = digitalRead(sensorPin);
   Serial.println("sensorState");
  Serial.println(sensorState);
  Serial.println(analogRead(sensorAnaPin));

if (sensorState == 1 && lastState == 0) {
  Serial.println("needs water, send notification");

  lastState = 1;
  delay(1000);
//send notification
    
  } 
  else if (sensorState == 1 && lastState == 1) {
    //do nothing, has not been watered yet
  Serial.println("has not been watered yet");
  delay(1000);
  }
  else {
    //st
    Serial.println("does not need water");
    lastState = 0;
    delay(1000);
  }
  
  delay(100);
}
