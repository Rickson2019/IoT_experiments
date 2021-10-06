
// ======= MQTT Broker settings ========
const char *mqtt_broker = "192.168.2.2";
char *topic = "wemos-room-monitor-02";
const char *mqtt_username = "";
const char *mqtt_password = "";
const int mqtt_port = 18080;
// -------------------------------------

// Deep Sleep Related
enum SLEEP_MODE
{
  SLEEP_FOR_HOURS,
  SLEEP_FOR_MINUTES,
  SLEEP_FOR_SECONDS
};

SLEEP_MODE deep_sleep_mode = SLEEP_FOR_SECONDS;

int sleep_duration = 0;
// --------------------

#include <ArduinoJson.h>
#include "utils/wifi_utils_v2.h" // configure wifi ssid and password here
#include "utils/deep_sleep_utils.h"
#include "utils/mqtt_utils_v2.h" // - configure broker settings here.
// - edit your call back function here
#include "utils/bmp280_utils_v2.h"
#include "utils/dht_utils_v2.h"

#include <Wire.h>

void setup()
{

  serial_setup();
  wifi_connect();
  //  wifi_start_server();
  connect_to_broker();
}

/**
   https://stackoverflow.com/questions/67639065/mqtt-client-not-subscribing-to-a-given-topic-or-callback-not-working-as-intende
   You need a client.loop(); line in your loop() main function. Without that line, you MQTT code never gets executed.
*/
void loop()
{
  client.loop(); // Something you must have.
  //  mqtt_update(topic, &read_BMP()[0]);
  //  mqtt_update(topic, &DHT_read()[0]);

  //  delay(10000); //间歇3秒更新
  //  Serial.println("10 sec passed");
  //  deep_sleep_for_seconds(5);
  if (sleep_duration != 0)
  {
    switch (deep_sleep_mode)
    {
    case SLEEP_FOR_HOURS:
    {
      deep_sleep_for_hours(sleep_duration);
    }
    case SLEEP_FOR_MINUTES:
    {
      deep_sleep_for_minutes
    }
    }
  }
}
