
#include "utils/ccs811_utils.h" //Install : Adafruit CCS811 Library by Adafruit
#include "utils/dht_utils.h"

void setup()
{
  Serial.begin(9600);
  heat_up_ccs();
}

void loop()
{
  if (ccs811.available())
  {
    if (!ccs811.readData())
    {
      Serial.print("CO2: ");
      Serial.print(ccs811.geteCO2());
      Serial.print("ppm, TVOC: ");
      Serial.println(ccs811.getTVOC());
    }
    else
    {
      Serial.println("ERROR!");
    }
  }

  Serial.println(DHT_read());

  delay(500);
}
