// *******************************
// SquareWear 2.0 Examples
// Light Sensor
// http://sqrwear.com/
// *******************************
//
// (NOTE: SquareWear Mini
//  does not have built-in
//  RGB LEDs).
//
// Internal connections:
// - red:          D8
// - light sensor: A0
//
// This program blinks the red LED
// at a frequency based on the
// light sensor reading.
// the brighter the ambient light,
// the faster the LED blinks.
//
// *******************************

#define red  8
#define light_sensor A0

void setup() {
  pinMode(red, OUTPUT);
  digitalWrite(red, LOW);
}

void loop() {
  int light_value;
  int t;
  light_value = analogRead(light_sensor);
  if (light_value < 400) light_value = 400;
  if (light_value > 800) light_value = 800;
  t = (800 - light_value) * 2 + 50;
  digitalWrite(red, HIGH);
  delay(t);
  digitalWrite(red, LOW);
  delay(t);
  
}
