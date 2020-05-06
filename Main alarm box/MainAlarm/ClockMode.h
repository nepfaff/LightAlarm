#ifndef ClockMode_h
#define ClockMode_h

#include "Arduino.h"
#include "TimeLib.h"

#include "IMode.h"
#include "ILogger.h"
#include "UserIO.h"

/*
    Functionality involving the current time.
*/
class ClockMode : public IMode
{
private:
  ILogger *logger;
  UserIO *io;

public:
  ClockMode(ILogger *_logger, UserIO *_io);

  void resetAll() override;

  //asks user to input new hour and minute, before setting time to user input
  void changeTimeFromUserInput();

  //sets time according to function arguments hour and minute
  void changeTime(int hour, int minute);

  //displays hour:minute in digital format
  void digitalClockDisplay() const;

  //getters
  int getHour() const;
  int getMinute() const;
};

#endif
