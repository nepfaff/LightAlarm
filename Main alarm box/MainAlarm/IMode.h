#ifndef IMode_h
#define IMode_h

#include "Arduino.h"

class IMode
{
  private:
    //display name of mode
    String modeName;

  public:
    IMode()
      : modeName{"Unspecified"} {}
    IMode(String _modeName)
      : modeName{_modeName} {}
    virtual ~IMode() {}

    String getModeName() {
      return modeName;
    }

    //reset all internal storage to default values
    virtual void resetAll() = 0;
};

#endif
