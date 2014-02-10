// *******************************
// SquareWear 2.0 LED Matrix Examples
// Blink icons
// http://sqrwear.com/
// *******************************
//
// This demo uses Adafruit's Neopixel
// library to display patterns/icons
// on the 5x7 LED matrix
//
// Connection:
// The IN (DATA) pin of the LED matrix
// is assumed to be wired to pin D10
// on SquareWear. Note that this is the
// default setup on SquareWear Mini.
//
// *******************************

#include <Adafruit_NeoPixel.h>

#define pinLEDPad 10 // pin D10

// the RGB LED pad has 5x7 = 35 LEDs
Adafruit_NeoPixel strip = Adafruit_NeoPixel(35, pinLEDPad, NEO_GRB + NEO_KHZ800);

byte smiley[7][5] = {{0,1,1,1,0},
                     {1,0,0,0,1},
                     {1,1,0,1,1},
                     {1,0,0,0,1},
                     {1,1,1,1,1},
                     {1,0,0,0,1},
                     {0,1,1,1,0}};
                   
byte skull[7][5] =  {{0,1,1,1,0},
                     {1,0,0,0,1},
                     {1,1,0,1,1},
                     {1,0,0,0,1},
                     {0,1,0,1,0},
                     {0,1,0,1,0},
                     {0,1,1,1,0}};
                    
uint32_t pumpkin[7][5] = {
                     {0x000000,0x00FF00,0x000000,0x000000,0x000000},
                     {0x000000,0x000000,0x00FF00,0x000000,0x000000},
                     {0x000000,0x802000,0x802000,0x802000,0x000000},
                     {0x802000,0x000000,0x000000,0x000000,0x802000},
                     {0x802000,0x000000,0x000000,0x000000,0x802000},
                     {0x802000,0x000000,0x000000,0x000000,0x802000},
                     {0x000000,0x802000,0x802000,0x802000,0x000000}};
                                   
#define B 0x000000
#define R 0xFF0000
uint32_t heart[7][5] = {B, B, B, B, B,
                        B, R, B, R, B,
                        R, R, R, R, R,
                        R, R, R, R, R,
                        B, R, R, R, B,
                        B, B, R, B, B,
                        B, B, B, B, B};
                        
void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(32); // set brightness low, to avoid burning your eyes!
}

void loop(){
  show_pattern(smiley, strip.Color(0,255,0));
  delay(1000);
  show_pattern(smiley, 0);
  delay(500);

  show_pattern(skull, strip.Color(0,0,255));
  delay(1000);
  show_pattern(skull, 0);
  delay(500);

  show_pattern(pumpkin, true);
  delay(1000);
  show_pattern(pumpkin, false);
  delay(500);
  
  show_pattern(heart, true);
  delay(1000);
  show_pattern(heart, false);
  delay(500);  
}


// This function shows a pattern defined by 
// a binary array (1 means on, 0 means off)
// and with the specified color.
void show_pattern(byte pattern[7][5], uint32_t color) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    if(pattern[i%7][i/7]) {
      strip.setPixelColor(i, color);
    } else {
      strip.setPixelColor(i, 0);
    }
  }
  strip.show(); 
}

// This function shows a pattern defined by
// a color array (each element stores the color
// of the pixel).
void show_pattern(uint32_t pattern[7][5], boolean show) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    if (show == true)
      strip.setPixelColor(i, pattern[i%7][i/7]);
    else
      strip.setPixelColor(i, 0);    
  }
  strip.show(); 
}


