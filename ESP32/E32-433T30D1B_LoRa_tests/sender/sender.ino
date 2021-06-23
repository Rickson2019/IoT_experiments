#include <ArduinoJson.h>
#include "utils/radio_transceiver_utils.h"
#include "utils/int_to_bytes.h"

#define debug false

int reference = 65535;

void setup()
{
  Serial.begin(115200); // First Step(important)

  transceiver_setup();
}


void loop()
{

  char integer_in_bytes[2] = {0, 0};
  int_to_bytes(reference, integer_in_bytes);

  transceiver.write(reference / 256);
  transceiver.write(reference % 256);
  //  transceiver.write(integer_in_bytes);
  //  transceiver.write("integer_in_bytes");
  //  Serial.write(integer_in_bytes);

  delay(2000);

#if debug
  Serial.println(bytes_to_int(integer_in_bytes));
  Serial.println(reference);
#endif

  reference ++;
}
