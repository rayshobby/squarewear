// *******************************
// SquareWear 2.0 LED Matrix Examples
// Scroll Text
// http://sqrwear.com/
// *******************************
//
// Connection:
// The IN (DATA) pin of the LED matrix
// is assumed to be wired to pin D10
// on SquareWear. Note that this is the
// default setup on SquareWear Mini.
//
// This example demonstrate using SquareWear
// and one or more 5x7 LED matrices to display
// and scroll text.
// 1. Click the on-board button to change color
// 2. Press and hold the button (more than 1 second)
//    to change scrolling speed.
// 3. You can cascade multiple LED matrices. Change
//    the macro define BOARDCOUNT accordingly.
//
// *******************************

#include "glcdfont.h"
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
 #include <avr/pgmspace.h>
#else
 #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif

#define BOARDCOUNT 1   // number of LED matrices
#define PINLED     10  // on SquareWear mini, the LED matrix data input
                       // is connected to D10 by default. Change this
                       // if you have a different pin assignment.
#define PINBUTTON  4   // button pin

Adafruit_NeoPixel strip = Adafruit_NeoPixel(35 * BOARDCOUNT, PINLED, NEO_GRB + NEO_KHZ800);

// predefined colors
const uint32_t colors[] = {0x00FF0000, 0x0000FF00, 0x000000FF,
                           0x00FFFF00, 0x00FF00FF, 0x0000FFFF,
                           0x00FFFFFF};
                      
// predefined scrolling delay time (controls scrolling speed)
const int delaytime[] = {75, 50, 120};

byte color_index = 0;
byte delay_index = 0;

void setup(){
  pinMode(PINBUTTON, INPUT);
  digitalWrite(PINBUTTON, HIGH);  // turn on internal pullup
  strip.begin();
  strip.show();
  strip.setBrightness(32);  // set default brightness to avoid burning your eyes!
}

void loop(){
  scrollString("Hello World!");
}

void clearStrip(){
  for(int i = 0; i < strip.numPixels(); i++){
    strip.setPixelColor(i, 0);
  }
  strip.show();
}

void scrollString(String s){
  for(int i = 0; i < BOARDCOUNT; i++){
    s = " " + s;
  }
  for(int i = 0; i < s.length(); i++){
    for(int j = 0; j < 6; j++){
      for(int k = 0; k < BOARDCOUNT + 1; k++){
        drawChar(s.charAt(i+k),j - k*6);
        drawCol(0,5-j+k*6);
      }
      process_button();
      delay(delaytime[delay_index]);  
      strip.show();
    }
  }
}

void drawChar(char c,int offset){
  char col;
  for(int i = 0; i < 5; i++){
    if(i - offset >= 0 && i - offset < 5*BOARDCOUNT){
      col = pgm_read_byte(font+(c*5)+i);
      drawCol(col,i - offset);
    }
  }

}

void drawCol(char c, int i){
  if(i >= 0 && i < 5*BOARDCOUNT){
    for(int j = 0; j < 7; j++){
        if(c & 0x1){
          strip.setPixelColor(i*7+j, colors[color_index]);
        } else {
          strip.setPixelColor(i*7+j, 0);
        }
        c >>= 1;
    }
  }
}

void process_button() {
  static byte button_status = 1;
  static unsigned long temp_t = 0;
  static byte is_longpress = 0;
  
  if(digitalRead(PINBUTTON) == 0) {   // if button is down
    if (button_status == 1) {         // if it was previously up
      temp_t = millis();              // record time stamp
      button_status = 0;
    } else {                          // if button is being held down
      if (millis() - temp_t > 1000) { // if 1 second has passed
        // change scrolling speed
        delay_index = (delay_index + 1) % (sizeof(delaytime)  / sizeof(int));
        is_longpress = 1;
        temp_t = millis();
      }
    }
  } else {                            // if button is up
    if (button_status == 0) {         // if it was previously down
      button_status = 1;
      // change display color
      if (!is_longpress) {
        color_index = (color_index + 1) % (sizeof(colors) / sizeof(uint32_t));
      }
      temp_t = millis();
    }
    is_longpress = 0;
  }
}
