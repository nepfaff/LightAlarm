#include "IMode.h"
#include "SerialLogger.h"
#include "UserIO.h"
#include "ClockMode.h"
#include "AlarmMode.h"
#include "TimerMode.h"
#include "SoundSystem.h"
#include "CommSystem.h"
#include "StorageSystem.h"

//keypad setup (needs to be prior to Setup, hence cannot do it inside UserIO constructor)
const byte rows PROGMEM = 4;
const byte cols PROGMEM = 4;
const char keys[rows][cols] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
const byte rowPins[rows] = {49, 48, 47, 46};
const byte colPins[cols] = {53, 52, 51, 50};
Keypad *keyIn = new Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

//logging service
SerialLogger *logger;

//user IO
UserIO *io;

//sound, communication and storage system
SoundSystem *soundSystem;
CommSystem *commSystem;
StorageSystem *storageSystem;

//setup different modes
ClockMode *clockMode;
AlarmMode *alarmMode;
TimerMode *timerMode;

//mode container for polymorthism
const byte modeNumber PROGMEM = 4; //default mode also counts as mode (see switch statement)
IMode* modes[modeNumber];

//current mode
int currentMode{0};

//default display of modes on screen (mode 0)
const unsigned long displayModeIntervalMS PROGMEM = 1500; //determines time between displaying different mode options on screen
unsigned long previousMillis{};
int currentDisplayedMode = 1; //don't display default mode 0

//time after last key press for screen's backlight to turn off
const unsigned long backLightIntervalMS PROGMEM = 300000; //5min
unsigned long lastKeyPressMillis{};

//light master enable for automatic light functionality
const byte lightStateToggleButton PROGMEM = 3;
const unsigned long lightMasterEnableIntervalMS = 1000; //prevent multiple toggles caused by one button press
unsigned long previousLightMasterEnableMillis{};

void setup()
{
  Serial.begin(9600);

  //add event to reset screen off timer when key is pressed
  keyIn->addEventListener(anyKeyPressed);

  //used by external button (enable/disable light automatic light functionality)
  pinMode(lightStateToggleButton, INPUT);

  //initialize pointers (need to do this after Serial.begin(9600) for it to work)
  logger = new SerialLogger();
  io = new UserIO(logger, keyIn);
  soundSystem = new SoundSystem(logger);
  commSystem = new CommSystem(logger);
  storageSystem = new StorageSystem(logger);
  clockMode = new ClockMode(logger, io);
  alarmMode = new AlarmMode(logger, io, clockMode, commSystem, storageSystem);
  timerMode = new TimerMode(logger, io);
  //leave modes[0] empty as this represents the default mode
  modes[1] = alarmMode;
  modes[2] = timerMode;
  modes[3] = clockMode;

  //turn of light if send turn on before power loss
  commSystem->disableLight();

  //set default time
  clockMode->changeTime(0, 0);
}

void loop()
{
  //display if an alarm is enabled
  //show by showing * in top right corner?

  //check if disable screen's backlight as no key pressed for specified time
  if ((unsigned long)(millis() - lastKeyPressMillis) >= backLightIntervalMS) {
    io->disableBacklight();
  }

  //logic for what happens if an alarm is active
  int activeAlarmId = alarmMode->getActiveAlarmId();
  if (activeAlarmId) {
    io->clearScreen();

    //enable light as light might have been disabled before alarm time (e.g. timer activated and deactivated light since light command has been send)
    commSystem->enableLightBasedOnDutyCycle(100); //dutyCycle = 100 corresponds to maximum brightness
    soundSystem->startRingingBuzzerAlarm();

    while (!io->getKey()) {
      io->setCursor(0, 0);
      clockMode->digitalClockDisplay();
      io->setCursor(0, 2);
      io->print(F("Press any key"));
      io->setCursor(0, 3);
      io->print(F("to disable the alarm"));
    }

    soundSystem->stopRingingBuzzerAlarm();
    commSystem->disableLight();
    alarmMode->changeExistingAlarmStatusOnId(activeAlarmId, false);

    currentMode = 0;
    io->clearScreen();
  }

  //happens prior to an alarm becoming active
  if (alarmMode->activateLight()) {
    commSystem->enableLightBasedOnTimeTillAlarmMin(alarmMode->getTimeToActivateLightMin());
  }

  //logic for what happens if the timer is over
  if (timerMode->hasTimerFinished()) {
    io->clearScreen();

    commSystem->enableLightBasedOnDutyCycle(100); //dutyCycle = 100 corresponds to maximum brightness
    soundSystem->startRingingBuzzerAlarm();

    while (!io->getKey()) {
      io->setCursor(0, 1);
      io->print(F("Press any key"));
      io->setCursor(0, 2);
      io->print(F("to disable the timer"));
    }

    soundSystem->stopRingingBuzzerAlarm();
    commSystem->disableLight();
    timerMode->disableTimer();

    currentMode = 0;
    io->clearScreen();
  }

  //enable/disable automatic light functionality
  unsigned long currentLightMasterEnableMillis = millis();
  if ((unsigned long)(currentLightMasterEnableMillis  - previousLightMasterEnableMillis) >= lightMasterEnableIntervalMS) {
    if (digitalRead(lightStateToggleButton)) {
      commSystem->toggleLightMasterEnable();

      previousLightMasterEnableMillis = currentLightMasterEnableMillis;
    }
  }

  //change functionality based on current mode
  if (currentMode != 0) {
    //reset currentDisplayMode to make sure that start in order when back at mode 0
    currentDisplayedMode = 1;
  }
  if (currentMode == 0) { //default mode
    //display current time
    io->setCursor(0, 0);
    clockMode->digitalClockDisplay();

    //display different mode options
    unsigned long currentMillis = millis();
    if ((unsigned long)(currentMillis - previousMillis) >= displayModeIntervalMS) {
      io->setCursor(0, 2);
      io->print(F("Select one:"));

      //display "A" if an alarm is enabled
      if (alarmMode->isAnyAlarmEnabled()) {
        io->setCursor(18, 2);
        io->print(F("A"));
      }else{
        //make sure that no invalid state is displayed
        //io->clearRow(2); //add better clearing command
      }
      //display "L" if automatic light functionality is enabled
      if (commSystem->getCurrentLightMasterEnabelState()) {
        io->setCursor(19, 2);
        io->print(F("L"));
      }else{
        //make sure that no invalid state is displayed
        //io->clearRow(2);
      }

      String name = modes[currentDisplayedMode]->getModeName();
      io->setCursor(0, 3);
      io->print(currentDisplayedMode);
      io->print(F(". "));
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
    if (currentMode != 0) {
      io->clearScreen();
    }
  } else if (currentMode == 1) { //alarm mode
    alarmMode->displayOptions();
    int currentOption = alarmMode->selectOption();

    //0 when no option selected yet, 100 when quit
    if (currentOption == 100) {
      currentMode = 0;
    } else if (currentOption) {
      alarmMode->executeOption(currentOption);
      io->clearScreen();
    }

    if (currentMode != 1) {
      io->clearScreen();
    }
  } else if (currentMode == 2) { //timer mode
    timerMode->displayTimeLeft();
    timerMode->displayOptions();
    int currentOption = timerMode->selectOption();

    //0 when no option selected yet, 100 when quit
    if (currentOption == 100) {
      currentMode = 0;
    } else if (currentOption) {
      timerMode->executeOption(currentOption);
      io->clearScreen();
    }

    if (currentMode != 2) {
      io->clearScreen();
    }
  } else if (currentMode == 3) { //clock mode
    //display current time
    io->setCursor(0, 0);
    clockMode->digitalClockDisplay();

    //set new time based on user input
    clockMode->changeTimeFromUserInput();

    currentMode = 0;
    io->clearScreen();
  } else {
    logger->logError(F("Tried to access non existing mode"), F("MainAlarm, switch statement"));
    currentMode = 0;
    logger->logInfo(F("Switched mode to default mode 0"));
  }
}

//turn of screen backlight when no key has been pressed for certain time
void anyKeyPressed(KeypadEvent key) {
  lastKeyPressMillis = millis();
  io->enableBacklight();
}
