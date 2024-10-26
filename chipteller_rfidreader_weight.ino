//libraries to include for project
#include <SPI.h>
#include <MFRC522.h>
#include <Arduino.h>
#include "HX711.h"
#include <Servo.h>

//for rfid
#define SS_PIN 10
#define RST_PIN 5
#define BL_LED 8

// Define stepper motor connections and steps per revolution:
#define dirPin1 31
#define stepPin1 32
#define dirPin2 33
#define stepPin2 34
#define stepsPerRevolution 100

//for servos
Servo servoONE;
Servo servoTWO;
Servo servoTHREE;
Servo servoFOUR;
Servo servoFIVE;

int servoDelay=250;
int servoOpen=55;
int servoClose=0;

//initialize rfid module
MFRC522 rfid(SS_PIN, RST_PIN);

// HX711 circuit wiring
// const int LOADCELL_DOUT_PIN = 50;
// const int LOADCELL_SCK_PIN = 51;
// HX711 scale;

// "database" value to be read from poker chip
byte chip5block[] = {0x4E, 0xF8, 0x2A, 0xA6};
byte chip50block[] = {0x01, 0x68, 0xE7, 0xFD};
byte chip100block[] = {0x53, 0xCF, 0xA1, 0x4D};
byte chip1000block[] = {0xCD, 0x93, 0x27, 0xE1};
byte chip5000block[] = {0x4c, 0x8A, 0x05, 0x26};

// Array of pointers to each block
byte* blocks[] = { chip5block, chip50block, chip100block, chip1000block, chip5000block };

enum chipValue {FIVE, FIFTY, HUNDRED, THOUSAND, FIVETHOUSAND, OTHER};
chipValue chip = OTHER; 

// for counting chips
int chipTotal = 0;
bool doneCounting = false;
int const timeDelay = 100;

//for matching rfid values stored in poker chip
bool pageMatch = false;

void setup() {

  pinMode(BL_LED, OUTPUT);
  Serial.begin(57600);
  SPI.begin(); // init SPI bus
  rfid.PCD_Init(); // init MFRC522

  // servo setup
  servoONE.attach(40);   
  servoTWO.attach(41);  
  servoTHREE.attach(42); 
  servoFOUR.attach(43); 
  servoFIVE.attach(44);

  // motor pins
  pinMode(stepPin1, OUTPUT);
  pinMode(dirPin1, OUTPUT);
  pinMode(stepPin2, OUTPUT);
  pinMode(dirPin2, OUTPUT);

  delay(3000);

  // load cell weight initialize
  // Serial.println("Load cell weight setup.\n");
  // scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  // scale.set_scale(113);
  // scale.tare();
  // Serial.println("Done.\n");

  // delay(3000);
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

  spin_motor();

  Serial.println("Chip inserted.");
  MFRC522::StatusCode status;
  byte blockAddr = 10; //page we want to read from chip
  byte buffer[18];
  byte size = sizeof(buffer);

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
  
  //Serial.print(F("Database value: ")); dump_byte_array(chip100block, 4); Serial.println();

  // Matching page of database to chip that was read.
  // for (byte i = 0; i < sizeof(chip5block); i++) {
  //   if (buffer[i] != chip5block[i]) {
  //     //Serial.println("No match.");
  //     pageMatch = false;
  //     break;
  //   }
  //   else {
  //     Serial.println("Match.");
  //   }
  // }
  checkForMatch(buffer);

  // No match
  // if (!pageMatch) {
  //   Serial.println("Invalid chip.");
  //   delay(500);
  //   chip = OTHER;
  // }
  // // match
  // else {
  //   chip = HUNDRED;
  //   // digitalWrite(BL_LED, HIGH);
  //   // delay(1000);
  //   // digitalWrite(BL_LED, LOW);
  // }

  //move motor here
  spin_motor();

  //perform weight check
  bool is_valid_weight = true;
  // Serial.println("Begin weight check.");
  // scale.tare();

  // delay(1000);
  // Serial.print(scale.get_units(),1); Serial.println(" g");

  // if (scale.get_units(10) > 4 && scale.get_units(10) < 6) {
  //   Serial.println("\nValid chip weight.");
  //   digitalWrite(LED_BUILTIN, HIGH);  
  //   delay(1000);                     
  //   digitalWrite(LED_BUILTIN, LOW);  
  //   delay(1000);
  // }
  // else {
  //  // is_valid_weight = false;
  // }

  //move motor here
  delay(100);
  spin_motor();

  //sorting logic w/ other motors here
  //if any boolean logic is fail; chips get spat out into reject pile
  switch(chip) {
        case FIVE:
            setServos(false, true, false, false, false);
            break;
        case FIFTY:
            setServos(false, false, true, false, false);
            break;
        case HUNDRED:
            setServos(false, false, false, true, false);
            break;
        case THOUSAND:
            setServos(false, false, false, false, true);
            break;
        case FIVETHOUSAND:
            setServos(false, false, false, false, false);
            break;
        default:
            setServos(true, false, false, false, false);
            break;
  }

  // chip total logic; check if all booleans are true
  if (pageMatch && is_valid_weight) {
    switch(chip) {
        case FIVE:
            chipTotal += 5;
            break;
        case FIFTY:
            chipTotal += 50;
            break;
        case HUNDRED:
            chipTotal += 100;
            break;
        case THOUSAND:
            chipTotal += 1000;
            break;
        case FIVETHOUSAND:
            chipTotal += 5000;
            break;
        default:
            chipTotal += 0;
            break;
    }
  }

  int i = 0;
  // check if more chips are being inserted
  while ( i < timeDelay) {

    if (rfid.PICC_IsNewCardPresent()) {
		  return;
    }

    Serial.print(".");
    i++;
	}

  Serial.println("Total: "); Serial.println(chipTotal);
  chipTotal = 0;

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
  digitalWrite(dirPin1, LOW);
  digitalWrite(dirPin2, LOW);

  // Spin the stepper motor 1 revolution 
  for (int i = 0; i < stepsPerRevolution; i++) {
    // These four lines result in 1 step:
    digitalWrite(stepPin1, HIGH);
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin1, LOW);
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }
}

// function to move the servo motors
void setServos(bool one, bool two, bool three, bool four, bool five) {

    servoONE.write(one ? servoOpen : servoClose);
    servoTWO.write(two ? servoOpen : servoClose);
    servoTHREE.write(three ? servoOpen : servoClose);
    servoFOUR.write(four ? servoOpen : servoClose);
    servoFIVE.write(five ? servoOpen : servoClose);
    delay(servoDelay);

}

// Function to check if chip matches 
void checkForMatch(byte* buffer) {
  // Iterate over each block in the database
  for (int j = 0; j < sizeof(blocks) / sizeof(blocks[0]); j++) {
    if (matchPage(buffer, blocks[j])) {
      Serial.println("Match found.");
      pageMatch = true;

      switch(j)  {
          case 0:
            chip = FIVE;
            break;
          case 1:
            chip = FIFTY;
            break;
          case 2:
            chip = HUNDRED;
            break;
          case 3:
            chip = THOUSAND;
            break;
          case 4:
            chip = FIVETHOUSAND;
            break;
          default:
            break;
      }

      break; // Exit the loop once a match is found
    }
  }

  if (!pageMatch) {
    Serial.println("No match found.");
    delay(100);
    chip = OTHER;
  }
}

// Function to compare two 8-byte arrays
bool matchPage(byte* buffer, byte* block) {

  Serial.print(F("Database value: ")); dump_byte_array(block, 4); Serial.println();
  for (byte i = 0; i < 4; i++) {
    //Serial.print(buffer[i], HEX); Serial.print(" "); Serial.println(block[i], HEX);
    if (buffer[i] != block[i]) {
      return false; // If any byte doesn't match, return false
    }
  }
  return true; // All bytes matched
}

