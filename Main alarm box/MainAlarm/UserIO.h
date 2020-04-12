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

    //helper function for getTime
    bool getTimeDigit(String &timeContainer, int maxDigit) {
      char input = getValidDigitOrHashBlocking();
      if (input == '#') {
        return 0;
      } else if (input && (int)input - 48 <= maxDigit) {
        print(input);
        timeContainer += input;
        return 1;
      } else {
        //restart user input
        return getTime();
      }
    }

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

    //utility function for digital time display: prints preceding colon and leading 0
    void printDigits(int digits, bool isFirst = false) const
    {
      if (!isFirst) {
        screen->print(":");
      }
      if (digits < 10) {
        screen->print("0");
      }
      screen->print(digits);
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

    bool enteredHash(){
      if(keyIn->getKey() == '#'){
        return true;
      }
      return false;
    }

    char getValidDigitOrHash(){
      char key = keyIn->getKey();
      if(isdigit(key) || key == '#'){
        return key;
      }else if(key){
        logger->logInfo("Key not a valid input");
      }
       return 0;
    }

    char getValidDigitOrHashBlocking(){
      char key = keyIn->waitForKey();
      if(isdigit(key) || key == '#'){
        return key;
      }
       logger->logInfo("Key not a valid input");
       return 0;
    }

    int getValidModeInt(const int modeNumber) const {
      char key = keyIn->getKey();
      if (!key) {
        return 0;
      } else if (isdigit(key)) {
        int num = (int)key - 48;
        if (num < modeNumber) {
          return num;
        }
      }
      logger->logInfo("Entered key doesn't correspond to a mode");
      logger->logInfo("Back at default mode");
      return 0; //0 indicates default mode
    }

    //returns [hour, minute] or 0 if quit
    int* getTime()
    {
      String hour{}, minute{};
      
      //remove mess that was potentially left behind by invalid input
      setCursor(0, 2);
      print("                    ");
      showCursor();

      //get hour
      setCursor(0, 1);
      print("Enter hour (#=Quit):");
      setCursor(0, 2);
      print("Hour: ");

      //get MSB hour digit
      if (!getTimeDigit(hour, 2)) {
        return 0;
      }
      //get LSB hour digit
      if (hour == "0" || hour == "1") {
        if (!getTimeDigit(hour, 9)) {
          return 0;
        }
      } else {
        if (!getTimeDigit(hour, 3)) {
          return 0;
        }
      }

      //get minute
      setCursor(0, 1);
      print("                    ");
      setCursor(0, 1);
      print("Enter min (#=Quit):");
      setCursor(0, 2);
      print("Minute: ");

      //get MSB minute digit
      if (!getTimeDigit(minute, 5)) {
        return 0;
      }
      //get LSB minute digit
      if (!getTimeDigit(minute, 9)) {
        return 0;
      }

      hideCursor();

      int* time = new int[2];
      time[0] = hour.toInt();
      time[1] = minute.toInt();
      return time;
    }
};

#endif
