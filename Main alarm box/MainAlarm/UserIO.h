#ifndef UserIO_h
#define UserIO_h

#include "Arduino.h"
#include "LiquidCrystal_I2C.h"
#include "Keypad.h"

#include "ILogger.h"

/*
   Provides combined interface for input from keypad and output to LCD screen
*/
class UserIO : public ILogger
{
  private:
    ILogger *logger;
    LiquidCrystal_I2C *screen;
    Keypad *keyIn;

  public:
    UserIO(ILogger *_logger)
      : logger{_logger} {
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
      keyIn = new Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

      //LCD screen setup
      screen = new LiquidCrystal_I2C(0x27, 20, 4);
      screen->init();
      screen->backlight();
    }

    //Output functionality
    void print(String data) {
      screen->print(data);
    }
    void print(char data) {
      screen->print(data);
    }
    void print(int data) {
      screen->print(data);
    }
    void print(float data) {
      screen->print(data);
    }
    void print(byte data) {
      screen->print(data);
    }
    void print(double data) {
      screen->print(data);
    }

    void setCursor(byte column, byte row) {
      screen->setCursor(column, row);
    }

    void enableBacklight() {
      screen->backlight();
    }
    void disableBacklight() {
      screen->noBacklight();
    }

    void showCursor() {
      screen->cursor();
    }
    void hideCursor() {
      screen->noCursor();
    }

    void clearScreen() {
      screen->clear();
    }


    //Input functionality
    char getKey() {
      return keyIn->getKey();
    }
};

#endif
