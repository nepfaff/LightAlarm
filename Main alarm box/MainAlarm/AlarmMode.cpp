#include "Arduino.h"

#include "AlarmMode.h"
#include "Alarm.h"
#include "IMode.h"
#include "ILogger.h"
#include "UserIO.h"
#include "CommSystem.h"
#include "StorageSystem.h"

AlarmMode::AlarmMode(ILogger *_logger, UserIO *_io, ClockMode *_clock, CommSystem *_commSystem, StorageSystem *_storageSystem)
    : logger{_logger}, io(_io), clock{_clock}, commSystem{_commSystem}, storageSystem{_storageSystem}, IMode("Alarm mode")
{

    //retrieve alarms that were stored in EEPROM
    currentAlarmQuantity = storageSystem->retrieveStoredAlarmQuantityEEPROM();
    Alarm *storedAlarms = storageSystem->retrieveAlarmsEEPROM(currentAlarmQuantity);
    for (int i{}; i < currentAlarmQuantity; i++)
    {
        alarms[i] = storedAlarms[i];
    }
    delete[] storedAlarms;
}

void AlarmMode::resetAll() override
{
    for (int i{}; i < maxAlarmQuantity; i++)
    {
        alarms[i] = {};
    }
    currentAlarmQuantity = 0;

    commSystem->disableLight();

    //does reseting for EEPROM
    storageSystem->clearEEPROM();
}

byte AlarmMode::getNumberOfOptions() const
{
    return numberOfOptions;
}

int AlarmMode::getTimeToActivateLightMin() const
{
    return timeToActivateLightMin;
}

void AlarmMode::displayOptions()
{
    const unsigned long displayOptionsIntervalMS PROGMEM = 3000;
    //display different alarm mode options
    unsigned long currentAlarmMillis = millis();
    if ((unsigned long)(currentAlarmMillis - previousAlarmMillis) >= displayOptionsIntervalMS)
    {
        io->clearScreen();
        io->setCursor(0, 0);
        io->print("Select one (#=Quit):");
        for (int i{1}; i < 4 && currentDisplayedOption < numberOfOptions; i++)
        {
            String optionName = optionNames[currentDisplayedOption];
            io->setCursor(0, i);
            io->print(optionName);
            currentDisplayedOption++;
        }

        //check if need to rollover to first option
        if (currentDisplayedOption >= numberOfOptions)
        {
            currentDisplayedOption = 0;
        }

        previousAlarmMillis = currentAlarmMillis; //restart display option interval
    }
}

byte AlarmMode::selectOption()
{
    byte option = io->selectOption(numberOfOptions);
    if (option)
    {
        currentDisplayedOption = 0; //make sure to start with displaying first option when displayOptions() is entered again
    }
    return option;
}

void AlarmMode::executeOption(int selectedOption)
{
    switch (selectedOption)
    {
    case 1:                      //display existing alarms
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
    case 6: //delete all existing alarms
        deleteAllExistingAlarms();
        break;
    default:
        logger->logError(F("Tried to execute non-existing alarm option"), F("AlarmMode, executeOption"));
        break;
    }
    previousAlarmMillis = 0; //eliminate any possible delay

    //store any updates made to alarms => can restore them after power outage
    storageSystem->updateAlarmsEEPROM(alarms, currentAlarmQuantity);
}

void AlarmMode::displayExistingAlarms()
{
    const unsigned long displayAlarmsIntervalMS PROGMEM = 3000;

    //continue alarm display until # is entered (quit)
    while (!io->enteredHash())
    {
        unsigned long currentAlarmMillis = millis();
        if ((unsigned long)(currentAlarmMillis - previousAlarmMillis) >= displayAlarmsIntervalMS)
        {
            io->clearScreen();
            io->setCursor(0, 0);
            io->print(F("Alarms (#=Quit):"));

            if (currentAlarmQuantity == 0)
            {
                io->setCursor(0, 2);
                io->print(F("0 alarms to display"));
            }
            else
            {
                Alarm alarm = alarms[currentDisplayedAlarm];
                io->setCursor(0, 1);
                io->print(F("Alarm "));
                io->print(currentDisplayedAlarm + 1); //first alarm has number 1 and is stored in alarms[0]
                io->setCursor(0, 2);
                io->print(F("Time: "));
                io->printDigits(alarm.getHour(), true);
                io->printDigits(alarm.getMinute());
                io->setCursor(0, 3);
                io->print(F("Status: "));
                io->print(alarm.getStatus() ? F("Enabled") : F("Disabled"));

                currentDisplayedAlarm++;

                //check if need to rollover to first option
                if (currentDisplayedAlarm >= currentAlarmQuantity)
                {
                    currentDisplayedAlarm = 0;
                }
            }

            previousAlarmMillis = currentAlarmMillis; //restart display option interval
        }
    }
    currentDisplayedAlarm = 0; //make sure to start with displaying first alarm when displayExistingAlarms is entered again
}

void AlarmMode::createNewAlarm()
{
    io->clearScreen();

    if (currentAlarmQuantity >= maxAlarmQuantity)
    {
        while (!io->enteredHash())
        {
            io->setCursor(0, 0);
            io->print(F("Max alarm quantity"));
            io->setCursor(0, 1);
            io->print(F("reached"));
            io->setCursor(0, 3);
            io->print(F("Enter # to quit"));
        }
    }
    else
    {
        io->setCursor(0, 0);
        io->print(F("Entering alarm time"));
        int *alarmTime = io->getTime();
        if (!alarmTime)
        {
            return;
        }

        //intentionally using a narrowing conversion
        byte hour{alarmTime[0]}, minute{alarmTime[1]};
        delete[] alarmTime;

        Alarm newAlarm(hour, minute, true);
        alarms[currentAlarmQuantity] = newAlarm;
        currentAlarmQuantity++;
    }
}

void AlarmMode::deleteExistingAlarm()
{
    io->clearScreen();

    if (currentAlarmQuantity == 0)
    {
        while (!io->enteredHash())
        {
            io->setCursor(0, 0);
            io->print(F("No alarm to display"));
            io->setCursor(0, 2);
            io->print(F("Enter # to quit"));
        }
    }
    else
    {
        int alarmNumber{}; //alarm index in alarms[] will be alarmNumber - 1

        io->setCursor(0, 3);
        io->print(F("Enter # to quit"));
        io->setCursor(0, 0);
        io->print(F("Enter number of"));
        io->setCursor(0, 1);
        io->print(F("alarm to delete: "));
        io->showCursor();
        char input = io->getValidDigitOrHashBlocking();

        if (input == '#')
        {
            return;
        }
        else if (input && (int)input - 48 <= currentAlarmQuantity)
        {
            io->print(input);
            io->hideCursor();
            delay(500); //for user to see their input

            int alarmIdx{(int)input - 49}; //alarm index in alarms[] (alarm number - 1)

            //disable light that was enabled due to an alarm ringing soon
            if (checkIfActivatedLightByAlarm(alarms[alarmIdx]))
            {
                commSystem->disableLight();
            }

            if (alarmNumber == currentAlarmQuantity - 1)
            {
                //remove last element from array
                alarms[alarmIdx] = {};
            }
            else
            {
                //removing element from middle of array and hence need to make sure that existing alarms fill first spaces of array and not existing alarms are at end of array
                Alarm remainingAlarms[maxAlarmQuantity]{};
                int j{};
                for (int i{}; i < maxAlarmQuantity; i++)
                {
                    if (i != alarmIdx)
                    {
                        remainingAlarms[j] = alarms[i];
                        j++;
                    }
                }
                memcpy(alarms, remainingAlarms, sizeof(alarms));
            }
            currentAlarmQuantity--;
        }
        else
        {
            //restart user input
            deleteExistingAlarm();
        }
    }
}

void AlarmMode::changeExistingAlarmStatus(bool newStatus)
{
    io->clearScreen();

    if (currentAlarmQuantity == 0)
    {
        while (!io->enteredHash())
        {
            io->setCursor(0, 0);
            if (newStatus)
            {
                io->print(F("No alarm to enable"));
            }
            else
            {
                io->print(F("No alarm to disable"));
            }
            io->setCursor(0, 2);
            io->print(F("Enter # to quit"));
        }
    }
    else
    {
        int alarmNumber{}; //alarm index in alarms[] will be alarmNumber - 1

        io->setCursor(0, 3);
        io->print(F("Enter # to quit"));
        io->setCursor(0, 0);
        io->print(F("Enter number of"));
        io->setCursor(0, 1);
        if (newStatus)
        {
            io->print(F("alarm to enable: "));
        }
        else
        {
            io->print(F("alarm to disable: "));
        }
        io->showCursor();
        char input = io->getValidDigitOrHashBlocking();

        if (input == '#')
        {
            return;
        }
        else if (input && (int)input - 48 <= currentAlarmQuantity)
        {
            io->print(input);
            io->hideCursor();
            delay(500); //for user to see their input

            int alarmIdx{(int)input - 49}; //alarm index in alarms[] (alarm number - 1)
            alarms[alarmIdx].setStatus(newStatus);
        }
        else
        {
            //restart user input
            deleteExistingAlarm();
        }
    }
}

void AlarmMode::deleteAllExistingAlarms()
{
    io->clearScreen();

    //disable light that was enabled due to an alarm ringing soon
    for (int i{}; i < currentAlarmQuantity; i++)
    {
        if (checkIfActivatedLightByAlarm(alarms[i]))
        {
            commSystem->disableLight();
        }
    }

    memset(alarms, 0, sizeof(alarms));
    currentAlarmQuantity = 0;

    io->setCursor(0, 1);
    io->print(F("Deleted all"));
    io->setCursor(0, 2);
    io->print(F("existing alarms"));
    delay(1300);
}

void AlarmMode::changeExistingAlarmStatusOnId(int id, bool newStatus)
{
    for (int i{}; i < currentAlarmQuantity; i++)
    {
        if (alarms[i].getId() == id)
        {
            alarms[i].setStatus(newStatus);
        }
    }
}

bool AlarmMode::isAnyAlarmEnabled() const
{
    for (int i{}; i < currentAlarmQuantity; i++)
    {
        if (alarms[i].getStatus())
        {
            return true;
        }
    }
    return false;
}

int AlarmMode::getActiveAlarmId() const
{
    for (int i{}; i < currentAlarmQuantity; i++)
    {
        if (alarms[i].getStatus() && alarms[i].getHour() == clock->getHour() && alarms[i].getMinute() == clock->getMinute())
        {
            return alarms[i].getId();
        }
    }
    return 0;
}

bool AlarmMode::activateLight() const
{
    for (int i{}; i < currentAlarmQuantity; i++)
    {
        if (alarms[i].getStatus() && alarms[i].getMinute() >= timeToActivateLightMin && alarms[i].getHour() == clock->getHour() && alarms[i].getMinute() - timeToActivateLightMin == clock->getMinute())
        {
            return true;
        }
        else if (alarms[i].getStatus() && (alarms[i].getHour() - 1) == clock->getHour() && (alarms[i].getMinute() + 60 - timeToActivateLightMin) == clock->getMinute())
        {
            return true;
        }
    }
    return false;
}

bool AlarmMode::checkIfActivatedLightByAlarm(const Alarm &alarm) const
{
    if (alarm.getStatus() && alarm.getMinute() >= timeToActivateLightMin && alarm.getHour() == clock->getHour() && alarm.getMinute() - timeToActivateLightMin <= clock->getMinute())
    {
        return true;
    }
    else if (alarm.getStatus() && (alarm.getHour() - 1) == clock->getHour() && (alarm.getMinute() + 60 - timeToActivateLightMin) <= clock->getMinute())
    {
        return true;
    }
    return false;
}