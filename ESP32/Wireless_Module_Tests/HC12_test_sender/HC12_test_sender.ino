#include <ArduinoJson.h>

void setup()
{

  Serial.begin(115200); // Serial port to computer
  Serial2.begin(9600);  // Serial2 port to HC12
}

void loop()
{
  Serial2.write("{\"test\":123}\n");


  while (Serial2.available())
  { // If Serial monitor has data
    Serial2.write(Serial.read()); // Send that data to HC-12
  }
  delay(3000);
  // put your main code here, to run repeatedly:
}
