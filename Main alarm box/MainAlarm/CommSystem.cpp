CommSystem::CommSystem(ILogger *_logger)
    : logger{_logger}
{

    BTSerial = new SoftwareSerial(42, 43); //RX | TX => RX beeds to be connected to TX and TX to RX for setup
    BTSerial->begin(38400);

    //ensure that state is consistent with state in light box
    resetLightMasterEnable();
}

bool CommSystem::getCurrentLightMasterEnabelState() const
{
    return currentLightMasterEnabelState;
}

void CommSystem::toggleLightMasterEnable()
{
    logger->logInfo("Toggeling light master enable");
    if (currentLightMasterEnabelState)
    {
        BTSerial->write("M"); //M = change master light enable state to state passed as data argument
        BTSerial->write(false);
        currentLightMasterEnabelState = false;
    }
    else
    {
        BTSerial->write("M");
        BTSerial->write(true);
        currentLightMasterEnabelState = true;
    }
}

void CommSystem::resetLightMasterEnable()
{
    //default state is true (enabled)
    currentLightMasterEnabelState = false;
    toggleLightMasterEnable();
}

void CommSystem::enableLightBasedOnDutyCycle(byte dutyCycle) const
{
    logger->logInfo(F("Sending command to activate light now"));

    BTSerial->write("D"); //D = enable light based on duty cycle argument
    BTSerial->write(dutyCycle);
}

void CommSystem::enableLightBasedOnTimeTillAlarmMin(int timeTillAlarmMin) const
{
    //only enable light every certain interval (otherwise bluetooth buffer will overflow and data will be corrupted)
    unsigned long currentLightEnableMS = millis();
    if ((unsigned long)(currentLightEnableMS - previousLightEnableMS) >= enableLightIntervalMS)
    {
        logger->logInfo(F("Sending command to activate light based on time"));

        BTSerial->write("T"); //T = enable light based on time argument
        BTSerial->write(timeTillAlarmMin);

        previousLightEnableMS = currentLightEnableMS;
    }
}

void CommSystem::disableLight() const
{
    logger->logInfo(F("Sending command to disable light"));

    BTSerial->write("X");
    BTSerial->write(1);
}