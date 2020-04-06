#ifndef AlarmSound_h
#define AlarmSound_h

#include "Arduino.h"

//rings buzzers, plays sd audio files, etc.
class AlarmSound : public ILogger
{
private:
    ILogger logger{};

public:
    AlarmSound(ILogger _logger)
        : logger{_logger} {}

    void startRingingBuzzerAlarm() //enables buzzers
    {
        logger.logInfo("Starting to ring buzzer alarm");
    }

    void stopRingingBuzzerAlarm()
    {
        logger.logInfo("Stopping to ring buzzer alarm");
    }
}

#endif