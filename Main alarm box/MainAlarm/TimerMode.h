#ifndef TimerMode_h
#define TimerMode_h

#include "Arduino.h"

//try to get time from keypad (hour, min, second), then convert into milliseconds and keep comparing with millis()
class TimerMode : public IMode
{
private:
    int timerDurationMS {}    //duration of timer in milliseconds
    int timeAtStartMS {}      //set this equal to millis when timer started
    int timeWhenFinishedMS {} //equal to millis + timer duration
public:
    void setNewTimer(int hours, int minutes, int seconds) {} //convert into one milliseconds number
    void pauseTimer() {}
    void resumeTimer() {}
    void restartTimer() {}         //reset timeAtStartMS
    bool checkIfTimerFinished() {} //comparison of millis and timeWhenFinishedMS
}

#endif