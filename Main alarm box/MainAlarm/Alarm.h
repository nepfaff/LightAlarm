#ifndef Alarm_h
#define Alarm_h

class Alarm
{
private:
  //id needs to start with 1 as 0 is used for returning false
  static byte currentId;

  byte id;
  byte hour;
  byte minute;
  bool isEnabled;
  //could pottetially add soundrack name to select from sd card

public:
  Alarm();
  Alarm(byte _hour, byte _minute, bool _isEnabled);

  //only provide getters => Cannot modify alarm time after creation (needs to be destoyed and recreated)
  byte getId() const;
  byte getHour() const;
  byte getMinute() const;
  bool getStatus() const;

  int setStatus(bool newStatus);
};

#endif
