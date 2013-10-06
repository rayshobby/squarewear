// *******************************
// SquareWear 2.0 Examples
// Temperature Sensor
// RAYSHOBBY.net
//
// The program turns on red, green
// and blue LEDs according to the
// ambient temperature.
// Normally the green LED is on
// When temperature rises, the red
// LED will turn on; and when temperature
// drops, the blue LED will turn on.
// Internal connections:
//   - red: pin D8
//   - green: pin D12
//   - blue: pin D13
//   - temp_sensor: A1
// *******************************
#include <HIDSerial.h>
#include <SoftPWM.h>

#define red    8
#define green  12
#define blue   13
#define temp_sensor A1

void setup() {
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);    
  SoftPWMBegin();
  SoftPWMSet(red, 0);
  SoftPWMSet(green, 0);
  SoftPWMSet(blue, 0);     
}

void loop() {
  float temp;
  float duty;
  
  temp = (float)analogRead(temp_sensor);
  
  // convert to celcius
  temp = (temp * 3.3 / 1024 - 0.5) / 0.01;

  // blue LED brightness
  // Gaussian centered at 15 degree Celcius
  duty = 255.0 * exp(-(temp-15)*(temp-15)/25.0);
  SoftPWMSet(blue, (byte)duty);

  // red LED brightness
  // Gaussian centered at 40 degree Celcius
  duty = 255.0 * exp(-(temp-40.0)*(temp-40.0)/25.0);
  SoftPWMSet(red, (byte)duty);

  // green LED brightness
  // Gaussian centered at 25 degree Celcius
  duty = 255.0 * exp(-(temp-25.0)*(temp-25.0)/25.0);
  SoftPWMSet(green, (byte)duty);
  delay(100);  
}
