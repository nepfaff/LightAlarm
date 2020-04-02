#include <SoftwareSerial.h>

//define new serial communication pins 
SoftwareSerial BTSerial(2, 3); //RX | TX => arduino RX must be connected to HC-05 TX, and TX to RX

void setup() {
  //serial used for master-slave bluetooth communication
  BTSerial.begin(38400);
}

void loop() {
  //send data to slave
  BTSerial.write(23);
  delay(20); //put delay(20) between seperate data sets
}
