#ifndef AlarmMode_h
#define AlarmMode_h

#include "Arduino.h"
#include "Alarm.h"

//use current time (fuctions take ClockMode object as argument) and compare this with set time
//provide multiple default alarms (start with 1-3). These are Alarm objects
class AlarmMode : public IMode, public ILogger
{
private:
    const int maxAlarmQuantity{3};

    vector<*Alarm> alarms{};
    ILogger logger{};

public:
    AlarmMode(ILogger _logger)
        : logger{_logger} {}

    void resetAll() override
    {
        alarms.clear();
    }

    void enableExistingAlarm() {}
    void disableExistingAlarm() {}
    bool createNewAlarm()
    {
        if (maxAlarmQuantity <= alarms.size())
        {
            logger.logError("Cannot exceed maximum alarm quantity", "AlarmMode, createNewAlarm");
            //write some error to LCD
            return false;
        }
        else
        {
            //ask user for alarm time and get data (LCD screen and keypad)
            //include error checking (use of letters, exceeding range for hour 0-23, range for minute 0-59)
            //probably best to check directly from keyboard e.g. first hour digit must be 0, 1 or 2 (all other inputs are illegal and result in user having to retsart entering time)
            int hour = 00;
            int minute = 00;

            alarms.push_back(new Alarm(hour, minute, true));
        }
    }
    void displayExistingAlarms()
    {
        for (auto alarm : alarms)
        {
            //log to LCD rather than Serial
            Serial.print("Alarm 1 => Time: ..., Status: ..."); //status is "Enabled" or "Disabled"
        }
    }                          //time and status (enabled or not)
    bool checkIfAlarmTime() {} //checks whether to ring alarm now (comparisson with current time)
}

#endif