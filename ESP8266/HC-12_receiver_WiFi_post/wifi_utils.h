//#include <WiFi.h>
//#include <HTTPClient.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

//#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

/*Put your SSID & Password*/
const char *ssid = "atoi()_Wi-Fi5";         // Enter SSID here
const char *password = "33184699"; //Enter Password here

ESP8266WiFiMulti wifiMulti;           // 建立ESP8266WiFiMulti
WiFiClient wifiClient;
HTTPClient http;

void wifi_connect(){
  wifiMulti.addAP(ssid, password); 

  Serial.println("in wifi_connect()");
  WiFi.mode(WIFI_STA);    // Set to station mode
//  WiFi.persistent(false);

   
  while (wifiMulti.run() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting..");
   }
   Serial.println("connected!");
   
}

void http_post(String content, String uri)
{

  // String endpoint = HOST_ADDRESS + "/DHT";
  http.begin(wifiClient, uri);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Node-Name", "ESP32-Bedroom");
  http.POST(content);
  http.end();
}
