#include <AccelStepper.h>

#define DEFAULT_STEPPER 1
#define PIN_DIR  2
#define PIN_PULSE 3

#define STEPS_PER_DEGREE 200 // NEMA 42 motor

#define X_AXIS_CENTRAL 489
#define Y_AXIS_CENTRAL 528
#define MARGIN 15

#define X_LEFT = -1;
#define X_RIGHT = 1;
#define X_CENTRAL = 0;

AccelStepper stepper(DEFAULT_STEPPER, PIN_PULSE, PIN_DIR);


const int PIN_JOYSTICK_VRX = A0;
const int PIN_JOYSTICK_VRY = A1;

int x_dir = 0;

void setup() {
  Serial.begin(9600);
  stepper.setCurrentPosition(0); // default position
  //  stepper.disableOutputs();
  stepper.enableOutputs();
}

void loop() {



  //    stepper.move(400);

  //  Serial.println("X: "+ String(getXPosition()));
  //  Serial.println("Y: "+ String(getYPosition()));



  if (getXPosition() > X_AXIS_CENTRAL + MARGIN) {

    Serial.println("Moving ClockWise !!!");
    stepper.setMaxSpeed( 3 * 720 * STEPS_PER_DEGREE);
    stepper.setAcceleration( 3 * 720 * STEPS_PER_DEGREE);
    stepper.move(1 * 360 *  STEPS_PER_DEGREE);
  }
  else if ( getXPosition() < X_AXIS_CENTRAL - MARGIN ) {
    Serial.println("Moving Counter-ClockWise !!!");
    stepper.setMaxSpeed(  3 * 720 * STEPS_PER_DEGREE);
    stepper.setAcceleration(  - 3 * 720 * STEPS_PER_DEGREE);
    stepper.move(- 1 * 360 * STEPS_PER_DEGREE);

  }
  stepper.run();





}

float getXPosition() {
  return analogRead(PIN_JOYSTICK_VRX);
}

float getYPosition() {
  return analogRead(PIN_JOYSTICK_VRY);
}
