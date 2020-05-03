#include "TimerMode.h"

TimerMode::TimerMode(ILogger *_logger, UserIO *_io)
    : logger{_logger}, io(_io), isEnabled{false}, isPaused{false}, IMode("Timer mode") {}

void resetAll() override
{
    timeAtStartMS = 0;
    timerDurationMS = 0;
    isEnabled = false;
    isPaused = false;
}

byte TimerMode::getNumberOfOptions() const
{
    return numberOfOptions;
}

void TimerMode::displayTimeLeft() const
{
    io->setCursor(0, 0);

    if (isEnabled)
    {
        const unsigned long timeLeftMS = isPaused ? (timeAtStartMS + timerDurationMS - timeAtPauseStartMS) : (timeAtStartMS + timerDurationMS - millis());
        int secondsLeft = (int)((timeLeftMS / 1000UL) % 60UL);
        int minutesLeft = (int)((timeLeftMS / (1000UL * 60UL)) % 60UL);
        int hoursLeft = (int)(timeLeftMS / (1000UL * 60UL * 60UL));

        io->print(F("Time left: "));
        io->printDigits(hoursLeft, true);
        io->printDigits(minutesLeft);
        io->printDigits(secondsLeft);
    }
    else
    {
        io->print(F("No timer set"));
    }
}

void TimerMode::displayOptions()
{
    const unsigned long displayOptionsIntervalMS PROGMEM = 2000;
    //display different timer mode options
    unsigned long currentTimerMillis = millis();
    if ((unsigned long)(currentTimerMillis - previousTimerMillis) >= displayOptionsIntervalMS)
    {
        //don't want to clear first row as used to display time left
        io->clearRow(1);
        io->clearRow(2);
        io->clearRow(3);
        io->setCursor(0, 1);
        io->print("Select one (#=Quit):");
        for (int i{2}; i < 4 && currentDisplayedOption < numberOfOptions; i++)
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

        previousTimerMillis = currentTimerMillis; //restart display option interval
    }
}

byte TimerMode::selectOption()
{
    byte option = io->selectOption(numberOfOptions);
    if (option)
    {
        currentDisplayedOption = 0; //make sure to start with displaying first option when displayOptions() is entered again
    }
    return option;
}

void TimerMode::executeOption(int selectedOption)
{
    switch (selectedOption)
    {
    case 1: //set new timer
        //blocking operation
        setNewTimer();
        break;
    case 2: //disable timer
        disableTimer();
        break;
    case 3: //pause timer
        pauseTimer();
        break;
    case 4: //resume timer
        resumeTimer();
        break;
    case 5: //restart timer
        restartTimer();
        break;
    default:
        logger->logError(F("Tried to execute non-existing timer option"), F("TimerMode, executeOption"));
        break;
    }
    previousTimerMillis = 0; //eliminate any possible delay
}

void TimerMode::setNewTimer()
{
    io->clearScreen();
    io->setCursor(0, 0);
    io->print(F("Entering timer time"));
    int *durations = io->getHoursMinutesSeconds();
    if (!durations)
    {
        return;
    }

    int hours{durations[0]}, minutes{durations[1]}, seconds{durations[2]};
    delete[] durations;

    isEnabled = true;
    timeAtStartMS = millis();
    timerDurationMS = (hours * 60UL * 60UL * 1000UL + minutes * 60UL * 1000UL + seconds * 1000UL);
}

void TimerMode::disableTimer()
{
    resetAll();
}

void TimerMode::pauseTimer()
{
    if (!isEnabled || isPaused)
    {
        logger->logError(F("Tried to pause already paused or disabled timer"), F("TimerMode, pauseTimer"));
        return;
    }
    isPaused = true;
    timeAtPauseStartMS = millis();
}

void TimerMode::resumeTimer()
{
    if (!isEnabled || !isPaused)
    {
        logger->logError(F("Tried to resume non-paused timer"), F("TimerMode, resumeTimer"));
        return;
    }
    timerDurationMS += millis() - timeAtPauseStartMS; //icnrease duration by pause time
    isPaused = false;
}

void TimerMode::restartTimer()
{
    timeAtStartMS = millis();
}

bool TimerMode::hasTimerFinished() const
{
    //also considers rare case of millis() overflow
    if (isEnabled && !isPaused && (unsigned long)(millis() - timeAtStartMS) >= timerDurationMS)
    {
        return true;
    }
    else
    {
        return false;
    }
}