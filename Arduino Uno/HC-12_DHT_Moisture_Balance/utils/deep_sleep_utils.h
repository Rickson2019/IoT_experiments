#include <LowPower.h>

// used in setup()
void lowPowerSetup()
{
    CLKPR = 0x80;
    CLKPR = 0x01;
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
}

void fallAsleep()
{
    digitalWrite(LED_BUILTIN, LOW);
    LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF,
                  SPI_OFF, USART0_OFF, TWI_OFF);
    Serial.println("Arduino:- Hey I just Woke up");
    Serial.println("");
    delay(2000);
}