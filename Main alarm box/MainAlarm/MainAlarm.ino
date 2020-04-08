#include "Vector.h"

#include "IMode.h"
#include "SerialLogger.h"
#include "UserIO.h"
#include "ClockMode.h"
#include "AlarmMode.h"
#include "TimerMode.h"

//user IO
UserIO *io;

//logging service
SerialLogger *logger{};

//setup different modes
ClockMode clockMode(logger, io);
AlarmMode alarmMode(logger, io);
TimerMode timerMode(logger, io);

//mode container for polymorthism
const byte modeNumber {
  3
};
IMode* modes[modeNumber] {&alarmMode, &timerMode, &clockMode};

//current mode
int currentMode{};

void setup()
{
  Serial.begin(9600);

  //set default time
  clockMode.changeTime(0, 0);
}

void loop()
{
   //Serial.println("Test0");
   logger->logInfo("Test0");
   
  //display current time
  clockMode.digitalClockDisplay();

  Serial.println("Test1");

  //display if an alarm is enabled
  //show by showing * in top right corner?
  

  //change functionality based on current mode
  switch (currentMode) {
    case 1:
      //alarm mode
      break;
    case 2:
      //timer mode
      break;
    case 3:
      //clock mode
      //setting new time
      break;
    default:
      //mode 0
      //clock and different mode option display
      io->setCursor(0, 2);
      io->print("Select one:");
      io->setCursor(0, 1);
      for (int i{}; i < modeNumber; i++) {
        String name = modes[i]->getModeName();
        io->print(i);
        io->print(" ");
        io->print(name);
        delay(500); //instead of using blocking delay, use interval checker to only replace mode name when specific interval is over
      }
      break;
  }

  //for general LCD display
  //probably good idea to allow keys 1-9 to select options but only always display 1 or 2 options from menu
  //and assign key for displaying next two options
}
