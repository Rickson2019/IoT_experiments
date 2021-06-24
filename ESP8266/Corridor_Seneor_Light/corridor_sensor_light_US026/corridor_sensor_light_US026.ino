#include <NewPing.h>
NewPing sonar(D5,D6,80);
//
//const int trigPin = D2; 
//const int echoPin = D3; 
//
//long duration;
//int distance;


// https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/
const int PIN_LIGHT_ANALOG = A0; //A0
const int PIN_LIGHT_DIGITAL = D1; //D1
const int PIN_BLUB_STRIPE_GND = D0; //A0

void setup()
{

  Serial.begin(9600);
  pinMode(PIN_LIGHT_DIGITAL, INPUT);
  pinMode(PIN_BLUB_STRIPE_GND, OUTPUT);


}

void loop()
{ 

//  Serial.println(analogRead(PIN_LIGHT_ANALOG));
  Serial.println("Distance:");
  Serial.println(sonar.ping_cm());
//  delay(1000);
  
  if(digitalRead(PIN_LIGHT_DIGITAL) == 1){
        
        digitalWrite(PIN_BLUB_STRIPE_GND, LOW);
  }
  else{
    digitalWrite(PIN_BLUB_STRIPE_GND, HIGH);
  }
  
}
