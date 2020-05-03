#include "StorageSystem.h"

StorageSystem(ILogger *_logger)
    : logger{_logger} {}

//EEPROM
void clearEEPROM()
{
    for (int i = 0; i < EEPROM.length(); i++)
    {
        EEPROM.update(i, 0);
    }
}

byte retrieveStoredAlarmQuantityEEPROM() const
{
    return EEPROM.read(storedAlarmQuantityEEPROMAddress);
}

void updateAlarmsEEPROM(const Alarm *alarms, byte currentAlarmQuantity) const
{
    EEPROM.update(storedAlarmQuantityEEPROMAddress, currentAlarmQuantity);

    for (int i{}; i < currentAlarmQuantity; i++)
    {
        EEPROM.update(alarmEEPROMAddress + i, alarms[i].getHour());
        EEPROM.update(alarmEEPROMAddress + i + 1, alarms[i].getMinute());
    }
}

Alarm *retrieveAlarmsEEPROM(byte storedAlarmQuantity) const
{
    byte id{}, hour{}, minute{};
    bool isEnabled{};
    Alarm *alarms = new Alarm[storedAlarmQuantity];

    for (int i{}; i < storedAlarmQuantity; i++)
    {
        hour = EEPROM.read(alarmEEPROMAddress + i);
        minute = EEPROM.read(alarmEEPROMAddress + i + 1);

        //disable all retrieved alarms to avoid surprises when arduino is powered on after a long time
        Alarm alarm(hour, minute, false);
        alarms[i] = alarm;
    }

    return alarms;
}