#ifndef Alarm_h
#define Alarm_h

#include "Arduino.h"

class Alarm
{
public:
    int hour;
    int minute;
    bool enabled;
    //could pottetially add soundrack name to select from sd card
}

#endif