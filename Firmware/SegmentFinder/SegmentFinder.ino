/*
LCD segment finder


*/
#define freq 5
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(8, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(9, LOW);   // turn the LED off by making the voltage LOW
  delay(freq);                      // wait for a second
  digitalWrite(8, LOW);   // turn the LED off by making the voltage LOW
  digitalWrite(9, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(freq);                      // wait for a second
}
