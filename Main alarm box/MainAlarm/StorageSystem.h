#ifndef StorageSystem_h
#define StorageSystem_h

#include "Arduino.h"
#include "EEPROM.h"

#include "ILogger.h"
#include "Alarm.h"

/*
   Responsible for everything that involves storage/memory.
   Examples are EEPROM and SD cards.
*/
class StorageSystem
{
  private:
    ILogger *logger{};

    const static int storedAlarmQuantityEEPROMAddress = 0;
    const static int alarmEEPROMAddress = 1; //keep 9*2=18 spaces free for storing alarms

  public:
    StorageSystem(ILogger *_logger);

    //clears the entire EEPROM by replacing all stored values with 0
    //update rather than set functionality is used
    void clearEEPROM();

    byte retrieveStoredAlarmQuantityEEPROM() const;

    //updates all alarms stored in EEPROM by comparing the stored alarms with the current alarms
    void updateAlarmsEEPROM(const Alarm *alarms, byte currentAlarmQuantity) const;

    //retrieves the number of alarms specified by argument from EEPROM
    Alarm *retrieveAlarmsEEPROM(byte storedAlarmQuantity) const;
};

#endif
