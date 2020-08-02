#include "DHT.h"
#include <LiquidCrystal.h>
#include <math.h>

#define DHTPIN 13     
#define RELAY 7
#define LED_RED 8
#define LED_YELLOW 9
#define LED_GREEN 10
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
  lcd.print("RH     AH       Temp");

      //Relay and leds
      pinMode(RELAY, OUTPUT);
      pinMode(LED_RED, OUTPUT);
      pinMode(LED_YELLOW, OUTPUT);
      pinMode(LED_GREEN, OUTPUT);

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

      
//Relay Humidity swiched
//if the absolute humidity increase less than 8 g/M3 the RELAY will turn OFF or if more, the RELAY will turn on. So whenever the absolute humidity goes above than 8 g/m3, 
//we need to make the RELAY Pin (PIN 8) low to make the Relay module OFF. And when the absolute humidity goes below 8 g/m3, we need to make the RELAY pin high to turn ON the Relay Module.
   if (ah < 8) digitalWrite(RELAY, HIGH),delay(2000);
else if (ah > 8.2) digitalWrite(RELAY, LOW),delay(2000); 

//LED RED swiched
//if the absolute humidity decrease lower than 6 g/M3 the Red Led will turn ON or if higher, the led remain off. So whenever the absolute humidity goes beneath 6 g/m3, 
//we need to make the LED Pin (PIN 8) high to make the Red LED ON. And when the absolute humidity goes higher then 6 g/m3, we need to make the RELAY pin high to turn ON the Relay Module.
   if (ah < 6.5) digitalWrite(LED_RED, HIGH);
else if (ah > 6.5) digitalWrite(LED_RED, LOW),delay(2000);
   if (ah < 8) digitalWrite(LED_YELLOW, HIGH);
else if (ah > 6) digitalWrite(LED_YELLOW, LOW),delay(2000);
  if (ah > 7.5) digitalWrite(LED_GREEN, HIGH);
else if (ah < 7.5) digitalWrite(LED_GREEN , LOW),delay(2000);





// CALCULATIONS
  tempK = t + 273.15;                                                // °C to K conversion
//  vp = 6.1078 * pow(10, ((7.500 * temp) / (237.300 + temp)));           // Saturation vapor pressure [hPa]
  vp = a0 + t * (a1 + t * ( a2 + t * (a3 + t * ( a4 + t * (a5 + a6 * t)))));
  vpe = (h * vp) / 100.00;                                            // Water vapor pressure
  ah = 2.16679 * vpe * 100 / tempK;                                     // Asolute humidity






  Serial.print(F("Relative Humidity: "));
  Serial.print(h);
  Serial.print(F("%   Absulte Humidity: "));
  Serial.print(ah);
  Serial.print(F("g/m3   Temperature: "));
  Serial.print(t);
  Serial.println(F("°C    "));


  lcd.setCursor(0, 1);
  lcd.print(h);
  lcd.print(F("% "));
  lcd.print(ah);
  lcd.print(F("g/m3 "));
  lcd.print(t);
  lcd.print(F("C "));
 
  // scroll 16 positions (display length + string length) to the left
  // to move it back to center:
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
    delay(150);
   
  }

  // delay at the end of the full loop:
  delay(2000);  
}
