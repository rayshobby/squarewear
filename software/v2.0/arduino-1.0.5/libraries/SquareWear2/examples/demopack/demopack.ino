// *******************************
// SquareWear 2.0 Examples
// Demonstration
// RAYSHOBBY.net
//
// This is a demo pack that includes
// 5 different demos:
// - blink all pins
// - blink rgb led
// - fade rgb led (using SoftPWM library)
// - buzzer (using Arduino's Tone function)
// - HID serial (requires running an HID
//   Serial Monitor on your host computer)
// To cycle through the demos, click the pushbutton
// *******************************
#include <PciManager.h>
#include <PciListenerImp.h>
#include <HIDSerial.h>
#include <SoftPWM.h>
#include "pitches.h"

#define uchar unsigned char
#define ulong unsigned long

#define pinButton 4
#define pinRed    8
#define pinGreen  12
#define pinBlue   13
#define pinBuzzer 9

#define pinLight  A0
#define pinTemp   A1

PciListenerImp listener(pinButton, onPinButtonChange);
HIDSerial serial;

uchar buttonPressed = 0;

void onPinButtonChange(byte changeKind) {
  static ulong t = 0;
  if (changeKind == 1 && millis() > t + 250) {
    t = millis();
    buttonPressed = 1;    
  }
}

void setup() {
  // set button as input, internal pull-up
  pinMode(pinButton, INPUT);
  digitalWrite(pinButton, HIGH);
  pinMode(pinBuzzer, OUTPUT);
  digitalWrite(pinBuzzer, LOW);
  PciManager.registerListener(pinButton, &listener);
}

// ====================
// blinkall demo
// ====================
const uchar pinNumbers[] = {0, 1, 3, 5, 6, 8, 10, 11, 12, 13, 16, 17, 18, 19};
  
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

// ====================
// hid serial demo
// ====================
void setup_hidserial() {
  pinMode(pinBlue, OUTPUT); 
  digitalWrite(pinBlue, HIGH);
  serial.begin();
}

void loop_hidserial() {
  static ulong t;
  uchar value = 0;
  while(!buttonPressed) {
    if(millis() > t + 500) {
      t = millis();
      serial.print("Temp = ");
      serial.print(analogRead(pinTemp));
      serial.print(", Light = ");
      serial.println(analogRead(pinLight));
      value = 1 - value;
      digitalWrite(pinBlue, value);
    }
    serial.poll();    
  }
}


void loop() {
  buttonPressed = 0;
  setup_blinkall();
  loop_blinkall();

  buttonPressed = 0;
  setup_blinkrgb();
  loop_blinkrgb();
  
  buttonPressed = 0;
  setup_fade();
  loop_fade();
  
  buttonPressed = 0;
  setup_buzzer();
  loop_buzzer();

  buttonPressed = 0;
  setup_hidserial();
  loop_hidserial();
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
