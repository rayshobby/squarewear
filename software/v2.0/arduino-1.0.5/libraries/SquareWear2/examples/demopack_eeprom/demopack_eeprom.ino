// *******************************
// SquareWear 2.0 Examples
// Demonstration
// RAYSHOBBY.net
// 
// This is a demo pack that includes
// 6 different demos:
// - blink all pins
// - blink rgb led
// - fade rgb led (using SoftPWM library)
// - buzzer (using Arduino's Tone function)
// - color LED strip / pad
// - buzzer on light sensor (i.e. musicbox demo)
// To iterate the demos, click the pushbutton
// This demo also makes use of the EEPROM to
// store the current demo index, so that next 
// time you turn on SquareWear it remembers
// which demo is was running.
// *******************************
#include <PciManager.h>
#include <PciListenerImp.h>
#include <SoftPWM.h>
#include <Adafruit_NeoPixel.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <EEPROM.h>
#include "pitches.h"

#define uchar unsigned char
#define ulong unsigned long

#define pinButton 4
#define pinRed    8
#define pinGreen  12
#define pinBlue   13
#define pinBuzzer 9
#define pinStrip 1

#define pinLight  A0
#define pinTemp   A1

PciListenerImp listener(pinButton, onPinButtonChange);

uchar buttonPressed = 0;
volatile int f_wdt = 1;

ISR(WDT_vect)
{
  if(f_wdt == 0)
  {
    f_wdt=1;
  }
}


void onPinButtonChange(byte changeKind) {
  static ulong t = 0;
  if (changeKind == 1 && millis() > t + 250) {
    t = millis();
    buttonPressed = 1;
    EEPROM.write(0,(EEPROM.read(0)+1)%6);
  }
}

void setup() {
  // set button as input, internal pull-up
  if(EEPROM.read(0) > 5){
    EEPROM.write(0,0);
  }
    
  pinMode(pinButton, INPUT);
  digitalWrite(pinButton, HIGH);
  pinMode(pinBuzzer, OUTPUT);
  digitalWrite(pinBuzzer, LOW);
  PciManager.registerListener(pinButton, &listener);
}

// ====================
// blinkall demo
// ====================
const uchar pinNumbers[] = {0,1, 3, 5, 6, 8, 10, 11, 12, 13, 16, 17, 18, 19};
  
void setup_blinkall() {
  int i;
  for(i=0;i<sizeof(pinNumbers);i++) {
    pinMode(pinNumbers[i], OUTPUT);
  }  
}

void loop_blinkall() {
  uchar i, value;
  value = 0;
  while(!buttonPressed) {
    for(i=0;i<sizeof(pinNumbers);i++) {
      digitalWrite(pinNumbers[i], value);
    }
    delay(250);
    value = 1 - value;
  }
}

// ====================
// blink rgb demo
// ====================
void setup_blinkrgb() {
  pinMode(pinRed, OUTPUT);
  pinMode(pinGreen, OUTPUT);
  pinMode(pinBlue, OUTPUT);
}

void loop_blinkrgb() {
  while(1) {
    digitalWrite(pinRed, HIGH);
    delay(500);
    digitalWrite(pinRed, LOW);

    if(buttonPressed)  break;

    digitalWrite(pinGreen, HIGH);
    delay(500);
    digitalWrite(pinGreen, LOW);

    if(buttonPressed)  break;
    
    digitalWrite(pinBlue, HIGH);
    delay(500);
    digitalWrite(pinBlue, LOW);

    if(buttonPressed)  break;

    digitalWrite(pinRed, HIGH);
    digitalWrite(pinGreen, HIGH);    
    delay(500);
    digitalWrite(pinRed, LOW);
    digitalWrite(pinGreen, LOW);    

    if(buttonPressed)  break;
    
    digitalWrite(pinRed, HIGH);
    digitalWrite(pinBlue, HIGH);    
    delay(500);
    digitalWrite(pinRed, LOW);
    digitalWrite(pinBlue, LOW);    

    if(buttonPressed)  break;
    
    digitalWrite(pinGreen, HIGH);
    digitalWrite(pinBlue, HIGH);    
    delay(500);
    digitalWrite(pinGreen, LOW);
    digitalWrite(pinBlue, LOW);    
    
    if(buttonPressed)  break;    
  }  
}

// ====================
// fade demo
// ====================

void setup_fade() {
  SoftPWMBegin();
  SoftPWMSet(pinRed, 0);
  SoftPWMSet(pinGreen, 0);
  SoftPWMSet(pinBlue, 0);
}

void loop_fade() {
    int v;
    unsigned char r, g, b;
    while(!buttonPressed) {
        for(v=0;v<255;v++) {
            SoftPWMSet(pinRed, v);
            delay(5);
            if(buttonPressed) break;
        }
        for(v=255;v>=0;v--) {
            SoftPWMSet(pinRed, v);
            delay(5);
            if(buttonPressed) break;
        }
        for(v=0;v<255;v++) {
            SoftPWMSet(pinGreen, v);
            delay(5);
            if(buttonPressed) break;
        }
        for(v=255;v>=0;v--) {
            SoftPWMSet(pinGreen, v);
            delay(5);
            if(buttonPressed) break;
        }        
        for(v=0;v<255;v++) {
            SoftPWMSet(pinBlue, v);
            delay(5);
            if(buttonPressed) break;
        }
        for(v=255;v>=0;v--) {
            SoftPWMSet(pinBlue, v);
            delay(5);
            if(buttonPressed) break;
        }        
        for(v=0;v<360;v++) {
          hsv2rgb(v, 100, 100, &r, &g, &b);
          SoftPWMSet(pinRed, r);
          SoftPWMSet(pinGreen, g);
          SoftPWMSet(pinBlue, b);                    
          delay(10);
          if(buttonPressed) break;
        }
        SoftPWMSet(pinRed, 0);
        SoftPWMSet(pinGreen, 0);
        SoftPWMSet(pinBlue, 0);
    }
    SoftPWMEnd(pinRed);
    SoftPWMEnd(pinGreen);
    SoftPWMEnd(pinBlue);
    SoftPWMEnd();
}

// ====================
// buzzer demo
// ====================
int melody[] = {
  NOTE_G4, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_C4,
  NOTE_A4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C4, NOTE_C4,
  NOTE_F4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4,
  NOTE_E4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_C4,
  NOTE_D4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_B3,
  NOTE_C4};

int durations[] = {
  2, 4, 4, 4, 4, 2, 2, 2,
  2, 4, 4, 4, 4, 2, 2, 2,
  2, 4, 4, 4, 4,
  2, 4, 4, 4, 4,
  2, 4, 4, 4, 4,
  1, 2};
  
void setup_buzzer() {
  pinMode(pinBuzzer, OUTPUT);
  digitalWrite(pinBuzzer, LOW);  
}

void loop_buzzer() {
  while(1) {
    for(int i=0;i<sizeof(melody)/sizeof(int);i++) {
      int d = 1000 / durations[i];
      tone(pinBuzzer, melody[i], d);
      int p = d * 1.1;
      delay(p);
      noTone(pinBuzzer);
      if(buttonPressed) break;
    }
    if(buttonPressed) break;
  }
}

Adafruit_NeoPixel strip = Adafruit_NeoPixel(35, pinStrip, NEO_GRB + NEO_KHZ800);

void setup_LEDStick() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(75);
}

void loop_LEDStick(){
  while(1){
    colorWipe(strip.Color(255, 0, 0), 50); // Red
    colorWipe(strip.Color(0, 255, 0), 50); // Green
    colorWipe(strip.Color(0, 0, 255), 50); // Blue
    rainbow(20);
    rainbowCycle(20);
    if(buttonPressed){
      for(uint16_t i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, 0);
        strip.show();
      }
      break;
    }
  }
}

void setup_musicBox(){
  pinMode(pinBuzzer, OUTPUT);    // make sure the buzzer is turned off
  digitalWrite(pinBuzzer, LOW);
  
  /* Clear the reset flag. */
  MCUSR &= ~(1<<WDRF);
  /* In order to change WDE or the prescaler, we need to
   * set WDCE (This will allow updates for 4 clock cycles).
   */
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  /* set new watchdog timeout prescaler value */
  WDTCSR = 1<<WDP0 | 1<<WDP1 | 1<<WDP2;  // 2.0 seconds
  /* other options (see page 57 of ATmega328 datasheet)
  WDTCSR = 1<<WDP2;                      // 0.25 seconds
  WDTCSR = 1<<WDP0 | 1<<WDP2;            // 0.5 seconds
  WDTCSR = 1<<WDP1 | 1<<WDP2;            // 1.0 seconds
  WDTCSR = 1<<WDP0 | 1<<WDP1 | 1<<WDP2;  // 2.0 seconds
  WDTCSR = 1<<WDP3;                      // 4.0 seconds
  WDTCSR = 1<<WDP3 | 1<<WDP0;            // 8.0 seconds
  */
  
  /* Enable the WD interrupt (note no reset). */
  WDTCSR |= _BV(WDIE);
}

void loop_musicBox(){
  while(1){
      int light_value;
      if(f_wdt==1) {
        light_value = analogRead(pinLight);
        if(light_value > 400){
          for(int i = 0; i < sizeof(melody)/sizeof(int); i++){
            int d = 1000 / durations[i];
            tone(pinBuzzer, melody[i], d);
            int p = d*1.1;
            delay(p);
            noTone(pinBuzzer);
            light_value = analogRead(pinLight);
            if(light_value <= 400){
              i = sizeof(melody)/sizeof(int);
            }
            if(buttonPressed) break;
          }
        }
        f_wdt = 0;
        enterSleep();
      }
      if(buttonPressed) break;
  }
}

void loop() {
  
  switch(EEPROM.read(0)){
    case 0:
      buttonPressed = 0;
      setup_blinkall();
      loop_blinkall();
      break;
    case 1:
      buttonPressed = 0;
      setup_blinkrgb();
      loop_blinkrgb();
      break;
    case 2:
      buttonPressed = 0;
      setup_fade();
      loop_fade();
      break;
    case 3:
      buttonPressed = 0;
      setup_buzzer();
      loop_buzzer();
      break;
    case 4:
      buttonPressed = 0;  
      setup_LEDStick();
      loop_LEDStick();
      break;
    case 5:
      buttonPressed = 0;
      setup_musicBox();
      loop_musicBox();
      break;
  }
}

void enterSleep(void)
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);   /* EDIT: could also use SLEEP_MODE_PWR_DOWN for lowest power consumption. */
  sleep_enable();
  /* Now enter sleep mode. */
  sleep_mode();  
  /* The program will continue from here after the WDT timeout*/
  sleep_disable(); /* First thing to do is disable sleep. */ 
  /* Re-enable the peripherals. */
  power_all_enable();
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      if(buttonPressed)break;
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
      if(buttonPressed)break;
    }
    if(buttonPressed)break;
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      if(buttonPressed)break;
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    if(buttonPressed)break;
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void hsv2rgb(float h, float s, float v, uchar *r, uchar *g, uchar *b)
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
