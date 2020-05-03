#include "SoundSystem.h"

SoundSystem::SoundSystem(ILogger *_logger)
    : logger{_logger}
{
    pinMode(activeBuzzerPin, OUTPUT);
}

void SoundSystem::startRingingBuzzerAlarm()
{
    logger->logInfo(F("Starting to ring buzzer alarm"));
    digitalWrite(activeBuzzerPin, HIGH);
}

void SoundSystem::stopRingingBuzzerAlarm()
{
    logger->logInfo(F("Stopping to ring buzzer alarm"));
    digitalWrite(activeBuzzerPin, LOW);
}