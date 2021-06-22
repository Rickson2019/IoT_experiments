#include <Servo.h>


#define PIN_SERVO_AUTO_HORIZONTAL A0

Servo servo_auto_horizontal; // - pin D3


void setup() {
  // put your setup code here, to run once:
  servo_auto_horizontal.attach(PIN_SERVO_AUTO_HORIZONTAL);
}

void loop() {

   servo_auto_horizontal.write(0);

}
