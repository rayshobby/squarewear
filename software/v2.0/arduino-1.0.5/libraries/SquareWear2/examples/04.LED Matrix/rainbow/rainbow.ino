// This is the demo provided in 
// Adafruit's neopixel library
// Changed to use pin D10 as 
// data pin

// *******************************
// SquareWear 2.0 LED Matrix Examples
// Rainbow Color
// http://sqrwear.com/
// *******************************
//
// This demo uses Adafruit's Neopixel
// library to display color patterns
// on the 5x7 LED matrix or strip
//
// Connection:
// The IN (DATA) pin of the LED matrix
// is assumed to be wired to pin D10
// on SquareWear. Note that this is the
// default setup on SquareWear Mini.
//
// You can cascade multiple LED matrices.
// Change the macro BOARDCOUNT accordingly.
// *******************************

#include <Adafruit_NeoPixel.h>

#define PIN 10
#define BOARDCOUNT 1   // number of LED matrices

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(35 * BOARDCOUNT, PIN, NEO_GRB + NEO_KHZ800);

double brightness = .1; // reduce brightness to avoid burning your eyes!

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  // Some example procedures showing how to display to the pixels:
  colorWipe(strip.Color(brightness * 255, 0, 0), 5); // Red
  colorWipe(strip.Color(0, brightness * 255, 0), 5); // Green
  colorWipe(strip.Color(0, 0, brightness * 255), 5); // Blue
  rainbow(5);
  rainbowCycle(5);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i,Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(brightness * WheelPos * 3, brightness * (255 - WheelPos * 3), 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(brightness * (255 - WheelPos * 3), 0, brightness * WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, brightness * WheelPos * 3, brightness * (255 - WheelPos * 3));
  }
}

