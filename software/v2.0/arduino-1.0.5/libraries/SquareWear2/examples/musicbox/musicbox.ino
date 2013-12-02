// *******************************
// SquareWear 2.0 Examples
// Sleep and wakeup using watchdog timer
// RAYSHOBBY.net
//
// This example makes use of the watchdog
// timer to put SquareWear to sleep and
// wake up every 2 seconds to blink the green
// LED. This allows maximal power saving and
// even with the on-board coin battery the
// program can run for a long time.
// 
// *******************************

#define green 12
#define pinbuzzer 9
#define light_sensor A0

#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include "pitches.h"

volatile int f_wdt = 1;

ISR(WDT_vect)
{
  if(f_wdt == 0)
  {
    f_wdt=1;
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

void setup() {
  
  pinMode(green, OUTPUT);
  digitalWrite(green, LOW);

  pinMode(pinbuzzer, OUTPUT);    // make sure the buzzer is turned off
  digitalWrite(pinbuzzer, LOW);
  
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



void loop() {
  int light_value;
  if(f_wdt==1) {
    light_value = analogRead(light_sensor);
    if(light_value > 400){
      for(int i = 0; i < sizeof(melody)/sizeof(int); i++){
        int d = 1000 / durations[i];
        tone(pinbuzzer, melody[i], d);
        int p = d*1.1;
        delay(p);
        noTone(pinbuzzer);
        light_value = analogRead(light_sensor);
        if(light_value <= 400){
          i = sizeof(melody)/sizeof(int);
        }
      }
    }
    f_wdt = 0;
    enterSleep();
  }
}


