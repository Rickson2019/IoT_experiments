#include "Adafruit_CCS811.h"
Adafruit_CCS811 ccs811;

// try heating up 10 times
void heat_up_ccs()
{

    for (int i = 0; i < 10; i++)
    {
        if (!ccs811.begin())
        {
            Serial.println("Failed to start sensor! Please check your wiring.");
            delay(100);
        }
    }
}
