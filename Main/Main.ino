#include "MotorControl.h"

// This is a sample of the motor controller.


void setup() {
    Serial.begin(9600);

    // Init pins:
    SetLeftPins(3, 7, 8);
    SetRightPins(5, 11, 10);

    // If required, you can invert the direction of one of the motors.
    // e.g. if the right motor is going the wrong way:
    InvertRight();




    
}

void loop() {
    // This is the code to go straight, stop, go backwards at half speed, stop, then do a right turn.

}