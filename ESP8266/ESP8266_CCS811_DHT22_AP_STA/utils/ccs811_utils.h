#include "Adafruit_CCS811.h"

#ifndef _ARDUINOJSON_H_ // preventing repeated imports
#define _ARDUINOJSON_H_
#include <ArduinoJson.h>
#endif

Adafruit_CCS811 ccs811;

// try heating up 10 times
void heat_up_ccs()
{

    for (int i = 0; i < 10; i++)
    {
        if (!ccs811.begin())
        {
            Serial.println("Failed to start sensor! Please check your wiring.");
            delay(100);
        }
    }
}

String CCS811_read()
{

    float CO2;  //ppm
    float TVOC; //ppm

    CO2 = ccs811.geteCO2();  // Gets the values of the temperature
    TVOC = ccs811.getTVOC(); // Gets the values of the humidity

    // Posting JSON to the edge device
    DynamicJsonDocument doc(128);

    doc["CCS811-CO2"] = CO2;
    doc["CCSS811-TVOC"] = TVOC;

    String json;
    serializeJson(doc, json);
    Serial.println(json);
    return json;
}
