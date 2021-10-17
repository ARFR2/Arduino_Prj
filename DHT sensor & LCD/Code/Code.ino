/* 
   Source: https://github.com/Fasihi-Rad/Arduino.Prj
   
   Arduino.Prj__DHT sensor & LCD
   Seyed Ali Mojtabavi
   Amir Reza Fasihi rad
 
 The circuit:
 * LCD RS pin to digital pin 13
 * LCD Enable pin to digital pin 12
 * LCD D4 pin to digital pin 7
 * LCD D5 pin to digital pin 6
 * LCD D6 pin to digital pin 5
 * LCD D7 pin to digital pin 4
 * LCD R/W pin to ground
 * Two 5V DC Relay 
 * ends to +5V and ground
*/ 
// Include The Library Code
#include "DHT.h"
#include <LiquidCrystal.h>

#define DHTPIN 3 // Sensor PIN
#define DHTTYPE DHT22 // Sensor Type

int MIN_temp = 28; // SET min Temperature
int MAX_hum = 20;  // SET min Humidity
DHT dht(DHTPIN,DHTTYPE); // SETUP DHT PIN
LiquidCrystal lcd(13, 12, 7, 6, 5, 4);  // SETUP LCD PIN

// PrintLCD Function 
void PrintLCD(int MIN_temp, int MAX_hum, float temp, float hum)
{
  // Print a message to the LCD:
  lcd.setCursor(0,0);
  lcd.print("Temperature:");
  lcd.setCursor(0,1);
  lcd.print("Humidity:");
  lcd.setCursor(13,0);
  lcd.print(temp);
  lcd.print("C");
  lcd.setCursor(13,1);
  lcd.print(hum);
  lcd.print("%");
  lcd.setCursor(0,2);
  lcd.print("MIN Temp:");
  lcd.setCursor(13,2);
  lcd.print(MIN_temp);
  lcd.print("C");
  lcd.setCursor(0,3);
  lcd.print("MAX Hum:");
  lcd.setCursor(13,3);
  lcd.print(MAX_hum);
  lcd.print("%");
}
// Observer Function 
void Observer(int MIN_temp, int MAX_hum, float temp, float hum)
{
    if (hum >= MAX_hum) //Check Humidity
    {
      digitalWrite(1, HIGH);//Turn LED1 On
    } 
    else if(hum < MAX_hum)//Check Humidity
    {
      digitalWrite(1, LOW);//Turn LED1 Off
    }
    if(temp <= MIN_temp)//Check Temperature
    {
      digitalWrite(2, HIGH);//Turn LED2 On
    }
    else if(temp > MIN_temp)//Check Temperature
    {
      digitalWrite(2, LOW);//Turn LED2 Off
    }
}
void setup() {
  // initialize the LED pin as an output:
  // setup the LCD's number of columns and rows:
  lcd.begin(20, 4);
  // setup dht :
  dht.begin();
  //Define LED Pin :
  pinMode(2,OUTPUT);
  pinMode(1,OUTPUT);
}
void loop() 
{
  float hum = dht.readHumidity(); // Read Humidity
  float temp = dht.readTemperature(); // Read Temperature
  
  Observer(MIN_temp, MAX_hum, temp, hum); //Call Observer Function
  PrintLCD(MIN_temp, MAX_hum, temp, hum); //Call PrintLCD Function
}
