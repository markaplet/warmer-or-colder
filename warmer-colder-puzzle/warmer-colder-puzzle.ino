#include <OneWire.h>
#include <DallasTemperature.h>

#define TEMP_SENSOR 8   // Sensor on digital pin 8

OneWire oneWire(TEMP_SENSOR);
DallasTemperature sensors(&oneWire);

float Celsius = 0;
float Fahrenheit = 0;
const int TEMP_DELTA = 10;  //Degree difference hotter or colder than base

float BASE_TEMP = 0;        
float HOT_TEMP_DELTA = 0;
float COLD_TEMP_DELTA = 0;




  
void setup() {
  sensors.begin();
  Serial.begin(9600);

  sensors.requestTemperatures();
  Celsius = sensors.getTempCByIndex(0);
  Fahrenheit = sensors.toFahrenheit(Celsius);

  BASE_TEMP = Fahrenheit;
    
  // Base Temp: 70.25 F
  Serial.print("Base Temp: ");
  Serial.print(BASE_TEMP);
  Serial.println(" F");  

  HOT_TEMP_DELTA = BASE_TEMP + TEMP_DELTA;
  COLD_TEMP_DELTA = BASE_TEMP - TEMP_DELTA;

  Serial.print("Hot Temp Target: ");
  Serial.print(HOT_TEMP_DELTA);
  Serial.println(" F");

  Serial.print("Cold Temp Target: ");
  Serial.print(COLD_TEMP_DELTA);
  Serial.println(" F");
}





void loop() {
  sensors.requestTemperatures();
  Celsius = sensors.getTempCByIndex(0);
  Fahrenheit = sensors.toFahrenheit(Celsius);

  Serial.print(Fahrenheit);
  Serial.println(" F");

  checkTemp();

  delay(1000);
}




// FUNCTIONS
void checkTemp() {
  if (Fahrenheit >= HOT_TEMP_DELTA || Fahrenheit <= COLD_TEMP_DELTA) {
     Serial.println("Congratulations");
     return true;
  }
}
