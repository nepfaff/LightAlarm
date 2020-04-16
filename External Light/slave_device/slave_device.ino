#include <SoftwareSerial.h>

//define new serial communication pins
SoftwareSerial BTSerial(2, 3); //RX | TX => arduino RX must be connected to HC-05 TX, and TX to RX

//define pins
const byte lightPin PROGMEM = A0;

//variables to store ms till duty cycle
unsigned long dutyCycle25, dutyCycle50, dutyCycle75, dutyCycle100;
bool lightByTimeEn{};

void setup() {
  //serial used for master-slave bluetooth communication
  BTSerial.begin(38400);
  Serial.begin(9600);

  //setup pins
  pinMode(lightPin, OUTPUT);
}

void loop() {
  //BTSerial.available() tells us how many bytes are waiting in buffer
  while (BTSerial.available() >= 2) {

    char instruction = BTSerial.read();
    int data = BTSerial.read();

    switch (instruction) {
      case 'T': //enable light based on time argument from data (calculate switch in duty cycle based on time)
        Serial.print(F("Starting to activate light using time till fully on of minutes: "));
        Serial.println(data);

        computeTimeForDutyCycles(data * 60000); //convert into ms
        lightByTimeEn = true;
        break;
      case 'D': //enable light based on duty cycle argument from data
        Serial.print(F("Starting to activate light using specified PWM duty cycle of "));
        Serial.println(data);

        enableLightBasedOnDutyCycle(data);
        break;
      case 'X': //disable any current light or light timer
        disableLigth();
        break;
      case 'O': //enable light with max brightness
        enableLightBasedOnDutyCycle(100);
        break;
      default:
        Serial.print(F("Unknown instruction: "));
        Serial.println(instruction);
        break;
    }
  }

  //check if currently running a 'T' instruction
  if (lightByTimeEn) {
    unsigned long currentTime = millis();
    if (currentTime > dutyCycle100) {
      analogWrite(lightPin, 255);
    } else if (currentTime > dutyCycle50) {
      analogWrite(lightPin, 127);
    } else if (currentTime > dutyCycle25) {
      analogWrite(lightPin, 64);
    }
  }
}

void computeTimeForDutyCycles(unsigned long timeTillFullyOnMS) {
  unsigned long timeForOneCycle = timeTillFullyOnMS / 3; //have 3 duty cycles till duty cycle = 100
  unsigned long currentTime = millis();
  dutyCycle25 = currentTime; //start this now
  dutyCycle50 = currentTime + timeForOneCycle;
  dutyCycle75 = currentTime + timeForOneCycle * 2;
  dutyCycle100 = currentTime + timeForOneCycle * 3;
}

void enableLightBasedOnDutyCycle(int dutyCycle) {
  //convert percentage into value accepted by analogWrite
  byte mappedDutyCycle = map(dutyCycle, 0, 100, 0, 255);

  analogWrite(lightPin, mappedDutyCycle);
}

void disableLigth() {
  analogWrite(lightPin, 0);
  lightByTimeEn = false;
}
