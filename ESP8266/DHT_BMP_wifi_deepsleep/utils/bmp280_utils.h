#include <Adafruit_BMP280.h>
Adafruit_BMP280 bmp; // I2C

#ifndef _ARDUINOJSON_H_
#define _ARDUINOJSON_H_
#include <ArduinoJson.h>
#endif

String read_BMP()
{
    Serial.println(F("BMP280:"));

    bmp.begin(0x76);
    // delay(2000);
    /* Default settings from datasheet. */
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                    Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

    float temp = bmp.readTemperature();
    float press = bmp.readPressure();

    // Posting JSON to the edge device
    DynamicJsonDocument doc(128);

    doc["BMP280-temperature"] = temp;
    doc["BMP280-pressure"] = press;

    String json;
    serializeJson(doc, json);

    Serial.println(F("BMP280:"));
    Serial.println(json);

    // publish to barometer
    const char *topic = "WeMos_18650/barometer";
    client.publish(topic, &json[0]);
    return json;
}