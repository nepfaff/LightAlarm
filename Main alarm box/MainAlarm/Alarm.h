#ifndef Alarm_h
#define Alarm_h

#include "Arduino.h"

class Alarm
{
  private:
    int id;
    int hour;
    int minute;
    bool enabled;
    //could pottetially add soundrack name to select from sd card
  public:
    Alarm(int _hour, int _minute, bool _enabled)
      : hour{_hour}, minute{_minute}, enabled{_enabled} {}

    //only provide getters => Cannot modify alarm time after creation (needs to be destoyed and recreated)
    int getHour() const
    {
      return hour;
    }
    int getMinute() const
    {
      return minute;
    }
    int getStatus() const
    {
      return enabled;
    }
    int setStatus(bool newStatus)
    {
      enabled = newStatus;
    }
};

#endif
