
//====== SPECIAL PIN NUM ======
#define DHT_DATA_PIN 13
#define PH_ADC_PIN 35
#define TDS_ADC_PIN 36
//^^^^^^ SPECIAL PIN NUM ^^^^^^

//====== DELAY IN ms ========
#define DISPLAY_READING_DELAY 3500

// LINE SPACING
#define LINE_SPACING 15

//====== PMS 5003 ST ========
#include "BLINKER_PMSX003ST.h"
BLINKER_PMSX003ST pms;


HardwareSerial pmsSerial(1);// UART1/Serial1 pins 16,17

//^^^^^^ PMS 5003 ST ^^^^^^^^






#include <WiFi.h>


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

  Serial.println("DHT TEMPERATURE: ");
  Serial.println(Temperature);
  Serial.println("DHT HUMIDITY:  ");
  Serial.println(Humidity);

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

  Serial.flush();

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
  Serial.begin(9600);

  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);

  pmsSerial.begin(9600);
  pms.begin(pmsSerial);
  pms.wakeUp();

  Serial.println();
  Serial.println();

}

void read_BMP() {
  bmp.begin(0x76);
  delay(500);
  Serial.flush();
  Serial.println(F("BMP280:"));


  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  float temp =  bmp.readTemperature() ;
  float press =  bmp.readPressure() ;

  Serial.println("temp");
  Serial.println(temp);
  Serial.println("pressure");
  Serial.println( press);

  // Posting JSON to the edge device

  DynamicJsonDocument doc(2048);

  doc["BMP280-temperature"] = temp;
  doc["BMP280-pressure"] = press;

  String json;
  serializeJson(doc, json);

  String endpoint = HOST_ADDRESS + "/BME280";

  Serial.println(json);

  delay(DISPLAY_READING_DELAY);
}


float analog_to_PH(float ana_input ) {
  float output = ana_input / 4096 * 12;
  return output ;
}

void ADC_func() {



  if (analogRead(35) >= 0) {

    float PH =  analogRead(35);

    //MQ2 Gas sensor connected to Pin35
    //  PH
    Serial.println("PH");
    Serial.println(analog_to_PH (analogRead(35)) );

    //MQ135 Gas sensor connected to Pin35
    //TDS
    Serial.println("TDS analog reading:");
    Serial.println(analogRead(36));

    DynamicJsonDocument doc(2048);

    doc["water-PH"] = analog_to_PH(PH);
    doc["water-TDS"] = analogRead(36);


    String url = HOST_ADDRESS + "/water-sensors";
//
//    send_json_post(doc, url);

    delay(DISPLAY_READING_DELAY);
  }
  else {

  }

}


void loop() {




  
  DHT_read();
  read_BMP();
  ADC_func();

//  pms.request();
//  if (!pms.read()) {
//    return;
//  }
//
//  show_results();
  delay(1000);
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


void show_results() {
  Serial.print("PM1.0(CF1)\t");
  Serial.print(pms.getPmCf1(1.0));
  Serial.println("ug/m3");
  Serial.print("PM2.5(CF1)\t");
  Serial.print(pms.getPmCf1(2.5));
  Serial.println("ug/m3");
  Serial.print("PM10(CF1)\t");
  Serial.print(pms.getPmCf1(10));
  Serial.println("ug/m3");
  Serial.print("PM1.0(ATO)\t");
  Serial.print(pms.getPmAto(1.0));
  Serial.println("ug/m3");
  Serial.print("PM2.5(ATO)\t");
  Serial.print(pms.getPmAto(2.5));
  Serial.println("ug/m3");
  Serial.print("PM10(ATO)\t");
  Serial.print(pms.getPmAto(10));
  Serial.println("ug/m3");
  Serial.print("  PCS0.3\t");
  Serial.print(pms.getPcs(0.3));
  Serial.println("pcs/0.1L");
  Serial.print("  PCS0.5\t");
  Serial.print(pms.getPcs(0.5));
  Serial.println("pcs/0.1L");
  Serial.print("  PCS1.0\t");
  Serial.print(pms.getPcs(1));
  Serial.println("pcs/0.1L");
  Serial.print("  PCS2.5\t");
  Serial.print(pms.getPcs(2.5));
  Serial.println("pcs/0.1L");
  Serial.print("  PCS5.0\t");
  Serial.print(pms.getPcs(5));
  Serial.println("pcs/0.1L");
  Serial.print("   PCS10\t");
  Serial.print(pms.getPcs(10));
  Serial.println("pcs/0.1L");
  Serial.print("Formalde\t");
  Serial.print(pms.getForm());
  Serial.println("ug/m3");
  Serial.print("Temperat\t");
  Serial.print(pms.getTemp());
  Serial.println("'C");
  Serial.print("Humidity\t");
  Serial.print(pms.getHumi());
  Serial.println("%");
  Serial.println();
}
