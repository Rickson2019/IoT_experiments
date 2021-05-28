//====== SPECIAL PIN NUM ======
#define DHT_DATA_PIN 5
//^^^^^^ SPECIAL PIN NUM ^^^^^^

#define DISPLAY_READING_DELAY 3500 //DELAY IN ms

#define LINE_SPACING 15 // LINE SPACING

//====== PMS 5003 ========
#include "PMS.h"
#include <SoftwareSerial.h>
//HardwareSerial pmsSerial(2);

PMS pms(Serial2);
PMS::DATA data;


//^^^^^^ PMS 5003  ^^^^^^^^


//====== Wireless Transmission =========
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

/*Put your SSID & Password*/
const char* ssid = "atoi()";  // Enter SSID here
const char* password = "33184699";  //Enter Password here

HTTPClient http;

String HOST_ADDRESS = "http://192.168.43.44:80";


//============ TEMPERATURE SENSORS ===============
/** REFERENCE:
    https://www.youtube.com/watch?v=ebJpXs2-WgM */

#include "DHT.h"

// Uncomment one of the lines below for whatever DHT sensor type you're using!
//#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321


// DHT Sensor
uint8_t DHTPin = DHT_DATA_PIN;
// Initialize DHT sensor.



float Temperature;
float Humidity;

/**
   @params
*/
void DHT_read() {
  //  /**Let D12 be the 3.3V output
  //     which powers up the DHT sensor:
  //     Signal: D13
  //     VCC   : D12
  //     GND   : GND
  //  */
  //
  //  pinMode(12, OUTPUT);
  //  digitalWrite(12, HIGH);


  DHT dht(DHTPin, DHTTYPE);
  dht.begin();


  Temperature = dht.readTemperature(); // Gets the values of the temperature
  Humidity = dht.readHumidity(); // Gets the values of the humidity

  // Posting JSON to the edge device
  DynamicJsonDocument doc(2048);

  doc["DHT-temperature"] = dht.readTemperature();
  doc["DHT-humidity"] = dht.readHumidity();

  String json;
  serializeJson(doc, json);

  String endpoint = HOST_ADDRESS + "/DHT";

  Serial.println("Temperature");
  Serial.println(Temperature);

  Serial.println("Humidity");
  Serial.println(Humidity);

//  Serial.flush();

  Serial.println(json);

  delay(DISPLAY_READING_DELAY);


};
//^^^^^^^^^^ TEMPERATURE SENSORS ^^^^^^^^^^^^^


//
void send_json_post(DynamicJsonDocument doc, String url) {
  // Posting JSON to the edge device


  String json;
  serializeJson(doc, json);

  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Node-Name", "ESP32-Bedroom");
  http.POST(json);
  http.end();
}






void setup() {



  /**Let D12 be the 3.3V output
    which powers up the DHT sensor:
    Signal: D13
    VCC   : D12
    GND   : GND
  */
  Serial.begin(9600);
//  Serial1.begin(9600);
  Serial2.begin(9600);
  pms.wakeUp();
//  pmsSerial.begin(9600);


//  initWiFi();
}



void loop() {




  



if (pms.read(data))
  {
    Serial .print("PM 1.0 (ug/m3): ");
    Serial .println(data.PM_AE_UG_1_0);

    Serial .print("PM 2.5 (ug/m3): ");
    Serial .println(data.PM_AE_UG_2_5);

    Serial .print("PM 10.0 (ug/m3): ");
    Serial .println(data.PM_AE_UG_10_0);

    Serial .println();
    
    delay(2000);
    
    DHT_read();
  }
  
  

}



// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println("");
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  String endpoint = HOST_ADDRESS + "/";

  http.begin(endpoint);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Node-Name", "ESP32-Bedroom");
  http.GET();
  http.end();


  Serial.println("");
  Serial.println(WiFi.localIP());
}
