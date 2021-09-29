
#include <ESP8266WiFi.h>

//REF: https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/station-examples.html
//参考： http://www.taichi-maker.com/homepage/iot-development/iot-dev-reference/esp8266-c-plus-plus-reference/esp8266wifimulti/addap/
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

// =================== AS A STATION ==========================

/*Put your SSID & Password*/
// DEFAULT ACCESS POINT 默认热点
const char *DEFAULT_SSID = "atoi()_Wi-Fi5"; // Enter SSID here
const char *DEFAULT_PASSWORD = "33184699";  //Enter Password here

// BACKUP SETTINGS 备用网络
const char *BACKUP_SSID = "DOG";
const char *BACKUP_PASSWORD = "33184699"; //Enter Password here

const int DEFAULT_SERVER_PORT = 80;

ESP8266WiFiMulti wifiMulti;

WiFiClient wifiClient; //  Client 客户端
HTTPClient http;       //  Does HTTP requests 用于进行HTTP访问

/**
 * Connects to wifi
 * 连接wifi
*/
void wifi_connect()
{
    wifiMulti.addAP(DEFAULT_SSID, DEFAULT_PASSWORD);
    wifiMulti.addAP(BACKUP_SSID, BACKUP_PASSWORD);

    Serial.println("in wifi_connect()");
    WiFi.mode(WIFI_STA); // Set to station mode
                         //  WiFi.persistent(false);

    Serial.println("Connecting..");
    while (wifiMulti.run() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
    }
    if (wifiMulti.run() == WL_CONNECTED)
    {
        Serial.println("Connection established!");
        Serial.print("IP address:");
        Serial.println(WiFi.localIP());
    }
}

/**
 * A helper function that does HTTP POST
 * 辅助功能: HTTP POST
 * @param content - A JSON String that you want to post. 
 *                - 你要POST的JSON String
 * 
 * @param uri     - The uri you are posting to.
 *                - 目的链接
*/
void http_json_post(String content, String uri)
{

    // String endpoint = HOST_ADDRESS + "/DHT";
    http.begin(wifiClient, uri);
    http.addHeader("Content-Type", "application/json");
    //TODO: add header param -> http.addHeader("Node-Name", "ESP32-Bedroom");
    http.POST(content);
    http.end();
}

// ^^^^^^^^^^^^^^^^^^^ AS A STATION ^^^^^^^^^^^^^^^^^^^^^^^^^^

// =================== AS AN ACCESS POINT ==========================
WiFiServer server(DEFAULT_SERVER_PORT);
/**
 * Starts a server in AP mode
 * 启动服务器
*/
void wifi_start_server()
{
    Serial.println("Setting soft-AP ... ");
    WiFi.softAP("WEMOS_D1_MINI(18650 Shield)", "12345678") ? "Ready" : "Failed!";
}

// ^^^^^^^^^^^^^^^^^^^ AS AN ACCESS POINT ^^^^^^^^^^^^^^^^^^^^^^^^^^
