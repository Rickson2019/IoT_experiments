/*    Arduino Long Range Wireless Communication using HC-12
                      Example 01
   by Dejan Nedelkovski, www.HowToMechatronics.com
*/
#include <SoftwareSerial.h>


SoftwareSerial HC12(D8, D9); // [0]:The pin HC-12 TX is connected to, [1]:The pin HC-12 RX is connected to

#include "wifi_utils.h" // wifi funcs


void setup() {

  Serial.begin(115200);             // Serial port to computer
  Serial.println("setup");

  HC12.begin(115200);               // Serial port to HC12
  Serial.println("Started");
  ESP.eraseConfig();
  wifi_connect();
}


String post_data = "";
int flag = 0;

void loop() {

  String hc12_data = "";
  while (HC12.available()) {        // If HC-12 has data
    char next_char = (char) HC12.read();
    hc12_data +=  next_char ;
    if (next_char == '{') {
      flag = 1;
      
    }
    else if (next_char == '}') {
      flag = 2;
    }

    //    Serial.write(HC12.read());      // Send the data to Serial monitor
  }
  // <<<<< never ever do delay() here!

  if (hc12_data.length() != 0) {
    Serial.print(hc12_data);

    if (flag != 0) {
      post_data += hc12_data;
    }

    Serial.println();
    Serial.println(post_data);
  }


  if (flag == 2)
  {
//    http_post(post_data, "http://192.168.2.2:3000/outdoor_1");
    post_data = "";
    flag = 0;
  }








  //  while (Serial.available()) {      // If Serial monitor has data
  //    HC12.write(Serial.read());      // Send that data to HC-12
  //  }
}
