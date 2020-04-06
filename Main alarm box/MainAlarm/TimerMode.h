#ifndef TimerMode_h
#define TimerMode_h

#include "Arduino.h"

//try to get time from keypad (hour, min, second), then convert into milliseconds and keep comparing with millis()
class TimerMode : public IMode, public ILogger
{
private:
    int timerDurationMS; //duration of timer in milliseconds
    int timeAtStartMS;   //set this equal to millis when timer started
    bool enabled;
    ILogger logger;

public:
    TimerMode(ILogger _logger)
        : logger{_logger}, timerDurationMS{0}, timeAtStartMS{0}, enabled{false} {}

    void resetAll() override
    {
        timeAtStartMs = 0;
        timerDurationMS = 0;
        enabled = false;
    }

    void setNewTimer(int hours, int minutes, int seconds)
    {
        enabled = true;
        timeAtStartMS = millis();
        timerDurationMS = hours * 60 * 60 * 1000 + minutes * 60 * 1000 + seconds * 1000;
    } //convert into one milliseconds number

    void pauseTimer() {} //pause time is simply added to timerDurationMS
    void resumeTimer() {}

    void restartTimer()
    {
        timeAtStartMS = millis();
    } //reset timeAtStartMS

    bool checkIfTimerFinished() //use result to ring alarm and display message on LCD (don't forget to reset timer once user pressed a key -> stop alarm ringing)
    {
        //also considers rare case of millis() overflow
        if (enabled && (unsigned long)(millis() - timeAtStartMS) >= timerDurationMS)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

#endif