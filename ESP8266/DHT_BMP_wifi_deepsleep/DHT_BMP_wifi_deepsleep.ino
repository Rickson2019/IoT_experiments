
#include "wifi_utils_v2.h"
#include "deep_sleep_utils.h"

void setup() {
  serial_setup();

  wifi_start_server();
  delay(15000);
}

void loop() {
  
}
