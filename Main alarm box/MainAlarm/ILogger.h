#ifndef ILogger_h
#define ILogger_h

#include "Arduino.h"

class ILogger
{
  public:
    virtual ~ILogger();
    //error location is a combination of class and function
    virtual void logError(String msg, String errorLocation) = 0;
    virtual void logInfo(String msg) = 0;
};

#endif
