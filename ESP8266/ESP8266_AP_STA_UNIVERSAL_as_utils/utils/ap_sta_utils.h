
/*
  Referred to Web Server tutorial by Rui Santos at https://RandomNerdTutorials.com
  Modified by Judao Zhong (JudaoZhong@gmail.com)
*/

// Import required libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <Wire.h>
#include <ArduinoJson.h>

#include "utils/important_attributes.h" // always comes first  重要设置，放在首位
#include "utils/deepsleep_utils.h"

// true if you have connected the sensor, false if you don't have those
#define DHT22 true
#define BMP280 false
#define CCS811 true

#ifdef DHT22
#include "utils/dht_utils.h" // DHT 温湿度传感器
#endif

#ifdef BMP280
#include "utils/bmp_utils.h"
#endif

#ifdef CCS811
#include "utils/ccs811_utils.h"
#endif

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

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
    if (var == "DHT_TEMPERATURE")
    {
        return DHT_read_temperature();
    }
    else if (var == "DHT_HUMIDITY")
    {
        return DHT_read_humidity();
    }
    // else if (var == "PRESSURE")
    // {
    //     return getPressure();
    // }
}

void setup_ap_sta()
{
    WiFi.mode(WIFI_AP_STA);
    // Serial port for debugging purposes
    Serial.begin(115200);

    // Initialize SPIFFS
    if (!SPIFFS.begin())
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    // Begin Access Point
    WiFi.softAP(ap_ssid, ap_password);
    Serial.println("softAP IP:");
    Serial.print(WiFi.softAPIP());
    Serial.println("AP_SSID");
    Serial.println(ap_ssid);
    Serial.println("AP_PASSWORD");
    Serial.println(ap_password);

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
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  //    request->send(SPIFFS, "/index.html", String(), false, processor);
                  request->send(SPIFFS, "/index.html", "text/html");
              });

    // Route to load index.css file
    server.on("/index.css", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/index.css", "text/css"); });

    // ___________________________________________________________________________

    // 设置页 的地址 Route to configuration
    server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/config.html", "text/html"); });

    // 设置页 的样式文件 Route to config.css
    server.on("/config.css", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/config.css", "text/css"); });

    // 设置页 的javascript文件 Route to config.js
    server.on("/config.js", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/config.js", "script/javascript"); });

    //_____________________________________________________________________________

    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^ Webpage Files ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    // =========================== DATA READ ENDPOINTS ==============================

#if DHT22
    server.on("/DHT", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/plain", DHT_read().c_str()); });

    server.on("/DHT-temperature", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/plain", DHT_read_temperature().c_str()); });

    server.on("/DHT-humidity", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/plain", DHT_read_temperature().c_str()); });
#endif

#if BMP280

#endif

    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^ DATA READ ENDPOINTS ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    // =========================== SETUPs ===========================================
    // 获取WiFi的SSID
    // Send WiFi SSID to the front end
    server.on("/sta_ssid", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/plain", get_sta_ssid().c_str()); });

    // 获取WiFi的密码
    // Send WiFi password to the front end
    server.on("/sta_password", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/plain", get_sta_password().c_str()); });

    // 更换要连接的WiFi SSID
    // Modify the WiFi SSID to the one you want to connect to.
    // https://github.com/me-no-dev/ESPAsyncWebServer/issues/195
    server.on("/update_sta", HTTP_POST, [](AsyncWebServerRequest *request)
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

                      const char *sta_ssid_new = doc["sta_ssid"];
                      const char *sta_password_new = doc["sta_password"];

                      strcpy(sta_ssid, sta_ssid_new);
                      strcpy(sta_password, sta_password_new);

                      Serial.println(sta_ssid_new);
                      Serial.println(sta_password_new);
                      //      request->send(200, "text/plain", request->getParam("body", true)->value());

                      WiFi.disconnect();
                  }
                  else
                  {
                      Serial.println("No body?!");
                      request->send(200, "text/plain", "No body?!\n");
                  }
              });

    // Change Deepsleep Status
    // 睡眠模式的时长设置
    server.on("/update_deepsleep", HTTP_POST, [](AsyncWebServerRequest *request)
              {
                  Serial.println("updating deepsleep config");
                  if (request->hasParam("body", true))
                  {
                      // This is important, otherwise the sketch will crash if there is no body
                      String json_body = String(request->getParam("body", true)->value());
                      Serial.println(json_body);

                      DynamicJsonDocument doc(1024);
                      deserializeJson(doc, &json_body[0]);

                      const char *deepsleep_minutes_char_arr = doc["deepsleep_minutes"];
                      const char *deepsleep_seconds_char_arr = doc["deepsleep_seconds"];

                      deepsleep_minutes = atoi(deepsleep_minutes_char_arr);
                      deepsleep_seconds = atoi(deepsleep_seconds_char_arr);
                  }
              });

    // Change AP settings
    // 睡眠模式的时长设置
    server.on("/update_ap", HTTP_POST, [](AsyncWebServerRequest *request)
              {
                  Serial.println("updating deepsleep config");
                  if (request->hasParam("body", true))
                  {
                      // This is important, otherwise the sketch will crash if there is no body
                      String json_body = String(request->getParam("body", true)->value());
                      Serial.println(json_body);

                      DynamicJsonDocument doc(1024);
                      deserializeJson(doc, &json_body[0]);

                      const char *ap_ssid_new = doc["ap_ssid"];
                      const char *ap_password_new = doc["ap_password"];

                      strcpy(ap_ssid, ap_ssid_new);
                      strcpy(ap_password, ap_password_new);
                      Serial.println(ap_ssid);
                      Serial.println(ap_password);
                  }
              });

    // =========================== SETUPs ===========================================

    // Start server
    server.begin();
}

void sta_reconnect()
{
    // 检查是否连接，如果断开就重连
    // Check status and reconnect
    if (WiFi.status() != WL_CONNECTED)
    {
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