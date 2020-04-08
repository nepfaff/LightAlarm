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

//mode container for polymorthism 


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
  //display current time
  clockMode.digitalClockDisplay();

  //change functionality based on current mode
  switch(currentMode){
    case 0:
      //clock and different mode option display
      
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    default:
      logger->logError("The currentMode value doesn't correspond to any existing mode", "main loop, switch statement");
      logger->logInfo("currentMode has been reset to 0");
      currentMode = 0;
      break;
  }

  //for general LCD display
  //probably good idea to allow keys 1-9 to select options but only always display 1 or 2 options from menu
  //and assign key for displaying next two options
}
