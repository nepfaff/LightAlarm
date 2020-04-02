#include "SoftwareSerial.h"
SoftwareSerial BTSerial(2, 3); //RX | TX => RX beeds to be connected to TX and TX to RX for setup

void setup(){ 
 Serial.begin(9600); 
 BTSerial.begin(38400);
 Serial.println("AT commands mode"); 
} 

void loop(){ 
 //from bluetooth to Terminal. 
 if(BTSerial.available()) 
   Serial.write(BTSerial.read()); 
 //from termial to bluetooth 
 if(Serial.available()) 
   BTSerial.write(Serial.read());
}
