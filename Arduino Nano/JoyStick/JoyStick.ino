const int PIN_JOYSTICK_VRX = A0;
const int PIN_JOYSTICK_VRY = A1;  

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  Serial.println("X: " + String(getXPosition()));
  Serial.println("Y: " + String(getYPosition()));
  delay(1000);
}

float getXPosition(){
  return analogRead(PIN_JOYSTICK_VRX);
}

float getYPosition(){
  return analogRead(PIN_JOYSTICK_VRY);
}
