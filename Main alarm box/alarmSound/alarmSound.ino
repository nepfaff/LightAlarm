const int buzzer = 2;

void setup() {
  pinMode(buzzer, OUTPUT);
}

void loop() {
  //connect one pin of buzzer to ground and other to pin 2

  //makes annoying noise suitable as alarm sound
  for(int i=200; i<2000; i++) {
    tone(buzzer, i);
    delay(10);
  }

  for(int i=2000; i>200; i--) {
    tone(buzzer, i);
    delay(10);
  }
}
