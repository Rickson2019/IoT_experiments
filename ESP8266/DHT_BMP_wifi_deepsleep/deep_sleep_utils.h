
void deep_sleep_for_seconds(int seconds)
{
    int sleep_micro_seconds = seconds * 1000 * 1000;
    Serial.println("I'm awake, but I'm going into deep sleep mode for" + String(seconds) + " seconds");
    ESP.deepSleepInstant(sleep_micro_seconds);
}

void serial_setup()
{
    Serial.begin(9600);
    Serial.setTimeout(2000);
    while (!Serial)
    {
    }
}