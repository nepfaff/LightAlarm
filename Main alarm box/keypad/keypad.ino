#include "Keypad.h"

//keypad setup
const byte rows = 4;
const byte cols = 4;
char keys[rows][cols] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[rows] {49, 48, 47, 46};
byte colPins[cols] {53, 52, 51, 50};
Keypad keypad1 = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols);

void setup() {
  Serial.begin(9600);
}

void loop() {

  //checking for pressed key
  char key = keypad1.getKey();

  if (key) {
    Serial.println(key);
  }
  

}
