// ***********************
// SquareWear 2.0 Examples
// Blinking RGB LEDs
// http://sqrwear.com/
// ***********************
//
// (NOTE: SquareWear Mini
//  does not have built-in
//  RGB LEDs).
//
// Internal connections:
// - red:   D8
// - green: D12
// - blue:  D13
//
// ***********************

#define red   8
#define green 12
#define blue  13

void setup() {
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  digitalWrite(blue, LOW);
}

void loop() {
  digitalWrite(red, HIGH);
  delay(1000);
  digitalWrite(red, LOW);
  delay(1000);
  
  digitalWrite(green, HIGH);
  delay(1000);
  digitalWrite(green, LOW);
  delay(1000);
  
  digitalWrite(blue, HIGH);
  delay(1000);
  digitalWrite(blue, LOW);  
  delay(1000);
}


