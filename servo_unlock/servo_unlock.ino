/*
 * Using servo.write(); with a delay() between a locked and unlocked position does not work. 
 * Using a variable to define initial lock and unlock state then toggling state seemed to work.
 */

// Servo Library
#include <Servo.h>

Servo LockServo;  // create servo object to control a servo
int POS = 0;      // variable to store the servo position
int LOCKED = 1;   // default lock state

void setup() {
   LockServo.attach(9);  // attaches the servo on pin 9 to the servo object
   LockServo.write(0);
   Serial.begin(9600);
}

void loop() {
  if (LOCKED == 1) {
    Serial.print("unlocking");
    LockServo.write(100);
    delay(2000);
    LOCKED = 0;
  } else if (LOCKED == 0) {
    Serial.print("locking");
    LockServo.write(0);
    delay(2000);
    LOCKED = 1;
  }
}
