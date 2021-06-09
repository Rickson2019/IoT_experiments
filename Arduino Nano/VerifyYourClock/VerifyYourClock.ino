#include <AccelStepper.h>

#define DEFAULT_STEPPER 1
#define PIN_DIR  2
#define PIN_PULSE 3

#define STEPS_PER_DEGREE 200 // NEMA 42 motor

AccelStepper stepper(DEFAULT_STEPPER, PIN_PULSE, PIN_DIR);
bool stopSteppers = false;

void setup() {
  Serial.begin(9600);
  stepper.setCurrentPosition(0); // default position
  stepper.disableOutputs();
  stepper.setMaxSpeed( 6 * 1 * STEPS_PER_DEGREE);
  stepper.setAcceleration( 1 * STEPS_PER_DEGREE);
  stepper.enableOutputs();

}

void loop() {





  if (stepper.currentPosition() == 0) {
    stepper.move(1 * 1 * STEPS_PER_DEGREE);
  }

  Serial.println(stepper.distanceToGo());
  if (stepper.distanceToGo() == 0) {
    Serial.println("delay 1000 ms");
    stepper.stop();
    stepper.setCurrentPosition(0); // default position
    delay(800);
  }
  stepper.run();
}
