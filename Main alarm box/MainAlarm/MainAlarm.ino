#include "IMode.h"
#include "SerialLogger.h"
#include "UserIO.h"
#include "ClockMode.h"
#include "AlarmMode.h"
#include "TimerMode.h"

//logging service
SerialLogger *logger;

//user IO
UserIO *io;;

//setup different modes
ClockMode *clockMode;
AlarmMode *alarmMode;
TimerMode *timerMode;

//mode container for polymorthism
const byte modeNumber {
  3
};
IMode* modes[modeNumber];

//current mode
int currentMode{};

void setup()
{
  Serial.begin(9600);

  //initialize pointers (need to do this after Serial.begin(9600) for it to work)
  logger = new SerialLogger();
  io = new UserIO(logger);
  clockMode = new ClockMode(logger, io);
  alarmMode = new AlarmMode(logger, io, clockMode);
  timerMode = new TimerMode(logger, io);
  modes[0] = alarmMode;
  modes[1] = timerMode;
  modes[2] = clockMode;

  //set default time
  clockMode->changeTime(0, 0);
}

void loop()
{
  //display current time
  clockMode->digitalClockDisplay();

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
      for (int i{}; i < modeNumber; i++) {
        String name = modes[i]->getModeName();
        io->setCursor(0, 3);
        io->print(i);
        io->print(" ");
        io->print(name);
        delay(1000); //instead of using blocking delay, use interval checker to only replace mode name when specific interval is over
      }
      break;
  }

  //for general LCD display
  //probably good idea to allow keys 1-9 to select options but only always display 1 or 2 options from menu
  //and assign key for displaying next two options
}
