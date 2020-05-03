#include "Arduino.h"
#include "TimeLib.h"

#include "ClockMode.h"
#include "IMode.h"
#include "ILogger.h"
#include "UserIO.h"

ClockMode::ClockMode(ILogger *_logger, UserIO *_io)
    : logger{_logger}, io(_io), IMode("Clock mode") {}

void ClockMode::resetAll() override
{
    changeTime(0, 0);
}

void ClockMode::changeTimeFromUserInput()
{
    int *newTime = io->getTime();
    if (!newTime)
    {
        return;
    }

    int hour{newTime[0]}, minute{newTime[1]};
    delete[] newTime;

    //hour, minute, second, day, month, year
    setTime(hour, minute, 0, 0, 0, 0);
}

void ClockMode::changeTime(int hour, int minute)
{
    //hour, minute, second, day, month, year
    setTime(hour, minute, 0, 0, 0, 0);
}

void ClockMode::digitalClockDisplay() const
{
    io->setCursor(0, 0);
    io->print(F("Current time: "));
    io->printDigits(hour(), true);
    io->printDigits(minute());
    io->print(" ");
}

int ClockMode::getHour() const
{
    return hour();
}

int ClockMode::getMinute() const
{
    return minute();
}