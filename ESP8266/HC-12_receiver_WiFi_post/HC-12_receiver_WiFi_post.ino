/*    Arduino Long Range Wireless Communication using HC-12
                      Example 01
   by Dejan Nedelkovski, www.HowToMechatronics.com
*/
#include <SoftwareSerial.h>


SoftwareSerial HC12(D8, D9); // [0]:The pin HC-12 TX is connected to, [1]:The pin HC-12 RX is connected to

#include "wifi_utils.h" // wifi funcs


void setup() {
  Serial.println("setup");
  Serial.begin(9600);             // Serial port to computer
  HC12.begin(9600);               // Serial port to HC12
  ESP.eraseConfig(); 
  wifi_connect();
}
void loop() {
  String hc12_data = "";
   
  while (HC12.available()) {        // If HC-12 has data
    hc12_data += (char) HC12.read();
//    Serial.write(HC12.read());      // Send the data to Serial monitor
  }
  delay(100);
  
  if(hc12_data.length() !=0){
    Serial.println(hc12_data);
  }

  http_post(hc12_data, "http://192.168.2.2:3000/outdoor_1");
  
  
  while (Serial.available()) {      // If Serial monitor has data
    HC12.write(Serial.read());      // Send that data to HC-12
  }
}
