#include <Servo.h>

Servo servoONE;
Servo servoTWO;
Servo servoTHREE;
Servo servoFOUR;
Servo servoFIVE;

int servoDelay=500;
int servoOpen=55;
int servoClose=0;
int chip;

void setup() {
  servoONE.attach(3);
  servoTWO.attach(5);
  servoTHREE.attach(6);
  servoFOUR.attach(9);
  servoFIVE.attach(10);

}

void loop() {
  chip = 12;
  if (chip == 5){
    servoONE.write(servoClose);
    servoTWO.write(servoOpen);
    servoTHREE.write(servoClose);
    servoFOUR.write(servoClose);
    servoFIVE.write(servoClose);
    delay(servoDelay);
    }
    else if(chip == 50){
      servoONE.write(servoClose);
      servoTWO.write(servoClose);
      servoTHREE.write(servoOpen);
      servoFOUR.write(servoClose);
      servoFIVE.write(servoClose);
      delay(servoDelay);
      }
        else if(chip = 100){
          servoONE.write(servoClose);
          servoTWO.write(servoClose);
          servoTHREE.write(servoClose);
          servoFOUR.write(servoOpen);
          servoFIVE.write(servoClose);
          delay(servoDelay);
          }
            else if(chip = 1000){
              servoONE.write(servoClose);
              servoTWO.write(servoClose);
              servoTHREE.write(servoClose);
              servoFOUR.write(servoClose);
              servoFIVE.write(servoOpen);
              delay(servoDelay);
              }
                else if(chip = 5000){
                  servoONE.write(servoClose);
                  servoTWO.write(servoClose);
                  servoTHREE.write(servoClose);
                  servoFOUR.write(servoClose);
                  servoFIVE.write(servoClose);
                  delay(servoDelay);
                }
                    else{
                      servoONE.write(servoOpen);
                      servoTWO.write(servoClose);
                      servoTHREE.write(servoClose);
                      servoFOUR.write(servoClose);
                      servoFIVE.write(servoClose);
                      delay(servoDelay);
                    }
}
