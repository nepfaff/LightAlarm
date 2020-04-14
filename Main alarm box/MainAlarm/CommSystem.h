#ifndef CommSystem_h
#define CommSystem_h

#include "Arduino.h"

#include "ILogger.h"

//responsible for communication with other microcontrollers
class CommSystem
{
  private:
    ILogger *logger{};

  public:
    CommSystem(ILogger *_logger)
      : logger{_logger} {}
};

#endif
