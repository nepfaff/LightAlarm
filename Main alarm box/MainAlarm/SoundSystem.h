#ifndef SoundSystem_h
#define SoundSystem_h

#include "Arduino.h"

#include "ILogger.h"

const int activeBuzzerPin PROGMEM = 2;

/*
   rings buzzers, plays sd audio files, etc.
*/
class SoundSystem
{
  private:
    ILogger *logger{};

  public:
    SoundSystem(ILogger *_logger)
      : logger{_logger} {
      pinMode(activeBuzzerPin, OUTPUT);
    }

    void startRingingBuzzerAlarm()
    {
      logger->logInfo(F("Starting to ring buzzer alarm"));
      digitalWrite(activeBuzzerPin, HIGH);
    }

    void stopRingingBuzzerAlarm()
    {
      logger->logInfo(F("Stopping to ring buzzer alarm"));
      digitalWrite(activeBuzzerPin, LOW);
    }
};

#endif
