# ESP8266_AP_STA

## BACKGROUND
The experiments I have done previously were not quite flexible, you will have to modify the sketch in order to connect to the WiFi in the vicinity, change MQTT topics and so on.

I really wanted to create an interface for configurations, which will require the MCU to be put in WIFI_AP mode. This is actually achieved by a lot of people, just investigating the myriad of blogs and youtube vids. I also built one myself, so is it the end of the story? No, of course, there is a longer way to go.

When talking of IoT, people always expect the thing to be connected to a network, either it's the wifi network or a wireless network using amater radio frequencies like the LoRa modules can provide and eventually being accessible through the Internet. Therefore, the MCU should also be able to function in the WIFI_STA mode.

I was wondering how I could run the WiFi with the two different modes at the same time when I came across a StackOverflow question on WIFI_AP_STA mode for ESP8266 modules, which I immediately discovered that it is something I am looking for.

## PURPOSE
This is an experiment whose result will help enabling users to modify their devices's MQTT settings, WiFi ssid and password, types of sensors, sensor update rate and so on.


## REFERENCES
How to upload webpage files to an esp8266:
https://randomnerdtutorials.com/esp8266-web-server-spiffs-nodemcu/