#ifndef AlarmMode_h
#define AlarmMode_h

#include "Arduino.h"

#include "Alarm.h"
#include "IMode.h"
#include "ILogger.h"
#include "UserIO.h"

class AlarmMode : public IMode
{
  private:
    const static byte maxAlarmQuantity PROGMEM = 3;
    const static byte timeToActivateLightMin PROGMEM = 1;

    int currentAlarmQuantity;
    int currentDisplayedAlarm;
    Alarm alarms[maxAlarmQuantity] {}; //position 0 in array holds alarm with number 1

    unsigned long previousAlarmMillis;

    const static byte numberOfOptions PROGMEM = 5;
    const String optionNames[numberOfOptions] PROGMEM = {"1. Display alarms", "2. New alarm", "3. Delete alarm", "4. Enable alarm", "5. Disable alarm"};
    byte currentDisplayedOption{};

    ILogger *logger;
    UserIO *io;
    ClockMode *clock;

  public:
    AlarmMode(ILogger *_logger, UserIO *_io, ClockMode *_clock)
      : logger{_logger}, io(_io), clock{_clock}, IMode("Alarm mode") {}

    void resetAll() override
    {
      for (int i{}; i < maxAlarmQuantity; i++)
      {
        alarms[i] = {};
      }
      currentAlarmQuantity = 0;
    }

    int getTimeToActivateLightMin() const {
      return timeToActivateLightMin;
    }

    void displayOptions() {
      const unsigned long displayOptionsIntervalMS PROGMEM = 3000;
      //display different alarm mode options
      unsigned long currentAlarmMillis = millis();
      if ((unsigned long)(currentAlarmMillis - previousAlarmMillis) >= displayOptionsIntervalMS) {
        io->clearScreen();
        io->setCursor(0, 0);
        io->print("Select one (#=Quit):");
        for (int i{1}; i < 4 && currentDisplayedOption < numberOfOptions; i++) {
          String optionName = optionNames[currentDisplayedOption];
          io->setCursor(0, i);
          io->print(optionName);
          currentDisplayedOption++;
        }

        //check if need to rollover to first option
        if (currentDisplayedOption >= numberOfOptions) {
          currentDisplayedOption = 0;
        }

        previousAlarmMillis = currentAlarmMillis; //restart display option interval
      }
    }

    //0 when no option selected yet, 100 when quit
    int selectOption() {
      char input = io->getValidDigitOrHash();
      if (input == '#') {
        currentDisplayedOption = 0; //make sure to start with displaying first option when alarmMode is entered again
        return 100;
      } else if (input && (int)input - 48 <= numberOfOptions) {
        currentDisplayedOption = 0;
        return (int)input - 48;
      } else {
        return 0;
      }
    }

    //executes selected option
    //blocking operation
    void executeOption(int selectedOption) {
      switch (selectedOption) {
        case 1: //display existing alarms
          previousAlarmMillis = 0; //eliminate any possible delay
          displayExistingAlarms();
          break;
        case 2: //create new alarm
          createNewAlarm();
          break;
        case 3: //delete existing alarm
          deleteExistingAlarm();
          break;
        case 4: //enable existing alarm
          changeExistingAlarmStatus(true);
          break;
        case 5: //disable existing alarm
          changeExistingAlarmStatus(false);
          break;
        default:
          logger->logError("Tried to execute non-existing alarm option", "AlarmMode, executeOption");
          break;
      }
    }

    //option 1
    void displayExistingAlarms()
    {
      const unsigned long displayAlarmsIntervalMS PROGMEM = 3000;

      //continue alarm display until # is entered (quit)
      while (!io->enteredHash()) {
        unsigned long currentAlarmMillis = millis();
        if ((unsigned long)(currentAlarmMillis - previousAlarmMillis) >= displayAlarmsIntervalMS) {
          io->clearScreen();
          io->setCursor(0, 0);
          io->print(F("Alarms (#=Quit):"));

          if (currentAlarmQuantity == 0) {
            io->setCursor(0, 2);
            io->print(F("0 alarms to display"));
          } else {
            Alarm alarm = alarms[currentDisplayedAlarm];
            io->setCursor(0, 1);
            io->print(F("Alarm "));
            io->print(currentDisplayedAlarm + 1); //first alarm has number 1 and is stored in alarms[0]
            io->setCursor(0, 2);
            io->print("Time: ");
            io->printDigits(alarm.getHour(), true);
            io->printDigits(alarm.getMinute());
            io->setCursor(0, 3);
            io->print("Status: ");
            io->print(alarm.getStatus() ? F("Enabled") : F("Disabled"));

            currentDisplayedAlarm++;

            //check if need to rollover to first option
            if (currentDisplayedAlarm >= currentAlarmQuantity) {
              currentDisplayedAlarm = 0;
            }
          }

          previousAlarmMillis = currentAlarmMillis; //restart display option interval
        }
      }
      currentDisplayedAlarm = 0; //make sure to start with displaying first alarm when displayExistingAlarms is entered again
    }

    //option 2
    void createNewAlarm()
    {
      io->clearScreen();

      if (currentAlarmQuantity >= maxAlarmQuantity)
      {
        while (!io->enteredHash()) {
          io->setCursor(0, 0);
          io->print(F("Max alarm quantity"));
          io->setCursor(0, 1);
          io->print(F("reached"));
          io->setCursor(0, 3);
          io->print(F("Enter # to quit"));
        }
      } else {
        io->setCursor(0, 0);
        io->print(F("Entering alarm time"));
        int* alarmTime = io->getTime();
        if (!alarmTime) {
          return;
        }

        int hour{alarmTime[0]}, minute{alarmTime[1]};
        delete[] alarmTime;

        Alarm newAlarm(hour, minute, true);
        alarms[currentAlarmQuantity] = newAlarm;
        currentAlarmQuantity++;
      }
    }

    //option 3
    void deleteExistingAlarm()
    {
      io->clearScreen();

      if (currentAlarmQuantity == 0)
      {
        while (!io->enteredHash()) {
          io->setCursor(0, 0);
          io->print(F("No alarm to display"));
          io->setCursor(0, 2);
          io->print(F("Enter # to quit"));
        }
      } else {
        int alarmNumber{}; //alarm index in alarms[] will be alarmNumber - 1

        io->setCursor(0, 3);
        io->print(F("Enter # to quit"));
        io->setCursor(0, 0);
        io->print(F("Enter number of"));
        io->setCursor(0, 1);
        io->print(F("alarm to delete: "));
        io->showCursor();
        char input = io->getValidDigitOrHashBlocking();

        if (input == '#') {
          return;
        } else if (input && (int)input - 48 <= currentAlarmQuantity) {
          io->print(input);
          io->hideCursor();
          delay(500); //for user to see their input

          int alarmIdx{(int)input - 49}; //alarm index in alarms[] (alarm number - 1)

          if (alarmNumber == currentAlarmQuantity - 1) {
            //remove last element from array
            alarms[alarmIdx] = {};
          } else {
            //removing element from middle of array and hence need to make sure that existing alarms fill first spaces of array and not existing alarms are at end of array
            Alarm remainingAlarms[maxAlarmQuantity] {};
            int j{};
            for (int i{}; i < maxAlarmQuantity; i++) {
              if (i != alarmIdx) {
                remainingAlarms[j] = alarms[i];
                j++;
              }
            }
            memcpy(alarms, remainingAlarms, sizeof alarms);
          }
          currentAlarmQuantity--;

        } else {
          //restart user input
          deleteExistingAlarm();
        }
      }
    }

    //option 4 and 5 (true to enable, false to disable)
    void changeExistingAlarmStatus(bool newStatus)
    {
      io->clearScreen();

      if (currentAlarmQuantity == 0)
      {
        while (!io->enteredHash()) {
          io->setCursor(0, 0);
          if (newStatus) {
            io->print(F("No alarm to enable"));
          } else {
            io->print(F("No alarm to disable"));
          }
          io->setCursor(0, 2);
          io->print(F("Enter # to quit"));
        }
      } else {
        int alarmNumber{}; //alarm index in alarms[] will be alarmNumber - 1

        io->setCursor(0, 3);
        io->print(F("Enter # to quit"));
        io->setCursor(0, 0);
        io->print(F("Enter number of"));
        io->setCursor(0, 1);
        if (newStatus) {
          io->print(F("alarm to enable: "));
        } else {
          io->print(F("alarm to disable: "));
        }
        io->showCursor();
        char input = io->getValidDigitOrHashBlocking();

        if (input == '#') {
          return;
        } else if (input && (int)input - 48 <= currentAlarmQuantity) {
          io->print(input);
          io->hideCursor();
          delay(500); //for user to see their input

          int alarmIdx{(int)input - 49}; //alarm index in alarms[] (alarm number - 1)
          alarms[alarmIdx].setStatus(newStatus);
        } else {
          //restart user input
          deleteExistingAlarm();
        }
      }
    }

    void changeExistingAlarmStatusOnId(int id, bool newStatus) {
      for (int i{}; i < currentAlarmQuantity; i++) {
        if (alarms[i].getId() == id) {
          alarms[i].setStatus(newStatus);
        }
      }
    }

    //checks whether it is time for an alarm to ring by comparing alarm time with current time
    //returns id of active alarm or 0 if no active alarm
    int getActiveAlarmId() const {
      for (int i{}; i < currentAlarmQuantity; i++) {
        if (alarms[i].getStatus() && alarms[i].getHour() == clock->getHour() && alarms[i].getMinute() == clock->getMinute())
        {
          return alarms[i].getId();
        }
      }
      return 0;
    }

    //start activating light set time before alarm starts
    bool activateLight() {
      for (int i{}; i < currentAlarmQuantity; i++) {
        if (alarms[i].getStatus() && alarms[i].getMinute() >= timeToActivateLightMin && alarms[i].getHour() == clock->getHour() && alarms[i].getMinute() - timeToActivateLightMin == clock->getMinute()) {
          return true;
        }
        else if (alarms[i].getStatus() && (alarms[i].getHour() - 1) == clock->getHour() && (alarms[i].getMinute() + 60 - timeToActivateLightMin) == clock->getMinute())
        {
          return true;
        }
      }
      return false;
    }
};

#endif
