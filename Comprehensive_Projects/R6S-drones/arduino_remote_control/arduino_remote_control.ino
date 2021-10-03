#define TXD 3
#define RXD 2
#include "utils/radio_transceiver_utils.h" // HC12 / LoRa module setups

#include "utils/PS3USB_utils.h"            // adapted from USB PS3 module

void setup()
{
  Serial.begin(115200); // First Step(important)

  transceiver_setup();

  ps3_controller_setup();

}

void loop()
{
//  transceiver_read();
  //Reads in the controller's operations
  controller_parsing();

  //Comes after the controller functions
  //writing bytes to the transceiver

  transceiver_write();
}
