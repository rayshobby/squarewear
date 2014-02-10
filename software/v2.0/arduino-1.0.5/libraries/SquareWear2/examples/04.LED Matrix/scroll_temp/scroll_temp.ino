// *******************************
// SquareWear 2.0 LED Matrix Examples
// Scroll Temperature
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
// and scroll current temperature.
// 1. Click the on-board button to toggle between
//    Celsius or Fahrenheit
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

#define BOARDCOUNT 1      // number of LED matrices
#define PINLED     10     // on SquareWear mini, the LED matrix data input
                          // is connected to D10 by default. Change this
                          // if you have a different pin assignment.
#define PINTEMP    A1     // on SquareWear, temperature sensor (MCP9700)
                          // is connected to analog pin A1
#define PINBUTTON  4      // button pin

Adafruit_NeoPixel strip = Adafruit_NeoPixel(35 * BOARDCOUNT, PINLED, NEO_GRB + NEO_KHZ800);

boolean display_celsius = true;

// predefined scrolling delay time (controls scrolling speed)
const int delaytime[] = {75, 50, 120};
byte delay_index = 0;

void setup(){
  pinMode(PINBUTTON, INPUT);
  digitalWrite(PINBUTTON, HIGH);  // turn on internal pullup
  
  strip.begin();
  strip.show();
  strip.setBrightness(32);        // set default brightness to avoid burning your eyes!
}

void loop(){
  float temp = (float)analogRead(PINTEMP);
  temp = (temp * 3.3 / 1024 - 0.5) / 0.01;    // MCP9700.MCP9700A
  //temp = (temp * 3.3 / 1024 - 0.4) / 0.0195;   // MCP9701/MCP9701A
  
  uint8_t r, g, b;
  float hue;
  // convert temperature (clamped between 0 and 40) to hue (in range 0 to 240)
  hue = (40.0 - temp) * 240 / 40;
  if (hue > 240) hue = 240;
  if (hue < 0)   hue = 0;

  hsv2rgb(hue, 100, 100, &r, &g, &b);
  
  if(!display_celsius){
    temp = 9.f*temp/5.f+32;
  }

  char buffer[4];
  sprintf(buffer,"%d", (int)temp);
  String s = String(buffer);
  s += (char)(254);
  if(display_celsius){
    s += "C";
  } else {
    s += "F";
  }
  scrollString(s,strip.Color(r, g, b));
}

void clearStrip(){
  for(int i = 0; i < strip.numPixels(); i++){
    strip.setPixelColor(i,0);
  }
  strip.show();
}

void scrollString(String s,uint32_t c){
  for(int i = 0; i < BOARDCOUNT; i++){
    s = " " + s;
  }
  for(int i = 0; i < s.length(); i++){
    for(int j = 0; j < 6; j++){
      for(int k = 0; k < BOARDCOUNT + 1; k++){
        drawChar(s.charAt(i+k),j - k*6,c);
        drawCol(0,5-j+k*6,c);
      }
      process_button();
      delay(delaytime[delay_index]);
      strip.show();
    }
  }
}

void drawChar(byte c,int offset,uint32_t color){
  char col;
  for(int i = 0; i < 5; i++){
    if(i - offset >= 0 && i - offset < 5*BOARDCOUNT){
      col = pgm_read_byte(font+(c*5)+i);
      drawCol(col,i - offset,color);
    }
  }
}

void drawCol(char c, int i,uint32_t color){
  if(i >= 0 && i < 5*BOARDCOUNT){
    for(int j = 0; j < 7; j++){
        if(c & 0x1){
          strip.setPixelColor(i*7+j, color);
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
        display_celsius = !display_celsius;
      }
      temp_t = millis();
    }
    is_longpress = 0;
  }
}

void hsv2rgb(float h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b)
{
  float i,f,p,q,t;
  
  h = max(0.0, min(360.0, h));
  s = max(0.0, min(100.0, s));
  v = max(0.0, min(100.0, v));
  
  s /= 100;
  v /= 100;
  
  if(s == 0) {
    // Achromatic (grey)
    *r = *g = *b = round(v*255);
    return;
  }
 
  h /= 60; // sector 0 to 5
  i = floor(h);
  f = h - i; // factorial part of h
  p = v * (1 - s);
  q = v * (1 - s * f);
  t = v * (1 - s * (1 - f));
  switch((int)i) {
    case 0:
      *r = round(255*v);
      *g = round(255*t);
      *b = round(255*p);
      break;
    case 1:
      *r = round(255*q);
      *g = round(255*v);
      *b = round(255*p);
      break;
    case 2:
      *r = round(255*p);
      *g = round(255*v);
      *b = round(255*t);
      break;
    case 3:
      *r = round(255*p);
      *g = round(255*q);
      *b = round(255*v);
      break;
    case 4:
      *r = round(255*t);
      *g = round(255*p);
      *b = round(255*v);
      break;
    default: // case 5:
      *r = round(255*v);
      *g = round(255*p);
      *b = round(255*q);
    }
}
