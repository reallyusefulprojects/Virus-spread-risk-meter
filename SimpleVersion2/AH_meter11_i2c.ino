


/*
  LCD Display with I2C Interface Demo
  lcd-i2c-demo.ino
  Use NewLiquidCrystal Library
  DroneBot Workshop 2018
  https://dronebotworkshop.com
*/
 
 //27-4-2020 combined scteches by Harold de Gouw.

// include the library code:
#include "DHT.h"
// Include Wire Library for I2C
#include <Wire.h>
// Include NewLiquidCrystal Library for I2C
#include <LiquidCrystal_I2C.h>
 

#include <math.h>

#define DHTPIN 11     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.    
#define RELAY 12
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

// Define LCD pinout
const int  en = 2, rw = 1, rs = 0, d4 = 4, d5 = 5, d6 = 6, d7 = 7, bl = 3;
 
// Define I2C Address - change if reqiuired
const int i2c_addr = 0x27;
 
LiquidCrystal_I2C lcd(i2c_addr, en, rw, rs, d4, d5, d6, d7, bl, POSITIVE);

//For calculating the Absolute humidity set all readings to zero
float t = 0; //Temperature °C
float pressavg = 0; //airpressure [hPa]
float h = 0; //Humidity Relative %
float tempK = 0; //Temperature °K
float vp = 0; // Saturation vapor pressure [hPa]
float mr = 0; //Mixing Ratio, W or specific Humidity g/kg
float ah = 0; //Absolute Humidity g/m³
float vpe = 0; // Water vapor pressure 

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
  // Print a message to the first line of the LCD. Virus spreading risk meter
  lcd.print("Virus verspreid-");
  lcd.setCursor(0, 1);
  lcd.print("ingsrisico meter");
  delay(6000);
  lcd.clear();
  lcd.print("Rood = Gevaar, ");
  lcd.setCursor(0, 1);
  lcd.print("Luchtv.te laag ");
  delay(4000);
  lcd.clear();
  lcd.print("Geel = Let Op!, ");
  lcd.setCursor(0, 1);
  lcd.print("Luchtv. te laag ");
  delay(4000);
  lcd.clear();
  lcd.print("Groen = Veilig, ");
  lcd.setCursor(0, 1);
  lcd.print("Luchtvochtigh OK ");
  delay(4000);
  lcd.clear();
  lcd.print("Virus meter by");
  lcd.setCursor(0, 1);
  lcd.print("Harold de Gouw");

      //Relay and leds
      pinMode(RELAY, OUTPUT);
      pinMode(LED_RED, OUTPUT);
      pinMode(LED_YELLOW, OUTPUT);
      pinMode(LED_GREEN, OUTPUT);
}

void loop() {
// MESUREMENT ACQUISITION
  // Wait a few seconds between DHT measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Air pressure hpa
  pressavg = 1013.25;

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  
//LED and Relay swiched
  //if the absolute humidity decrease lower than 6 g/M3 the Red Led will turn ON or if higher, the led remain off. So whenever the absolute humidity goes beneath 6 g/m3, 
  //we need to make the LED Pin (PIN 8) high to make the Red LED ON. And when the absolute humidity goes higher then 6 g/m3, we need to make the RELAY pin high to turn ON the Relay Module. And so on.
  //Relay Humidity swiched
  //if the absolute humidity increase less than 8 g/M3 the RELAY will turn OFF or if more, the RELAY will turn on. So whenever the absolute humidity goes above than 8 g/m3, 
  //we need to make the RELAY Pin (PIN 8) low to make the Relay module OFF. And when the absolute humidity goes below 8 g/m3, we need to make the RELAY pin high to turn ON the Relay Module.
   if (mr < 6.5) digitalWrite(LED_RED, HIGH);
else if (mr > 6.5) digitalWrite(LED_RED, LOW),delay(2000);
   if (mr < 8) digitalWrite(LED_YELLOW, HIGH);
else if (mr > 6 ) digitalWrite(LED_YELLOW, LOW),delay(2000);
  if (mr > 7.5) digitalWrite(LED_GREEN, HIGH);
else if (mr < 7.5) digitalWrite(LED_GREEN , LOW),delay(2000);
   if (mr < 8) digitalWrite(RELAY, HIGH),delay(2000);
else if (mr > 8.2) digitalWrite(RELAY, LOW),delay(2000); 


// CALCULATIONS
  tempK = t + 273.15;                                                // °C to K conversion
  //  vp = 6.1078 * pow(10, ((7.500 * temp) / (237.300 + temp)));           // Saturation vapor pressure [hPa]
  vp = a0 + t * (a1 + t * ( a2 + t * (a3 + t * ( a4 + t * (a5 + a6 * t)))));
  vpe = (h * vp) / 100.00;                                            // Water vapor pressure
  mr = 621.9907 * vpe / (pressavg - vpe);                                  // Mixing ratio / Specific humidity
  ah = 2.16679 * vpe * 100 / tempK;                                     // Asolute humidity
 

//Print sensor readings in serial monitor
  Serial.print(F("Relative Humidity: "));
  Serial.print(h);
  Serial.print(F("%   Absulte Humidity: "));
  Serial.print(ah);
  Serial.print(F("g/m3   Temperature: "));
  Serial.print(t);
  Serial.print(F("°C    SH"));
  Serial.print(mr);
  Serial.println(F("g/kg"));

  

//Print sensor readings in lcd screen
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  
  lcd.setCursor(0, 0);
  // Print a message to the first line of the LCD.
  lcd.print("RH     SH       Temp");
  
  lcd.setCursor(0, 1);
  lcd.print(h);
  lcd.print(F("% "));
  lcd.print(mr);
  lcd.print(F("g/kg "));
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
  delay(4000);
  for (int positionCounter = 0; positionCounter < 7; positionCounter++) {
    // scroll one position right:
    lcd.scrollDisplayRight();
    // wait a bit:
    delay(150); }
  // delay at the end of the full loop:
  delay(1);  
}
