
#include "utils/HC12_func.h"

void setup() {
  Serial.begin(115200);             // Serial port to computer
  Serial.println("readdig");
  HC12.begin(9600);               // Serial port to HC12
}
void loop() {
  String hc12_data = "";

  //不要delay！
  while (HC12.available()) {        // If HC-12 has data
    hc12_data += (char) HC12.read();
  

    //    Serial.write(HC12.read());      // Send the data to Serial monitor
  }


  if (hc12_data.length() != 0) {
    Serial.print(hc12_data);
  }
}
