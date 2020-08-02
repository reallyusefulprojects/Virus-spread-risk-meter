


/*
  LCD Display with I2C Interface Demo
  lcd-i2c-demo.ino
  Use NewLiquidCrystal Library
  DroneBot Workshop 2018
  https://dronebotworkshop.com
*/
 
 //20-7-2020 combined scteches by Harold de Gouw.

// include the library code:
#include <Adafruit_Sensor.h> //install library Adafruit Unified Sensor
#include <Adafruit_BME280.h>//install library Adafruit BME280 Library 
// Include Wire Library for I2C
#include <Wire.h>
// Include NewLiquidCrystal Library for I2C
#include <LiquidCrystal_I2C.h>
#include <math.h>

//#define DHTPIN 11 oude sensor 
#define RELAY 10
#define LED_RED 11
#define LED_YELLOW 13
#define LED_GREEN 12
#define repeat(n) { for (int i = 0; i < n; i++) 
#define endrepeat }



Adafruit_BME280 bme;    

// Define LCD pinout
const int  en = 2, rw = 1, rs = 0, d4 = 4, d5 = 5, d6 = 6, d7 = 7, bl = 3;
 
// Define I2C Address - change if reqiuired
const int i2c_addr = 0x27;
 
LiquidCrystal_I2C lcd(i2c_addr, en, rw, rs, d4, d5, d6, d7, bl, POSITIVE);

//create custom made degrees sign: °C
byte customChar[] = {
  B01100,
  B01100,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};


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
    Serial.begin(9600);
    
    if (!bme.begin(0x76)) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
      while (1);
    }
  
  Serial.println(F("Humidity Mesurement by Harold de Gouw; versie 17-7-2020"));
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  
  // Print a message to the first line of the LCD. Virus spreading risk meter
  lcd.print("Virus verspreid-");
  lcd.setCursor(0, 1);
  lcd.print("ingsrisico meter");
  delay(3000);
  lcd.clear();
  lcd.print("Rood = Gevaar, ");
  lcd.setCursor(0, 1);
  lcd.print("Luchtv.te laag ");
  delay(2000);
  lcd.clear();
  lcd.print("Geel = Let op!, ");
  lcd.setCursor(0, 1);
  lcd.print("Luchtv. te laag ");
  delay(2000);
  lcd.clear();
  lcd.print("Groen = Veilig, ");
  lcd.setCursor(0, 1);
  lcd.print("Luchtvochtigh ok");
  delay(2000);
  lcd.clear();
  lcd.print("Virus meter by");
  lcd.setCursor(0, 1);
  lcd.print("Harold de Gouw");
  delay(1000);
  lcd.createChar(0, customChar);
  
      //Relay and leds
      pinMode(RELAY, OUTPUT);
      pinMode(LED_RED, OUTPUT);
      pinMode(LED_YELLOW, OUTPUT);
      pinMode(LED_GREEN, OUTPUT);
}

void loop() {
// MESUREMENT ACQUISITION
  // Wait a few seconds between BME measurements.
  delay(500);

  // Reading temperature or humidity takes about 250 milliseconds!
 
  float h = (bme.readHumidity()/ 0.855);
  // Read temperature as Celsius (the default)
  float t = (bme.readTemperature() - 1.68);
  // Air pressure hpa
  float pressavg = (bme.readPressure() / 100.0F - 0.20);



  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(pressavg)) {
    Serial.println(F("Failed to read from BME sensor!"));
    return;
  }

  
//LED and Relay swiched
  //if the absolute humidity decrease lower than 6 g/M3 the Red Led will turn ON or if higher, the led remain off. So whenever the absolute humidity goes beneath 6 g/m3, 
  //we need to make the LED Pin (PIN 8) high to make the Red LED ON. And when the absolute humidity goes higher then 6 g/m3, we need to make the RELAY pin high to turn ON the Relay Module. And so on.
  //Relay Humidity swiched
  //if the absolute humidity increase less than 8 g/M3 the RELAY will turn OFF or if more, the RELAY will turn on. So whenever the absolute humidity goes above than 8 g/m3, 
  //we need to make the RELAY Pin (PIN 8) low to make the Relay module OFF. And when the absolute humidity goes below 8 g/m3, we need to make the RELAY pin high to turn ON the Relay Module.
   if (mr < 6.5) digitalWrite(LED_RED, HIGH);
else if (mr > 6.5) digitalWrite(LED_RED, LOW),delay(1000);
   if (mr < 8) digitalWrite(LED_YELLOW, HIGH);
else if (mr > 6 ) digitalWrite(LED_YELLOW, LOW),delay(1000);
  if (mr > 7.5) digitalWrite(LED_GREEN, HIGH);
else if (mr < 7.5) digitalWrite(LED_GREEN , LOW),delay(1000);
   if (mr < 8) digitalWrite(RELAY, HIGH),delay(1000);
else if (mr > 8.2) digitalWrite(RELAY, LOW),delay(1000); 


// CALCULATIONS
  tempK = t + 273.15;                                                // °C to K conversion
  //  vp = 6.1078 * pow(10, ((7.500 * temp) / (237.300 + temp)));           // Saturation vapor pressure [hPa]
  vp = a0 + t * (a1 + t * ( a2 + t * (a3 + t * ( a4 + t * (a5 + a6 * t)))));
  vpe = (h * vp) / 100.00;                                            // Water vapor pressure
  mr = 621.9907 * vpe / (pressavg - vpe);                                  // Mixing ratio / Specific humidity
  ah = 2.16679 * vpe * 100 / tempK;                                     // Asolute humidity
 

//Print sensor readings in serial monitor
  Serial.print(F("Relative Humidity: "));
  Serial.print(h,1);
  Serial.print(F("%   Absulte Humidity: "));
  Serial.print(ah,1);
  Serial.print(F("g/m3   Temperature: "));
  Serial.print(t,1);
  Serial.print(F("°C    Specific Humidity: "));
  Serial.print(mr,1);
  Serial.print(F("g/kg   Pressure: "));
  Serial.print (pressavg,1);
  Serial.println("hPa");
  delay(3000);
  

//Print sensor readings in lcd screen
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  
repeat(3) {
    //do stuff 3x
  
  lcd.setCursor(0, 0);
  // Print a message to the first line of the LCD.
  lcd.print("RH: ");
  lcd.print(h,1);
  lcd.print(F(" %    >>T:   "));
  lcd.setCursor(21, 0);
  if (t < 10)  // allign the points if the number has only 1 digit before the point 
  {
  lcd.print(" ");
  }
  lcd.print(t,1);
  
  {//write custom made degrees sign: °C
  lcd.setCursor(26, 0);
  lcd.write((byte)0);
  }
  lcd.print(F("C  <<"));
    
  lcd.setCursor(0, 1);
  lcd.print("SH: ");
  lcd.setCursor(4, 1);
  if (mr < 10)  // allign the points if the number has only 1 digit before the point 
  {
  lcd.print(" ");
  }
  lcd.print(mr,1);
  lcd.print(F(" g/kg >>P: "));
  lcd.print(pressavg,1);
  lcd.print(F(" hPa <<"));
 
  // scroll 7 positions to the left
  // to move it back to beginning:

  delay(3000);
  for (int positionCounter = 0; positionCounter < 16; positionCounter++) {
      // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(150); }
  // delay at the end of the full loop:
  delay(3000);
  for (int positionCounter = 0; positionCounter < 16; positionCounter++) {
    // scroll one position right:
    lcd.scrollDisplayRight();
    // wait a bit:
    delay(100); }
  // delay at the end of the full loop:
  delay(1);  
  } endrepeat;
  
     if (mr > 7.5) { 
     lcd.clear();
     lcd.print("Virus versprei-");
     lcd.setCursor(0, 1);
     lcd.print("dingsrisico laag");}
          
     if ((mr >=6) && (mr<= 7.5)) { 
     lcd.clear();
     lcd.print("Let op !!!");
     lcd.setCursor(0, 1);
     lcd.print("Gevaar dreigt");
     delay(3500);
     lcd.clear();
     lcd.print("Luchtvochtigheid");
     lcd.setCursor(0, 1);
     lcd.print("bijna te laag!!");}
     
     if (mr < 6) { 
     lcd.clear();
     lcd.print("Waarschuwing !!!");
     lcd.setCursor(0, 1);
     lcd.print("Gevaar!!");
     delay(3500);
     lcd.clear();
     lcd.print("Luchtvochtigheid");
     lcd.setCursor(0, 1);
     lcd.print("te laag!!");}          


                   
}
