// -------------------- Network Settings --------------------
// Replace with credentials for your WiFi
// that is connected to the Internet(for STA mode)
// 接入互联网的WiFi名和密码(STA mode)
char *sta_ssid = "NO.1";
char *sta_password = "Home84039746";

// Replace with credentials for the WiFi AccessPoint
// that is running on your ESP8266
// 8266本机的AP名和密码(AP mode)
char *ap_ssid = "ESP8266_Weather_Station";
char *ap_password = "12345678";

// 获取原Wifi名字
// sta_ssid getter()
String get_sta_ssid()
{
    return sta_ssid;
}

// 获取原密码
// sta_password getter()
String get_sta_password()
{
    return sta_ssid;
}

// 设置要连接的WiFi名 New WiFi SSID you want to connect to
// 和 and
// 设置新的密码 New password for that WiFi
void set_sta_ssid_password(String new_sta_ssid, String new_sta_password)
{
    sta_ssid = &new_sta_ssid[0];
    sta_password = &new_sta_password[0];
}
// ____________________________________________________________

// ------------------ Power Saving Sleep Settings ---------------
// for deepsleep config
int deepsleep_minutes = 0;
int deepsleep_seconds = 0;

//
int get_deepsleep_minutes()
{
    return deepsleep_minutes;
}

//
int get_deepsleep_seconds()
{
    return deepsleep_seconds;
}

//
void set_deepsleep_minutes(int ds_minutes)
{
    deepsleep_minutes = ds_minutes;
}

//
void set_deepsleep_seconds(int ds_seconds)
{
    deepsleep_seconds = ds_seconds;
}
// ______________________________________________________________