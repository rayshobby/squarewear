// *******************************
// SquareWear 2.0 Examples
// RGB LED pad demo using Adafruit
// neopixel library
// RAYSHOBBY.net
//
// This demo shows three different
// patterns. To cycle through
// the patterns, click the pushbutton
// - Connection:
//   the DATA pin of the LED pad
//   should be connected to pin
//   D1 on SquareWear2
// *******************************

#include <Adafruit_NeoPixel.h>
#include <PciManager.h>
#include <PciListenerImp.h>

#define uchar unsigned char
#define ulong unsigned long

#define pinButton 4
#define pinLEDPad 1 // D1

// the RGB LED pad has 5x7 = 35 LEDs
Adafruit_NeoPixel strip = Adafruit_NeoPixel(35, pinLEDPad, NEO_GRB + NEO_KHZ800);
PciListenerImp listener(pinButton, onPinButtonChange);
uchar buttonPressed = 0;

void onPinButtonChange(byte changeKind) {
  static ulong t = 0;
  if (changeKind == 1 && millis() > t + 250) {
    t = millis();
    buttonPressed = 1;
  }
}


int letter = 0;

int empty[7][5] = {{0,0,0,0,0},
                   {0,0,0,0,0},
                   {0,0,0,0,0},
                   {0,0,0,0,0},
                   {0,0,0,0,0},
                   {0,0,0,0,0},
                   {0,0,0,0,0}
                  };


int skull[7][5] = {{0,1,1,1,0},
                   {1,0,0,0,1},
                   {1,1,0,1,1},
                   {1,0,0,0,1},
                   {0,1,0,1,0},
                   {0,1,0,1,0},
                   {0,1,1,1,0}};
                   
// The color is specified as an integer
// In HEX it's formated as 0xRRGGBB
// where RR is the value (00 to FF) for red
// GG is for green
// and BB is for blue.
// For example, 0xFF0000 is pure red
// 0X200080 is purple-ish, and so on.
uint32_t pumpkin[7][5] = {
                     {0x000000,0x00FF00,0x000000,0x000000,0x000000},
                     {0x000000,0x000000,0x00FF00,0x000000,0x000000},
                     {0x000000,0x802000,0x802000,0x802000,0x000000},
                     {0x802000,0x000000,0x000000,0x000000,0x802000},
                     {0x802000,0x000000,0x000000,0x000000,0x802000},
                     {0x802000,0x000000,0x000000,0x000000,0x802000},
                     {0x000000,0x802000,0x802000,0x802000,0x000000}};

int H[7][5]= {{1,0,0,0,1},
              {1,0,0,0,1},
              {1,0,0,0,1},
              {1,1,1,1,1},
              {1,0,0,0,1},
              {1,0,0,0,1},
              {1,0,0,0,1}};

int E[7][5]= {{1,1,1,1,1},
              {1,0,0,0,0},
              {1,0,0,0,0},
              {1,1,1,1,1},
              {1,0,0,0,0},
              {1,0,0,0,0},
              {1,1,1,1,1}};
     
int L[7][5]= {{1,0,0,0,0},
              {1,0,0,0,0},
              {1,0,0,0,0},
              {1,0,0,0,0},
              {1,0,0,0,0},
              {1,0,0,0,0},
              {1,1,1,1,1}};
           
int O[7][5]= {{1,1,1,1,1},
              {1,0,0,0,1},
              {1,0,0,0,1},
              {1,0,0,0,1},
              {1,0,0,0,1},
              {1,0,0,0,1},
              {1,1,1,1,1}};

int W[7][5]= {{1,0,0,0,1},
              {1,0,0,0,1},
              {1,0,0,0,1},
              {1,0,0,0,1},
              {1,0,1,0,1},
              {1,1,0,1,1},
              {1,0,0,0,1}};
              
int R[7][5]= {{1,1,1,1,1},
              {1,0,0,0,1},
              {1,0,0,0,1},
              {1,1,1,1,0},
              {1,0,1,0,0},
              {1,0,0,1,0},
              {1,0,0,0,1}};
              
int D[7][5]= {{1,1,1,1,0},
              {1,0,0,0,1},
              {1,0,0,0,1},
              {1,0,0,0,1},
              {1,0,0,0,1},
              {1,0,0,0,1},
              {1,1,1,1,0}};
 
void loop_helloWorld(){
  while(1){
    if(buttonPressed) break;

    letter = 0;
    show_pattern(strip.Color(255, 0, 0), 0); // Red
    delay(1000);
    show_pattern(strip.Color(0,0,0),0);
    if(buttonPressed) break;
    
    letter = 1;
    show_pattern(strip.Color(255,0,0),0);
    delay(1000);
    show_pattern(strip.Color(0,0,0),0);
    if(buttonPressed) break;
  
    letter = 2;
    show_pattern(strip.Color(255,0,0),0);
    delay(1000);
    show_pattern(strip.Color(0,0,0),0);
    if(buttonPressed) break;
  
    show_pattern(strip.Color(255,0,0),0);
    delay(1000);
    show_pattern(strip.Color(0,0,0),0);
    if(buttonPressed) break;
    
    letter = 3;
    show_pattern(strip.Color(255,0,0),0);
    delay(1000);
    show_pattern(strip.Color(0,0,0),0);
    if(buttonPressed) break;
    
    letter = -1;
    show_pattern(strip.Color(255,0,0),0);
    delay(1000);
    show_pattern(strip.Color(0,0,0),0);
    if(buttonPressed) break;
    
    letter = 4;
    show_pattern(strip.Color(255,0,0),0);
    delay(1000);
    show_pattern(strip.Color(0,0,0),0);
    if(buttonPressed) break;
    
    letter = 3;
    show_pattern(strip.Color(255,0,0),0);
    delay(1000);
    show_pattern(strip.Color(0,0,0),0);
    if(buttonPressed) break;
  
    letter = 5;
    show_pattern(strip.Color(255,0,0),0);
    delay(1000);
    show_pattern(strip.Color(0,0,0),0);
    if(buttonPressed) break;
  
    letter = 2;
    show_pattern(strip.Color(255,0,0),0);
    delay(1000);
    show_pattern(strip.Color(0,0,0),0);
    if(buttonPressed) break;
  
    letter = 6;
    show_pattern(strip.Color(255,0,0),0);
    delay(1000);
    show_pattern(strip.Color(0,0,0),0);
    if(buttonPressed) break;

    delay(1000);
  }
}

void loop_skull(){
  while(1){
    letter = -2;
    show_pattern(strip.Color(255,255,255),0);
    delay(1000);
    if(buttonPressed) break;

    letter = -1;
    show_pattern(strip.Color(0,0,0),0);
    delay(1000);
    if(buttonPressed) break;

  }
}

void loop_pumpkin(){
  while(1){
    letter = -3;
    show_pattern(strip.Color(0,0,0),0);
    delay(1000);
    if(buttonPressed) break;
  
    letter = -1;
    show_pattern(strip.Color(0,0,0),0);
    delay(1000);
    if(buttonPressed) break;
  }
}
 
 
              
              
void setup() {
  pinMode(pinButton, INPUT);
  digitalWrite(pinButton, HIGH);
  PciManager.registerListener(pinButton, &listener);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(127);
}

int demo = 0;
void loop() {
  switch(demo){
    case 0:
       buttonPressed = 0;
       loop_helloWorld();
       demo = 1;
       break;
    case 1:
        buttonPressed = 0;
        loop_skull();
        demo = 2;
        break;
    case 2:
        buttonPressed = 0;
        loop_pumpkin();
        demo = 0;
        break;
    default:
        break;
  }
}

// Fill the dots one after the other with a color
void show_pattern(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      uint32_t isOn = 0;
      switch(letter){
        case -3:
           isOn = pumpkin[i%7][i/7];
           break;
        case -2:
           isOn = skull[i%7][i/7];
           break;        
        case 0:
          isOn = H[i%7][i/7];
          break;
        case 1:
          isOn = E[i%7][i/7];
          break;
        case 2:
          isOn = L[i%7][i/7];
          break;
        case 3:
          isOn = O[i%7][i/7];
          break;
        case 4:
          isOn = W[i%7][i/7];
          break;
        case 5:
          isOn = R[i%7][i/7];
          break;
        case 6:
          isOn = D[i%7][i/7];
          break;
        default:
          isOn = empty[i%7][i/7];
          break;
      }    
      if(isOn == 1){
        strip.setPixelColor(i, c);
      } else if(isOn == 0){
        strip.setPixelColor(i,0);
      } else {
        strip.setPixelColor(i,isOn);
      }
      strip.show();
      delay(wait);
  }
}

