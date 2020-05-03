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
  void logError(String msg, String errorLocation) const override;

  void logInfo(String msg) const override;
};

#endif
