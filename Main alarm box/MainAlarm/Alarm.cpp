#include "Arduino.h"

#include "Alarm.h"

Alarm::Alarm()
  : id{255}, hour{255}, minute{255}, isEnabled{false} {}

Alarm::Alarm(byte _hour, byte _minute, bool _isEnabled)
  : id{currentId}, hour{_hour}, minute{_minute}, isEnabled{_isEnabled}
{
  currentId++;

  //make sure that currentId is never zero (overflow)
  if (currentId == 0)
  {
    currentId++;
  }
}

byte Alarm::getId() const
{
  return id;
}
byte Alarm::getHour() const
{
  return hour;
}
byte Alarm::getMinute() const
{
  return minute;
}
bool Alarm::getStatus() const
{
  return isEnabled;
}

int Alarm::setStatus(bool newStatus)
{
  isEnabled = newStatus;
}

byte Alarm::currentId = 1;
