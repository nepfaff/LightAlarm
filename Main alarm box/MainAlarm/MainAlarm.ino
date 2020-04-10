#include "IMode.h"
#include "SerialLogger.h"
#include "UserIO.h"
#include "ClockMode.h"
#include "AlarmMode.h"
#include "TimerMode.h"

//keypad setup (needs to be prior to Setup, hence cannot do it inside UserIO constructor)
const byte rows = 4;
const byte cols = 4;
char keys[rows][cols] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[rows] {49, 48, 47, 46};
byte colPins[cols] {53, 52, 51, 50};
Keypad *keyIn = new Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

//logging service
SerialLogger *logger;

//user IO
UserIO *io;

//setup different modes
ClockMode *clockMode;
AlarmMode *alarmMode;
TimerMode *timerMode;

//mode container for polymorthism
const byte modeNumber {
  4
}; //default mode also counts as mode (see switch statement)
IMode* modes[modeNumber];

//current mode
int currentMode{0};

//default display of modes on screen (mode 0)
int displayModeIntervalMS{1500}; //determines time between displaying different mode options on screen
unsigned long previousMillis{};
int currentDisplayedMode{1}; //don't display default mode 0

void setup()
{
  Serial.begin(9600);

  //initialize pointers (need to do this after Serial.begin(9600) for it to work)
  logger = new SerialLogger();
  io = new UserIO(logger, keyIn);
  clockMode = new ClockMode(logger, io);
  alarmMode = new AlarmMode(logger, io, clockMode);
  timerMode = new TimerMode(logger, io);
  //leave modes[0] empty as this represents the default mode
  modes[1] = alarmMode;
  modes[2] = timerMode;
  modes[3] = clockMode;

  //set default time
  clockMode->changeTime(0, 0);
}

void loop()
{
  //display if an alarm is enabled
  //show by showing * in top right corner?


  //change functionality based on current mode
  if (currentMode != 0) {
    //reset currentDisplayMode to make sure that start in order when back at mode 0
    currentDisplayedMode = 1;
  }
  switch (currentMode) {
    case 0:
      //display current time
      io->setCursor(0, 0);
      clockMode->digitalClockDisplay();

      //display different mode options
      unsigned long currentMillis = millis();
      if ((unsigned long)(currentMillis - previousMillis) >= displayModeIntervalMS) {
        io->setCursor(0, 2);
        io->print("Select one:");
        String name = modes[currentDisplayedMode]->getModeName();
        io->setCursor(0, 3);
        io->print(currentDisplayedMode);
        io->print(". ");
        io->print(name);

        //decide which mode will be displayed next
        if (currentDisplayedMode < modeNumber - 1) {
          currentDisplayedMode++;
        } else {
          currentDisplayedMode = 1;
        }

        previousMillis = currentMillis; //restart display mode interval
      }

      //act on user input (based on mode display)
      currentMode = io->getValidModeInt(modeNumber);
      break;
    case 1:
      //alarm mode
      break;
    case 2:
      //timer mode
      //setting new time
      break;
    case 3:
      //clock mode
      break;
    default:
      logger->logError("Tried to access non existing mode", "MainAlarm, switch statement");
      currentMode = 0;
      logger->logInfo("Switched mode to default mode 0");
      break;
  }

  //for general LCD display
  //probably good idea to allow keys 1-9 to select options but only always display 1 or 2 options from menu
  //and assign key for displaying next two options
}
