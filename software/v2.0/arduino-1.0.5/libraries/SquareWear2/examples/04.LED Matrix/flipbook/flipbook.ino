#include <HIDSerial.h>
#include <Adafruit_NeoPixel.h>
#include <avr/eeprom.h>
#include <EEPROM.h>

#include "Wire.h"
#include "pitchesArray.h"

#define I2C_EEPROM_DEVICE_ADDR 0x50
#define pinLED 10
#define pinBuzzer 9
#define pinButton 4

Adafruit_NeoPixel strip = Adafruit_NeoPixel(35*12, pinLED, NEO_GRB + NEO_KHZ800);

HIDSerial serial;
boolean animating = false;
boolean altered = false;
unsigned char buffer[16];
int nBoardX;
int nBoardY;
int nFrames;
int nBright;
int frameTime;
int frameTone;

#define ADDR_FRAME_DATA 4

struct frameData {
  //uint16_t frame_time;  // frame time, 0ms to 65535ms
  //byte tone_index;      // 255 means no tone
  //byte fade : 1;        // fade to the next frame
  byte time16, time8, time;
  byte tone;
};


int currentFrame = 0;
unsigned long t = 0;


void load_board_configuration() {
  // first four bytes of EEPROM store:
  // - total number of frames
  // - number of boards in X direction
  // - number of boards in Y direction
  // - global nBright
  nFrames = eeprom_read_byte((unsigned char*)0);
  nBoardX = eeprom_read_byte((unsigned char*)1);
  nBoardY = eeprom_read_byte((unsigned char*)2);
  nBright = eeprom_read_byte((unsigned char*)3);
}

void save_board_configuration() {
  eeprom_write_byte((unsigned char*)0, nFrames);
  eeprom_write_byte((unsigned char*)1, nBoardX);
  eeprom_write_byte((unsigned char*)2, nBoardY);
  eeprom_write_byte((unsigned char*)3, nBright);
}

void load_frame_header(int fidx, int& ftime, int& ftone) {
  unsigned char* addr = (unsigned char*)ADDR_FRAME_DATA + fidx * sizeof(frameData);
  ftime = (eeprom_read_byte(addr+0))*256+
          (eeprom_read_byte(addr+1))*16+
          (eeprom_read_byte(addr+2));
  ftone =  eeprom_read_byte(addr+3);
}

void save_frame_header(int fidx, int ftime, int ftone) {
  unsigned char* addr = (unsigned char*)ADDR_FRAME_DATA + fidx * sizeof(frameData);
  eeprom_write_byte((addr+0), ftime/256);
  eeprom_write_byte((addr+1), ftime%256/16);  
  eeprom_write_byte((addr+2), ftime%16);
  eeprom_write_byte((addr+3), ftone);
}

void setup() {
  load_board_configuration();

  serial.begin();
  strip.begin();
  //strip.setBrightness(128/(nBoardX*nBoardY));
  strip.show();

  pinMode(pinButton, INPUT);
  digitalWrite(pinButton, HIGH);

  Wire.begin();
  pinMode(pinBuzzer, OUTPUT);
  digitalWrite(pinBuzzer, LOW);
}

void loop(){
  byte color[3];
  if(animating){
    for(unsigned int i = 0; i < 35*nBoardX*nBoardY; i++){
      for(unsigned int j = 0 ; j < 3; j++){
        ext_eeprom_read_buffer((i+currentFrame*35*nBoardX*nBoardY)*3, color, 3);
      }
      strip.setPixelColor(i,color[1],color[0],color[2]);
    }
    strip.show();
    int ftime, ftone;
    load_frame_header(currentFrame, ftime, ftone);
    if (ftime == 0) ftime = 100;
    int to = pgm_read_word(pitches+ftone);
    if (to != 0)
      tone(pinBuzzer, to, ftime);
    delay(ftime);
    noTone(pinBuzzer);
    currentFrame = (currentFrame+1)%nFrames;
  } else {
    loopInteractive();
  }
  
  // process button
  static byte button_status = 1;
  static unsigned long t = 0;
  if (digitalRead(pinButton)==0) {
    if (button_status == 1) {
      button_status = 0;
      t = millis();
    }
  } else {
    if (button_status == 0) {
      button_status = 1;
      if (millis() > t + 10) {
        animating = !animating;
        if(!animating){
          for(int i = 0; i < strip.numPixels(); i++){
            strip.setPixelColor(i,0,0,0);
          }
        }        
      }
    }
  }  
}

void loopInteractive() {
  if(serial.available()) {
    int size = serial.read(buffer);
    if(size == 1){
      if(buffer[0] == 'x'){
        save_board_configuration();
        save_frame_header(currentFrame, frameTime, frameTone);
        for(unsigned int i = 0; i < 35*nBoardX*nBoardY*3; i++){
          ext_eeprom_write_byte(i+currentFrame*35*nBoardX*nBoardY*3,(strip.getPixels()[i]));
        }
      } else if (buffer[0] == 'l'){
        byte buf[4];
        load_board_configuration();
        buf[0] = nFrames + '0';
        buf[1] = nBoardX + '0';
        buf[2] = nBoardY + '0';
        buf[3] = nBright + '0';
        serial.write(buf,4);
        for(unsigned int i = 0; i < 35*nBoardX*nBoardY*nFrames; i++){
          unsigned int color[3];
          for(unsigned int j = 0 ; j < 3; j++){
            color[j] = ext_eeprom_read_byte(i*3+j);
          }
          strip.setPixelColor(i%(35*nBoardX*nBoardY),color[1],color[0],color[2]);
          currentFrame = i/(35*nBoardX*nBoardY);
          // to do: inefficient
          sendPixel(i,color[1],color[0],color[2]);
        }
        byte buff[6];
        for(int i = 0; i < nFrames; i++){
          buff[0] = '0' + i;
          buff[1] = EEPROM.read(i*4+4)+'0';
          buff[2] = EEPROM.read(i*4+5)+'0';
          buff[3] = EEPROM.read(i*4+6)+'0';
          buff[4] = EEPROM.read(i*4+7)/16+'0';
          buff[5] = EEPROM.read(i*4+7)%16+'0';
          serial.write(buff,6);
        }
      }
    }
    /*
    if(size == 2) {
      int oldnBright = nBright;
      nBright = (buffer[0] -'0')*16 + buffer[1] - '0';
      nBright = min(nBright,128/(nBoardX*nBoardY));
      if(nBright != oldnBright){
        for(int i = 0; i < 35*nBoardX*nBoardY; i++) {
          int g = strip.getPixels()[i*3];
          int r = strip.getPixels()[i*3+1];
          int b = strip.getPixels()[i*3+2];
          r = r*nBright/oldnBright;
          g = g*nBright/oldnBright;
          b = b*nBright/oldnBright;
          strip.setPixelColor(i,r,g,b);
        }
        strip.show();
      }
    }*/
    if(size == 4){
      nFrames = buffer[0] - '0';
      nBoardX = buffer[1] - '0';
      nBoardY = buffer[2] - '0';
      /*
      int oldnBright = nBright;
      nBright = min(nBright,32/(nBoardX*nBoardY));
      if(nBright != oldnBright) {
        for(int i = 0; i < 35*nBoardX*nBoardY; i++) {
          int g = strip.getPixels()[i*3];
          int r = strip.getPixels()[i*3+1];
          int b = strip.getPixels()[i*3+2];
          r = r*nBright/oldnBright;
          g = g*nBright/oldnBright;
          b = b*nBright/oldnBright;
          strip.setPixelColor(i,r,g,b);
        }
        strip.show();
      } */       
      currentFrame = buffer[3] - '0';
      frameTime = EEPROM.read(4+currentFrame*4)*256+EEPROM.read(5+currentFrame*4)*16+EEPROM.read(6+currentFrame*4);
    }   
    if(size == 5) {
      for(int i = 0; i < size; i++){
        buffer[i] = buffer[i]-'0';
      }
      frameTime = 256*buffer[0]+16*buffer[1]+buffer[2];
      frameTone = 16*buffer[3] + buffer[4];
      if(frameTime > 0) {
        int to = pgm_read_word(pitches+frameTone);
        if (to != 0 )
          tone(pinBuzzer,pgm_read_word(pitches+frameTone),frameTime);
        delay(frameTime);
        noTone(pinBuzzer);
      }
    } 
    if(size == 8) {
      for(int i = 0; i < size; i++){
        buffer[i] -= '0';
      }
      int pixel = getPixelNumber(buffer[0],buffer[1]);
      int r = buffer[2] * 16 + buffer[3];
      int g = buffer[4] * 16 + buffer[5];
      int b = buffer[6] * 16 + buffer[7];
      strip.setPixelColor(pixel,r,g,b);
      altered = true;
    }
    t = millis();
  }
  if (altered && millis() > t+500) {
    strip.show();
    t=millis();
    altered = false;
  }
  serial.poll();
}

int sendPixel(int p, int r, int g, int b){ 
  byte buffer[9];
  int currentFrame = p/(nBoardX*nBoardY*35);
  p = p%(nBoardX*nBoardY*35);
  int boardNum = p/35;
  int boardIdx = p%35;
  int xBoard = boardNum/nBoardY;
  int yBoard = boardNum%nBoardY;
  int x = xBoard*5+boardIdx/7;
  int y = yBoard*7+boardIdx%7;
  buffer[0] = currentFrame;
  buffer[1] = x;
  buffer[2] = y;
  buffer[3] = r/16;
  buffer[4] = r%16;
  buffer[5] = g/16;
  buffer[6] = g%16;
  buffer[7] = b/16;
  buffer[8] = b%16;
  for(int i = 0; i < 9; i++){
    buffer[i] += '0';
  }
  serial.write(buffer,9);
}

int getPixelNumber(int x,int y){
  return (y/7)*35*nBoardX+(x/5)*35 + (x%5)*7+(y%7);
}

// maybe let's not read more than 30 or 32 bytes at a time!
void ext_eeprom_read_buffer(unsigned int eeaddress, byte *buffer, int length) {
  Wire.beginTransmission(I2C_EEPROM_DEVICE_ADDR);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(I2C_EEPROM_DEVICE_ADDR,length);
  int c = 0;
  for ( c = 0; c < length; c++ )
    if (Wire.available()) buffer[c] = Wire.read();
}

byte ext_eeprom_read_byte(unsigned int eeaddress) {
  byte rdata = 0xFF;
  Wire.beginTransmission(I2C_EEPROM_DEVICE_ADDR);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(I2C_EEPROM_DEVICE_ADDR,1);
  if (Wire.available()) rdata = Wire.read();
  return rdata;
}

void ext_eeprom_write_byte(unsigned int eeaddress, byte data) {
  int rdata = data;
  Wire.beginTransmission(I2C_EEPROM_DEVICE_ADDR);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(rdata);
  Wire.endTransmission();
  delay(3);
}

