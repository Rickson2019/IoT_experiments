/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

// Modified by JudaoZhong

// Import required libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include <ArduinoJson.h>

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

// Replace with your network credentials
char *sta_ssid = "NO.1";
char *sta_password = "Home84039746";

char* ap_ssid = "ESP8266_Weather_Station";
char* ap_password = "12345678";


// Set LED GPIO
const int ledPin = 2;
// Stores LED state
String ledState;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String getTemperature()
{
  float temperature = bme.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float temperature = 1.8 * bme.readTemperature() + 32;
  Serial.println(temperature);
  return String(temperature);
}

String getHumidity()
{
  float humidity = bme.readHumidity();
  Serial.println(humidity);
  return String(humidity);
}

String getPressure()
{
  float pressure = bme.readPressure() / 100.0F;
  Serial.println(pressure);
  return String(pressure);
}

// 获取原Wifi名字
String get_sta_ssid() {
  return sta_ssid;
}

// 获取原密码
String get_sta_password()
{
  return sta_ssid;
}

// 设置要连接的WiFi名 New WiFi SSID you want to connect to
// 和 and
// 设置新的密码 New password for that WiFi
void set_sta_ssid_password(String new_sta_ssid, String new_sta_password)
{
  sta_ssid = &new_sta_ssid[0];
  sta_password = &new_sta_password[0];
}

// Replaces placeholder with LED state value
String processor(const String &var)
{
  Serial.println(var);
  //  if (var == "STATE")
  //  {
  //    if (digitalRead(ledPin))
  //    {
  //      ledState = "ON";
  //    }
  //    else
  //    {
  //      ledState = "OFF";
  //    }
  //    Serial.print(ledState);
  //    return ledState;
  //  }
  //  else if (var == "TEMPERATURE")
  //  {
  //    return getTemperature();
  //  }
  //  else if (var == "HUMIDITY")
  //  {
  //    return getHumidity();
  //  }
  //  else if (var == "PRESSURE")
  //  {
  //    return getPressure();
  //  }
}

void setup()
{
  WiFi.mode(WIFI_AP_STA);
  // Serial port for debugging purposes
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  //  // Initialize the sensor
  //  if (!bme.begin(0x76))
  //  {
  //    Serial.println("Could not find a valid BME280 sensor, check wiring!");
  //    while (1)
  //      ;
  //  }

  // Initialize SPIFFS
  if (!SPIFFS.begin())
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }


  //  // Begin Access Point
  //  WiFi.softAP(ap_ssid, ap_password);
  //  Serial.println("softAP IP:");
  //  Serial.print(WiFi.softAPIP());
  //  Serial.println("AP_SSID");
  //  Serial.println(ap_ssid);
  //  Serial.println("AP_PASSWORD");
  //  Serial.println(ap_password);


  // Connect to Wi-Fi
  WiFi.begin(sta_ssid, sta_password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // =========================== Webpage Files =================================

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    //    request->send(SPIFFS, "/index.html", String(), false, processor);
    request->send(SPIFFS, "/index.html", "text/html");

  });

  // Route to load index.css file
  server.on("/index.css", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(SPIFFS, "/index.css", "text/css");
  });

  // ___________________________________________________________________________

  // 设置页 的地址 Route to configuration
  server.on("/config", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(SPIFFS, "/config.html", "text/html");
  });

  // 设置页 的样式文件 Route to config.css
  server.on("/config.css", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(SPIFFS, "/config.css", "text/css");
  });

  // 设置页 的javascript文件 Route to config.js
  server.on("/config.css", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(SPIFFS, "/config.js", "script/javascript");
  });

  // ^^^^^^^^^^^^^^^^^^^^^^^^^^^ Webpage Files ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  // =========================== GPIO Manipulation ==============================

  // Route to set GPIO to HIGH
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    digitalWrite(ledPin, HIGH);
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Route to set GPIO to LOW
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    digitalWrite(ledPin, LOW);
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // ^^^^^^^^^^^^^^^^^^^^^^^^^^^ GPIO Manipulation ^^^^^^^^^^^^^^^^^^^^^^^^^^^

  // =========================== DATA READ ENDPOINTS ==============================

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send_P(200, "text/plain", getTemperature().c_str());
  });

  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send_P(200, "text/plain", getHumidity().c_str());
  });

  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send_P(200, "text/plain", getPressure().c_str());
  });

  // ^^^^^^^^^^^^^^^^^^^^^^^^^^^ DATA READ ENDPOINTS ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  // 获取WiFi的SSID
  // Send WiFi SSID to the front end
  server.on("/sta_ssid", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send_P(200, "text/plain", get_sta_ssid().c_str());
  });

  // 获取WiFi的密码
  // Send WiFi password to the front end
  server.on("/sta_password", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send_P(200, "text/plain", get_sta_password().c_str());
  });

  // 更换要连接的WiFi SSID
  // Modify the WiFi SSID to the one you want to connect to.
  // https://github.com/me-no-dev/ESPAsyncWebServer/issues/195
  server.on("/update_sta", HTTP_POST, [](AsyncWebServerRequest * request)
  {
    // Count number of params
    int params = request->params(); // 0
    Serial.println(params);
    if (request->hasParam("body", true))
    { // This is important, otherwise the sketch will crash if there is no body
      String json_body = String(request->getParam("body", true)->value());
      Serial.println(json_body);
      //      set_sta_ssid_password(ssid, pass);

      DynamicJsonDocument doc(1024);
      deserializeJson(doc, &json_body[0]);

      const char* sta_ssid_new = doc["sta_ssid"];
      const char* sta_password_new = doc["sta_password"];

      strcpy(sta_ssid , sta_ssid_new);
      strcpy(sta_password , sta_password_new);
      
      Serial.println(sta_ssid_new);
      Serial.println(sta_password_new);
      //      request->send(200, "text/plain", request->getParam("body", true)->value());


      WiFi.disconnect();
      ////      WiFi.mode(WIFI_OFF);
      //      delay(4000);
      //
      //      Serial.println("disconencted");
      ////      WiFi.mode(WIFI_AP_STA);
      ////
      //
      //      // Print ESP32 Local IP Address
      //      Serial.println(WiFi.localIP());
      //      // Start server
      //      server.begin();

    }
    else
    {
      Serial.println("No body?!");
      request->send(200, "text/plain", "No body?!\n");
    }
  });

  // Start server
  server.begin();
}

void loop()
{
  if (WiFi.status() !=  WL_CONNECTED) {
    WiFi.begin(sta_ssid, sta_password);

    while (WiFi.status() != WL_CONNECTED)
    {
      delay(1000);
      Serial.println("Connecting to WiFi..");
    }
    
    // Print ESP32 Local IP Address
    Serial.println(WiFi.localIP());
    // Start server
    server.begin();

  }
}
