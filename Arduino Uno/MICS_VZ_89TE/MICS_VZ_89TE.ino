#include "MICS-VZ-89TE.h"

MICS_VZ_89TE vz_89te;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  vz_89te.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  vz_89te.readSensor();
  delay(1000);

  Serial.println("vz_89te.getVOC():");
  Serial.println(vz_89te.getVOC());

  Serial.println("vz_89te.getCO2():");
  Serial.println(vz_89te.getCO2());
  delay(1000);
 
}
