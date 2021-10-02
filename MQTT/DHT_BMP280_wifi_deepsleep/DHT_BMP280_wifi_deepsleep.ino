
// MQTT Broker
const char *mqtt_broker = "192.168.2.2";
char *topic = "wemos-room-monitor-02";
const char *mqtt_username = "";
const char *mqtt_password = "";
const int mqtt_port = 18080;

#include <ArduinoJson.h>
#include "utils/wifi_utils_v2.h"
#include "utils/deep_sleep_utils.h"
#include "utils/mqtt_utils_v2.h"
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

void loop()
{

  mqtt_update(topic, &read_BMP()[0]);
  mqtt_update(topic, &DHT_read()[0]);

  delay(3000); //间歇3秒更新
//  deep_sleep_for_seconds(5);
}

//void merge(JsonObject dest, JsonObjectConst src)
//{
//   for (auto kvp : src)
//   {
//     dest[kvp.key()] = kvp.value();
//   }
//}
