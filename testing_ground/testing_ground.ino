#include "Wire.h"
#include "LiquidCrystal_I2C.h"
#include "Keypad.h"

//keypad setup
const byte rows = 4;
const byte cols = 4;
char keys[rows][cols] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[rows]{49,48,47,46};
byte colPins[cols]{53,52,51,50};
Keypad keypad1 = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols);

//lcd setup
LiquidCrystal_I2C screen = LiquidCrystal_I2C(0x27, 20, 4); //0x27 is the address of the LCD

void setup() {
  Serial.begin(9600);
  
  screen.init();
  screen.backlight(); //disable when keypad not touched for 10min (then use flag to use first keypress solely to enable backlight again
  //screen.noBacklight();
}

void loop() {
  //checking for pressed key
  char key = keypad1.getKey();
 
  if(key){
    screen.print(key);
  }
  
  /*screen.setCursor(0, 0); //column, row
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
  delay(1000);*/
}
