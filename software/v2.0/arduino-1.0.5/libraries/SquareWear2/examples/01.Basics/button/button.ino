// ***********************
// SquareWear 2.0 Examples
// Button press
// http://sqrwear.com/
// ***********************
//
// (NOTE: SquareWear Mini
//  does not have built-in
//  RGB LEDs).
//
// Internal connections:
// - green:  D12
// - button: D4
//
// The red LED lights up
// upon button press.
//
// ***********************

#define button 4
#define green  12

void setup() {
  pinMode(green, OUTPUT);
  pinMode(button, INPUT);
  digitalWrite(button, HIGH);   // activates internal pullup
  digitalWrite(green, LOW);
}

void loop() {
  if(digitalRead(button) == LOW) {
    digitalWrite(green, HIGH);
  } else {
    digitalWrite(green, LOW);
  }
  delay(50); 
}
