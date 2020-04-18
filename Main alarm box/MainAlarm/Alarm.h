#ifndef Alarm_h
#define Alarm_h

#include "Arduino.h"

class Alarm
{
  private:
    static byte currentId; //id needs to start with 1 as 0 is used for returning false

    byte id;
    byte hour;
    byte minute;
    bool isEnabled;
    //could pottetially add soundrack name to select from sd card

  public:
    Alarm()
      : id{255}, hour{255}, minute{255}, isEnabled{false} {}
    Alarm(byte _hour, byte _minute, bool _isEnabled)
      : id{currentId}, hour{_hour}, minute{_minute}, isEnabled{_isEnabled} {
      currentId++;

      //make sure that currentId is never zero (overflow)
      if(currentId == 0){
        currentId++;
      }
    }

    //only provide getters => Cannot modify alarm time after creation (needs to be destoyed and recreated)
    byte getId() const
    {
      return id;
    }
    byte getHour() const
    {
      return hour;
    }
    byte getMinute() const
    {
      return minute;
    }
    bool getStatus() const
    {
      return isEnabled;
    }
    
    int setStatus(bool newStatus)
    {
      isEnabled = newStatus;
    }
};

byte Alarm::currentId = 1;

#endif
