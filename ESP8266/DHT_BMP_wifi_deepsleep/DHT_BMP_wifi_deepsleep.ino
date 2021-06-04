
#include <ArduinoJson.h>
#include "utils/wifi_utils_v2.h"
#include "utils/deep_sleep_utils.h"
#include "utils/mqtt_utils.h"
#include "utils/bmp280_utils.h"
#include "utils/dht_utils.h"

#include <Wire.h>



void setup() {

  serial_setup();
//  Wire.begin (D4, D5);   // sda= D2 /scl= D1                 ESP8266
//  Wire.beginTransmission (118); 
  wifi_connect();
//  wifi_start_server();
  connect_to_broker();

//    if (!bmp.begin(0x76)) {
//    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
//    while (1);
//  }
//    // delay(2000);
//    /* Default settings from datasheet. */
//    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
//                    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
//                    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
//                    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
//                    Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
                    
}

void loop() {

  read_BMP();
  DHT_read();
  
  delay(1500);
  deep_sleep_for_seconds(45);
  
}
