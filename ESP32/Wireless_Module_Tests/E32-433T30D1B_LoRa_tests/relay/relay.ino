#include "utils/radio_transceiver_utils.h"

void setup()
{
  Serial.begin(115200); // First Step(important)

  transceiver_setup();  // Set up reeiver at it's port
}

void loop()
{
  //If not available, it doesn't proceed to the next step
  while (transceiver.available())
  {
    char received_char = transceiver.read(); //Received 
    transceiver.write(received_char);        //Send that data to Lora Module.
    
    Serial.print(received_char);             //Debugging 
  }
}
