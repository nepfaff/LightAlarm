#ifndef CommSystem_h
#define CommSystem_h

#include "Arduino.h"
#include "SoftwareSerial.h"

#include "ILogger.h"

//used for bluetooth communication with light box
SoftwareSerial *BTSerial;

//keep track of current light master enable state in light box => will be reset automatically on reboot
bool currentLightMasterEnabelState;

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

      //ensure that state is consistent with state in light box
      resetLightMasterEnable();
    }

    //applies for automatic light functionality (manuel functinality unaffected)
    void toggleLightMasterEnable(){
      logger->logInfo("Toggeling light master enable");
      if(currentLightMasterEnabelState){
        BTSerial->write("M"); //M = change master light enable state to state passed as data argument
        BTSerial->write(false);
        currentLightMasterEnabelState = false;
      }else{
        BTSerial->write("M"); 
        BTSerial->write(true);
        currentLightMasterEnabelState = true;
      }
    }

    //used to ensure that stored state is consistent with state in light box
    void resetLightMasterEnable(){
      //default state is true (enabled)
      currentLightMasterEnabelState = false;
      toggleLightMasterEnable();
    }

    void enableLightBasedOnDutyCycle(byte dutyCycle) const{
      logger->logInfo(F("Sending command to activate light now"));

        BTSerial->write("D"); //D = enable light based on duty cycle argument
        BTSerial->write(dutyCycle);
    }

    void enableLightBasedOnTimeTillAlarmMin(int timeTillAlarmMin) const {
      //only enable light every certain interval (otherwise bluetooth buffer will overflow and data will be corrupted)
      unsigned long currentLightEnableMS = millis();
      if ((unsigned long)(currentLightEnableMS - previousLightEnableMS) >= enableLightIntervalMS) {
        logger->logInfo(F("Sending command to activate light based on time"));

        BTSerial->write("T"); //T = enable light based on time argument
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
