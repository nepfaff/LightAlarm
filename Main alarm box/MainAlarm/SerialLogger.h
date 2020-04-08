#ifndef SerialLogger_h
#define SerialLogger_h

#include "Arduino.h"

#include "ILogger.h"

/*
    Logging service using Arduino's serial 9600 communication channel
*/
class SerialLogger : public ILogger
{
  public:
    virtual void logError(String msg, String errorLocation) const override
    {
      Serial.print("ERROR: ");
      Serial.println(msg);
      Serial.print("ERROR LOCATION: ");
      Serial.println(errorLocation);
    }
    virtual void logInfo(String msg) const override
    {
      Serial.print("Info: ");
      Serial.println(msg);
    }
};

#endif
