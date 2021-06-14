#include <SPI.h>
#include <MFRC522.h>

//USES HARDWARE SPI INSTEAD OF THE SOFTWARE ONE!
#define HSPI_MISO 12
#define HSPI_MOSI 13
#define HSPI_SCLK 14
#define HSPI_SS 15

const int RST_PIN = 26; // Configurable, see typical pin layout above
const int SS_PIN = 15;  // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

void init_rfid(int ss_pin, int rst_pin)
{
    MFRC522 mfrc522_obj(ss_pin, rst_pin); // Create MFRC522 instance
    mfrc522 = mfrc522_obj;
}

// immediately returns if no card is
int check_card_reader()
{
    // Look for new cards
    if (!mfrc522.PICC_IsNewCardPresent())
    {
        return EXIT_SUCCESS;
    }

    // Select one of the cards
    if (!mfrc522.PICC_ReadCardSerial())
    {
        return EXIT_SUCCESS;
    }
}

byte *get_uid()
{
    byte rfid[] = {0, 0, 0, 0};
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
        if (mfrc522.uid.uidByte[i] < 0x10)
            Serial.print(F(" 0"));
        else
            Serial.print(F(" "));
        Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println();

    return mfrc522.uid.uidByte;
}
