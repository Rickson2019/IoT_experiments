#include "utils/radio_transceiver_utils.h"
#include "utils/oled_utils.h"

void setup()
{
  Serial.begin(115200); // First Step(important)

  transceiver_setup();
  init_oled();
}

void loop()
{

  while (!transceiver.available())
  {
    ;
  }

  byte b0 = transceiver.read();

  while (!transceiver.available())
  {
    ;
  }

  byte b1 = transceiver.read();

  int val = b0 * 256 + b1;
  Serial.println(val);

  display.clearDisplay();
  display.setTextSize(1);              // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);             // Start at top-left corner
  display.cp437(true);                 // Use full 256 char 'Code Page 437' font

  display.write(&String(val)[0]); // Send the data to Serial monitor

  display.display();

  //    transceiver_read();
  //    transceiver_write();
}
