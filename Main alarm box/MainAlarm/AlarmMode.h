#ifndef AlarmMode_h
#define AlarmMode_h

#include "Arduino.h"
#include "Alarm.h"

//use current time (fuctions take ClockMode object as argument) and compare this with set time
//provide multiple default alarms (start with 1-3). These are Alarm objects
class AlarmMode : public IMode
{
private:
    vector<Alarm> alarms {}

public:
    void enableExistingAlarm() {}
    void disableExistingAlarm() {}
    void createNewAlarm() {}
    void displayExistingAlarms() {} //time and status (enabled or not)
    bool checkIfAlarmTime() {}      //checks whether to ring alarm now (comparisson with current time)
}

#endif