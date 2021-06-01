#include "gy-302.h"


// https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/
const int PIN_LIGHT_DIGITAL = D0; //A0
const int PIN_LIGHT_ANALOG = A0; //A0

const int PIN_ESP8266_SCL = D1; //D1
const int PIN_ESP8266_SDA = D2; //D2

const int PIN_BLUB_STRIPE_GND = D4; //D4


void setup()
{
  Serial.begin(9600);
  pinMode(PIN_LIGHT_DIGITAL, INPUT);
  pinMode(PIN_BLUB_STRIPE_GND, OUTPUT);

  //GY-302 init, read light in Lux
  Wire.begin(PIN_ESP8266_SDA, PIN_ESP8266_SCL);

}

void loop()
{

  int i;

  uint16_t val = 0;

  BH1750_Init(BH1750address);

  delay(200);

  if (2 == BH1750_Read(BH1750address))

  {

    val = ((buff[0] << 8) | buff[1]) / 1.2;

    Serial.print(val, DEC);

    Serial.println("[lx]");

  }
  delay(150);

  Serial.println("analogRead:" + String(analogRead(PIN_LIGHT_ANALOG)) );

  if (digitalRead(PIN_LIGHT_DIGITAL) == 1) {
    Serial.println("开灯");
    digitalWrite(PIN_BLUB_STRIPE_GND, LOW);
  }
  else {
    Serial.println("熄灯");
    digitalWrite(PIN_BLUB_STRIPE_GND, HIGH);
  }

}
