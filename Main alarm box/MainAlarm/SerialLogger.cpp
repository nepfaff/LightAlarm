#include "Arduino.h"

#include "SerialLogger.h"
#include "ILogger.h"

void SerialLogger::logError(String msg, String errorLocation) const
{
    Serial.print(F("ERROR: "));
    Serial.println(msg);
    Serial.print(F("ERROR LOCATION: "));
    Serial.println(errorLocation);
}

void SerialLogger::logInfo(String msg) const
{
    Serial.print(F("Info: "));
    Serial.println(msg);
}
