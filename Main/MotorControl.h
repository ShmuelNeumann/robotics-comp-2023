// Note this function is required to create custom importable functions, along with the `.ccp` file.
// This .h file contains the basic "blueprints" of the functions in the .cpp file.

# include "Arduino.h"

void SetRightPins(int enable, int in1, int in2);
void SetLeftPins(int enable, int in1, int in2);

void SetMotor(float leftSpeed, float rightSpeed);

void InvertRight();
void InvertLeft();