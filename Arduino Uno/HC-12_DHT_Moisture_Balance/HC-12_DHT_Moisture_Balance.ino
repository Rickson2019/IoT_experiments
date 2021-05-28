#include <SoftwareSerial.h>
#include <ArduinoJson.h>

#include "DHT_func.h"
#include "HC12_func.h"

int PIN_MOISTURE_SENSOR = A0; //

void setup() {
  
  Serial.begin(9600);             // Serial port to computer

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  HC12.begin(9600);               // Serial port to HC12
  HC12.write("Testing, testing...123");
}
void loop() {
  float retval[2] = {0.00,0.00};
  
  DynamicJsonDocument doc(128);
  String json;

  
  DHT_read(retval);
  

  doc["DHT-temperature"] = retval[0];
  doc["DHT-humidity"] = retval[1];
  serializeJson(doc, json);
//  Serial.println(json);
  Serial.println(json);
  writeString(json);
    
//  while (Serial.available()) {      // If Serial monitor has data
//    HC12.write(Serial.read());      // Send that data to HC-12
//  }

  while (HC12.available()) {        // If HC-12 has data
    Serial.write(HC12.read());      // Send the data to Serial monitor
  }

  
//  // Finally reads and writes data
//  if (HC12.available()) {        // If HC-12 has data
//    HC12_write();      // Send the data to Serial monitor
//  }
//
//  if (Serial.available()) {      // If Serial monitor has data
//    HC12_read();      // Send that data to HC-12
//  }

}

void writeString(String stringData) { // Used to serially push out a String with Serial.write()

  for (int i = 0; i < stringData.length(); i++)
  {
    Serial.write(stringData[i]);   // Push each char 1 by 1 on each loop pass
    HC12.write(stringData[i]);   // Push each char 1 by 1 on each loop pass
  }

}// end writeString
