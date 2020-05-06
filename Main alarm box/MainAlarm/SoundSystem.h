#ifndef SoundSystem_h
#define SoundSystem_h

#include "Arduino.h"

#include "ILogger.h"

const byte activeBuzzerPin PROGMEM = 2;

/*
   Responsible for ringing buzzers, playing sd audio files.
   All functionality that involves sound specific, external hardware should be added here.
*/
class SoundSystem
{
  private:
    ILogger *logger{};

  public:
    SoundSystem(ILogger *_logger);

    //enables the buzzer alarm
    void startRingingBuzzerAlarm();

    //disables the buzzer alarm
    void stopRingingBuzzerAlarm();
};

#endif
