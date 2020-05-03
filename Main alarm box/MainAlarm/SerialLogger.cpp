#include "SerialLogger.h"

virtual void SerialLogger::logError(String msg, String errorLocation) const override
{
    Serial.print(F("ERROR: "));
    Serial.println(msg);
    Serial.print(F("ERROR LOCATION: "));
    Serial.println(errorLocation);
}
virtual void SerialLogger::logInfo(String msg) const override
{
    Serial.print(F("Info: "));
    Serial.println(msg);
}