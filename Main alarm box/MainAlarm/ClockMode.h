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
      int* newTime = io->getTime();
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
      io->printDigits(hour(), true);
      io->printDigits(minute());
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
