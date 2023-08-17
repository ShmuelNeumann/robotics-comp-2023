// Note that to create custom importable functions, you also need a .h file.
// See that one for more details.

# include "MotorControl.h"

// Pins for the right/left motors
// Initialised to -1
int r_enable = -1;
int r_in1 = -1;
int r_in2 = -1;

int l_enable = -1;
int l_in1 = -1;
int l_in2 = -1;

// Vars to invert the individual motor direction
// `1` is the normal direction, `-1` is the inverted direction
int rightMultiplier = 1;
int leftMultiplier = 1;

// Used to assign the pin numbers for the right motor. This must be run before SetMotor().
// `enable` needs to be analog, `in1` and `in2` should be digital
void SetRightPins(int enable, int in1, int in2) {
  r_enable = enable;
  r_in1 = in1;
  r_in2 = in2;
}

// Used to assign the pin numbers for the left motor. This must be run before SetMotor().
// `enable` needs to be analog, `in1` and `in2` should be digital
void SetLeftPins(int enable, int in1, int in2) {
  l_enable = enable;
  l_in1 = in1;
  l_in2 = in2;
}


// Used to run the motors. SetLeftPins() and SetRightPins() must be run first.
//
// `leftSpeed` / `rightSpeed` each range from -1 to 1,
// where 0 is neutral, 1 is full speed forward, -1 is full speed backwards.
void SetMotor(float leftSpeed, float rightSpeed) {

  // Check that the pins have been assigned.
  if (r_enable == -1 || r_in1 == -1 || r_in2 == -1 ||l_enable == -1 || l_in1 == -1 || l_in2 == -1) {
    Serial.println("Missing Pin Allocations!\nRun SetLeftPins() and SetRightPins() before SetMotor()");
  }

  // Set the right motor in1/in2 to forward or backwards.
  if ((rightSpeed > 0 && rightMultiplier == 1) || (rightSpeed < 0 && rightMultiplier == -1)) {
    digitalWrite(r_in1, 1);
    digitalWrite(r_in2, 0);
  } 
  else {
    digitalWrite(r_in1, 0);
    digitalWrite(r_in2, 1);
  }
  // Set the speed of the right motor.
  analogWrite(r_enable, abs(rightSpeed * 255));


  // Set the left motor in1/in2 to forward or backwards.
  if ((leftSpeed > 0 && leftMultiplier == 1) || (leftSpeed < 0 && leftMultiplier == -1)) {
    digitalWrite(l_in1, 1);
    digitalWrite(l_in2, 0);
  } 
  else {
    digitalWrite(l_in1, 0);
    digitalWrite(l_in2, 1);
  }
  // Set the speed of the right motor.
  analogWrite(l_enable, abs(leftSpeed * 255));
}

// If the right motor is going the wrong way, call this value in the Setup() function to swap the direction.
void InvertRight() {
  rightMultiplier *= -1;
}

// If the left motor is going the wrong way, call this value in the Setup() function to swap the direction.
void InvertLeft() {
  leftMultiplier *= -1;
}
