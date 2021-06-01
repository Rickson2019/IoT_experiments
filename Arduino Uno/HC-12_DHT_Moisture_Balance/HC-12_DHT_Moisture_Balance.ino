#include <SoftwareSerial.h>
#include <ArduinoJson.h>

#include "DHT_func.h"
#include "HC12_func.h"

int PIN_MOISTURE_SENSOR = A0; //
int PIN_BALANCE_SENSOR = 12; //

void setup() {
  pinMode(PIN_BALANCE_SENSOR, INPUT);
  Serial.begin(9600);             // Serial port to computer

  HC12.begin(9600);               // Serial port to HC12
  delay(100);
  HC12.println("Transmission Started");
}
void loop() {

  
  float retval[2] = {0.00,0.00};
  
  DynamicJsonDocument doc(128);
  String json;

  
  DHT_read(retval);
  

  doc["DHT-temperature"] = retval[0];
  doc["DHT-humidity"] = retval[1];
  doc["Moisture-reading"] = analogRead(PIN_MOISTURE_SENSOR);
  doc["balanced"] = digitalRead(PIN_BALANCE_SENSOR);
  
  serializeJson(doc, json);
//  Serial.println(json);

  HC12.println(json);

  delay(1000);  
//  while (Serial.available()) {      // If Serial monitor has data
//    HC12.write(Serial.read());      // Send that data to HC-12
//  }

  while (HC12.available()) {        // If HC-12 has data
    Serial.write(HC12.read());      // Send the data to Serial monitor
  }

 }
