#ifndef CommSystem_h
#define CommSystem_h

#include "Arduino.h"
#include "SoftwareSerial.h"

#include "ILogger.h"

//used for bluetooth communication with light box
SoftwareSerial *BTSerial;

/*
   responsible for communication with other microcontrollers
*/
class CommSystem
{
  private:
    ILogger *logger{};

  public:
    CommSystem(ILogger *_logger)
      : logger{_logger} {

      BTSerial = new SoftwareSerial(42, 43); //RX | TX => RX beeds to be connected to TX and TX to RX for setup
      BTSerial->begin(38400);
    }

    void enableLightBasedOnTimeTillAlarm(int timeTillAlarm) {
      logger->logInfo(F("Sending command to activate light based on time"));

      BTSerial->write(F("T")); //T = enable light based on time parameter
      BTSerial->write(timeTillAlarm);
    }
};

#endif
