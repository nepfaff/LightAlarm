#ifndef ClockMode_h
#define ClockMode_h

#include "Arduino.h"
#include "TimeLib.h"
#include "Wire.h"
#include "LiquidCrystal_I2C.h"
#include "Keypad.h"

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
    //LiquidCrystal_I2C *screen;
    //Keypad keyIn;

    int hour{};
    int minute{};

    // utility function for digital clock display: prints preceding colon and leading 0
    void printDigits(int digits, bool isFirst = false)
    {
      if (!isFirst) {
        io->print(":");
      }
      if (digits < 10) {
        io->print("0");
      }
      io->print(digits);
    }

  public:
    ClockMode(ILogger *_logger, UserIO *_io)
      : logger{_logger}, io(_io) {}

    void resetAll() override
    {
      hour = 0;
      minute = 0;
      changeTime(hour, minute);
    }

    int* askForNewTime()
    {
      int hour{}, minute{};
      //write script that displays questions on LCD and uses keypad to get input

      //include checking if user entered invalid time e.g. hour = 25 or minute = A1
      //maybe do checking by only allowing certain input from keyboard and otherwise just wait till valid input entered

      //provide quit option (keep current time) e.g. #

      int* time = new int[2];
      time[0] = hour;
      time[1] = minute;
      return time;
    }

    //asks user to input new hour and minute before setting time to user input
    void changeTime()
    {
      int* newTime = askForNewTime();
      int hour{newTime[0]}, minutes{newTime[1]};
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

    void digitalClockDisplay()
    {
      io->setCursor(0, 0);
      io->print("Current time: ");
      printDigits(hour, true);
      printDigits(minute);
      io->print(" ");
    }

    int getHour()
    {
      return hour;
    }

    int getMinute()
    {
      return minute;
    }
};

#endif
