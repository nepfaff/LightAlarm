#ifndef ClockMode_h
#define ClockMode_h

#include "Arduino.h"
#include "TimeLib.h"

#include "IMode.h"
#include "ILogger.h"
#include "UserIO.h"

/*
    Functionality involving the current time
*/
class ClockMode : public IMode
{
  private:
    ILogger *logger;
    UserIO *io;

    // utility function for digital clock display: prints preceding colon and leading 0
    void printDigits(int digits, bool isFirst = false) const
    {
      if (!isFirst) {
        io->print(":");
      }
      if (digits < 10) {
        io->print("0");
      }
      io->print(digits);
    }

    bool askForTimeDigit(String &timeContainer, int maxDigit) {
      char input = io->getValidDigitOrHashBlocking();
      if (input == '#') {
        return 0;
      } else if (input && (int)input - 48 <= maxDigit) {
        io->print(input);
        timeContainer += input;
        return 1;
      } else {
        //restart user input
        return getNewTime();
      }
    }

    //deals with user input and validation
    int* getNewTime()
    {
      String hour{}, minute{};
      
      //remove mess that was potentially left behind by invalid input
      io->setCursor(0, 2);
      io->print("                    ");
      io->showCursor();

      //get hour
      io->setCursor(0, 1);
      io->print("Enter hour (#=Quit):");
      io->setCursor(0, 2);
      io->print("Hour: ");

      //get MSB hour digit
      if (!askForTimeDigit(hour, 2)) {
        return 0;
      }
      //get LSB hour digit
      if (hour == "1") {
        if (!askForTimeDigit(hour, 9)) {
          return 0;
        }
      } else {
        if (!askForTimeDigit(hour, 3)) {
          return 0;
        }
      }

      //get minute
      io->setCursor(0, 1);
      io->print("                    ");
      io->setCursor(0, 1);
      io->print("Enter min (#=Quit):");
      io->setCursor(0, 2);
      io->print("Minute: ");

      //get MSB minute digit
      if (!askForTimeDigit(minute, 5)) {
        return 0;
      }
      //get LSB minute digit
      if (!askForTimeDigit(minute, 9)) {
        return 0;
      }

      io->hideCursor();

      int* time = new int[2];
      time[0] = hour.toInt();
      time[1] = minute.toInt();
      return time;
    }

  public:
    ClockMode(ILogger *_logger, UserIO *_io)
      : logger{_logger}, io(_io), IMode("Clock mode") {}

    void resetAll() override
    {
      changeTime(0, 0);
    }

    //asks user to input new hour and minute before setting time to user input
    void changeTimeFromUserInput()
    {
      int* newTime = getNewTime();
      if (!newTime) {
        return;
      }

      int hour{newTime[0]}, minute{newTime[1]};
      delete[] newTime;

      //hour, minute, second, day, month, year
      setTime(hour, minute, 0, 0, 0, 0);
    }
    //sets time according to function arguments hour and minute
    void changeTime(int hour, int minute)
    {
      //hour, minute, second, day, month, year
      setTime(hour, minute, 0, 0, 0, 0);
    }

    void digitalClockDisplay() const
    {
      io->setCursor(0, 0);
      io->print("Current time: ");
      printDigits(hour(), true);
      printDigits(minute());
      io->print(" ");
    }

    int getHour() const
    {
      return hour();
    }

    int getMinute() const
    {
      return minute();
    }
};

#endif
