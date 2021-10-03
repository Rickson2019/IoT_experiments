/*    Arduino Long Range Wireless Communication using HC-12
                      Example 01
   by Dejan Nedelkovski, www.HowToMechatronics.com
*/

 
void setup() {
  Serial.begin(9600);             // Serial port to computer
  
  Serial1.begin(9600); //This is the UART, pipes to sensors attached to board

  Serial.println("started!");
}


int received_vals[2] = {0, 0};

void loop() {
//  Serial.println("loop");
  Serial1.flush();
  
  if (Serial1.available()) {        // If HC-12 has data
    char temp = Serial1.read();
    if (temp == 'L') {
      Serial.println("L");
      byte temp0 = Serial1.read() ;
      Serial.println((int)temp0);
      //      received_vals[0] = temp0 ;
    }
    if (temp == 'R') {
      Serial.println("R");
      byte temp1 = Serial1.read() ;
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


  Serial1.flush();
//    delay(50);
  
  while (Serial.available()) {      // If Serial monitor has data
    Serial1.write(Serial.read());      // Send that data to HC-12
  }
}
