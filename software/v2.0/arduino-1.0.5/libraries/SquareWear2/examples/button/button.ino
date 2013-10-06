// *******************************
// SquareWear 2.0 Examples
// Button press
// RAYSHOBBY.net
//
// Internal connections:
//   - green: pin D12
//   - pushbutton: D4
// *******************************

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
