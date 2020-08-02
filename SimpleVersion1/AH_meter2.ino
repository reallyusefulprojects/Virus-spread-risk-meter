#include "DHT.h"
#include <LiquidCrystal.h>
#include <math.h>

#define DHTPIN 13     

#define DHTTYPE DHT11     
DHT dht(DHTPIN, DHTTYPE);

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

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
  Serial.begin(9600);
  
  Serial.println(F("Humidity Mesurement by Harold"));

  dht.begin();

  lcd.begin(16, 2);
  lcd.print("AH         RH     Temp");
}

void loop() {
  // Wait a few seconds between measurements.
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

// CALCULATIONS
  tempK = t + 273.15;                                                // °C to K conversion
//  vp = 6.1078 * pow(10, ((7.500 * temp) / (237.300 + temp)));           // Saturation vapor pressure [hPa]
  vp = a0 + t * (a1 + t * ( a2 + t * (a3 + t * ( a4 + t * (a5 + a6 * t)))));
  vpe = (h * vp) / 100.00;                                            // Water vapor pressure
  ah = 2.16679 * vpe * 100 / tempK;                                     // Asolute humidity

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C  AH "));
  Serial.print(ah);
  Serial.println(F("g/m3"));

  lcd.setCursor(0, 1);
  lcd.print(ah);
  lcd.println(F("g/m3 "));
  lcd.print(h);
  lcd.print(F("% "));
  lcd.print(t);
  lcd.print(F("C "));
 
  // scroll 16 positions (display length + string length) to the left
  // to move it back to center:
  for (int positionCounter = 0; positionCounter < 11; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(200); }

  // delay at the end of the full loop:
  delay(3000);
  


for (int positionCounter = 0; positionCounter < 11; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayRight();
    // wait a bit:
    delay(150);
  
  
  }

  // delay at the end of the full loop:
  delay(2000);  
}
