/* 
   Source: https://github.com/Fasihi-Rad/Arduino.Prj
   
   Arduino.Prj__Light sensor & LCD
   Seyed Ali Mojtabavi
   Amir Reza Fasihi rad
 
 The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

*/ 

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int ledPin = 13; // initialize the LED pin number
const int KeyPin = 0;  // initialize the Key pin number

void Constants() 
{
  lcd.setCursor(0,0); // set LCD Courser position on (0,0)
  lcd.print("LED: "); // print on LCD
  lcd.setCursor(0,1); // set LCD Courser position on (0,1)
  lcd.print("Sensor: "); // print on LCD
}

void setup() 
{
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the Key pin as an input:
  pinMode(KeyPin, INPUT);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  Constants();
}

void loop() 
{
  // read input values:
  int Sensvalue = analogRead(A0);
  int Keyvalue = digitalRead(PD0);

  // check if the button is pressed. If it is, the Keyvalue is HIGH:
  if (Keyvalue == HIGH)
  {
   
    lcd.display();  // Turn on the display
    lcd.setCursor(9,1); // set LCD Courser on position on (9,1)
    lcd.print(Sensvalue); // print on LCD
    if (Sensvalue <= 500)
    {
      digitalWrite(ledPin, HIGH); // turn the LED on (HIGH is the voltage level)
      lcd.setCursor(9,0); // set LCD Courser position on (9,0)
      lcd.print("ON "); // print on LCD
    }
    else
    {
      digitalWrite(ledPin, LOW);  // turn the LED off
      lcd.setCursor(9,0); // set LCD Courser position on (9,0)
      lcd.print("OFF"); // print on LCD
    }
  }
  else
  {
    digitalWrite(ledPin, LOW); // turn the LED off
    lcd.noDisplay(); // Turn off the display
  }
}
