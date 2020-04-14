#include <SoftwareSerial.h>

//define new serial communication pins
SoftwareSerial BTSerial(2, 3); //RX | TX => arduino RX must be connected to HC-05 TX, and TX to RX

void setup() {
  //serial used for master-slave bluetooth communication
  BTSerial.begin(38400);
  Serial.begin(9600);
}

void loop() {
  //BTSerial.available() tells us how many bytes are waiting in buffer
  while (BTSerial.available() >= 2) {
    char instruction = BTSerial.read();
    int data = BTSerial.read();

    switch (instruction) {
      case 'T': //enable light based on time argument from data
        Serial.println("Starting to active light using PWM");
        //add code here e.g. timer to increase duty cycle based on fraction of total time left
        //once received, want to wait one minute before acccepting other similar instruction (exception is instruction 'D' for disable) => will receive same instruction for whole minute due to sending and compare functionality 
        break;
      case 'D': //disable any current light or light timer
        //e.g. when user disables alarm after light command has already been send => need to disable any light
        break;
      default:
        Serial.print("Unknown instruction: ");
        Serial.println(instruction);
        break;
    }
  }
}
