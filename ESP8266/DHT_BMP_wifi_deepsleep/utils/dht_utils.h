/**
 * ESP8266 18650 Shield Version :
 * 
 * connect VCC to the 3.3V output
   Signal: D2
*/

#include "DHT.h"

#ifndef _ARDUINOJSON_H_
#define _ARDUINOJSON_H_
#include <ArduinoJson.h>
#endif

// Uncomment one of the lines below for whatever DHT sensor type you're using!
// #define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
#define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321

#define DHT_DATA_PIN D3

uint8_t DHTPin = DHT_DATA_PIN;
// Initialize DHT sensor.

String DHT_read()
{

  float Temperature;
  float Humidity;

  DHT dht(DHTPin, DHTTYPE);
  dht.begin();

  Temperature = dht.readTemperature(); // Gets the values of the temperature
  Humidity = dht.readHumidity();       // Gets the values of the humidity

  // Posting JSON to the edge device
  DynamicJsonDocument doc(128);

  doc["DHT-temperature"] = dht.readTemperature();
  doc["DHT-humidity"] = dht.readHumidity();

  String json;
  serializeJson(doc, json);
  Serial.println(json);

  // publish to barometer
  const char *topic = "WeMos_18650/temp_hum";
  client.publish(topic, &json[0]);
  return json;
}