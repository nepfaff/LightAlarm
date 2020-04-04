#ifndef SerialLogger_h
#define SerialLogger_h

#include "Arduino.h"

class SerialLogger : ILogger
{
public:
    virtual void logError(String msg, String errorLocation) override
    {
        Serial.print("ERROR: ");
        Serial.prinln(msg);
        Serial.print("ERROR LOCATION: ");
        Serial.println(errorLocation);
    }
    virtual void logInfo(String msg) override
    {
        Serial.print("Info: ");
        Serial.println(msg);
    }
}

#endif