// @Author JUDAO ZHONG
// Tested on NodeMCU

// ======= MQTT Broker settings ========
const char *mqtt_broker = "192.168.2.2";
char *topic = "wemos-room-monitor-02";
const char *mqtt_username = "";
const char *mqtt_password = "";
const int mqtt_port = 18080;
// -------------------------------------

#include "gy-302.h"

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

// TODO: use these
// #include "utils/wifi_utils.h" // configure wifi ssid and password here
// #include "utils/mqtt_utils.h"

// Developed using ESP8266 NodeMCU
// https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/
const int PIN_LIGHT_DIGITAL = D7; //A7
//const int PIN_LIGHT_DIGITAL = D0; //A0

const int PIN_HUMAN_DETECTOR = A0; //A0

const int PIN_ESP8266_SCL = D1; //D1
const int PIN_ESP8266_SDA = D2; //D2

const int PIN_BLUB_STRIPE_GND = D4; //D4

// 触发开灯的条件
const int VAL_DIM_LIGHT_THRES = 25;        // 亮度低于25
const int VAL_HUMAN_DETECTION_THRES = 600; // IR探头数值为600，判断为检测到有人经过

// 触发亮灯后，保持点亮的持续时长
const int VAL_LIGHTS_ON_MILLISECONDS = 10 * 1000; // 15  sec

// 控制该设备的WIFI账号与密码
char *ap_ssid = "ESP8266_corridor_light";
char *ap_password = "12345678";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

//bool LIGHT_MANUAL_ON = 0;
//bool LIGHT_MANUAL_OFF = 0;

//工作模式
enum modes
{
  AUTO_DETECTION,   // 自动检测是否开关
  LIGHT_MANUAL_ON,  // 手动开启
  LIGHT_MANUAL_OFF, // 手动关闭
};                  // 注意这里 ';' 不能漏

modes work_mode = AUTO_DETECTION;

void setup()
{
  Serial.begin(9600);
  pinMode(PIN_LIGHT_DIGITAL, INPUT);
  pinMode(PIN_BLUB_STRIPE_GND, OUTPUT);

  //GY-302 init, read light in Lux
  Wire.begin(PIN_ESP8266_SDA, PIN_ESP8266_SCL);

  // Begin Access Point
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ap_ssid, ap_password);
  Serial.println("softAP IP:");
  Serial.print(WiFi.softAPIP());
  Serial.println("AP_SSID");
  Serial.println(ap_ssid);
  Serial.println("AP_PASSWORD");
  Serial.println(ap_password);

  //    // Route for root / web page
  //  server.on("/on", HTTP_GET, [](AsyncWebServerRequest * request)
  //  {
  //    //    request->send(SPIFFS, "/index.html", String(), false, processor);
  //    request->send(SPIFFS, "/index.html", "text/html");
  //  });
  //

  // 开灯
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              //    LIGHT_MANUAL_ON = 1;
              //    LIGHT_MANUAL_OFF = 0;
              work_mode = LIGHT_MANUAL_ON;
            });

  // 关灯
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              //    LIGHT_MANUAL_ON = 0;
              //    LIGHT_MANUAL_OFF = 1;
              work_mode = LIGHT_MANUAL_OFF;
            });

  // 传感器自动探测
  server.on("/auto", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              //    LIGHT_MANUAL_ON = 0;
              //    LIGHT_MANUAL_OFF = 0;
              work_mode = AUTO_DETECTION;
            });
}

void loop()
{

  bool LIGHT_IS_DIM = 0;        // 外部环境亮时为0，暗时为1
  bool HUMAN_BODY_DETECTED = 0; // 检测到有人时为1，没有人时候为0

  // The user specifies it to run at AUTO_DETECTION mode
  if (work_mode == AUTO_DETECTION)
  {
    Serial.println("digitalRead(PIN_LIGHT_DIGITAL)");
    Serial.println(digitalRead(PIN_LIGHT_DIGITAL));

    if (digitalRead(PIN_LIGHT_DIGITAL) == 1)
    {
      LIGHT_IS_DIM = 1;
      Serial.println("LIGHT IS DIM");
    }

    if (analogRead(PIN_HUMAN_DETECTOR) >= VAL_HUMAN_DETECTION_THRES)
    {
      HUMAN_BODY_DETECTED = 1;
      Serial.println("HUMAN BODY DETECTED");
    }

    // The light is dim, and human body is detected:
    // 暗的时候，有人出现在探测范围内
    if (LIGHT_IS_DIM == 1 && (HUMAN_BODY_DETECTED == 1))
    {
      Serial.println("Lights ON !!!");
      digitalWrite(PIN_BLUB_STRIPE_GND, LOW);
      delay(VAL_LIGHTS_ON_MILLISECONDS);
    }
    // 环境太亮了则关灯
    else
    {
      Serial.println("Lights OFF !!!");
      digitalWrite(PIN_BLUB_STRIPE_GND, HIGH);
    }
  }

  //  The user manually turns off the light.
  //  开灯  //  The user manually turns off the light.
  else if (work_mode == LIGHT_MANUAL_ON)
  {
    lightsOn();
  }

  //  The user manually turns off the light.
  //  关灯
  else if (work_mode == LIGHT_MANUAL_OFF)
  {
    lightsOff();
  }
}

void lightsOff()
{
  digitalWrite(PIN_BLUB_STRIPE_GND, HIGH);
}

void lightsOn()
{
  digitalWrite(PIN_BLUB_STRIPE_GND, LOW);
}

void lightsDetectionMode()
{
  digitalWrite(PIN_BLUB_STRIPE_GND, LOW);
}

// 通过BH1750读取LUX亮度
float read_lux()
{
  uint16_t val = 0;

  BH1750_Init(BH1750address);

  if (2 == BH1750_Read(BH1750address))

  {

    val = ((buff[0] << 8) | buff[1]) / 1.2;

    Serial.print(val, DEC);

    Serial.println("[lx]");
  }
  return val;
}
