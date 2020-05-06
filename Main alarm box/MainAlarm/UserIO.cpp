#include "Arduino.h"
#include "Wire.h"
#include "LiquidCrystal_I2C.h"
#include "Keypad.h"

#include "UserIO.h"
#include "ILogger.h"
#include "CommSystem.h"

bool UserIO::getTimeDigit(String callerFunction, String &timeContainer, int maxDigit)
{
  char input = getValidDigitOrHashBlocking();
  if (input == '#')
  {
    return 0;
  }
  else if (input && (int)input - 48 <= maxDigit)
  {
    print(input);
    timeContainer += input;
    return 1;
  }
  else
  {
    //restart user input
    if (callerFunction == "getTime")
    {
      return getTime();
    }
    else if (callerFunction == "getHoursMinutesSeconds")
    {
      return getHoursMinutesSeconds();
    }
    else
    {
      logger->logError(F("Invalid caller function"), F("UserIO, getTimeDigit"));
    }
  }
}

UserIO::UserIO(ILogger *_logger, Keypad *_keypad)
  : logger{_logger}, keyIn{_keypad}
{

  //LCD screen setup
  screen = new LiquidCrystal_I2C(0x27, 20, 4);
  screen->init();
  screen->backlight();
}

void UserIO::print(String data)
{
  screen->print(data);
}
void UserIO::print(char data)
{
  screen->print(data);
}
void UserIO::print(int data)
{
  screen->print(data);
}
void UserIO::print(float data)
{
  screen->print(data);
}
void UserIO::print(byte data)
{
  screen->print(data);
}
void UserIO::print(double data)
{
  screen->print(data);
}

void UserIO::printDigits(int digits, bool isFirst = false) const
{
  if (!isFirst)
  {
    screen->print(":");
  }
  if (digits < 10)
  {
    screen->print("0");
  }
  screen->print(digits);
}

void UserIO::setCursor(byte column, byte row)
{
  screen->setCursor(column, row);
}

void UserIO::clearRow(byte row)
{
  setCursor(0, row);
  print(F("                    "));
}

void UserIO::clearField(byte column, byte row)
{
  setCursor(column, row);
  print(F(" "));
}

void UserIO::enableBacklight()
{
  screen->backlight();
}
void UserIO::disableBacklight()
{
  screen->noBacklight();
}

void UserIO::showCursor()
{
  screen->cursor();
}
void UserIO::hideCursor()
{
  screen->noCursor();
}

void UserIO::clearScreen()
{
  screen->clear();
}

char UserIO::getKey() const
{
  return keyIn->getKey();
}

bool UserIO::enteredHash()
{
  if (keyIn->getKey() == '#')
  {
    return true;
  }
  return false;
}

char UserIO::getValidDigitOrHash()
{
  char key = keyIn->getKey();
  if (isdigit(key) || key == '#')
  {
    return key;
  }
  else if (key)
  {
    logger->logInfo(F("Key not a valid input"));
  }
  return 0;
}

char UserIO::getValidDigitOrHashBlocking()
{
  char key = keyIn->waitForKey();
  if (isdigit(key) || key == '#')
  {
    return key;
  }
  logger->logInfo(F("Key not a valid input"));
  return 0;
}

int UserIO::getValidModeInt(const int modeNumber) const
{
  char key = keyIn->getKey();
  if (!key)
  {
    return 0;
  }
  else if (isdigit(key))
  {
    int num = (int)key - 48;
    if (num < modeNumber)
    {
      return num;
    }
  }
  logger->logInfo(F("Entered key doesn't correspond to a mode"));
  logger->logInfo(F("Back at default mode"));
  return 0; //0 indicates default mode
}

byte UserIO::selectOption(byte numberOfOptions)
{
  char input = getValidDigitOrHash();
  if (input == '#')
  {
    return 100;
  }
  else if (input && (int)input - 48 <= numberOfOptions)
  {
    return (int)input - 48;
  }
  else
  {
    return 0;
  }
}

int *UserIO::getTime()
{
  String hour{}, minute{};

  //remove mess that was potentially left behind by invalid input
  clearRow(2);
  showCursor();

  //get hour
  setCursor(0, 1);
  print(F("Enter hour (#=Quit):"));
  setCursor(0, 2);
  print(F("Hour: "));

  //get MSB hour digit
  if (!getTimeDigit("getTime", hour, 2))
  {
    return 0;
  }
  //get LSB hour digit
  if (hour == "0" || hour == "1")
  {
    if (!getTimeDigit("getTime", hour, 9))
    {
      return 0;
    }
  }
  else
  {
    if (!getTimeDigit("getTime", hour, 3))
    {
      return 0;
    }
  }

  //get minute
  clearRow(1);
  setCursor(0, 1);
  print(F("Enter min (#=Quit):"));
  setCursor(0, 2);
  print(F("Minute: "));

  //get MSB minute digit
  if (!getTimeDigit("getTime", minute, 5))
  {
    return 0;
  }
  //get LSB minute digit
  if (!getTimeDigit("getTime", minute, 9))
  {
    return 0;
  }

  hideCursor();

  int *time = new int[2];
  time[0] = hour.toInt();
  time[1] = minute.toInt();
  return time;
}

int *UserIO::getHoursMinutesSeconds()
{
  String hours{}, minutes{}, seconds{};

  //remove mess that was potentially left behind by invalid input
  clearRow(2);
  showCursor();

  //get hours
  setCursor(0, 1);
  print(F("Enter hour (#=Quit):"));
  setCursor(0, 2);
  print(F("Hours: "));

  //get MSB hours digit
  if (!getTimeDigit("getHoursMinutesSeconds", hours, 9))
  {
    return 0;
  }
  //get LSB hours digit
  if (!getTimeDigit("getHoursMinutesSeconds", hours, 9))
  {
    return 0;
  }

  //get minutes
  clearRow(1);
  clearRow(2);
  setCursor(0, 1);
  print(F("Enter mins (#=Quit):"));
  setCursor(0, 2);
  print(F("Minutes: "));

  //get MSB minutes digit
  if (!getTimeDigit("getHoursMinutesSeconds", minutes, 5))
  {
    return 0;
  }
  //get LSB minutes digit
  if (!getTimeDigit("getHoursMinutesSeconds", minutes, 9))
  {
    return 0;
  }

  //get seconds
  clearRow(1);
  clearRow(2);
  setCursor(0, 1);
  print(F("Enter secs (#=Quit):"));
  setCursor(0, 2);
  print(F("Seconds: "));

  //get MSB seconds digit
  if (!getTimeDigit("getHoursMinutesSeconds", seconds, 5))
  {
    return 0;
  }
  //get LSB seconds digit
  if (!getTimeDigit("getHoursMinutesSeconds", seconds, 9))
  {
    return 0;
  }

  hideCursor();

  int *durations = new int[3];
  durations[0] = hours.toInt();
  durations[1] = minutes.toInt();
  durations[2] = seconds.toInt();
  return durations;
}
