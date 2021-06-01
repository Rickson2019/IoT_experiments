
const int PIN_BUZZER_GND = 3; // when this pin is LOW, it buzzes
const int PIN_NH3_ANALOG = A0; // analogRead() this pin.

// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
#include "OLED.h"

//========== Adopted from: https://www.instructables.com/MQ137-PPM-Sensor/ =============================
const float m = -0.263; //Slope
const float b = 0.42; //Y-Intercept
const float R0 = 2.19; //Sensor Resistance in fresh air from previous code
//^^^^^^^^^^ Adopted from: https://www.instructables.com/MQ137-PPM-Sensor/ ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

void setup() {
  
  Serial.begin(9600);
  oled_setup();
  pinMode(PIN_BUZZER_GND, OUTPUT);    // sets the digital pin 13 as output
  pinMode(PIN_NH3_ANALOG, INPUT);    // sets the digital pin 13 as output

}



void loop() {
  //  read_NH3_analog();
  alert_at_detection();
}

void alert_at_detection() {
  if (read_NH3_analog() > 350) {
    digitalWrite(PIN_BUZZER_GND, HIGH);
    delay(1000);
    digitalWrite(PIN_BUZZER_GND, LOW);
    delay(1000);
  }
  else {
    digitalWrite(3, HIGH);
  }
}

int read_NH3_analog() {
  display.clearDisplay();


  //========== Adopted from: https://www.instructables.com/MQ137-PPM-Sensor/ =============================float sensor_volt; //Define variable for sensor voltage
  float RS_gas; //Define variable for sensor resistance
  float ratio; //Define variable for ratio
  float sensor_volt;
  
  float sensorValue = analogRead(PIN_NH3_ANALOG); //Read analog values of sensor
  sensor_volt = sensorValue * (5.0 / 1023.0); //Convert analog values to voltage
  RS_gas = ((5.0 * 10.0) / sensor_volt) - 10.0; //Get value of RS in a gas
  ratio = RS_gas / R0; // Get ratio RS_gas/RS_air

  double ppm_log = (log10(ratio) - b) / m; //Get ppm value in linear scale according to the the ratio value
  double ppm = pow(10, ppm_log); //Convert ppm value to log scale
  double percentage = ppm / 10000; //Convert to percentage
  int PotSP = analogRead(A1);
  int Zero = analogRead(A2);
  int Spam = analogRead(A3);
  double ppmsp;
  double zerotrim;
  

  Serial.print("analogRead(PIN_NH3_ANALOG):");

  Serial.println(String(ppm - zerotrim) + " ppm");

  

  display.setCursor(0, 30);
  display.println(ppm - zerotrim);

    // Display static text
  display.setCursor(35, 30);
  display.println("ppm" );

  
  //^^^^^^^^^^^^^^ Adopted from: https://www.instructables.com/MQ137-PPM-Sensor/ ^^^^^^^^^^^^^^^^^^^^^^^^^^

  
  display.display();

  return sensorValue;
}
