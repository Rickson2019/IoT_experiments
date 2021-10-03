#include "ServoUtils.h"
#include "Ps2JoyStickUtils.h"

/*
  Servo servo_auto_horizontal - Pin 3
  servo_X_axis                - Pin 5
  servo_Y_axis                - Pin ?
*/

bool go = true;
void setup()
{
  Serial.begin(115200);
  joystick_setup();
  setupServos();
  ps2x.read_gamepad(false, vibrate);     // is a MUST     //read controller and set large motor to spin at 'vibrate' speed

}

float X_degrees = 90;
float Y_degrees = 90;

void loop()
{
  if (go == true) {

    //    for (int pos = 0 ; pos <= 180; pos = pos + 1) {
    //      servo_auto_horizontal.write(pos);
    //      delay(45);
    //      servo_Y_axis.write(pos);
    //      delay(25);
    //
    //    }
    ps2x.read_gamepad(false, vibrate);     // is a MUST     //read controller and set large motor to spin at 'vibrate' speed

    float X_val = ps2x.Analog(PSS_LX);


    float Y_val = ps2x.Analog(PSS_LY);

    moveSlowly();

    Serial.println("X_val:");
    Serial.println(X_val);
    Serial.println("Y_val:");
    Serial.println(Y_val);

    if (ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) // print stick values if either is TRUE
    {
      X_degrees = (X_val / 255) * 180;
      Y_degrees = (Y_val / 255) * 180;
    }

    if (X_degrees > 170) {
      X_degrees -= 5;
    }
    else if (X_degrees < 10) {
      X_degrees += 5;
    }

    if (Y_degrees > 170) {
      Y_degrees -= 5;
    }
    else if (Y_degrees < 10) {
      Y_degrees += 5;
    }

    Serial.println("X_degrees:");
    Serial.println(X_degrees);

    Serial.println("Y_degrees:");
    Serial.println(Y_degrees);
    //    if (X_val >= 128) {
    //      X_degrees = 90 +  ((X_val - 128) / 128) * 90;
    //
    //    }
    //    else {
    //      X_degrees = 90 - (X_val  - 128) * 90;
    //    }
    //    Serial.println(X_degrees);
    //
    //    if (Y_val >= 128) {
    //      Y_degrees = 90 +  ((Y_val - 128) / 128) * 90;
    //    }
    //    else {
    //      Y_degrees = 90 - (Y_val - 128) * 90;
    //    }
    //    Serial.println(Y_degrees);
    //




    servo_auto_horizontal.write(int(X_degrees));
    servo_Y_axis.write(int(Y_degrees));
  }


  //  Serial.print("Stick Values:");
  //  Serial.print(ps2x.Analog(PSS_LY), DEC); // LY
  //  Serial.print(",");
  //  Serial.print(ps2x.Analog(PSS_LX), DEC); // LX
  //  Serial.print(",");
  //  Serial.print(ps2x.Analog(PSS_RY), DEC); // RY
  //  Serial.print(",");
  //  Serial.println(ps2x.Analog(PSS_RX), DEC); // RX
  delay(50);
}



void moveSlowly() {
  if (ps2x.Button(PSB_PAD_UP)) {        //will be TRUE as long as button is pressed
    Serial.println("UP");
    Y_degrees += 5;

  }
  if (ps2x.Button(PSB_PAD_RIGHT)) {
    Serial.println("Right");
    X_degrees += 5;
  }
  if (ps2x.Button(PSB_PAD_LEFT)) {
    Serial.println("Left");
    X_degrees -= 5;
  }
  if (ps2x.Button(PSB_PAD_DOWN)) {
    Serial.println("DOWN");
    Y_degrees -= 5;
  }
}
