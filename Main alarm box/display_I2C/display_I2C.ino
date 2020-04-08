#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C screen = LiquidCrystal_I2C(0x27, 20, 4); //0x27 is the address of the LCD

void setup() {
  screen.init();
  screen.backlight(); //disable when keypad not touched for 10min (then use flag to use first keypress solely to enable backlight again
  //screen.noBacklight();
}

void loop() {
  screen.setCursor(0, 0); //column, row
  screen.print("An alarm clock");

  int hours{15}, minutes{21};
  screen.setCursor(10, 2);
  screen.print(hours);
  screen.print(":");
  screen.print(minutes);

  //add blinking underscore (use this when user is entering something e.g. setting new time for alarm)
  //default location will be current cursor location (like printing text)
  screen.cursor();
  delay(500);
  screen.noCursor();
  delay(500);

  //clears LCD screen and sets cursor to (0,0) => use this when want to display different content on screen (first clear old, then write new)
  screen.clear();
  delay(1000);
}
