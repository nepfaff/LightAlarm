#ifndef UserIO_h
#define UserIO_h

#include "Arduino.h"
#include "Wire.h"
#include "LiquidCrystal_I2C.h"
#include "Keypad.h"

#include "ILogger.h"
#include "CommSystem.h"

/*
   Provides combined interface for input from keypad and output to LCD screen.
*/
class UserIO
{
private:
  ILogger *logger;
  LiquidCrystal_I2C *screen;
  Keypad *keyIn;

  //helper function for getTime() and getHoursMinutesSeconds()
  bool getTimeDigit(String callerFunction, String &timeContainer, int maxDigit);

public:
  UserIO(ILogger *_logger, Keypad *_keypad);

  //Output functionality
  void print(String data);
  void print(char data);
  void print(int data);
  void print(float data);
  void print(byte data);
  void print(double data);

  //utility function for digital time display: prints preceding colon and leading 0
  void printDigits(int digits, bool isFirst = false) const;

  void setCursor(byte column, byte row);

  void clearRow(byte row);

  void clearField(byte column, byte row);

  void enableBacklight();
  void disableBacklight();

  //show or hide current cursor position
  //used to provide improved input experience for the user
  void showCursor();
  void hideCursor();

  //clears the entire screen
  void clearScreen();

  //Input functionality
  char getKey() const;

  //returns true if the user entered #
  bool enteredHash();

  //validates user input and returns 0 if no/no valid input
  char getValidDigitOrHash();

  //blocks till it gets a user input
  //returns 0 if the user input is neither a digit nor a #
  char getValidDigitOrHashBlocking();

  //validates input based on the argument indicating the maximum number of modes available
  //returns 0 (corresponding to default mode) if no/no valid user input
  int getValidModeInt(const int modeNumber) const;

  //0 when no option selected yet, 100 when quit
  byte selectOption(byte numberOfOptions);

  //returns [hour, minute] or 0 if quit
  int *getTime();

  //returns [hours, minutes, seconds] or 0 if quit
  int *getHoursMinutesSeconds();
};

#endif
