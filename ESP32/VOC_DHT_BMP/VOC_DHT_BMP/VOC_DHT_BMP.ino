
#include "DHT.h"

// https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/
const int DHT_DATA_PIN =  5; // 
const int sensorAnaPin =  34; //D34

int sensorState = 0;
int lastState = 0;


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

  Serial.println("Temperature");
  Serial.println(Temperature);
  Serial.println("Humidity");
  Serial.println(Humidity);

};
//^^^^^^^^^^ TEMPERATURE SENSORS ^^^^^^^^^^^^^


// ========= VOC Sensor ====================
void VOC_read(){
  float read_num = analogRead(sensorAnaPin);
  Serial.print("analogRead(sensorAnaPin): ");
  Serial.println(analogRead(sensorAnaPin));
  float voltage = read_num / 4096;
  
  Serial.println("voltage" + String(voltage));
  float log_voc_ppm = (-1.095) + 0.627 * voltage;
   
  float log_toluene_ppm = (-3.478) + 1.104 * voltage;
    
  Serial.print("Formaldehyde(VOC) log(ppm): ");
  Serial.print(log_voc_ppm);
  Serial.print(" PPM\n");

  Serial.print("Toluene(甲苯)      log(ppm): ");
  Serial.print(log_toluene_ppm);
  
  Serial.print(" PPM\n");
  
 
   
}


// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

void setup()
{
  Serial.begin(9600);
  pinMode(DHT_DATA_PIN, INPUT);
}

void loop()
{ 

  DHT_read();
  VOC_read();
  delay(1500);
}
