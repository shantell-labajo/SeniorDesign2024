/**
 * Complete project details at https://RandomNerdTutorials.com/arduino-load-cell-hx711/
 *
 * HX711 library for Arduino - example file
 * https://github.com/bogde/HX711
 *
 * MIT License
 * (c) 2018 Bogdan Necula
 *
**/

#include <Arduino.h>
#include "HX711.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;

HX711 scale;

void setup() {
  SerialUSB.begin(57600);
  SerialUSB.println("HX711 Demo");
  SerialUSB.println("Initializing the scale");

  pinMode(LED_BUILTIN, OUTPUT);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  SerialUSB.println("Before setting up the scale:");
  SerialUSB.print("read: \t\t");
  SerialUSB.println(scale.read());      // print a raw reading from the ADC

  SerialUSB.print("read average: \t\t");
  SerialUSB.println(scale.read_average(20));   // print the average of 20 readings from the ADC

  SerialUSB.print("get value: \t\t");
  SerialUSB.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight (not set yet)

  SerialUSB.print("get units: \t\t");
  SerialUSB.println(scale.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight (not set) divided
            // by the SCALE parameter (not set yet)
          
  scale.set_scale(-217);
  //scale.set_scale(-471.497);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();               // reset the scale to 0

  SerialUSB.println("After setting up the scale:");

  SerialUSB.print("read: \t\t");
  SerialUSB.println(scale.read());                 // print a raw reading from the ADC

  SerialUSB.print("read average: \t\t");
  SerialUSB.println(scale.read_average(20));       // print the average of 20 readings from the ADC

  SerialUSB.print("get value: \t\t");
  SerialUSB.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight, set with tare()

  SerialUSB.print("get units: \t\t");
  SerialUSB.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided
            // by the SCALE parameter set with set_scale

  SerialUSB.println("Readings:");
}

void loop() {

  SerialUSB.print("one reading:\t");
  SerialUSB.print(scale.get_units(), 1);
  //SerialUSB.print("\t| average:\t\n");
  //SerialUSB.println(scale.get_units(10), 5);
  if (scale.get_units(10) > 8 && scale.get_units(10) < 10) {
    SerialUSB.println("Valid chip weight.");
    digitalWrite(LED_BUILTIN, HIGH);  
    delay(1000);                     
    digitalWrite(LED_BUILTIN, LOW);  
    delay(1000);
  }

  delay(5000);
}
