#include <Servo.h>

Servo servo_auto_horizontal, // - pin D3
    servo_X_axis,            // - pin D5
    servo_Y_axis;            // - pin D8

void setupServos()
{
    servo_auto_horizontal.attach(A1);
    servo_Y_axis.attach(A2);
}
