// *******************************
// SquareWear 2.0 Examples
// Light Sensor and Buzzer
// http://sqrwear.com/
// *******************************
//
// Internal connections:
// - buzzer:       D9
// - light sensor: A0
//
// This program makes the buzzer
// sound at a frequency determined
// by the light sensor value. So you
// can use it as an optical Theremin.
//
// *******************************

#define buzzer 9
#define light_sensor A0

void setup() {
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);  
}

void loop() {
  int light_value;
  light_value = analogRead(light_sensor);
  tone(buzzer, light_value, 10);
  delay(5);
}
