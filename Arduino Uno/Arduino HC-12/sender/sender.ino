/*    Arduino Long Range Wireless Communication using HC-12
                      Example 01
   by Dejan Nedelkovski, www.HowToMechatronics.com
*/
#include <SoftwareSerial.h>


// HC12
SoftwareSerial HC12(2, 3); // HC-12 TX Pin, HC-12 RX Pin




void setup() {
  Serial.begin(9600);             // Serial port to computer

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  HC12.begin(9600);               // Serial port to HC12

  delay(100);
  HC12.print("helloworld");
}
void loop() {
  HC12.write("helloworld loop");
  while (HC12.available()) {        // If HC-12 has data
    Serial.write(HC12.read());      // Send the data to Serial monitor
  }
  while (Serial.available()) {      // If Serial monitor has data
    HC12.write(Serial.read());      // Send that data to HC-12
  }
}
