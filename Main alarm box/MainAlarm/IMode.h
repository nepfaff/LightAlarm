#ifndef IMode_h
#define IMode_h

#include "Arduino.h"

class IMode
{
  public:
    virtual ~IMode() {};

    //reset all internal storage to default values
    virtual void resetAll() = 0;
};

#endif
