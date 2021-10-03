#include <SoftwareSerial.h>

// HC12 / LoRa :
/**
 *@param param0 : the module's TX Pin, the board's RXD
  @param param1 : the module's RX Pin, the board's TXD
*/
SoftwareSerial transceiver(RXD, TXD);

void transceiver_setup()
{
  transceiver.begin(9600); // Serial port to the modules
  delay(100);

  transceiver.println("transceiver ready");
}

// Reads from the transceiver and prints what's read to
// the Serial Monitor
void transceiver_read()
{
  if (!Serial)
  {
    ;
  }
  while (transceiver.available())
  {                                   // If HC-12 has data
    Serial.write(transceiver.read()); // Send the data to Serial monitor
  }
}

// Reads from the Serial Monitor and
// Send it to the transceiver, which will be broadcasted.
void transceiver_write()
{
  if (!Serial)
  {
    ;
  }
  while (Serial.available())
  {                                   // If Serial monitor has data
    transceiver.write(Serial.read()); // Send that data to HC-12
  }
}