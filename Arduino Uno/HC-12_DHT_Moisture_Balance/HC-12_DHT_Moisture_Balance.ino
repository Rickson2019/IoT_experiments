
//https://pastebin.com/bDVemTtt
#include <LowPower.h> //the library for the low power

#include <ArduinoJson.h>

#include "utils/DHT_func.h"
#include "utils/HC12_func.h"
#include "utils/deep_sleep_utils.h"
#include "utils/bmp280_utils.h"

const int PIN_MOISTURE_SENSOR_1 = A0; //
const int PIN_MOISTURE_SENSOR_2 = A2; //

const int PIN_LIGHT_SENSOR = A1;
const int PIN_RAINDROP_SENSOR = A3;

const int PIN_BALANCE_SENSOR = 12; //

void setup()
{

  //  lowPowerSetup(); //enable sleep

  pinMode(PIN_BALANCE_SENSOR, INPUT);
  Serial.begin(115200); // Serial port to computer

  HC12.begin(9600); // Serial port to HC12

  // HEAT  UP
  BMP_heatup();
  DHT_heatup();

  delay(100);
  HC12.println("Transmission Started");
}
void loop()
{
  //  for (int i = 0; i < 21; i ++) {
  //    pinMode(i, INPUT);
  //  }

  DynamicJsonDocument doc(128);
  String json;

  // ------------ DHT 11 ---------------
  // [0] - temperature
  // [1] - humidity
  float dht_retval[2] = {0.00, 0.00};
  DHT_read(dht_retval);
  doc["DHT-11_temperature"] = dht_retval[0];
  doc["DHT-11_humidity"] = dht_retval[1];

  // ------------ BMP 280 ---------------
  // [0] - temperature
  // [1] - pressure
  float bmp_retval[2] = {0.00, 0.00};
  BMP_read(bmp_retval);
  doc["BMP-280_temperature"] = bmp_retval[0];
  doc["BMP-280_humidity"] = bmp_retval[1];

  // ------------- Moisture---------------
  doc["Moisture-reading-#1"] = String(float(1023 - analogRead(PIN_MOISTURE_SENSOR_1)) / 1023 * 100) + "%";
  doc["Moisture-reading-#2"] = String(float(1023 - analogRead(PIN_MOISTURE_SENSOR_2)) / 1023 * 100) + "%";

  // ------------- LIGHTNESS---------------
  doc["Lightness"] = String(float(1023 - analogRead(PIN_LIGHT_SENSOR)) / 1023 * 100) + "%";

  // ------------- Raindrop---------------
  doc["Raindrop-Coverage"] = String(float(1023 - analogRead(PIN_RAINDROP_SENSOR)) / 1023 * 100) + "%";

  // ------------- Balance ---------------
  //  doc["balanced"] = digitalRead(PIN_BALANCE_SENSOR);

  serializeJson(doc, json);
  HC12.println(json);
  // delay(1000);

  while (HC12.available())
  {                            // If HC-12 has data
    Serial.write(HC12.read()); // Send the data to Serial monitor
  }
  // delay(1500);

  //  for (int i = 0; i < 21; i ++) {
  //    pinMode(i, OUTPUT);
  //    digitalWrite(i, LOW);       // put all the PINS to LOW
  //  }

  for (int i = 0; i < 5; i++)
  {
    LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF,
                  SPI_OFF, USART0_OFF, TWI_OFF);
    LowPower.idle(SLEEP_2S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF,
                  SPI_OFF, USART0_OFF, TWI_OFF);
  }

      LowPower.idle(SLEEP_2S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF,
                  SPI_OFF, USART0_OFF, TWI_OFF);
}
