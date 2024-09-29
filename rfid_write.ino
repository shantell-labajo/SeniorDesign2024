/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-rfid-nfc
 */

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 5
#define BL_LED 8

MFRC522 rfid(SS_PIN, RST_PIN);

// void setup() {
//   pinMode(BL_LED, OUTPUT);
//   Serial.begin(9600);
//   SPI.begin(); // init SPI bus
//   rfid.PCD_Init(); // init MFRC522

//   delay(3000);
//   Serial.println("Setup success.");
// }

// void loop() {
//   if (rfid.PICC_IsNewCardPresent()) { // new tag is available
//     if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
//       digitalWrite(BL_LED, HIGH);
//       MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
//       Serial.print("RFID/NFC Tag Type: ");
//       Serial.println(rfid.PICC_GetTypeName(piccType));

//       // print NUID in Serial Monitor in the hex format
//       Serial.print("UID:");
//       for (int i = 0; i < rfid.uid.size; i++) {
//         Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
//         Serial.print(rfid.uid.uidByte[i], HEX);
//       }
//       Serial.println();

//       rfid.PICC_HaltA(); // halt PICC
//       rfid.PCD_StopCrypto1(); // stop encryption on PCD
//       delay(1000);
//       digitalWrite(BL_LED, LOW);
//     }
//   }
  
// }
void setup() {
	Serial.begin(9600);		// Initialize serial communications with the PC
	while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
	SPI.begin();			// Init SPI bus
	rfid.PCD_Init();		// Init MFRC522
	delay(4);				// Optional delay. Some board do need more time after init to be ready, see Readme
	rfid.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop() {
	// Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
	if ( ! rfid.PICC_IsNewCardPresent()) {
		return;
	}

	// Select one of the cards
	if ( ! rfid.PICC_ReadCardSerial()) {
		return;
	}

  byte page = 10;  // Page we want to write to (4 to 15 for writable pages in Ultralight)
  byte dataPage[] = { 0xAA, 0xBB, 0xCC, 0xDD };  // Example data (4 bytes)

  // Write data to the page (4 bytes for Ultralight)
  MFRC522::StatusCode status = rfid.MIFARE_Write(page, dataPage, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Write failed: "));
    Serial.println(rfid.GetStatusCodeName(status));
    delay(1000);
    return;
  }

  Serial.println("Write successful!");

	// Dump debug info about the card; PICC_HaltA() is automatically called
	rfid.PICC_DumpMifareUltralightToSerial();

  Serial.println(F("Finished reading pages."));

  delay(3000);

}
