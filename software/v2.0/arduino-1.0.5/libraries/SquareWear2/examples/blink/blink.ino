// *******************************
// SquareWear 2.0 Examples
// Blink RGB LEDs
// RAYSHOBBY.net
//
// Internal connections:
//   - red: pin D8
//   - green: pin D12
//   - blue: pin D13
// *******************************

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


