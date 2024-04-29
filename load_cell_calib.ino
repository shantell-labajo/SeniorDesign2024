/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/arduino-load-cell-hx711/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

// Calibrating the load cell
#include <Arduino.h>
#include "HX711.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;

HX711 scale;

void setup() {
  SerialUSB.begin(57600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
}

void loop() {

  
  if (scale.is_ready()) {
    scale.set_scale();    
    SerialUSB.println("Tare... remove any weights from the scale.");
    delay(5000);
    scale.tare();
    SerialUSB.println("Tare done...");
    SerialUSB.print("Place a known weight on the scale...");
    delay(5000);
    long reading = scale.get_units(10);
    SerialUSB.print("Result: ");
    SerialUSB.println(reading);

    scale.set_scale(reading);

    Serial.print("one reading:\t");
    Serial.print(scale.get_units(), 1);
    Serial.print("\t| average:\t");
    Serial.println(scale.get_units(10), 5);

  } 
  else {
    SerialUSB.println("HX711 not found.");
  }
  delay(1000);
}

//calibration factor will be the (reading)/(known weight)
//around (-)1450-1550 with the chip next to the load cell / 9 (grams)
