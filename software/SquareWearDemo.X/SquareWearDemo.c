/* SquareWear Demo Program Pack 1.0
 *
 * All content is published under:
 * Creative Commons Attribution ShareAlike 3.0 License
 *
 * Oct 7 2012, Rayshobby.net
 */


#include "SquareWear.h"
#include <stdlib.h>
#include <usart.h>

/* ===========================================================
 * -----------------------------------------------------------
 * To switch to a different demo, right-click the project name
 * SquareWearDemo.x in MPLABX, then in the drop-down menu go to
 * 'Set Configuration ->' and select a demo name there. The
 * output .hex file will be in each demo's individual folder.
 *
 * Alternatively, select 'default' as the project configuration,
 * and define the demo macro below. Note that in this case the
 * output files will ALL be in the folder named 'default'.
 * -----------------------------------------------------------
 * =========================================================== */

#ifdef DEFAULT_CONFIG
    #define DEMO_BLINK
    //#define DEMO_BLINK_NODELAY
    //#define DEMO_USB_SERIAL_OUTPUT
    // ........
#endif

// LED blink time (in milliseconds)
static uint delaytime = 500;

// ====== Blink LED Demo ======
// Blink built-in LED (pinC7).
#ifdef DEMO_BLINK
void main(void) {
    initSquareWear();       // must call initialization function first
    setModeOutput(pinC7);   // set pinC7 as digital OUTPUT pin
    while(1) {
        latC7 = !latC7 ;    // toggle pin C7 value
        delayMilliseconds(delaytime);
    }
}
#endif  // DEMO_BLINK



// ====== Blink LED (no delay) Demo ======
// Blink built-in LED (pinC7) without using delay function.
#ifdef DEMO_BLINK_NODELAY
void main(void) {
    ulong prev;
    initSquareWear();
    setModeOutput(pinC7);
    prev = millis();
    while(1) {
        // if delaytime has passed
        if (millis() >= (prev+delaytime)) {
            prev = millis();
            latC7 = !latC7; // toggle pinC7
        }
    }
}
#endif  // DEMO_BLINK_NODELAY



// ====== Blink All Pins Demo ======
// Blink all 12 digital pins
// This demo assumes common source: i.e. the positive lead
// of each LED is connected to Vdd (common source),
// and the negative lead is connected to individual pin.
#ifdef DEMO_BLINK_ALL
void main(void) {
    // array of pin names
    byte pins[12] = {pinC0, pinC1, pinC2, pinC3,
                     pinC4, pinC5, pinC6, pinC7,
                     pinB4, pinB5, pinB6, pinB7};
    // since pins C4-C7 are current sinks, they are active HIGH,
    // the other pins are active LOW.
    byte highs[12]= {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0};
    byte i;
    initSquareWear();
    // portC pins are set as output mode by default
    setModeOutput(pinB4);
    setModeOutput(pinB5);
    setModeOutput(pinB6);
    setModeOutput(pinB7);
    while(1) {
        for(i=0;i<12;i++) {
            setValue(pins[i], highs[i]);
        }
        delayMilliseconds(delaytime);
        for(i=0;i<12;i++) {
            setValue(pins[i], 1-highs[i]);
        }
        delayMilliseconds(delaytime);
    }
}
#endif // DEMO_BLINK_ALL

// ====== Button Press Demo ======
// Turn on LED (pinC7) when built-in button is pressed.
#ifdef DEMO_BUTTON_PRESS
void main(void) {
    initSquareWear();
    setModeOutput(pinC7);
    while(1) {
        if (buttonPressed()) {
            latC7 = 1;
        } else {
            latC7 = 0;
        }
    }
}
#endif // DEMO_BUTTON_PRESS

// ====== Digital Input Demo ======
// Turn on LED (pinC7) when pinB4 is low (connected to ground)
#ifdef DEMO_DIGITAL_INPUT
void main(void) {
    initSquareWear();
    setModeOutput(pinC7);
    setModeInput(pinB4);
    enablePullup(pinB4);    // enable internal pullup
    while(1) {
        if (getValue(pinB4)==0) {
            latC7 = 1;
        } else {
            latC7 = 0;
        }
    }
}
#endif // DEMO_DIGITAL_INPUT


// ====== Software PWM Demo ======
// Fade LED (pinC7) and pinC3 using software PWM.
// Since pinC7 is connected to high-current sink, LEDs connected
// to pinC7 look much brighter than other digital pins.
#ifdef DEMO_SOFTWARE_PWM
void main(void) {
    byte duty = 0;
    char incr = 1;
    initSquareWear();
    setModePWM(pinC7);  // set pinC7 and pinC3 as PWM output pins
    setModePWM(pinC3);

    while(1) {
        setValue(pinC7, duty);  // set duty cycle
        setValue(pinC3, duty);
        duty += incr;
        if (duty == 31) incr = -1;  // maximum duty cycle is 31
        else if (duty == 0) incr = +1;
        delayMilliseconds(100);
    }
}
#endif  // DEMO_SOFTWARE_PWM


// ====== Hardware PWM Demo ======
// Fade pinC5 using hardware PWM
// pinC5 is the only hardware PWM pin supported by SquareWear library
#ifdef DEMO_HARDWARE_PWM
void main(void) {
    uint duty = 0;
    char incr = 1;
    initSquareWear();

    // open hardware pwm
    openHardwarePWM();

    while(1) {
        setHardwarePWMduty(duty);
        duty += incr;
        if (duty == 1023) incr = -1;  // hardware pwm maximum duty cycle 1023
        else if (duty == 0) incr = +1;
        delayMilliseconds(2);
    }
}
#endif  // DEMO_HARDWARE_PWM



// ====== USB Serial Output Demo ======
// Toggle LED (pinC7) and outputs the amount of time
// since the last toggle to USB serial
#ifdef DEMO_USB_SERIAL_OUTPUT
void main(void) {
    ulong t;
    char msg[8];        // buffer to store display message
    initSquareWear();
    setModeOutput(pinC7);
    openUSBSerial();    // open USB serial port
    putrsUSBSerial("Begin.\r\n");    // write a constant string
    t=millis();
    while(1) {
        // if at least 1 second has passed
        if (millis() > t+1000) {
            t = millis();
            ltoa(t, msg);       // convert time to string
            putsUSBSerial(msg); // output to USB serial
            putrsUSBSerial("\r\n"); // carriage return
            latC7 = !latC7;     // toggle pinC7
        }
        pollUSBSerial();        // call this as often as you can
                                // in the inner loop
    }
}
#endif  // DEMO_USB_SERIAL_OUTPUT



// ====== USB Serial Input Demo ======
// Press a single digit number (e.g. 4) and turn on LED (pinC7)
// for the specified amount of seconds.
#ifdef DEMO_USB_SERIAL_INPUT
void main(void) {
    byte len, i;
    char msg[1];
    int value;
    
    initSquareWear();
    setModeOutput(pinC7);
    latC7 = 0;
    openUSBSerial();    // open USB serial port
    putrsUSBSerial("Type a single digit number:\r\n");
    while(1) {
        len=getUSBSerial(msg, 1);   // get one character from USB serial
        if(len!=0 && msg[0]>='0' && msg[0]<='9') {
            value = msg[0]-'0'; // extract value
            if (value<10) {
                putrsUSBSerial("On for ");
                putnUSBSerial(msg, 1);
                putrsUSBSerial(" seconds.\r\n");
                latC7 = 1;
                delaySeconds(value);
                latC7 = 0;
            }
        }
        pollUSBSerial();
    }
}
#endif  // DEMO_USB_SERIAL_INPUT



// ====== ADC (Analod Read) Demo 1 ======
// Read value from pinADC4, and use the value
// to control brightness of LED (pinC7).
#ifdef DEMO_ADC_PWM
void main(void) {
    uint value;
    initSquareWear();
    
    setModeADC(pinADC4);    // set ADC4 as analog read pin
    setModePWM(pinC7);      // set C7 as PWM output pin

    while(1) {
        // read 10-bit analog value
        value = getValue(pinADC4);
        // convert the analog value to 5-bit PWM value
        // and set it for pinC7
        setValue(pinC7, value>>5);
        delayMilliseconds(5);
    }
}
#endif  // DEMO_ADC_PWM



// ====== ADC (Analod Read) Demo 2 ======
// Read value from pinADC4, and use the value
// to control the blinking frequency of LED (pinC7);
#ifdef DEMO_ADC_BLINK
void main(void) {
    uint value;

    initSquareWear();
    setModeADC(pinADC4);    // set ADC4 as analog read pin
    setModeOutput(pinC7);   // set C7 as PWM output pin

    while(1) {
        // read analog value
        value = getValue(pinADC4);
        latC7 = !latC7;
        delayMilliseconds(value);   // set delay time
    }
}
#endif  // DEMO_ADC_BLINK



// ====== ADC (Analod Read) Demo 3 ======
// Read value from pinADC4, output to USB serial
#ifdef DEMO_ADC_USB_SERIAL
void main(void) {
    uint value;
    ulong t;
    char msg[8];
    initSquareWear();
    setModeADC(pinADC4);
    openUSBSerial();
    putrsUSBSerial("Begin.\r\n");
    t = millis();
    while(1) {
        // if at least 1 second has passed
        if(millis()>t+1000) {
            t = millis();
            // read value from pinADC4
            value = getValue(pinADC4);
            // outputs to USART serial
            itoa(value, msg);
            putsUSBSerial(msg);
            putrsUSBSerial("\r\n");
        }
        // must call this as often as possible in the inner loop
        pollUSBSerial();
    }
}
#endif  // DEMO_ADC_USB_SERIAL



// ====== Timer Interrupt Demo ======
// This demo uses timer interrupt to toggle LED (pinC7),
// at the same time fades pinC6 using PWM.
// This shows how to use timer interrupt to perform periodic tasks
#ifdef DEMO_TIMER_INTERRUPT

// Timer interrupt callback function
void toggle(void) {
    // toggle LED (pinC7)
    latC7 = !latC7;
}

void main(void) {
    byte duty = 0;
    char incr = 1;
    initSquareWear();
    setModeOutput(pinC7);   // set pinC7 as output pin
    setModePWM(pinC6);      // set pinC6 as PWM pin

    // open timer interrupt, use 'delaytime' as interval'
    // and 'toggle' as callback function
    openTimerInterrupt(delaytime, toggle);
    while(1) {
        // fade pinC6
        setValue(pinC6, duty);
        duty += incr;
        if (duty == 31) incr = -1;
        else if (duty == 0) incr = +1;
        delayMilliseconds(100);
    }
}
#endif  // DEMO_TIMER_INTERRUPT


// ====== Button interrupt Demo 1 ======
// This demo uses button interrupt to wake up CPU from deep sleep.
// In deep sleep, the CPU consumes minimal power and
// thus can run on battery for a long time.
#ifdef DEMO_BUTTON_INTERRUPT

// button interrupt callback function
void callback(void) {
    // do thing
    // the purpose of this function in this demo is
    // to wake up CPU from deep sleep
}

void main(void) {
    byte duty=0;
    initSquareWear();
    setModePWM(pinC7);

    // open button interrupt before going to sleep
    openOnBoardButtonInterrupt(callback);
    
    while(1) {
        // fade LED (pinC7)
        for(duty=0;duty<32;duty++) {
            setValue(pinC7, duty);
            delayMilliseconds(50);
        }
        for(duty=32;duty!=0;duty--) {
            setValue(pinC7, duty-1);
            delayMilliseconds(50);
        }
        // enter deep sleep: CPU clock will stop running
        // the only way to wake up CPU is through external,
        // pin-change, watchdog interrupts, or reset.
        deepSleep();
    }
}
#endif // DEMO_BUTTON_INTERRUPT



// ====== External Interrupt Demo ======
// This demo uses external interrupt to detect rising edges
// of an external signal, and counts the number of edges per second.
// It can be used as frequency meter.
// Input signal is connected to pinC1 (INT1).
#ifdef DEMO_EXT_INTERRUPT

// any variable that will be changed in an interrupt
// callback function must be declared as volatile
static volatile int counter = 0;

// external interrupt callback function
void count(void) {
    counter ++;
}

void main(void) {
    char msg[6];
    ulong t;
    initSquareWear();
    openUSBSerial();
    putrsUSBSerial("Begin.\r\n");

    // open external interrupt 1 (pinC1),
    // detect rising edge, use 'count' as callback function
    openExtInterrupt(1, RISING, count);
    t=millis();
    // the main loop displays count value per second
    while(1) {
        if(millis()>t+1000) {
            t=millis();
            itoa(counter, msg);
            putrsUSBSerial("counter = ");
            putsUSBSerial(msg);
            putrsUSBSerial("\r\n");
        }
        pollUSBSerial();
    }
    
}
#endif // DEMO_EXT_INTERRUPT



// ====== USART Output Demo ======
// This demo uses USART to output messages to serial.
// Requires an external USB-to-Serial converter.
// Connection: (serial converter -> SquareWear)
//    GND -> GND
//    RX  -> TX (pinB7)
#ifdef DEMO_USART_OUTPUT
void main(void) {
    char msg[6];
    int counter=0;
    initSquareWear();

    // open USART, default baud rate 57600 (defined in SquareWear.h)
    openUSART();
    putrsUSART("Begin.\r\n");   // write a constant string
    while(1) {
        itoa(counter, msg);
        putsUSART(msg);         // write a string
        putrsUSART("\r\n");
        latC7 = !latC7;
        delaySeconds(1);
        counter++;
    }
}
#endif  // DEMO_USART_OUTPUT



// ====== USART Input Demo ======
// This demo uses USART to read values from serial.
// Input a single digit number and turn on LED (pinC7) for
// the specified amount of seconds.
// Requires an external USB-to-Serial converter.
// Connection: (serial converter -> SquareWear)
//    GND -> GND
//    RX  -> TX (pinB7)
//    TX  -> RX (pinB5)
#ifdef DEMO_USART_INPUT
void main(void) {
    char msg[1];
    int value;

    initSquareWear();
    latC7 = 0;
    openUSART();
    putrsUSART("Type a single digit number:\r\n");
    while(1) {
        getsUSART(msg, 1);
        if(msg[0]>='0' && msg[0]<='9') {
            value = msg[0]-'0';
            if (value<10) {
                putrsUSART("On for ");
                WriteUSART(msg[0]);
                putrsUSART(" seconds.\r\n");
                latC7 = 1;
                delaySeconds(value);
                latC7 = 0;
            }
        }
    }
}
#endif // DEMO_USART_INPUT



// ====== EEPROM Demo ======
// This program blinks LED at three different frequencies.
// Each time the device restarts, it changes to the next frequency.
// Implemented by storing the index of the next frequency at EEPROM.
#ifdef DEMO_EEPROM
void main(void) {
    byte evalue;
    initSquareWear();
    setModeOutput(pinC7);
    latC7 = 0;
    delayMilliseconds(250); // short delay for EEPROM to stablize
    evalue = readEEPROM(0x0);   // read EEPROM address 0
    switch(evalue) {
        case 0:
            writeEEPROM(0x0, 1);    // write next index to EEPROM address 0
            busyEEPROM();           // wait till write completes
            while(1) {
                latC7 = !latC7;
                delayMilliseconds(100);
            }
            break;
        case 1:
            writeEEPROM(0x0, 2);
            busyEEPROM();
            while(1) {
                latC7 = !latC7;
                delayMilliseconds(500);
            }
            break;
        default:
            writeEEPROM(0x0, 0);
            busyEEPROM();
            while(1) {
                latC7 = !latC7;
                delayMilliseconds(1000);
            }
            break;

    }
}
#endif  // DEMO_EEPROM



// ====== Fancy PWM Demo ======
// This demo uses PWM to turn on all eight portC pins (C0-C7)
// with a waving brightness pattern.
// Connect the positive leads of eight LEDs to common SOURCE (Vdd),
// and the negative lead of each LED to the corresponding portC pin.

#ifdef DEMO_FANCY_PWM
#include <math.h>
byte pins[] = {pinC7, pinC6, pinC5, pinC4, pinC3, pinC2, pinC1, pinC0};
byte onvs[] = {0, 0, 0, 0, 31, 31, 31, 31};
void main(void) {
    byte i;
    float c = 0;
    float b;
    initSquareWear();
    for(i=0;i<8;i++) {
        // set all portC pins for PWM output
        setModePWM(pins[i]);
    }
    while(1) {
        for(i=0; i<8; i++) {
            // compute PWM value
            b = (sin(3.14/4*i+c) + 1.0) / 2.0;
            b = sqrt(b) * 31;
            if (onvs[i] == 0)
                b = 31 - b;
            // set PWM value
            setValue(pins[i], b);
        }
        delayMilliseconds(10);
        c += 0.1;
    }
}
#endif  // DEMO_FANCY_PWM



// ====== Pulse Sensor Demo ======
// This demo detects pulse from an external pulse sensor, and
// blinks the built-in LED (pinC7) according to the detected pulse.
// The calculated heart rate is displayed to USB serial output.
// Assumes pulse sensor's data pin is connected to pinADC4.
#ifdef DEMO_PULSE_SENSOR
#define OUTPUT_TO_USB_SERIAL 0
// data history size
#define NHIST   7
// data is stored in a ring buffer
static volatile uint rawdata[NHIST];    // raw sensor values
static volatile uint data[NHIST];       // filtered sensor values (running average)
static volatile byte idx = 0;           // where the current value should be stored to
static volatile byte climb = 0;         // climb status
static volatile ulong t_prev_peak = 0;  // time of the previous detected peak

void sample(void) {
    char msg[5];
    byte i;
    rawdata[idx] = getValue(pinADC4);   // read sensor value
    data[idx]=0;
    // calculate filtered value (running average of rawdata)
    for(i=0;i<NHIST;i++) {
        data[idx]+=rawdata[i];
    }
    data[idx]=data[idx]/NHIST;

    // check climb status
    if(climb) {
        // if values are climbing in the previous step
        // the condition of continued climbing is that
        // the current value is no less than previous (history) values.
        for(i=idx+1;i<idx+NHIST;i++) {
            if(data[idx]<data[i%NHIST])
                break;
        }
    } else {
        // if values are not climbing in the previous step
        // the condition of climbing is that the current value
        // is higher than all previous (history) values.
        for(i=idx+1;i<idx+NHIST;i++) {
            if(data[idx]<=data[i%NHIST])
                break;
        }
    }

    if(i==idx+NHIST) {
        // climb status is true
        climb = 1;
    } else {
        // detect peak
        // peak is defined as the point where values have just stopped climbing
        // (i.e. climb status was true in the previous step but not current step
        if(climb==1) {  // previous status is true
#if OUTPUT_TO_USB_SERIAL
            // output heart rate
            ltoa(60000/(millis()-t_prev_peak), msg);
            putrsUSBSerial("Heart rate: ");
            putsUSBSerial(msg);
            putrsUSBSerial("\r\n");
#endif
            t_prev_peak=millis();   // update previous peak time
            climb = 0;              // set climb status to false

        }
    }
#if OUTPUT_TO_USB_SERIAL
    // output filtered sensor data
    itoa(data[idx], msg);
    putsUSBSerial(msg);
    putrsUSBSerial("\r\n");
#endif
    // increment index
    idx=(idx+1)%NHIST;
}

void main(void) {
    ulong t;
    byte i;
    initSquareWear();
    setModePWM(pinC7);      // set pinC7 as PWM output
    setModeADC(pinADC4);    // set pinADC4 as analog read
#if OUTPUT_TO_USB_SERIAL
    openUSBSerial();        // open USB serial port
#endif
    // use timer interrupt to sample pulse sensor data
    // open timer interrupt, 20ms interval (50Hz frequency)
    openTimerInterrupt(20, sample);
    t=t_prev_peak;
    while(1) {
        // inner loop blinks LED
        if(t!=t_prev_peak) {
            // if a new peak has been detected
            t=t_prev_peak;
            // quickly fade LED (pinC7)
            for(i=0;i<32;i++) {
                setValue(pinC7, i);
#if OUTPUT_TO_USB_SERIAL
                // need to call pollUSBSerial as often as we can
                pollUSBSerial();
#endif
                delayMilliseconds(1);
            }
            for(i=0;i<32;i++) {
                setValue(pinC7, 31-i);
#if OUTPUT_TO_USB_SERIAL
                // need to call pollUSBSerial as often as we can
                pollUSBSerial();
#endif   
                delayMilliseconds(1);
            }
        }
#if OUTPUT_TO_USB_SERIAL
        pollUSBSerial();
#endif
    }
}
#endif // DEMO_PULSE_SENSOR



// ====== Temperature Sensor Demo ======
// This demo reads temperature value from a MCP9701 temperature sensor,
// and use the sensor value to control the brightness of three LEDs:
// blue, green (indicates , and red.
// Blue corresponds to cold temperature, red corresponds to hot temperature,
// and green corresponds to middle temperature.
// The lower the temperature, the brighter the blue LED, and so on.
// Assumes temperature sensor data pin is connected to pinADC11,
// the three LEDs are common SOURCE (Vdd), and the negative leads are
// connected to pins C6 (blue), C5 (green), and C4 (red).
#ifdef DEMO_TEMPERATURE_SENSOR
#include <math.h>
void main(void) {
    float temp;
    float duty;
    initSquareWear();
    setModePWM(pinC4);
    setModePWM(pinC5);
    setModePWM(pinC6);
    setModeADC(pinADC11);

    while(1) {
        // read temperature sensor value
        temp = (float)getValue(pinADC11);
        // converts to Celcius
        temp = (temp * 4.1 / 1024 - 0.5) / 0.01;

        // blue LED brightness
        // Gaussian centered at 10 degree Celcius
        duty = 31.0 * exp(-(temp-10)*(temp-10)/100.0) * 0.25;
        setValue(pinC6, (byte)duty);

        // red LED brightness
        // Gaussian centered at 50 degree Celcius
        duty = 31.0 * exp(-(temp-50.0)*(temp-50.0)/100.0);
        setValue(pinC4, (byte)duty);

        // green LED brightness
        // Gaussian centered at 27 degree Celcius
        duty = 31.0 * exp(-(temp-27.0)*(temp-27.0)/50.0);
        setValue(pinC5, (byte)duty);
        delayMilliseconds(100);

    }
}
#endif  // DEMO_TEMPERATURE_SENSOR


// ====== LED Multi Pattern Demo ======
// This demo allows you to use the on-board push-button
// to change the LED flashing/fading pattern
// There are a total of 6 patterns, including
// 3 blinking patterns at different frequencies
// and 3 fading patterns at different frequencies
#ifdef DEMO_LED_MULTI_PATTERN
byte pinnames[] = {pinC0, pinC1, pinC2, pinC3,
                   pinC4, pinC5, pinC6, pinC7,
                   pinB4, pinB5, pinB6, pinB7};
volatile byte change = 0;
void initBlink(void) {
    byte i;
    for(i=0;i<12;i++) {
        setModeOutput(pinnames[i]);
        setValue(pinnames[i], 0);
    }
}

void Blink(int delayms) {
    byte i;
    for(i=0;i<12;i++) {
        setValue(pinnames[i], 1-getValue(pinnames[i])); // toggle
        
    }
    
    delayMilliseconds(delayms);
}

void initFade(void) {
    byte i;
    for(i=0;i<12;i++) {
        setModePWM(pinnames[i]);
    }
}

void Fade(int delayms) {
    byte i;
    byte v;
    for(v=0;v<32;v++) { // increase PWM values
        for(i=0;i<12;i++) {
            setValue(pinnames[i], v);
        }
        delayMilliseconds(delayms);
    }
    for(v=32;v!=0;v--) { // decrease PWM values
        for(i=0;i<12;i++) {
            setValue(pinnames[i], v-1);
        }
        delayMilliseconds(delayms);
    }
}

void button_callback(void) {
    if (buttonPressed()) {
        delayMilliseconds(50);   // wait 50ms for debounce
        if(buttonPressed())
        {
            change=1;
        }
    }
}

void main(void) {
    byte pattern=0;
    initSquareWear();
    initBlink();
    // open button interrupt
    openOnBoardButtonInterrupt(button_callback);
    while(1) {
        switch(pattern) {
            case 0:
                Blink(500); // blink at 500ms interval
                break;
            case 1:
                Blink(250); // blink at 250ms interval
                break;
            case 2:
                Blink(1000); // blink at 100ms interval
                break;
            case 3:
                Fade(15);   // fade at 50ms interval
                break;
            case 4:
                Fade(50);    // fade at 1ms interval
                break;
            case 5:
                Fade(4);    // fade at 2ms interval
                break;
        }
        if(change) {
            pattern=(pattern+1)%6;  // change pattern
            // call corresponding initialization function
            if(pattern==0 || pattern==1 || pattern==2)  {
                initBlink();
            } else {
                initFade();
            }
            change=0;
        }

    }

}
#endif // DEMO_LED_MULTI_PATTERN
