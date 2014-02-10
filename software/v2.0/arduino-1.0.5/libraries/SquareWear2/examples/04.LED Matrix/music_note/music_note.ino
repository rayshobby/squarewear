// *******************************
// SquareWear 2.0 LED Matrix Examples
// Touch Sensing and Display Music Notes
// http://sqrwear.com/
// *******************************
//
// Connection:
// The IN (DATA) pin of the LED matrix
// is assumed to be wired to pin D10
// on SquareWear. Note that this is the
// default setup on SquareWear Mini.
//
// This example demonstrate combining buzzer,
// touch sensing and a 5x7 LED matrix to sing
// a note upon touching a pad and to display
// the note onto the LED matrix. Pins A4, A2,
// A3, A5, D0, D1, and D11 correspond to notes
// 1(C), 2(D), 3(E), 4(F), 5(G), 6(A), 7(B).
// The button can be used to toggle between
// three octaves (low, medium, high). 
//
// A quirk:
// * Moistening your fingers help increase
//   the touch sensitivity.
//
// *******************************

#include "glcdfont.h"
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
 #include <avr/pgmspace.h>
#else
 #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif

#define PINLED     10  // on SquareWear mini, the LED matrix data input
                       // is connected to D10 by default. Change this
                       // if you have a different pin assignment.
#define PINBUTTON  4   // button pin

Adafruit_NeoPixel strip = Adafruit_NeoPixel(35, PINLED, NEO_GRB + NEO_KHZ800);

// ===============================
// readCapacitivePin function from
// http://playground.arduino.cc/Code/CapacitiveSensor
// ===============================
//  Input: Arduino pin number
//  Output: A number, from 0 to 17 expressing
//  how much capacitance is on the pin
//  When you touch the pin, or whatever you have
//  attached to it, the number will get higher
#include "pins_arduino.h" // Arduino pre-1.0 needs this
uint8_t readCapacitivePin(int pinToMeasure) {
  // Variables used to translate from Arduino to AVR pin naming
  volatile uint8_t* port;
  volatile uint8_t* ddr;
  volatile uint8_t* pin;
  // Here we translate the input pin number from
  //  Arduino pin number to the AVR PORT, PIN, DDR,
  //  and which bit of those registers we care about.
  byte bitmask;
  port = portOutputRegister(digitalPinToPort(pinToMeasure));
  ddr = portModeRegister(digitalPinToPort(pinToMeasure));
  bitmask = digitalPinToBitMask(pinToMeasure);
  pin = portInputRegister(digitalPinToPort(pinToMeasure));
  // Discharge the pin first by setting it low and output
  *port &= ~(bitmask);
  *ddr  |= bitmask;
  delay(1);
  // Prevent the timer IRQ from disturbing our measurement
  noInterrupts();
  // Make the pin an input with the internal pull-up on
  *ddr &= ~(bitmask);
  *port |= bitmask;

  // Now see how long the pin to get pulled up. This manual unrolling of the loop
  // decreases the number of hardware cycles between each read of the pin,
  // thus increasing sensitivity.
  uint8_t cycles = 17;
       if (*pin & bitmask) { cycles =  0;}
  else if (*pin & bitmask) { cycles =  1;}
  else if (*pin & bitmask) { cycles =  2;}
  else if (*pin & bitmask) { cycles =  3;}
  else if (*pin & bitmask) { cycles =  4;}
  else if (*pin & bitmask) { cycles =  5;}
  else if (*pin & bitmask) { cycles =  6;}
  else if (*pin & bitmask) { cycles =  7;}
  else if (*pin & bitmask) { cycles =  8;}
  else if (*pin & bitmask) { cycles =  9;}
  else if (*pin & bitmask) { cycles = 10;}
  else if (*pin & bitmask) { cycles = 11;}
  else if (*pin & bitmask) { cycles = 12;}
  else if (*pin & bitmask) { cycles = 13;}
  else if (*pin & bitmask) { cycles = 14;}
  else if (*pin & bitmask) { cycles = 15;}
  else if (*pin & bitmask) { cycles = 16;}

  // End of timing-critical section
  interrupts();

  // Discharge the pin again by setting it low and output
  //  It's important to leave the pins low if you want to 
  //  be able to touch more than 1 sensor at a time - if
  //  the sensor is left pulled high, when you touch
  //  two sensors, your body will transfer the charge between
  //  sensors.
  *port &= ~(bitmask);
  *ddr  |= bitmask;

  return cycles;
}

// ===============================
// Pin Definitions
// ===============================
#define pinBuzzer 9
#define pinButton 4
#define pinRed    8
#define pinGreen  12
#define pinBlue   13

// this is the threshold for capacitive sensing
// you may have to increase or decrease it to fit your need
#define CAP_THRESHOLD  3

//#define HIDSERIAL_DEBUG

#include "pitches.h"

int notes_low[] = {NOTE_C3, NOTE_D3, NOTE_E3, NOTE_F3, NOTE_G3, NOTE_A3, NOTE_B3};
int notes_med[] = {NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4};
int notes_high[]= {NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5};

// index to notes and led pins by octave
int *notes[] = {notes_low, notes_med, notes_high};
uint32_t colors[]= {0x00FF0000, 0x0000FF00, 0x000000FF};

#ifdef HIDSERIAL_DEBUG  
  #include <HIDSerial.h>
  HIDSerial serial;
#endif


byte color_index = 0;
byte delay_index = 0;

void setup() {
#ifdef HIDSERIAL_DEBUG  
  serial.begin();
#endif
  // initialize buzzer, button, and 
  pinMode(pinBuzzer, OUTPUT);
  digitalWrite(pinBuzzer, LOW);    

  pinMode(pinButton, INPUT);
  digitalWrite(pinButton, HIGH);   // turn on internal pullup
  
  strip.begin();
  strip.show();
  strip.setBrightness(32);  // set default brightness to avoid burning your eyes!  
}

byte octave = 1;

void sing(int n) {
  // sing a note for 75 milliseconds. also display note on LED matrix
  drawNote(n+1, colors[octave]);
  tone(pinBuzzer, notes[octave][n], 100);  
  delay(90);
  //noTone(pinBuzzer);
  drawNote(n+1, 0);
}

// these pins correpond to A4, A2, A3, A5, 0, 1, 11
// This matches the pins from left to right, buttom to top on SquareWear 2.0
const int pins[] = {18, 16, 17, 19, 0, 1, 11};

void loop() {
  // scan all pins and detect the one that has the largest capacitive sensing value 
  byte maxvalue = 0;
  byte maxindex = 0;
  for(int i=0; i<sizeof(pins)/sizeof(int); i++) {
    byte value = readCapacitivePin(pins[i]);
    if (value >= maxvalue) {
      maxvalue = value;
      maxindex = i;
    }
  }
  
#ifdef HIDSERIAL_DEBUG  
  serial.println(maxvalue);
  serial.poll();
#endif  
  
  // if the pin with the largest value exceeds a threshold
  if (maxvalue > CAP_THRESHOLD) {
    sing(maxindex);
  }
 
  if (digitalRead(pinButton) == LOW) {
    while(digitalRead(pinButton) != HIGH) ;
    octave = (octave + 1) % 3;
  }
}

void drawNote(byte n, uint32_t color){
  char c = n + '0';
  for(int i = 0; i < 5; i++){
    char col = pgm_read_byte(font+(c*5)+i);
    drawCol(col, i, color);
  }
  strip.show();
}

void drawCol(char col, int i, uint32_t color){
  for(int j = 0; j < 7; j++){
    if(col & 0x1){
      strip.setPixelColor(i*7+j, color);
    } else {
      strip.setPixelColor(i*7+j, 0);
    }
    col >>= 1;
  }
}

