/*
 * Using servo.write(); with a delay() between a locked and unlocked position does not work. 
 * Using a variable to define initial lock and unlock state then toggling state seemed to work.
 */

// Servo Library
#include <Servo.h>

#define BUTTON_PIN        2  // Button
#define DELAY            20  // Delay per loop in ms

Servo LockServo;  // create servo object to control a servo
int POS = 0;      // variable to store the servo position
int LOCKED = 1;   // default lock state


boolean handle_button(){
  int button_pressed = !digitalRead(BUTTON_PIN); // pin low -> pressed
  return button_pressed;
}


void setup() {
  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(BUTTON_PIN, HIGH); // pull-up
  Serial.begin(9600);
  
   LockServo.attach(9);  // attaches the servo on pin 9 to the servo object
   LockServo.write(0);
   Serial.begin(9600);
}

void loop() {
   boolean button_pressed = handle_button();

   switch (button_pressed) {
    case 1:
      LockServo.write(120);
      break;
    case 0:
      LockServo.write(0);
      break;
  }

 
  
  delay(DELAY);
}
