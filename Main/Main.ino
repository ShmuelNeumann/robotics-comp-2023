#include "MotorControl.h"
#include "MazeNavigator.h"


// This is a sample of the motor contr  oller.


void setup() {
    Serial.begin(9600);

    // Init motor pins:
    SetLeftPins(3, 6, 7);
    SetRightPins(5, 8, 9);

    SetMotor(0, 0);


}

void loop() {
    // go the first way through the maze
    NavigateMaze(5,5,1,1, true, true);

    // blink the LED

    // go the first way through the maze
    NavigateMaze(1,1,5,5, false, true);

    // blink the LED


    delay(10000);
}