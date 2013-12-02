// *******************************
// SquareWear 2.0 Examples
// RGB LED pad demo using Adafruit
// neopixel library
// RAYSHOBBY.net
//
// This demo shows how to use the 
// neopixel library to display
// a pattern on the 5x7 LED pad
// - Connection:
//   the DATA pin of the LED pad
//   should be connected to pin
//   D1 on SquareWear2
// *******************************

#include <Adafruit_NeoPixel.h>

#define pinLEDPad 1 // D1

// the RGB LED pad has 5x7 = 35 LEDs
Adafruit_NeoPixel strip = Adafruit_NeoPixel(35, pinLEDPad, NEO_GRB + NEO_KHZ800);

int pattern[7][5] = {{0,1,1,1,0},
                     {1,0,0,0,1},
                     {1,1,0,1,1},
                     {1,0,0,0,1},
                     {1,1,1,1,1},
                     {1,0,0,0,1},
                     {0,1,1,1,0}};
                   
void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(127); // set brightness to medium, 255 is too high
}

void loop(){
  show_pattern(true);
  delay(1000);
  show_pattern(false);
  delay(1000);
}

void show_pattern(bool show) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    if(pattern[i%7][i/7] && show) {
      strip.setPixelColor(i, strip.Color(255,0,0));
    } else {
      strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
    strip.show(); 
  }
}

