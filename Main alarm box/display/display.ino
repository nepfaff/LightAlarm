#include <LiquidCrystal.h>


LiquidCrystal lcd(12, 11, 4, 3, 2, 1);

void setup() {
  lcd.begin(16, 2);
  
  pinMode(9, OUTPUT); //used to controll backlight of LCD screen
}

void loop() {
  digitalWrite(9, HIGH); //turns on backlight

  //message
  lcd.setCursor(0, 0);
  lcd.print("Nein!");
  lcd.setCursor(0, 1);
  lcd.print(millis() / 1000);

  //turn backlight off for 2s (find way to only turn backlight on for 30s after a button/similar has been pressed => timer variable and if statement)
  delay(2000);
  digitalWrite(9, LOW);
  delay(2000);
}
