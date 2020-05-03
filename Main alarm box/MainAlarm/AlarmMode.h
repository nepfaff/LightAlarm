#ifndef AlarmMode_h
#define AlarmMode_h

#include "Arduino.h"

#include "Alarm.h"
#include "IMode.h"
#include "ILogger.h"
#include "UserIO.h"
#include "CommSystem.h"
#include "StorageSystem.h"

/*
  Provides an alarm functionality.
  The user is able to set multiple alarms (max quantity is hard coded below but can be changed without breaking the functionality).
*/
class AlarmMode : public IMode
{
private:
  const static byte maxAlarmQuantity PROGMEM = 3;        //current maximum supported quantity is 9
  const static byte timeToActivateLightMin PROGMEM = 20; //change based on testing experience

  int currentAlarmQuantity{};
  int currentDisplayedAlarm{};
  Alarm alarms[maxAlarmQuantity]{}; //position 0 in array holds alarm with number 1

  unsigned long previousAlarmMillis;

  const static byte numberOfOptions PROGMEM = 6;
  const String optionNames[numberOfOptions] = {"1. Display alarms", "2. New alarm", "3. Delete alarm", "4. Enable alarm", "5. Disable alarm", "6. Delete all alarms"};
  byte currentDisplayedOption{};

  ILogger *logger;
  UserIO *io;
  ClockMode *clock;
  CommSystem *commSystem;
  StorageSystem *storageSystem;

public:
  AlarmMode(ILogger *_logger, UserIO *_io, ClockMode *_clock, CommSystem *_commSystem, StorageSystem *_storageSystem);

  void resetAll() override;

  byte getNumberOfOptions() const;

  int getTimeToActivateLightMin() const;

  //displays all available options
  void displayOptions();

  //uses UserIO to get the selected option form the user input
  byte selectOption();

  //executeds an option based on the option's number
  void executeOption(int selectedOption);

  //option 1
  void displayExistingAlarms();

  //option 2
  void createNewAlarm();

  //option 3
  void deleteExistingAlarm();

  //option 4 and 5 (true to enable, false to disable)
  void changeExistingAlarmStatus(bool newStatus);

  //option 6
  void deleteAllExistingAlarms();

  //true to enable, false to disable
  void changeExistingAlarmStatusOnId(int id, bool newStatus);

  //checks if any alarm is enabled
  bool isAnyAlarmEnabled() const;

  //checks whether it is time for an alarm to ring by comparing alarm time with current time
  //returns id of active alarm, or 0 if no active alarm exists
  int getActiveAlarmId() const;

  //returns true if should activate the light (alarm is going to ring soon as defined by set time)
  bool activateLight() const;

  //returns true if the light has been activated due to an alarm ringing soon
  //this is usefull for knowing if we might have to turn of the light again
  bool checkIfActivatedLightByAlarm(const Alarm &alarm) const;
};

#endif
