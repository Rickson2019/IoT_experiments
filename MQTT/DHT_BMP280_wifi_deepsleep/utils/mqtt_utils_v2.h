#include <PubSubClient.h>

//"wifiClient" defined in wifi_utils_v2.h
PubSubClient client(wifiClient);
void callback_function(char *topic, byte *payload, unsigned int length)
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
        String client_id = "esp8266-client-";
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
