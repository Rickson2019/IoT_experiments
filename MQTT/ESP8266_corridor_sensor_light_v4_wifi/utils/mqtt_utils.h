#include <PubSubClient.h>
//"wifiClient" defined in wifi_utils_v2.h
PubSubClient client(wifiClient);

void handle_change_deep_sleep_time(char *payload)
{
    Serial.println("handle_change_deep_sleep_time");

    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, payload);
    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }

    double deep_sleep_mode = doc["deep_sleep_mode"];
    double sleep_duration = doc["sleep_duration"];

    Serial.println(payload);
    Serial.println(deep_sleep_mode);
    Serial.println(sleep_duration);
}

void callback_function(char *topic, byte *payload, unsigned int length)
{

    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    String received_msg = "";

    StaticJsonDocument<200> doc;

    for (int i = 0; i < length; i++)
    {
        // Serial.print((char)payload[i]);
        received_msg += (char)payload[i];
    }

    Serial.println(received_msg);
    DeserializationError error = deserializeJson(doc, received_msg);

    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }
    const char *setting = doc["setting"];

    Serial.println(setting);

    if (setting == "deep_sleep")
    {
        Serial.println("deep_sleep");
        handle_change_deep_sleep_time(&received_msg[0]);
    }

    Serial.println();
    Serial.println("--------------- Done -----------------------");
}

//
void check_same_topic(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
}

// connect to endpoint
void connect_to_broker()
{
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback_function);

    while (!client.connected())
    {
        String client_id = "wemos-room-monitor-02";
        client_id += String(WiFi.macAddress());
        Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
        if (client.connect(client_id.c_str(), mqtt_username, mqtt_password))
        {
            Serial.println("Public emqx mqtt broker connected");
        }
        else
        {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }

    client.subscribe(topic);
    // publish and subscribe
    client.publish(topic, "{\"updating_device_info\" :true, \"device_name\": \"wemos-room-monitor-02\", \"sensors\":[\"DHT22\", \"BMP280\"]}");
}

// Update sensor data to MQTT
void mqtt_update(char *topic_endpoint, char *sensor_data_json)
{
    // publish to the mqtt server
    client.publish(topic_endpoint, &sensor_data_json[0]);
}

//void merge(JsonObject dest, JsonObjectConst src)
//{
//   for (auto kvp : src)
//   {
//     dest[kvp.key()] = kvp.value();
//   }
//}