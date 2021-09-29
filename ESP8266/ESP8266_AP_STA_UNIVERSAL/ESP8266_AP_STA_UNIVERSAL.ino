// Originally by Rui Santos:
/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/
// Modified by Judao Zhong (JudaoZhong@gmail.com)

// Import required libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <Wire.h>
#include <ArduinoJson.h>

#include "utils/deepsleep_utils.h"

// -------------------- Network Settings --------------------
// Replace with credentials for your WiFi
// that is connected to the Internet(for STA mode)
// 接入互联网的WiFi名和密码(STA mode)
char *sta_ssid = "NO.1";
char *sta_password = "Home84039746";

// Replace with credentials for the WiFi AccessPoint
// that is running on your ESP8266
// 8266本机的AP名和密码(AP mode)
char* ap_ssid = "ESP8266_Weather_Station";
char* ap_password = "12345678";

// 获取原Wifi名字
// sta_ssid getter()
String get_sta_ssid() {
  return sta_ssid;
}

// 获取原密码
// sta_password getter()
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
// ____________________________________________________________


// ------------------ Power Saving Sleep Settings ---------------
// for deepsleep config
int deepsleep_minutes = 0;
int deepsleep_seconds = 0;

//
int get_deepsleep_minutes() {
  return deepsleep_minutes;
}

//
int get_deepsleep_seconds() {
  return deepsleep_seconds;
}

//
void set_deepsleep_minutes(int ds_minutes) {
  deepsleep_minutes = ds_minutes;
}

//
void set_deepsleep_seconds(int ds_seconds) {
  deepsleep_seconds = ds_seconds;
}
// ______________________________________________________________

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
  server.on("/config.js", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(SPIFFS, "/config.js", "script/javascript");
  });

  //_____________________________________________________________________________

  // ^^^^^^^^^^^^^^^^^^^^^^^^^^^ Webpage Files ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  // =========================== GPIO Manipulation ==============================


  // ^^^^^^^^^^^^^^^^^^^^^^^^^^^ GPIO Manipulation ^^^^^^^^^^^^^^^^^^^^^^^^^^^

  // =========================== DATA READ ENDPOINTS ==============================

  //  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest * request)
  //  {
  //    request->send_P(200, "text/plain", getTemperature().c_str());
  //  });
  // ^^^^^^^^^^^^^^^^^^^^^^^^^^^ DATA READ ENDPOINTS ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


  // =========================== SETUPs ===========================================
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
    }
    else
    {
      Serial.println("No body?!");
      request->send(200, "text/plain", "No body?!\n");
    }
  });

  // Change Deepsleep Status
  // 睡眠模式的时长设置
  server.on("/update_deepsleep", HTTP_POST, [](AsyncWebServerRequest * request) {
    Serial.println("updating deepsleep config");
    if (request->hasParam("body", true))
    {
      // This is important, otherwise the sketch will crash if there is no body
      String json_body = String(request->getParam("body", true)->value());
      Serial.println(json_body);

      DynamicJsonDocument doc(1024);
      deserializeJson(doc, &json_body[0]);

      const char* deepsleep_minutes_char_arr = doc["deepsleep_minutes"];
      const char* deepsleep_seconds_char_arr = doc["deepsleep_seconds"];

      deepsleep_minutes = atoi(deepsleep_minutes_char_arr);
      deepsleep_seconds = atoi(deepsleep_seconds_char_arr);
    }
  });

  // Change AP settings
  // 睡眠模式的时长设置
  server.on("/update_ap", HTTP_POST, [](AsyncWebServerRequest * request) {
    Serial.println("updating deepsleep config");
    if (request->hasParam("body", true))
    {
      // This is important, otherwise the sketch will crash if there is no body
      String json_body = String(request->getParam("body", true)->value());
      Serial.println(json_body);

      DynamicJsonDocument doc(1024);
      deserializeJson(doc, &json_body[0]);

      const char* ap_ssid_new = doc["ap_ssid"];
      const char* ap_password_new = doc["ap_password"];


      strcpy(ap_ssid , ap_ssid_new);
      strcpy(ap_password , ap_password_new);
      Serial.println(ap_ssid);
      Serial.println(ap_password);
    }
    
  });


  // =========================== SETUPs ===========================================


  // Start server
  server.begin();
}

void loop()
{
  // 检查是否连接，如果断开就重连
  // Check status and reconnect
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

  // 如果设置了deepsleep 那么就进入睡眠模式
  // If deepsleep is required
  // then we put it into the sleep mode
  if ((get_deepsleep_minutes() != 0)
      && ( get_deepsleep_seconds() != 0 )
     )
  {
    deepsleep_for_minutes(get_deepsleep_minutes());
    deepsleep_for_seconds(get_deepsleep_seconds());
  }

}
