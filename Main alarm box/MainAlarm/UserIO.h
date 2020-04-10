#ifndef UserIO_h
#define UserIO_h

#include "Arduino.h"
#include "Wire.h"
#include "LiquidCrystal_I2C.h"
#include "Keypad.h"

#include "ILogger.h"

/*
   Provides combined interface for input from keypad and output to LCD screen.
*/
class UserIO
{
  private:
    ILogger *logger;
    LiquidCrystal_I2C *screen;
    Keypad *keyIn;

  public:
    UserIO(ILogger *_logger, Keypad *_keypad)
      : logger{_logger}, keyIn{_keypad} {
      
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
    char getKey() const {
      return keyIn->getKey();
    }
    
    int getValidModeInt(const int modeNumber) const{
      char key = keyIn->getKey();
      if(isdigit(key)){
        int num = atoi(key);
        if(num < modeNumber){
          return num;
        }
      }
      logger->logError("Entered key doesn't correspond to a mode", "UserIO, getValidModeInt");
      logger->logInfo("Back at default mode");
      return 0; //0 indicates default mode
    }
};

#endif
