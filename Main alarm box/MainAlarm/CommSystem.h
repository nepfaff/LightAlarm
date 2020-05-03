#ifndef CommSystem_h
#define CommSystem_h

#include "Arduino.h"
#include "SoftwareSerial.h"

#include "ILogger.h"

/*
   responsible for communication with other microcontrollers
*/
class CommSystem
{
private:
  //keep track of current light master enable state in light box => will be reset automatically on reboot
  bool currentLightMasterEnabelState;

  //required for enableLightBasedOnTimeTillAlarmMin()
  const unsigned long enableLightIntervalMS PROGMEM = 60000;
  unsigned long previousLightEnableMS{};

  //used for bluetooth communication with light box
  SoftwareSerial *BTSerial;

  ILogger *logger{};

public:
  CommSystem(ILogger *_logger);

  bool getCurrentLightMasterEnabelState() const;

  //turns off ability to enable/disable light from light box
  //applies for automatic light functionality (manuel functinality unaffected)
  void toggleLightMasterEnable();

  //resets the light master enable at both the alarm and light box to its default value (enabled)
  //used to ensure that stored state is consistent with state in light box
  void resetLightMasterEnable();

  //turns on the light box light based on duty cycle for PWM modulation
  void enableLightBasedOnDutyCycle(byte dutyCycle) const;

  //tells the light box to slowly increase its light's duty cycle till the alarm time is reached
  void enableLightBasedOnTimeTillAlarmMin(int timeTillAlarmMin) const;

  //tells the light box to turn of any light that is currently on
  void disableLight() const;
};

#endif
