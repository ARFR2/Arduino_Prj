/* 
   Source: https://github.com/Fasihi-Rad/Arduino.Prj
   
   Arduino.Prj__DHT sensor & LCD
   Seyed Ali Mojtabavi
   Amir Reza Fasihi rad
*/

// Include The Library Code
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include "DHT.h"

#define DHTPIN 3      // Sensor PIN
#define DHTTYPE DHT22 // Sensor Type

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

//define the symbol on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
    {'7', '8', '9', '%'},
    {'4', '5', '6', '*'},
    {'1', '2', '3', '-'},
    {'C', '0', '=', '+'}};

byte rowPins[ROWS] = {11, 10, 9, 8}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {7, 6, 5, 4};   //connect to the column pinouts of the keypad

// Set the initial value:
char Data[3] = {0};  //make Char array
byte data_count = 0; //make Pointer for Char array
int MIN_temp = 25;   // SET min Temperature
int MAX_hum = 20;    // SET min Humidity

// Initialize temperature sensor
DHT dht(DHTPIN, DHTTYPE);
//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

LiquidCrystal_I2C lcd(0x20, 20, 4);

// PrintLCD Function
void PrintLCD(int MIN_temp, int MAX_hum, float t, float h)
{
  // Print a message to the LCD:
  lcd.setCursor(0, 0);
  lcd.print("Temperature:");
  lcd.setCursor(0, 1);
  lcd.print("Humidity:");
  lcd.setCursor(13, 0);
  lcd.print(t);
  lcd.print("C");
  lcd.setCursor(13, 1);
  lcd.print(h);
  lcd.print("%");
  lcd.setCursor(0, 2);
  lcd.write("MIN Temp:",HIGH);
  lcd.setCursor(13, 2);
  lcd.print(MIN_temp);
  lcd.print("C");
  lcd.setCursor(0, 3);
  lcd.print("MAX Hum:");
  lcd.setCursor(13, 3);
  lcd.print(MAX_hum);
  lcd.print("%");
}

// LCD_Windows Function for display selection Windows
void LCD_Windows(int MIN_temp, int MAX_hum, bool status)
{
  lcd.clear();         //clear LCD
  lcd.setCursor(0, 0); //Set LCD Cursor to [0,0]
  // Check status:
  // True => MAX Humidity window :
  if (status)
  {
    // Print MAX Humidity to the LCD:
    lcd.print("MAX Humidity:");
    lcd.setCursor(16, 0);
    lcd.print(MAX_hum);
    lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print("Set MAX Humidity:");
  }
  // False => MIN Temperature window :
  else
  {
    // Print MIN Temperature to the LCD:
    lcd.print("MIN Temperature:");
    lcd.setCursor(16, 0);
    lcd.setCursor(16, 0);
    lcd.print(MIN_temp);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("Set MIN Temperature: ");
  }
}
// Observer Function
void Observer(int MIN_temp, int MAX_hum, float temp, float hum)
{
  if (hum >= MAX_hum) //Check Humidity
  {
    digitalWrite(1, HIGH); //Turn LED1 On
  }
  else if (hum < MAX_hum) //Check Humidity
  {
    digitalWrite(1, LOW); //Turn LED1 Off
  }
  if (temp <= MIN_temp) //Check Temperature
  {
    digitalWrite(2, HIGH); //Turn LED2 On
  }
  else if (temp > MIN_temp) //Check Temperature
  {
    digitalWrite(2, LOW); //Turn LED2 Off
  }
}

void setup()
{
  lcd.init();                                 // Initialize LCD
  lcd.backlight();                            //Turn on LCD backlight
  dht.begin();                                // Initialize temperature sensor
  customKeypad.addEventListener(keypadEvent); // Add an event listener for the keypad
}

void loop()
{
  float hum = dht.readHumidity();     // Read Humidity
  float temp = dht.readTemperature(); // Read Temperature
  char key = customKeypad.getKey();   //getting Key value

  Observer(MIN_temp, MAX_hum, temp, hum); //Call Observer Function
  PrintLCD(MIN_temp, MAX_hum, temp, hum); //Call PrintLCD Function
}

// Taking care of special events:
void keypadEvent(KeypadEvent key)
{
  switch (customKeypad.getState())
  {
  case PRESSED:
    // if key Pressed :
    if (key == '*') //check for '*' input
    {
      LCD_Windows(MIN_temp, MAX_hum, false); //Call LCD_Windows Function
      while (true)                           //Make Loop
      {
        char Nkey = customKeypad.getKey(); //getting New Key value
        if (Nkey == '0' ||
            Nkey == '1' ||
            Nkey == '2' ||
            Nkey == '3' ||
            Nkey == '4' ||
            Nkey == '5' ||
            Nkey == '6' ||
            Nkey == '7' ||
            Nkey == '8' ||
            Nkey == '9')
        {
          lcd.setCursor(data_count, 2); //set Cursor on Pointer position on line 2
          Data[data_count] = Nkey;      //Save The input Key on Data array
          lcd.print(Nkey);              //display the Key
          data_count++;                 //increase the Pointer
        }
        if (Nkey == '-' && data_count > 0) ////check for '-' input AND Data Length > 0
        {
          data_count--;                 //Decrease the Pointer
          lcd.setCursor(data_count, 2); //set Cursor on Pointer position on line 2
          lcd.print(" ");               //print None For delete the last Character
        }
        if (Nkey == 'C') //check for 'C' input
        {
          Data[3] = {0};  //set the Data array to zero
          data_count = 0; //set the data_count to zero
          lcd.clear();    //clear LCD
          break;          //Break the Loop
        }
        if (Nkey == '=' || data_count == 3) //check for '=' input OR maximum valid Data Length
        {
          lcd.setCursor(0, 3);
          lcd.print("Data Saved !"); //Print "Data Saved !"
          MIN_temp = atoi(Data);     //Convert Data array to int and put it on MIN_temp
          Data[3] = {0};             //set the Data array to zero
          data_count = 0;            //set the data_count to zero
          delay(1500);               //Make Delay
          lcd.clear();               //clear LCD
          break;                     //Break the Loop
        }
      }
    }
    if (key == '%') //check for '%' input
    {
      LCD_Windows(MIN_temp, MAX_hum, true); //Call LCD_Windows Function
      while (true)                          //Make Loop
      {
        char Nkey = customKeypad.getKey();
        if (Nkey == '0' ||
            Nkey == '1' ||
            Nkey == '2' ||
            Nkey == '3' ||
            Nkey == '4' ||
            Nkey == '5' ||
            Nkey == '6' ||
            Nkey == '7' ||
            Nkey == '8' ||
            Nkey == '9')
        {
          lcd.setCursor(data_count, 2);
          Data[data_count] = Nkey; //Save The input Key on Data array
          lcd.print(Nkey);         //display the Key
          data_count++;            //increase the Pointer
        }
        if (Nkey == 'C') //check for 'C' input
        {
          Data[3] = {0};  //set the Data array to zero
          data_count = 0; //set the data_count to zero
          lcd.clear();    //clear LCD
          break;          //Break the Loop
        }
        if (Nkey == '-' && data_count > 0) //check for '-' input AND Data Length > 0
        {
          data_count--;                 //Decrease the Pointer
          lcd.setCursor(data_count, 2); //set Cursor on Pointer position on line 2
          lcd.print(" ");               //print None For delete the last Character
        }
        if (Nkey == '=' || data_count == 3) //check for '=' input or maximum valid Data Length
        {
          int tmp = atoi(Data); //Convert Data array to int and put it in to tmp
          Data[3] = {0};        //set the Data array to zero
          data_count = 0;       //set the data_count to zero
          if (tmp > 100)        //Check for input value be under 100%
          {
            lcd.setCursor(0, 3);
            lcd.print("Should be under 100%");    //Print "Should be under 100%"
            delay(2500);                          //Make Delay
            LCD_Windows(MIN_temp, MAX_hum, true); //Call LCD_Windows Function
          }
          else
          {
            lcd.setCursor(0, 3);
            lcd.print("Data Saved !"); //Print "Data Saved !"
            MAX_hum = tmp;             //put tmp in to MAX_hum
            delay(1500);               //Make Delay
            lcd.clear();               //clear LCD
            break;                     //Break the Loop
          }
        }
      }
    }
  }
}
