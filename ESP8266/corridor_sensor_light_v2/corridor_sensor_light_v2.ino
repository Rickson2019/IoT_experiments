#include "gy-302.h"

// Developed using ESP8266 NodeMCU
// https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/
const int PIN_LIGHT_DIGITAL = D0; //A0
const int PIN_HUMAN_DETECTOR = A0; //A0

const int PIN_ESP8266_SCL = D1; //D1
const int PIN_ESP8266_SDA = D2; //D2

const int PIN_BLUB_STRIPE_GND = D4; //D4


const int VAL_DIM_LIGHT_THRES = 25;
const int VAL_HUMAN_DETECTION_THRES = 600;

const int VAL_LIGHTS_ON_MILLISECONDS = 10 * 1000; // 15  sec

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
  bool LIGHT_IS_DIM = 0;
  bool HUMAN_BODY_DETECTED = 0;


  if( read_lux() < VAL_DIM_LIGHT_THRES){
    LIGHT_IS_DIM = 1;
    Serial.println("LIGHT IS DIM");
    
  }

  
  if(analogRead(PIN_HUMAN_DETECTOR) >= VAL_HUMAN_DETECTION_THRES){
    HUMAN_BODY_DETECTED = 1;
    Serial.println("HUMAN BODY DETECTED");
  }



// The light is dim, and human body is detected:   
  if (LIGHT_IS_DIM == 1 && (HUMAN_BODY_DETECTED == 1) ) {
    Serial.println("Lights ON !!!");
    digitalWrite(PIN_BLUB_STRIPE_GND, LOW);
    delay(VAL_LIGHTS_ON_MILLISECONDS);
  }

  else {
    Serial.println("Lights OFF !!!");
    digitalWrite(PIN_BLUB_STRIPE_GND, HIGH);
  }

}


float read_lux() {
  uint16_t val = 0;

  BH1750_Init(BH1750address);


  if (2 == BH1750_Read(BH1750address))

  {

    val = ((buff[0] << 8) | buff[1]) / 1.2;

    Serial.print(val, DEC);

    Serial.println("[lx]");

  }
  return val;
}
