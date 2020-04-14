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
          io->print("Max alarm quantity");
          io->setCursor(0, 1);
          io->print("reached");
          io->setCursor(0, 3);
          io->print("Enter # to quit");
        }
      } else {
        io->setCursor(0, 0);
        io->print("Entering alarm time");
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
          io->print("No alarm to display");
          io->setCursor(0, 2);
          io->print("Enter # to quit");
        }
      } else {
        int alarmNumber{}; //alarm index in alarms[] will be alarmNumber - 1

        io->setCursor(0, 3);
        io->print("Enter # to quit");
        io->setCursor(0, 0);
        io->print("Enter number of");
        io->setCursor(0, 1);
        io->print("alarm to delete: ");
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
            io->print("No alarm to enable");
          } else {
            io->print("No alarm to disable");
          }
          io->setCursor(0, 2);
          io->print("Enter # to quit");
        }
      } else {
        int alarmNumber{}; //alarm index in alarms[] will be alarmNumber - 1

        io->setCursor(0, 3);
        io->print("Enter # to quit");
        io->setCursor(0, 0);
        io->print("Enter number of");
        io->setCursor(0, 1);
        if (newStatus) {
          io->print("alarm to enable: ");
        } else {
          io->print("alarm to disable: ");
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

    void changeExistingAlarmStatusOnId(int id, bool newStatus){
      for(int i{}; i<currentAlarmQuantity; i++){
        if(alarms[i].getId() == id){
          alarms[i].setStatus(newStatus);
        }
      }
    }
};

#endif
