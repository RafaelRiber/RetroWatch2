/**
  The MIT License (MIT)

  Copyright (c) 2016 Rafael Riber

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
**/

//Include required libraries
#include <Chrono.h>
#include <LightChrono.h>
#include <RTClib.h>
#include <Wire.h>
#include <SevSeg.h>
#include <EEPROM.h>
#include <Button.h>

//Define constants
#define BUTTON1 11
#define VBATPIN A9
#define BUTTON2 0
#define BUTTON3 1

//Init objects
Button button1(BUTTON1, true,  true, 25);
Button button2(BUTTON2, true,  true, 25);
Button button3(BUTTON3, true,  true, 25);
SevSeg sevSeg;
RTC_DS3231 rtc;

Chrono myChrono(Chrono::SECONDS);

//Declare vars
byte temp_msb;
byte temp_lsb;
unsigned long timer;
unsigned int seconds;
unsigned int minutes;
unsigned int hours;
unsigned int months;
unsigned int days;
unsigned int year;
unsigned int doy;
unsigned int dow;
unsigned int woy;
unsigned int time_show;
unsigned long display_time;
unsigned long display_date;
unsigned long deciSecond;
int count = 1;
int addr = 0;
int menu;
int mode;
int watch;
int brightness;
int date;
int vbat;
float measuredvbat;

//Get seconds from RTC
unsigned int get_seconds()
{
  DateTime now = rtc.now();
  seconds = now.second();
  return seconds;
}

//Get minutes from RTC
unsigned int get_minutes()
{
  DateTime now = rtc.now();
  minutes = now.minute();
  return minutes;
}

//Get hours from RTC
unsigned int get_hours()
{
  DateTime now = rtc.now();
  hours = now.hour();
  return hours;
}

//Get days from RTC
unsigned int get_days()
{
  DateTime now = rtc.now();
  days = now.day();

  return days;
}

//Get months from RTC
unsigned int get_months()
{
  DateTime now = rtc.now();
  months = now.month();
  return months;
}

//Get year from RTC
unsigned int get_year()
{
  DateTime now = rtc.now();
  year = now.year();
  return year;
}

unsigned int get_doy()
{
  DateTime now = rtc.now();
  doy = now.dayOfYear();
  return doy;
}

unsigned int get_dow()
{
  DateTime now = rtc.now();
  dow = now.dayOfTheWeek();
  return dow;
}

//Show hours and minutes on display
void show_hm()
{
  get_seconds();
  get_minutes();
  get_hours();

  display_time = (hours * 100) + minutes;

  char tempString[5];
  sprintf(tempString, "%04d", display_time);
  sevSeg.DisplayString(tempString, 2);
}

//Show minutes and seconds on display
void show_ms()
{
  get_seconds();
  get_minutes();
  get_hours();

  display_time = (minutes * 100) + seconds;

  char tempString[5];
  sprintf(tempString, "%04d", display_time);
  sevSeg.DisplayString(tempString, 2);
}

//Show date on display
void show_date()
{
  char tempString[5];
  switch (date)
  {
    case 0:
      get_days();
      get_months();
      display_date = (days * 100) + months;
      sprintf(tempString, "%04d", display_date);
      sevSeg.DisplayString(tempString, 2);
      break;
    case 1:
      get_year();
      sprintf(tempString, "%4d", year);
      sevSeg.DisplayString(tempString, 0);
      break;
    case 2:
      get_doy();
      sprintf(tempString, "%4d", doy);
      sevSeg.DisplayString(tempString, 0);
      break;
    case 3:
      get_dow();
      sprintf(tempString, "%4d", dow);
      sevSeg.DisplayString(tempString, 0);
      break;
    case 4:
      get_doy();
      woy = doy / 7;
      sprintf(tempString, "%4d", woy);
      sevSeg.DisplayString(tempString, 0);
  }
  if (button2.wasPressed())
  {
    date = date - 1;
  }
  if (button3.wasPressed())
  {
    date++;
  }
  if (date < 0)
  {
    date = 4;
  }
  if (date > 4)
  {
    date = 0;
  }
}

//Stopwatch function
void stopwatch()
{
  switch (count)
  {
    case 0:
      char tempString[5];
      if (myChrono.hasPassed(1))
      {
        deciSecond++;
        sprintf(tempString, "%04d", deciSecond);
        if (deciSecond < 10000)
        {
          sevSeg.DisplayString(tempString, 2);
        }
        if (deciSecond >= 10000)
        {
          sevSeg.DisplayString(tempString, 4);
        }
      }
      break;
    case 1:
      sprintf(tempString, "%04d", deciSecond);
      if (deciSecond < 10000)
      {
        sevSeg.DisplayString(tempString, 2);
      }
      if (deciSecond >= 10000)
      {
        sevSeg.DisplayString(tempString, 4);
      }
      break;
  }
  if (button3.wasPressed())
  {
    if (count <= 1)
    {
      count++;
    }
    if (count > 1)
    {
      count = 0;
    }
  }
  if (button2.wasPressed())
  {
    deciSecond = 0;
  }
}


void time()
{
  switch (time_show)
  {
    case 0:
      show_hm();
      break;
    case 1:
      show_ms();
      break;
  }
  if (button2.wasPressed())
  {
    time_show = time_show - 1;
  }
  if (button3.wasPressed())
  {
    time_show++;
  }
  if (time_show < 0)
  {
    time_show = 1;
  }
  if (time_show > 1)
  {
    time_show = 0;
  }
}

//Set brightness and save to EEPROM if different than before
void brightness_set()
{
  char tempString[5];
  sprintf(tempString, "%4d", brightness);
  sevSeg.DisplayString(tempString, 0);

  if (button2.wasPressed())
  {
    brightness = brightness - 1;
  }
  if (button3.wasPressed())
  {
    brightness = brightness + 1;
  }
  if (brightness > 10)
  {
    brightness = 10;
  }
  if (brightness < 0)
  {
    brightness = 0;
  }
  if (button1.wasPressed())
  {
    //Save brightness to EEPROM
    if (brightness != EEPROM.read(0))
    {
      EEPROM.write(0, brightness);
    }
    mode = 3;
    watch = 0;
  }
}

void batt_show()
{
  float measuredvbat = analogRead(VBATPIN);
  measuredvbat *= 2;    // we divided by 2, so multiply back
  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024; // convert to voltage

  vbat = measuredvbat * 100;
  char tempString[5];
  sprintf(tempString, "%4d", vbat);
  sevSeg.DisplayString(tempString, 2);
}

void batt_check()
{
  float measuredvbat = analogRead(VBATPIN);
  measuredvbat *= 2;    // we divided by 2, so multiply back
  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024; // convert to voltage

  if (measuredvbat < 3.6)
  {
    watch = 2;
  }
  if (button1.wasPressed())
  {
    watch = 0;
  }
}

//Mode selection menu
void menu_select()
{
  switch (menu)
  {
    case 0:
      sevSeg.DisplayString("Hour", 0);
      if (button1.wasPressed())
      {
        mode = 0;
        watch = 0;
      }
      break;
    case 1:
      sevSeg.DisplayString("Date", 0);
      if (button1.wasPressed())
      {
        mode = 1;
        watch = 0;
      }
      break;
    case 2:
      sevSeg.DisplayString("Chro", 0);
      if (button1.wasPressed())
      {
        mode = 2;
        watch = 0;
      }
      break;
    case 3:
      sevSeg.DisplayString("Brtx", 0);
      if (button1.wasPressed())
      {
        mode = 3;
        watch = 0;
      }
      break;
    case 4:
      sevSeg.DisplayString("Batt", 0);
      if (button1.wasPressed())
      {
        mode = 4;
        watch = 0;
      }
      break;
  }

  if (button2.wasPressed())
  {
    menu = menu - 1;
  }
  if (button3.wasPressed())
  {
    menu++;
  }
  if (menu < 0)
  {
    menu = 0;
  }
  if (menu > 4)
  {
    menu = 4;
  }
}

//Modes
void modes()
{
  switch (mode)
  {
    case 0:
      time();
      break;
    case 1:
      show_date();
      break;
    case 2:
      stopwatch();
      break;
    case 3:
      brightness_set();
      break;
    case 4:
      batt_show();
      break;
  }
  if (button1.wasPressed())
  {
    watch++;
    if (watch > 1)
    {
      watch = 0;
    }
  }
  if (mode < 0)
  {
    mode = 0;
  }
  if (mode > 4)
  {
    mode = 4;
  }
}

//Initial setup
void setup()
{
  delay(10);
  rtc.begin();
  timer = millis();
  pinMode(9, INPUT);

  //Set time if RTC lost power
  if (rtc.lostPower())
  {
    // The following line sets the RTC to the date & time this sketch was compiled
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time:
    rtc.adjust(DateTime(2016, 10, 2, 18, 35, 0));
  }
  //Init display vars
  int displayType = COMMON_CATHODE;

  int digit1 = A5;
  int digit2 = 6;
  int digit3 = A2;
  int digit4 = A0;

  int segA = 4;
  int segB = 12;
  int segC = A3;
  int segD = 10;
  int segE = A4;
  int segF = 8;
  int segG = 5;
  int segDP = A1;

  int numberOfDigits = 4;

  sevSeg.Begin(displayType, numberOfDigits, digit1, digit2, digit3, digit4, segA, segB, segC, segD, segE, segF, segG, segDP);
  brightness = EEPROM.read(0);
  batt_check();
}

//Main loop
void loop()
{
  button1.read();
  button2.read();
  button3.read();
  sevSeg.SetBrightness(brightness * 10);
  switch (watch)
  {
    case 0:
      modes();
      break;
    case 1:
      menu_select();
      break;
    case 2:
      for (int i; i < 100; i++)
      {
        sevSeg.DisplayString("BATL", 0);
      }
      watch = 0;
      break;
  }
}
