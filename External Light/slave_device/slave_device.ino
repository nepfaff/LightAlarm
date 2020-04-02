#include <SoftwareSerial.h>

//define new serial communication pins
SoftwareSerial BTSerial(2, 3); //RX | TX => arduino RX must be connected to HC-05 TX, and TX to RX

void setup() {
  //serial used for master-slave bluetooth communication
  BTSerial.begin(38400); 
  Serial.begin(9600);
}

void loop() {
  //BTSerial.available() tells us how many bytes are waiting in buffer (currently reading every time something is send, could change to >= 2 to only read when two items have been send)
  while(BTSerial.available() >= 1){
    int x = BTSerial.read();
    Serial.println(x);
  }
}
