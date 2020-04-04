#include <TimeLib.h>


void setup() {
  Serial.begin(9600);
  
  //time, min, sec, date, month, year
  setTime(14, 27, 00, 14, 12, 2015);
}

void loop() {
  digitalClockDisplay();
  //for general LCD display
  //probably good idea to allow keys 1-9 to select options but only always display 1 or 2 options from menu 
  //and assign key for displaying next two options
 
}

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  Serial.println("");
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
