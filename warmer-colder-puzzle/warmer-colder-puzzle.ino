/*

 __        __                                ____      _     _           
 \ \      / /_ _ _ __ _ __ ___   ___ _ __   / ___|___ | | __| | ___ _ __ 
  \ \ /\ / / _` | '__| '_ ` _ \ / _ \ '__| | |   / _ \| |/ _` |/ _ \ '__|
   \ V  V / (_| | |  | | | | | |  __/ |    | |__| (_) | | (_| |  __/ |   
    \_/\_/ \__,_|_|  |_| |_| |_|\___|_|     \____\___/|_|\__,_|\___|_|   
                                                                         

Geocaching puzzle game where cachers must use resources on hand to either
warm or cool a temperature probe by 40 degrees to unlock the hidden cache.

NOTES:
Probe uses a 4.7k resistor between data and vcc. See https://playground.arduino.cc/Learning/OneWire/
Probe data pin: 8
I2C LCD: SCL = A5, SDA = A4
Servo motor = 9
*/

// Temperature Probe Libraries
#include <OneWire.h>
#include <DallasTemperature.h>

// LCD Libraries
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Servo Library
#include <Servo.h>

Servo LockServo;        // create servo object to control a servo
int POS = 0;            // variable to store the servo position
int LOCKED = 1;         // default lock state

#define TEMP_DELTA 40   //Degree difference hotter or colder than base
//#define DEBUG 1       //Enables serial print debugging

boolean start = true;   // used to toggle running of main loop

// Temperature Probe Setup
#define TEMP_SENSOR 8   // Sensor on digital pin 8

OneWire oneWire(TEMP_SENSOR);
DallasTemperature sensors(&oneWire);

float Celsius = 0;
float Fahrenheit = 0;

float BASE_TEMP = 0;        
float HOT_TEMP_DELTA = 0;
float COLD_TEMP_DELTA = 0;

// Define LCD Pins and address
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// Creat a set of new characters for the congratulations screen
const uint8_t charBitmap[][8] = {
   { 0xc, 0x12, 0x12, 0xc, 0, 0, 0, 0 },
   { 0x6, 0x9, 0x9, 0x6, 0, 0, 0, 0 },
   { 0x0, 0x6, 0x9, 0x9, 0x6, 0, 0, 0x0 },
   { 0x0, 0xc, 0x12, 0x12, 0xc, 0, 0, 0x0 },
   { 0x0, 0x0, 0xc, 0x12, 0x12, 0xc, 0, 0x0 },
   { 0x0, 0x0, 0x6, 0x9, 0x9, 0x6, 0, 0x0 },
   { 0x0, 0x0, 0x0, 0x6, 0x9, 0x9, 0x6, 0x0 },
   { 0x0, 0x0, 0x0, 0xc, 0x12, 0x12, 0xc, 0x0 }
   
};



// SETUP
void setup() { 
  #ifdef DEBUG
    Serial.begin(9600);
  #endif
  
  int charBitmapSize = (sizeof(charBitmap ) / sizeof (charBitmap[0])); // congrats chars
  sensors.begin();
  lcd.begin(16,2);
  
  for ( int i = 0; i < charBitmapSize; i++ ){
    lcd.createChar ( i, (uint8_t *)charBitmap[i] ); //congrats chars
  }

  LockServo.attach(9);  // attaches the servo on pin 9 to the servo object
  LockServo.write(0);   // ensure latch is enguaged

  // Start LCD with mission brief
  yourMission();

  // Set the base temp and target deltas
  setBaseTemp();
}



// MAIN LOOP
void loop() {
  if(start == true){ 
    checkTemp();
  } else if (start == false) {
    congratsParty();
    unlockCache();
  }
}




// FUNCTIONS
void setBaseTemp() {
  sensors.requestTemperatures();
  Celsius = sensors.getTempCByIndex(0);
  Fahrenheit = sensors.toFahrenheit(Celsius);

  BASE_TEMP = Fahrenheit;
  HOT_TEMP_DELTA = BASE_TEMP + TEMP_DELTA;
  COLD_TEMP_DELTA = BASE_TEMP - TEMP_DELTA;

  int BASE_TEMP_TRUNC = trunc(BASE_TEMP);
  int BASE_TEMP_ROUND = round(BASE_TEMP);
  
  lcd.clear();
  lcd.print("Start Temp ");
  lcd.print(BASE_TEMP_ROUND);
  lcd.print((char) 223);
  lcd.print("F");

  #ifdef DEBUG
    // Base Temp: 70.25 F
    Serial.print("Base Temp: ");
    Serial.print(BASE_TEMP);
    Serial.println(" F");
    
    Serial.print("Hot Temp Target: ");
    Serial.print(HOT_TEMP_DELTA);
    Serial.println(" F");
  
    Serial.print("Cold Temp Target: ");
    Serial.print(COLD_TEMP_DELTA);
    Serial.println(" F");
  #endif
}

void checkTemp() {
  // Read probe and convert celsius to fahrenheit
  sensors.requestTemperatures();
  Celsius = sensors.getTempCByIndex(0);
  Fahrenheit = sensors.toFahrenheit(Celsius);

  int PROBE_TEMP_ROUND = round(Fahrenheit);
    
  lcd.setCursor (0, 1);
  lcd.print("               ");
  lcd.setCursor (0, 1);
  lcd.print("Probe ");
  lcd.print(PROBE_TEMP_ROUND);
  lcd.print((char) 223);
  lcd.print("F");
  delay(1000);

  #ifdef DEBUG
    Serial.print(Fahrenheit);
    Serial.println(" F");
  #endif
  
  if (Fahrenheit >= HOT_TEMP_DELTA || Fahrenheit <= COLD_TEMP_DELTA) {
    lcd.clear();
    lcd.print("CONGRATULATIONS!");
    
    #ifdef DEBUG
      Serial.println("Congratulations");
    #endif
    start = false;
    return true;
  }
}

void congratsParty(){
  lcd.setCursor ( 0, 1 );
    // Do a little animation by writing to the same location
   for ( int i = 0; i < 2; i++ ){
      for ( int j = 0; j < 16; j++ ){
         lcd.print (char(random(7)));
      }
      //lcd.setCursor ( 0, 1 );
   }
   delay (300);
}

void yourMission() {
  //** This is a **
  //**  Geocache **
  lcd.clear();
  lcd.print((char) 42);
  lcd.print((char) 42);
  lcd.print(" This is a  ");
  lcd.print((char) 42);
  lcd.print((char) 42);
  lcd.setCursor (0, 1);
  lcd.print((char) 42);
  lcd.print((char) 42);
  lcd.print("  Geocache  ");
  lcd.print((char) 42);
  lcd.print((char) 42);
  delay(5000);
  
  lcd.clear();
  lcd.print("Your Mission");
  delay(5000);
  
  lcd.clear();
  lcd.print("40 Degrees");
  lcd.setCursor (0, 1);
  lcd.print("hotter or colder");
  delay(5000);

  lcd.clear();
  lcd.print("Gently use the");
  lcd.setCursor (0, 1);
  lcd.print("probe and your");
  delay(5000);

  lcd.clear();
  lcd.print("source of hot or");
  lcd.setCursor (0, 1);
  lcd.print("cold to unlock");
  delay(5000);

  lcd.clear();
  lcd.print("USE OF FIRE WILL");
  lcd.setCursor (0, 1);
  lcd.print("DAMAGE THE CACHE");
  delay(5000);

  lcd.clear();
  lcd.print("Are you ready?");
  lcd.setCursor (0, 1);
  lcd.print("................");
  // Count down removes periods
  for (int i = 16; i >= 0; i--) {
    lcd.setCursor (i, 1);
    lcd.print(" ");
    delay(500);
  }
}

void unlockCache(){
  #ifdef DEBUG
    Serial.println("Unlock Cache");
  #endif

  if (LOCKED == 1) {
    #ifdef DEBUG
      Serial.println("unlocking");
    #endif
    
    LockServo.write(100);
    delay(2000);
    LOCKED = 0;
    
  } else if (LOCKED == 0) {
    #ifdef DEBUG
      Serial.println("locking");
    #endif
    
    LockServo.write(0);
    //LOCKED = 1;
  }
}
