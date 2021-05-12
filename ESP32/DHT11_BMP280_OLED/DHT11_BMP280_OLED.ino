
//====== SPECIAL PIN NUM ======
#define DHT_DATA_PIN 13
#define PH_ADC_PIN 35
#define TDS_ADC_PIN 36
//^^^^^^ SPECIAL PIN NUM ^^^^^^

//====== DELAY IN ms ========
#define DISPLAY_READING_DELAY 3500

// LINE SPACING
#define LINE_SPACING 15

#include <WiFi.h>

#include "SPIFFS.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include <Adafruit_BMP280.h>
Adafruit_BMP280 bmp; // I2C


/*Put your SSID & Password*/
const char* ssid = "DOG";  // Enter SSID here
const char* password = "33184699";  //Enter Password here

HTTPClient http;

String HOST_ADDRESS = "http://192.168.43.44:80";

// Include the correct display library
// For a connection via I2C using the Arduino Wire include:
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"


// Optionally include custom images
#include "images.h"


// Initialize the OLED display using Arduino Wire:
SSD1306Wire display(0x3c, SDA, SCL);   // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h
// SSD1306Wire display(0x3c, D3, D5);  // ADDRESS, SDA, SCL  -  If not, they can be specified manually.
// SSD1306Wire display(0x3c, SDA, SCL, GEOMETRY_128_32);  // ADDRESS, SDA, SCL, OLEDDISPLAY_GEOMETRY  -  Extra param required for 128x32 displays.
// SH1106 display(0x3c, SDA, SCL);     // ADDRESS, SDA, SCL

// Initialize the OLED display using brzo_i2c:
// SSD1306Brzo display(0x3c, D3, D5);  // ADDRESS, SDA, SCL
// or
// SH1106Brzo display(0x3c, D3, D5);   // ADDRESS, SDA, SCL

// Initialize the OLED display using SPI:
// D5 -> CLK
// D7 -> MOSI (DOUT)
// D0 -> RES
// D2 -> DC
// D8 -> CS
// SSD1306Spi display(D0, D2, D8);  // RES, DC, CS
// or
// SH1106Spi display(D0, D2);       // RES, DC





//============ TEMPERATURE SENSORS ===============
/** REFERENCE:
    https://www.youtube.com/watch?v=ebJpXs2-WgM */

#include "DHT.h"

// Uncomment one of the lines below for whatever DHT sensor type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321


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


  display.setFont(ArialMT_Plain_16);
  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_CENTER);


  display.drawString(64, 0, "DHT Temp: " );
  display.drawString(64, LINE_SPACING,  String(dht.readTemperature()) + " °C" );

  display.drawString(64, LINE_SPACING * 2, "DHT Hum: "  );
  display.drawString(64, LINE_SPACING * 3, String(dht.readHumidity()) + " %");


 
  // Posting JSON to the edge device  
  DynamicJsonDocument doc(2048);
  
  doc["DHT-temperature"] = dht.readTemperature();
  doc["DHT-humidity"] = dht.readHumidity();

  String json;
  serializeJson(doc, json);

  String endpoint = HOST_ADDRESS + "/DHT";
  http.begin(endpoint);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Node-Name", "ESP32-Bedroom");
  
  http.POST(json);
  http.end();

  display.display();


  Serial.println("Temperature");
  Serial.println(Temperature);

  Serial.println("Humidity");
  Serial.println(Humidity);

  Serial.flush();


  delay(DISPLAY_READING_DELAY);


};
//^^^^^^^^^^ TEMPERATURE SENSORS ^^^^^^^^^^^^^


//
void send_json_post(DynamicJsonDocument doc, String url){
    // Posting JSON to the edge device  

 
  String json;
  serializeJson(doc, json);
  
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Node-Name", "ESP32-Bedroom");
  http.POST(json);
  http.end();
}




#define DEMO_DURATION 3000
typedef void (*Demo)(void);

int demoMode = 0;


int demoLength = 100;
long timeSinceLastModeSwitch = 0;

int counter = 1;
int progress = 0;
int stop_progress = 0;

void drawProgress() {
  if (progress == 99) {
    progress = 100;
    stop_progress = 1;
  }

  else {
    progress = (counter / 5) % 100;

    // draw the progress bar
    display.drawProgressBar(0, 48, 120, 10, progress);

    if (progress < 45) {
      display.setTextAlignment(TEXT_ALIGN_CENTER);
      display.drawString(64, 5, "Initializing");
      display.drawString(64, 25, "DHT sensor");
    }

    if (progress > 45) {
      display.setTextAlignment(TEXT_ALIGN_CENTER);
      display.drawString(64, 5, "Powering up");
      display.drawString(64, 25, "BME sensor");
    }

    // draw the percentage as String
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 35, String(progress) + "%");


    display.display();
    counter ++;

    delay(40);
  }

  //  delay(100);
}

void setup() {



  /**Let D12 be the 3.3V output
    which powers up the DHT sensor:
    Signal: D13
    VCC   : D12
    GND   : GND
  */

  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);


  //  pinMode(32, OUTPUT);
  //  pinMode(35, OUTPUT);
  //  digitalWrite(32, LOW);
  //  digitalWrite(35, HIGH);


  Serial.begin(9600);
  initWiFi();
  analogSetClockDiv(1);
  adcAttachPin(35);
  adcAttachPin(36);


  Serial.println();
  Serial.println();

  // Initialising the UI will init the display too.
  display.init();
  // Draws the progress bar:
  //  drawProgress();
  //  display.display();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

}

void read_BMP() {
  Serial.flush();
  delay(500);


  //  Serial.begin(115200);
  delay(3000);

  Serial.println(F("BMP280:"));

  bmp.begin(0x76);
  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  float temp =  bmp.readTemperature() ;
  float press =  bmp.readPressure() ;

  Serial.println(temp);
  Serial.println(press);

  display.clear();
  //  display.init();

  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_CENTER);

  display.drawString(64, 0, "BME Temp");
  display.drawString(64, LINE_SPACING, String(temp) + " °C" );

  display.drawString(64, LINE_SPACING * 2, "BME Press");
  display.drawString(64, LINE_SPACING * 3 ,  String(press)    + " Pa" );

  display.display();

  Serial.flush();


// Posting JSON to the edge device  

  DynamicJsonDocument doc(2048);
  
  doc["BMP280-temperature"] = temp;
  doc["BMP280-pressure"] = press;

  String json;
  serializeJson(doc, json);
  
  String endpoint = HOST_ADDRESS + "/BME280";

  // BEGIN HAS TO COME BEFORE THE addHeader() !
  http.begin(endpoint);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Node-Name", "ESP32-Bedroom");
  Serial.println(endpoint);
 
  http.POST(json);
  http.end();
  
  delay(DISPLAY_READING_DELAY);
}


float analog_to_PH(float ana_input ){
  float output = ana_input / 4096 * 12;
  return output ; 
}

void ADC_func() {
//  Serial.flush();
  
  //  display.init();
  display.clear();

  delay(1000);

  Serial.println("Printing!");
  Serial.println(analogRead(35));
  //  display.flipScreenVertically();


  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16);

//   display.drawString(64, 3, "PH:");

  
  if (analogRead(35) >= 0) {
    display.clear();

    float PH =  analogRead(35);

    display.drawString(64, 0, "PH:");
    
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, LINE_SPACING, String(analog_to_PH (analogRead(35))) );

    display.setFont(ArialMT_Plain_16);
    display.drawString(64, LINE_SPACING * 2, "TDS ana:");
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, LINE_SPACING * 3,  String(analogRead(36)) );

    //MQ2 Gas sensor connected to Pin35
    //  PH
    Serial.println("PH");
    Serial.println(analog_to_PH (analogRead(35)) );

    //MQ135 Gas sensor connected to Pin35
    //TDS
    Serial.println("TDS analog reading:");
    Serial.println(analogRead(36));
    display.display();

    
    DynamicJsonDocument doc(2048);

    doc["water-PH"] = analog_to_PH(PH);
    doc["water-TDS"] = analogRead(36);


    String url = HOST_ADDRESS + "/water-sensors";
    
    send_json_post(doc,url);
    
    delay(DISPLAY_READING_DELAY);
  }
  else{
    
  }

}


void loop() {
  // init
  //  Serial.println(stop_progress);
  //  Serial.println(progress);
  //  Serial.begin(115200);

  if (progress < 99 || stop_progress == 0) {
    display.clear();
    drawProgress();
  }
  else {
    
    // clear the display
    display.clear();
    read_BMP();

    display.clear();
    DHT_read();

    display.clear();
    ADC_func();

    //    delay(1000);
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

void initSPIFFS() {
  if (!SPIFFS.begin()) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}


//void start_web_server() {
//
//  initWiFi();
//  initSPIFFS();
//  
//    // Handle Web Server
//  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
//    request->send(SPIFFS, "/index.html", "text/html");
//  });
//
//  server.serveStatic("/", SPIFFS, "/");
//
//  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
//
//    request->send(200, "text/plain", "OK");
//    
//  });
//
//  server.on("/DHT", HTTP_GET, [](AsyncWebServerRequest *request){
//
//    request->send(200, "text/plain", "OK");
//  });
//
//  server.on("/BME280", HTTP_GET, [](AsyncWebServerRequest *request){
//
//    request->send(200, "text/plain", "OK");
//  });
//
//  server.on("/WATER", HTTP_GET, [](AsyncWebServerRequest *request){
//    request->send(200, "text/plain", "OK");
//  });
//
//  // Handle Web Server Events
//  events.onConnect([](AsyncEventSourceClient *client){
//    if(client->lastId()){
//      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
//    }
//    // send event with message "hello!", id current millis
//    // and set reconnect delay to 1 second
//    client->send("hello!", NULL, millis(), 10000);
//  });
//  server.addHandler(&events);
//
//  server.begin();
//}
