#ifndef Alarm_h
#define Alarm_h

#include "Arduino.h"

class Alarm
{
  private:
    static int currentId; //id needs to start with 1 as 0 is used for returning false

    int id;
    int hour;
    int minute;
    bool enabled;
    //could pottetially add soundrack name to select from sd card

  public:
    Alarm()
      : id{ -1}, hour{ -1}, minute{ -1}, enabled{false} {}
    Alarm(int _hour, int _minute, bool _enabled)
      : id{currentId}, hour{_hour}, minute{_minute}, enabled{_enabled} {
      currentId++;

      //make sure that currentId is never zero (overflow)
      if(currentId == 0){
        currentId++;
      }
    }

    //only provide getters => Cannot modify alarm time after creation (needs to be destoyed and recreated)
    int getId() const
    {
      return id;
    }
    int getHour() const
    {
      return hour;
    }
    int getMinute() const
    {
      return minute;
    }
    bool getStatus() const
    {
      return enabled;
    }
    
    int setStatus(bool newStatus)
    {
      enabled = newStatus;
    }
};

int Alarm::currentId = 1;

#endif
