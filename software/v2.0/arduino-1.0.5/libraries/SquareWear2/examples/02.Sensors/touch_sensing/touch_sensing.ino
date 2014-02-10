// *********************************************
// SquareWear 2.0 Examples
// Capacitive Touch Sensing
// http://sqrwear.com/
// *********************************************
//
// (NOTE: SquareWear Mini
//  does not have built-in
//  RGB LEDs).
//
// This demo shows how to use capacitive
// sensing and buzzer to make an electronic
// piano. All 12 pins on SquareWear respond
// to touch sensing. The button is used
// to switch between three octaves (low, 
// medium, high). The demo works the best
// with a USB cable connected to a computer.
// You may need to adjust the capacitive 
// touch threshold (CAP_THRESHOLD).
//
// A few quirks:
// * Works best if you plug in a USB cable
//   connected to a computer
// * Moistening your fingers help increase
//   the touch sensitivity
// * Without the USB cable, it generally
//   helps if you hold the battery, or VCC
//   or GND pins. You may have to mask out
//   the ISP pins on the back side to avoid
//   touching them.
// * The MOSFET pins (D3, D5, D6) do not work
//   without the USB cable. Still investigating.
// *********************************************

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

int notes_low[] = {NOTE_C3, NOTE_D3, NOTE_E3, NOTE_F3, NOTE_G3, NOTE_A3, NOTE_B3, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4};
int notes_med[] = {NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5};
int notes_high[]= {NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6, NOTE_D6, NOTE_E6, NOTE_F6, NOTE_G6};

// index to notes and led pins by octave
int *notes[] = {notes_low, notes_med, notes_high};
int ledpins[] = {pinRed, pinGreen, pinBlue};

#ifdef HIDSERIAL_DEBUG  
  #include <HIDSerial.h>
  HIDSerial serial;
#endif

void setup() {
#ifdef HIDSERIAL_DEBUG  
  serial.begin();
#endif
  // initialize buzzer, red LED, and button
  pinMode(pinBuzzer, OUTPUT);
  digitalWrite(pinBuzzer, LOW);    

  pinMode(pinRed, OUTPUT);
  digitalWrite(pinRed, LOW);

  pinMode(pinGreen, OUTPUT);
  digitalWrite(pinGreen, LOW);

  pinMode(pinBlue, OUTPUT);
  digitalWrite(pinBlue, LOW);
  
  pinMode(pinButton, INPUT);
  digitalWrite(pinButton, HIGH);   // turn on internal pullup
}

byte octave = 1;

void sing(int n) {
  // sing a note for 80 milliseconds. also blink led
  digitalWrite(ledpins[octave], HIGH);
  tone(pinBuzzer, notes[octave][n], 100);  
  delay(75);
  //noTone(pinBuzzer);
  digitalWrite(ledpins[octave], LOW);
}

// these pins correpond to A4, A2, A3, A5, 0, 1, 3, 5, 6, 10, 11, 13
// This matches the pins from left to right, buttom to top on SquareWear 2.0
const int pins[] = {18, 16, 17, 19, 0, 1, 3, 5, 6, 10, 11, 13};

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
