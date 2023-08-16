#include "MotorControl.h"

// This is a sample of the motor controller.


void setup() {
    Serial.begin(9600);

    // Init pins:
    SetLeftPins(1, 2, 3);
    SetRightPins(4, 5, 6);

    // If required, you can invert the direction of one of the motors.
    // e.g. if the right motor is going the wrong way:
    InvertRight();
    
}

void loop() {
    // This is the code to go straight, stop, go backwards at half speed, stop, then do a right turn.


    SetMotor(1, 1); // full speed ahead
    delay(2000); // wait 2 seconds

    SetMotor(0, 0); // stop
    delay(1000); // wait a second

    SetMotor(-0.5, -0.5); // go backwards at half speed
    delay(2000); // wait 2 seconds

    SetMotor(0, 0); // stop
    delay(1000); // wait a second

    SetMotor(-0.5, -0.5); // do a sharp right turn at half speed
    delay(2000); // wait 2 seconds

    SetMotor(0, 0); // stop
}