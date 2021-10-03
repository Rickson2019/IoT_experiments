/*    Arduino Long Range Wireless Communication using HC-12
                      Example 01
   by Dejan Nedelkovski, www.HowToMechatronics.com
*/
#include <SoftwareSerial.h>


SoftwareSerial HC12(2, 3); // HC-12 TX Pin, HC-12 RX Pin
//Serial2

void setup() {
  Serial.begin(9600);             // Serial port to computer
  HC12.begin(9600);               // Serial port to HC12
  
  Serial1.begin(9600); //This is the UART, pipes to sensors attached to board

  Serial.println("started!");
}


int received_vals[2] = {0, 0};

void loop() {
  Serial.println("loop");
  HC12.flush();
  
  if (HC12.available()) {        // If HC-12 has data
    char temp = HC12.read();
    if (temp == 'L') {
      Serial.println("L");
      byte temp0 = HC12.read() ;
      Serial.println((int)temp0);
      //      received_vals[0] = temp0 ;
    }
    if (temp == 'R') {
      Serial.println("R");
      byte temp1 = HC12.read() ;
      Serial.println((int)temp1);
      //      received_vals[1] = temp;
    }
    //    Serial.println((int)HC12.read());
  }

  //  Serial.println("L:");
  //  Serial.println(received_vals[0]);      // Send the data to Serial monitor  }
  //
  //  Serial.println("R:");
  //  Serial.println(received_vals[1]);      // Send the data to Serial monitor  }


  HC12.flush();
//    delay(50);
  
  while (Serial.available()) {      // If Serial monitor has data
    HC12.write(Serial.read());      // Send that data to HC-12
  }
}
