
/*
  LiquidCrystal Library 

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints the sensor redings to the LCD

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 //27-4-2020 combined scteches by Harold de Gouw.
*/

// include the library code:
#include "DHT.h"
#include <LiquidCrystal.h>
#include <math.h>

#define DHTPIN 13     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.    
#define RELAY 7
#define LED_RED 8
#define LED_YELLOW 9
#define LED_GREEN 10

// Uncomment whatever type you're using!
#define DHTTYPE DHT11 // DHT 11    

// DHT: Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//For calculating the Absolute humidity set all readings to zero
float t = 0;
float h = 0;
float tempK = 0;
float vp = 0;
float ah = 0;
float vpe = 0;

// CONSTANTS FOR SATURATED VAPOR PRESSURE CALCULATION
float a0 = 6.107799961;
float a1 = 0.4436518521;
float a2 = 0.01428945805;
float a3 = 0.0002650648471;
float a4 = 0.000003031240396;
float a5 = 0.00000002034080948;
float a6 = 0.00000000006136820929;



void setup() {
  //DHT sensor:
  Serial.begin(9600);
  Serial.println(F("Humidity Mesurement by Harold de Gouw"));
  dht.begin();

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the first line of the LCD.
  lcd.print("RH     AH       Temp");

      //Relay and leds
      pinMode(RELAY, OUTPUT);
      pinMode(LED_RED, OUTPUT);
      pinMode(LED_YELLOW, OUTPUT);
      pinMode(LED_GREEN, OUTPUT);
}


void loop() {
  // Wait a few seconds between DHT measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  
//LED RED swiched
//if the absolute humidity decrease lower than 6 g/M3 the Red Led will turn ON or if higher, the led remain off. So whenever the absolute humidity goes beneath 6 g/m3, 
//we need to make the LED Pin (PIN 8) high to make the Red LED ON. And when the absolute humidity goes higher then 6 g/m3, we need to make the RELAY pin high to turn ON the Relay Module. And so on.
//Relay Humidity swiched
//if the absolute humidity increase less than 8 g/M3 the RELAY will turn OFF or if more, the RELAY will turn on. So whenever the absolute humidity goes above than 8 g/m3, 
//we need to make the RELAY Pin (PIN 8) low to make the Relay module OFF. And when the absolute humidity goes below 8 g/m3, we need to make the RELAY pin high to turn ON the Relay Module.
   if (ah < 6.5) digitalWrite(LED_RED, HIGH);
else if (ah > 6.5) digitalWrite(LED_RED, LOW),delay(2000);
   if (ah < 8) digitalWrite(LED_YELLOW, HIGH);
else if (ah > 6 ) digitalWrite(LED_YELLOW, LOW),delay(2000);
  if (ah > 7.5) digitalWrite(LED_GREEN, HIGH);
else if (ah < 7.5) digitalWrite(LED_GREEN , LOW),delay(2000);
   if (ah < 8) digitalWrite(RELAY, HIGH),delay(2000);
else if (ah > 8.2) digitalWrite(RELAY, LOW),delay(2000); 


// CALCULATIONS
  tempK = t + 273.15;                                                // °C to K conversion
//  vp = 6.1078 * pow(10, ((7.500 * temp) / (237.300 + temp)));           // Saturation vapor pressure [hPa]
  vp = a0 + t * (a1 + t * ( a2 + t * (a3 + t * ( a4 + t * (a5 + a6 * t)))));
  vpe = (h * vp) / 100.00;                                            // Water vapor pressure
  ah = 2.16679 * vpe * 100 / tempK;                                     // Asolute humidity


  //Print sensor readings in serial monitor
  Serial.print(F("Relative Humidity: "));
  Serial.print(h);
  Serial.print(F("%   Absulte Humidity: "));
  Serial.print(ah);
  Serial.print(F("g/m3   Temperature: "));
  Serial.print(t);
  Serial.println(F("°C    "));

  //Print sensor readings in lcd screen
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  lcd.print(h);
  lcd.print(F("% "));
  lcd.print(ah);
  lcd.print(F("g/m3 "));
  lcd.print(t);
  lcd.print(F("C "));
 
  // scroll 7 positions to the left
  // to move it back to beginning:
  for (int positionCounter = 0; positionCounter < 7; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(150); }
  // delay at the end of the full loop:
  delay(2000);
  for (int positionCounter = 0; positionCounter < 7; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayRight();
    // wait a bit:
    delay(150); }
  // delay at the end of the full loop:
  delay(2000);  
}
