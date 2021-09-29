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

// #ifndef DHT_DATA_PIN
// #define DHT_DATA_PIN A0
// #endif

// #define DHT_DATA_PIN D3

uint8_t DHTPin = 0; //D3 = GPIO 0
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
    return json;
}

String DHT_read_temperature()
{

    float Temperature;

    DHT dht(DHTPin, DHTTYPE);
    dht.begin();

    Temperature = dht.readTemperature(); // Gets the values of the temperature

    // Posting JSON to the edge device
    DynamicJsonDocument doc(64);

    doc["DHT-temperature"] = dht.readTemperature();

    String json;
    serializeJson(doc, json);
    Serial.println(json);
    return json;
}

String DHT_read_humidity()
{
    float Temperature;

    DHT dht(DHTPin, DHTTYPE);
    dht.begin();

    Temperature = dht.readTemperature(); // Gets the values of the temperature

    // Posting JSON to the edge device
    DynamicJsonDocument doc(64);

    doc["DHT-humidity"] = dht.readHumidity();

    String json;
    serializeJson(doc, json);
    Serial.println(json);
    return json;
}
