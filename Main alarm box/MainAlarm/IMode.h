#ifndef IMode_h
#define IMode_h

#include "Arduino.h"

/*
  Interface that all modes for alarm box should be derived from.
  A mode is a mode that the user can enter (through the main menu), to acsess a set of mode-specific functionalities.
  This allows for dynamic polymorphism.
*/
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

    String getModeName()
    {
      return modeName;
    }

    //reset all internal storage to default values
    virtual void resetAll() = 0;
};

#endif
