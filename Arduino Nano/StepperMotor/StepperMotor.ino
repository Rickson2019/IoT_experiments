#include <AccelStepper.h>

#define DEFAULT_STEPPER 1
#define PIN_DIR  2
#define PIN_PULSE 3

#define STEPS_PER_DEGREE 200 // NEMA 42 motor

AccelStepper stepper(DEFAULT_STEPPER, PIN_PULSE, PIN_DIR);

void setup() {
  Serial.begin(9600);
  stepper.setCurrentPosition(0); // default position
  stepper.disableOutputs();
  stepper.setMaxSpeed( 6 * 10000 * STEPS_PER_DEGREE);
  stepper.setAcceleration( 2 * 6 * STEPS_PER_DEGREE);
  stepper.enableOutputs();
}

void loop() {

  Serial.println("Moving!");
  stepper.run();

  stepper.move(1 * 7 * STEPS_PER_DEGREE);
 
//  delay(1000);

  



  //  ste
}
