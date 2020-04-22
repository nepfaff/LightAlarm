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
  screen.backlight(); 

  //pinMode(3, INPUT);

  attachInterrupt(digitalPinToInterrupt(3), buttonPressed, RISING);
  
}

void loop() {
  /*int buttonState = digitalRead(3);
  if(buttonState == HIGH){
    Serial.print("Button pressed ");
    Serial.println(buttonState);
  }*/
  
}

void buttonPressed(){
  Serial.println("Interrup called due to button pressed");
}
