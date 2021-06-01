// HC12
SoftwareSerial HC12(10, 11); // HC-12 TX Pin, HC-12 RX Pin

void HC12_write()
{

    while (HC12.available())
    {                              // If HC-12 has data
        Serial.write(HC12.read()); // Send the data to Serial monitor
    }
}

void HC12_read()
{
    while (Serial.available())
    {                              // If Serial monitor has data
        HC12.write(Serial.read()); // Send that data to HC-12
    }
}
