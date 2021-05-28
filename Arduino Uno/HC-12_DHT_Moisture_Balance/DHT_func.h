//============ TEMPERATURE SENSORS ===============
/** REFERENCE:
    https://www.youtube.com/watch?v=ebJpXs2-WgM */

#include "DHT.h"

// #include <ArduinoJson.h>

// Uncomment one of the lines below for whatever DHT sensor type you're using!
#define DHTTYPE DHT11 // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

#define DHT_DATA_PIN 13 // GPIO13

// DHT Sensor
uint8_t DHTPin = DHT_DATA_PIN;
// Initialize DHT sensor.

float Temperature;
float Humidity;

/**
   @params
*/
void DHT_read(float *retval)
{
  //  /**Let D12 be the 3.3V output
  //     which powers up the DHT sensor:
  //     Signal: D13
  //     VCC   : D12
  //     GND   : GND
  //  */
  //s
  //  pinMode(12, OUTPUT);
  //  digitalWrite(12, HIGH);

  DHT dht(DHTPin, DHTTYPE);
  dht.begin();

  Temperature = dht.readTemperature(); // Gets the values of the temperature
  Humidity = dht.readHumidity();       // Gets the values of the humidity

  // Posting JSON to the edge device
  DynamicJsonDocument doc(2048);

  doc["DHT-temperature"] = dht.readTemperature();
  doc["DHT-humidity"] = dht.readHumidity();

  String json;
  serializeJson(doc, json);

//  Serial.flush();

  retval[0] = Temperature;
  retval[1] = Humidity;
  delay(1000);
};
//^^^^^^^^^^ TEMPERATURE SENSORS ^^^^^^^^^^^^^
