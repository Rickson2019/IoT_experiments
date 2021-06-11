#include <Adafruit_BMP280.h>
Adafruit_BMP280 bmp; // I2C

void BMP_read(float *retval);
void BMP_heatup(float *retval);

void BMP_heatup()
{
    float dht_retval[2] = {0.00, 0.00};
    BMP_read(dht_retval);
    delay(1000);
}

void BMP_read(float *retval)
{
    Serial.println(F("BMP280:"));

    bmp.begin(0x76);

    // delay(2000);
    /* Default settings from datasheet. */
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                    Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

    float temp = bmp.readTemperature();
    float press = bmp.readPressure();

    retval[0] = temp;
    retval[1] = press;
}