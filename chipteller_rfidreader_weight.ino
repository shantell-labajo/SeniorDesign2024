//libraries to include for project
#include <SPI.h>
#include <MFRC522.h>
#include <Arduino.h>
#include "HX711.h"

//for rfid
#define SS_PIN 10
#define RST_PIN 5
#define BL_LED 8

// // Define stepper motor connections and steps per revolution:
#define dirPin 31
#define stepPin 32
#define stepsPerRevolution 200

//initialize rfid module
MFRC522 rfid(SS_PIN, RST_PIN);

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;
HX711 scale;

// "database" value to be read from $100 poker chip
byte chip100block[] = {0xAA, 0xBB, 0xCC, 0xDD};

enum chipValue {FIVE, FIFTY, HUNDRED, THOUSAND, FIVETHOUSAND};
chipValue chip; 

// for counting chips
int chipTotal = 0;
bool doneCounting = false;

void setup() {

  pinMode(BL_LED, OUTPUT);
  Serial.begin(57600);
  SPI.begin(); // init SPI bus
  rfid.PCD_Init(); // init MFRC522

  // motor pins
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  //load cell weight initialize
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(113);
  scale.tare();

  delay(3000);
  Serial.println("Setup success."); //for rfid reader
  // Reset the loop if no new chip present on the sensor/reader. This saves the entire process when idle.
	Serial.println("Please insert chips.");
}

void loop() {
  
  if ( ! rfid.PICC_IsNewCardPresent()) {
		return;
	}

	// Select one of the cards
	if ( ! rfid.PICC_ReadCardSerial()) {
		return;
	}

  Serial.println("Chip inserted.");
  MFRC522::StatusCode status;
  byte blockAddr = 10; //page we want to read from chip
  byte buffer[18];
  byte size = sizeof(buffer);

  //for matching rfid values stored in poker chip
  bool pageMatch = true;

  // reading data in chip to match database
  Serial.print(F("Reading data from page ")); Serial.print(blockAddr);
  Serial.println(F("..."));
  status = (MFRC522::StatusCode) rfid.MIFARE_Read(blockAddr, buffer, &size);
  if (status != MFRC522::STATUS_OK) {
      Serial.print(F("MIFARE_Read() failed: "));
      Serial.println(rfid.GetStatusCodeName(status));
  }
  Serial.print(F("Data in page ")); Serial.print(blockAddr); Serial.println(F(":"));
  dump_byte_array(buffer, 4); 
  Serial.println();
  
  Serial.print(F("Database value: ")); dump_byte_array(chip100block, 4); Serial.println();

  // Matching page of database to chip that was read.
  for (byte i = 0; i < sizeof(chip100block); i++) {
    if (buffer[i] != chip100block[i]) {
      Serial.println("No match.");
      pageMatch = false;
      break;
    }
    else {
      Serial.println("Match.");
    }
  }

  // No match
  if (!pageMatch) {
    Serial.println("Invalid chip.");
    delay(1000);
  }
  // match
  else {
    chip = HUNDRED;
    digitalWrite(BL_LED, HIGH);
    delay(1000);
    digitalWrite(BL_LED, LOW);
  }

  //move motor here
  spin_motor();

  //perform weight check
  bool is_valid_weight = true;
  Serial.println("Begin weight check.");
  scale.tare();

  delay(2000);
  Serial.print(scale.get_units(),1); Serial.println(" g");

  if (scale.get_units(10) > 4 && scale.get_units(10) < 6) {
    Serial.println("\nValid chip weight.");
    digitalWrite(LED_BUILTIN, HIGH);  
    delay(1000);                     
    digitalWrite(LED_BUILTIN, LOW);  
    delay(1000);
  }
  else {
   // is_valid_weight = false;
  }

  //move motor here
  spin_motor();

  //sorting logic w/ other motors here
  //if any boolean logic is fail; chips get spat out into reject pile

  // chip total logic; check if all booleans are true
  if (pageMatch && is_valid_weight) {
    chipTotal += 100;
  }

  delay(3000);

  // check if more chips are being inserted
  if (rfid.PICC_IsNewCardPresent()) {
		return;
	}
  else {
    Serial.println("Total: "); Serial.println(chipTotal);
    chipTotal = 0;
  }

}

// Helper routine to dump a byte array as hex values to Serial.
void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}

// function to spin stepper motors for transfer of poker chips
void spin_motor() {
// Set the spinning direction counterclockwise:
  digitalWrite(dirPin, LOW);

  // Spin the stepper motor 1 revolution 
  for (int i = 0; i < stepsPerRevolution; i++) {
    // These four lines result in 1 step:
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(2000);
  }
}
