
void deep_sleep_for_seconds(int seconds)
{
    int sleep_micro_seconds = seconds * 1000 * 1000;
    Serial.println("I'm going into deep sleep mode for" + String(seconds) + " seconds");
    ESP.deepSleep(sleep_micro_seconds);
}

void deep_sleep_for_minutes(int minutes)
{
    // 60 sec 10e6 us
    int sleep_micro_seconds = minutes * 60 * 1000 * 1000;
    Serial.println("I'm going into deep sleep mode for" + String(minutes) + " minutes");
    ESP.deepSleep(sleep_micro_seconds);
}

void deep_sleep_for_hours(int hours)
{
    // 60min 60 sec 10e6 us
    int sleep_micro_seconds = hours * 60 * 60 * 1000 * 1000;
    Serial.println("I'm going into deep sleep mode for" + String(hours) + " hours");
    ESP.deepSleep(sleep_micro_seconds);
}

void serial_setup()
{
    Serial.begin(9600);
    Serial.setTimeout(2000);
    while (!Serial)
    {
    }
}