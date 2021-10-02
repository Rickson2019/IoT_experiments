
char *topic = "wemos-room-monitor"; // device name
char *mqtt_username = "";           // mqtt server's credentials
char *mqtt_password = "";           // mqtt server's credentials
int mqtt_port = 18080;              // port that mqtt server runs on
#include "utils/wifi_utils_v2.h"    // Connection comes first

#include "utils/ccs811_utils.h" // Install : Adafruit CCS811 Library by Adafruit
#include "utils/dht_utils_v2.h"
#include "utils/mqtt_utils_v2.h" // Requires WiFi connection functions like the first import at the top

void setup()
{
  Serial.begin(9600);
  wifi_connect(); // connects to the hotspot
  heat_up_ccs();
  connect_to_broker(); // connects to the node.js hosted aedes server

  // load_mqtt_config(topic, mqtt_username, mqtt_password, mqtt_port);
}

void loop()
{
  if (ccs811.available())
  {
    //    TODO: Find out WHY !css811
    if (!ccs811.readData())
    //    if (ccs811.readData())
    {
      // mqtt_update("CCS811", &CCS811_read()[0]);
      mqtt_update("wemos-room-monitor", &CCS811_read()[0]);
    }
    else
    {
      Serial.println("ERROR!");
    }
  }
  //  mqtt_update("DHT22", &DHT_read()[0]);
  mqtt_update("wemos-room-monitor", &DHT_read()[0]);

  Serial.println(CCS811_read());
  Serial.println(DHT_read());

  delay(500);
}
