
#include <ArduinoJson.h>
#include <Wire.h>
#include "utils/wifi_utils_v2.h"
#include "utils/bmp280_utils.h"
#include "utils/dht_utils.h"

void setup()
{

  serial_setup();
  //  Wire.begin (D4, D5);   // sda= D2 /scl= D1                 ESP8266
  //  Wire.beginTransmission (118);
  wifi_connect();
  wifi_start_server();
}

void loop()
{

  read_BMP();
  DHT_read();

  delay(1500);
}

void serial_setup()
{
  Serial.begin(9600);
  Serial.setTimeout(2000);
  while (!Serial)
  {
  }
}
