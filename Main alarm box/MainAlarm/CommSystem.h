#ifndef CommSystem_h
#define CommSystem_h

#include "Arduino.h"
#include "SoftwareSerial.h"

#include "ILogger.h"

//used for bluetooth communication with light box
SoftwareSerial *BTSerial;

const unsigned long enableLightIntervalMS PROGMEM = 60000;
unsigned long previousLightEnableMS{};

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

    void enableLightBasedOnTimeTillAlarmMin(int timeTillAlarmMin) const {
      Serial.println(millis());
      //only enable light every certain interval (otherwise bluetooth buffer will overflow and data will be corrupted)
      unsigned long currentLightEnableMS = millis();
      if ((unsigned long)(currentLightEnableMS - previousLightEnableMS) >= enableLightIntervalMS) {
        logger->logInfo(F("Sending command to activate light based on time"));

        BTSerial->write("T"); //T = enable light based on time parameter
        BTSerial->write(timeTillAlarmMin);

        previousLightEnableMS = currentLightEnableMS;
      }
    }

    void disableLight() const {
      logger->logInfo(F("Sending command to disable light"));

      BTSerial->write("X");
      BTSerial->write(1);
    }
};

#endif
