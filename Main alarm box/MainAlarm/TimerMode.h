#ifndef TimerMode_h
#define TimerMode_h

#include "Arduino.h"

#include "IMode.h"
#include "ILogger.h"
#include "UserIO.h"

/*
  Provides a basic timer functionality.
*/
class TimerMode : public IMode
{
  private:
    unsigned long timerDurationMS;
    unsigned long timeAtStartMS;
    bool isEnabled;
    bool isPaused;
    unsigned long timeAtPauseStartMS;

    const static byte numberOfOptions PROGMEM = 6;
    const String optionNames[numberOfOptions] = {"1. New timer", "2. Disable timer", "3. Pause timer", "4. Resume timer", "5. Restart timer"};
    byte currentDisplayedOption{};

    unsigned long previousTimerMillis;

    ILogger *logger;
    UserIO *io;

  public:
    TimerMode(ILogger *_logger, UserIO *_io);

    void resetAll() override;

    byte getNumberOfOptions() const;

    //displayes the time left until the timer is over
    //if no timer is enabled, a message indicating this will be displayed instead
    void displayTimeLeft() const;

    //displays all available options
    void displayOptions();

    //uses UserIO to get the selected option form the user input
    byte selectOption();

    //executes selected option
    void executeOption(int selectedOption);

    //option 1
    void setNewTimer();

    //option 2
    void disableTimer();

    //option 3
    void pauseTimer();

    //option 4
    void resumeTimer();

    //option 5
    void restartTimer();

    //returns true if the timer's time is over
    bool hasTimerFinished() const;
};

#endif
