
#include "utils/RFID_utils.h"
#include "utils/OLED_utils.h"

#define DOOR_UNLOCKED true
#define DOOR_LOCKED false

byte white_listed[4] = {0xFC, 0x89, 0x14, 0x39};
bool door_status = DOOR_LOCKED;

void setup()
{
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  Serial.begin(115200);                                // Initialize serial communications with the PC
  SPI.begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_SS); // Init HARDWARE SPI bus
  mfrc522.PCD_Init();                                  // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial();                   // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));

  init_oled();

}

void loop()
{


  //  check_card_reader();
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  //  byte* retval =  get_uid();

  //  Serial.println("===get_uid()[i], HEX===");
  //
  //  for (int i = 0 ; i < 3; i ++) {
  //    Serial.print(retval[i], HEX);
  //  }
  //  Serial.println();
  //  Serial.println("---get_uid()[i], HEX---");
  //
  //  for (byte i = 0; i < 4; i++)
  //  {
  //    if (white_listed[i] < 0x10)
  //      Serial.print(F(" 0"));
  //    else
  //      Serial.print(F(" "));
  //    Serial.print(white_listed[i], HEX);
  //  }
  //  Serial.println();


  if ( memcmp(get_uid(), white_listed, 4 ) == 0) {
    if (door_status == DOOR_LOCKED) {
      door_status = DOOR_UNLOCKED;

      digitalWrite(2, LOW);
      delay(300);
      digitalWrite(2, HIGH);

      Serial.println("The door is UNLOCKED");
    } else {
      door_status = DOOR_LOCKED;
      draw_locked();

      digitalWrite(2, LOW);
      delay(50);
      digitalWrite(2, HIGH);
      delay(50);
      digitalWrite(2, LOW);
      delay(50);
      digitalWrite(2, HIGH);
      delay(50);
      digitalWrite(2, LOW);
      delay(50);
      digitalWrite(2, HIGH);

      Serial.println("The door is LOCKED");
    }
  }
  else {
    Serial.println("Invalid Card!");
  }

  if (door_status == DOOR_LOCKED) {
    draw_unlocked();
  }
  else {
    draw_locked();
  }

  delay(2000);

}
