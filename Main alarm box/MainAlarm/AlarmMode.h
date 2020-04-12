#ifndef AlarmMode_h
#define AlarmMode_h

#include "Arduino.h"

#include "Alarm.h"
#include "IMode.h"
#include "ILogger.h"
#include "UserIO.h"

//use current time (fuctions take ClockMode object as argument) and compare this with set time
//provide multiple default alarms (start with 1-3). These are Alarm objects
class AlarmMode : public IMode
{
  private:
    const static int maxAlarmQuantity {
      3
    };
    int currentAlarmQuantity{};
    int currentDisplayedAlarm{};
    Alarm alarms[maxAlarmQuantity] {}; //position 0 in array holds alarm with number 1

    unsigned long previousAlarmMillis{};

    const static int numberOfOptions{5};
    const String optionNames[numberOfOptions] {"1. Display alarms", "2. New alarm", "3. Delete alarm", "4. Enable alarm", "5. Disable alarm"};
    int currentDisplayedOption{};

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

    void displayOptions() {
      const int displayOptionsIntervalMS{3000};

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
        currentDisplayedOption = 0; //will make sure to start with displaying first option when alarmMode is entered again
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
          displayExistingAlarms();
          break;
        case 2: //create new alarm
          break;
        case 3: //delete existing alarm
          break;
        case 4: //enable existing alarm
          break;
        case 5: //disable existing alarm
          break;
        default:
          logger->logError("Tried to execute non-existing alarm option", "AlarmMode, executeOption");
          break;
      }
    }

    //option 1
    void displayExistingAlarms()
    {
      const int displayAlarmsIntervalMS{3000};

      //continue alarm display until # is entered (quit)
      while (!io->enteredHash()) {
        unsigned long currentAlarmMillis = millis();
        if ((unsigned long)(currentAlarmMillis - previousAlarmMillis) >= displayAlarmsIntervalMS) {
          io->clearScreen();
          io->setCursor(0, 0);
          io->print("Alarms (#=Quit):");

          if (currentAlarmQuantity == 0) {
            io->setCursor(0, 2);
            io->print("0 alarms to display");
          } else {
            Alarm alarm = alarms[currentDisplayedAlarm];
            io->setCursor(0, 1);
            io->print("Alarm ");
            io->print(currentDisplayedAlarm + 1); //first alarm has number 1 and is stored in alarms[0]
            io->setCursor(0, 2);
            io->print("Time: ");
            io->printDigits(alarm.getHour(), true);
            io->printDigits(alarm.getMinute());
            io->setCursor(0, 3);
            io->print("Status: ");
            io->print(alarm.getStatus() ? "Enabled" : "Disabled");

            currentDisplayedAlarm++;

            //check if need to rollover to first option
            if (currentDisplayedAlarm >= currentAlarmQuantity) {
              currentDisplayedAlarm = 0;
            }
          }

          previousAlarmMillis = currentAlarmMillis; //restart display option interval
        }
      }
    }

    //option 2
    bool createNewAlarm()
    {
      //some basic input validation here
      if (currentAlarmQuantity >= maxAlarmQuantity)
      {
        logger->logError("Cannot exceed maximum alarm quantity", "AlarmMode, createNewAlarm");
        //write some error to LCD
        return false;
      }

      //ask user for alarm time and get data (LCD screen and keypad)
      //include error checking (use of letters, exceeding range for hour 0-23, range for minute 0-59)
      //probably best to check directly from keyboard e.g. first hour digit must be 0, 1 or 2 (all other inputs are illegal and result in user having to retsart entering time)
      int hour = 00;
      int minute = 00;

      Alarm newAlarm(hour, minute, true);
      alarms[currentAlarmQuantity] = newAlarm;
      currentAlarmQuantity++;
      return true;
    }

    //option 3
    bool deleteExistingAlarm()
    {
      //get alarm number from input (represents an index)
      int alarmNumber{};
      if (alarmNumber > currentAlarmQuantity - 1) {
        logger->logError("Trying to delete an alarm based on a not existing alarmNumber", "AlarmMode, deleteExistingAlarm");
        return false;
      }

      if (alarmNumber == currentAlarmQuantity - 1) {
        //remove last element from array
        alarms[alarmNumber] = {};
      } else {
        //removing element from middle of array and hence need to make sure that existing alarms fill first spaces of array and not existing alarms are at end of array
        Alarm remainingAlarms[maxAlarmQuantity] {};
        int j{};
        for (int i{}; i < maxAlarmQuantity; i++) {
          if (i != alarmNumber) {
            remainingAlarms[j] = alarms[i];
            j++;
          }
        }
      }
      currentAlarmQuantity--;
      return true;
    }

    //option 4
    void enableExistingAlarm()
    {
      //get alarmNumber from keypad (use input validation e.g. checking that <= currentAlarmQuantity)
      int alarmNumber{}; //alarmNumber corresponds to position in alarms array
      alarms[alarmNumber].setStatus(true);
    }

    //option 5
    void disableExistingAlarm()
    {
      //get alarmNumber from keypad (use input validation)
      int alarmNumber{};
      alarms[alarmNumber].setStatus(false);
    }

    //checks whether to ring sound system now (comparisson with current time)
    //when true, need to ring sound system and disable alarm
    bool checkIfAlarmTime(const Alarm &alarm, const ClockMode *clock)
    {
      if (alarm.getHour() == clock->getHour() && alarm.getMinute() <= clock->getMinute())
      { //check if alarm time is now or has just passed
        return true;
      }
      else
      {
        return false;
      }
    }
};

#endif
