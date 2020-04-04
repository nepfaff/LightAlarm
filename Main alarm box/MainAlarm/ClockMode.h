#ifndef ClockMode_h
#define ClockMode_h

#include "Arduino.h"
#include <TimeLib.h>

/*
    Functionality involving the current time
*/
class ClockMode : public IMode, public ILogger
{
private:
    ILogger logger{};

    int hour{};
    int minutes{};

    // utility function for digital clock display: prints preceding colon and leading 0
    void printDigits(int digits)
    {
        Serial.print(":");
        if (digits < 10)
            Serial.print('0');
        Serial.print(digits);
    }

public:
    ClockMode(ILogger _logger)
        : logger{_logger} {}

    void resetAll() override
    {
        hour = 0;
        minutes = 0;
    }

    int[2] askForNewTime()
    {
        int hour{}, minute{};
        //write script that displays questions on LCD and uses keypad to get input

        //include checking if user entered invalid time e.g. hour = 25 or minute = A1
        //maybe do checking by only allowing certain input from keyboard and otherwise just wait till valid input entered

        //provide quit option (keep current time) e.g. #

        return [ hour, minute ];
    }

    void changeTime()
    {
        int newTime[2] = askForNewTime();
        int hour{newTime[0]}, minutes{newTime[1]};

        //hour, minute, second, day, month, year
        setTime(hour, minute, 0, 0, 0, 0);
    }

    void digitalClockDisplay()
    {
        // digital clock display of the time => change this to print to LCD screen
        Serial.print(hour());
        printDigits(minute());
        Serial.println("");
    }

    int getHour()
    {
        return hour();
    }

    int getMinute()
    {
        return minute();
    }
}

#endif