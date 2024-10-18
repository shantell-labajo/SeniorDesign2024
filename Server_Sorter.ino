#include <Servo.h>

Servo servoONE;
Servo servoTWO;
Servo servoTHREE;
Servo servoFOUR;
Servo servoFIVE;

int servoDelay=500;
int servoOpen=55;
int servoClose=0;
int chip=0;

void setup() {
    servoONE.attach(3);   
    servoTWO.attach(5);  
    servoTHREE.attach(6); 
    servoFOUR.attach(9); 
    servoFIVE.attach(10);  
}

void setServos(bool one, bool two, bool three, bool four, bool five) {
    servoONE.write(one ? servoOpen : servoClose);
    servoTWO.write(two ? servoOpen : servoClose);
    servoTHREE.write(three ? servoOpen : servoClose);
    servoFOUR.write(four ? servoOpen : servoClose);
    servoFIVE.write(five ? servoOpen : servoClose);
    delay(servoDelay);
}

void loop() {
    switch(chip) {
        case 5:
            setServos(false, true, false, false, false);
            break;
        case 50:
            setServos(false, false, true, false, false);
            break;
        case 100:
            setServos(false, false, false, true, false);
            break;
        case 1000:
            setServos(false, false, false, false, true);
            break;
        case 5000:
            setServos(false, false, false, false, false);
            break;
        default:
            setServos(true, false, false, false, false);
            break;
    }
}
