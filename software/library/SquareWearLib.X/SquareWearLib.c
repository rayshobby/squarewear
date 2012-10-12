#include "SquareWear.h"
#include "usb_config.h"

// --------------------------------
// ------ Internal Functions ------
// --------------------------------
// Startup, high and low priority interrupt isrs
// Defined according to Microchip's USB HID bootloader
void _startup(void);
void _sqw_high_isr(void);   // interrupt service routine (high priority)
void _sqw_low_isr (void);   // interrupt service routine (low priority)

// Due to bootloader, startup function is moved to address 0x1000
#pragma code REMAPPED_RESET_VECTOR = 0x1000
void _reset(void) {
    _asm goto _startup _endasm
}
#pragma code

#pragma code REMAPPED_HIGH_INTERRUPT_VECTOR = 0x1008
void Remapped_High_ISR (void)
{
     _asm goto _sqw_high_isr _endasm
}
#pragma code

#pragma code REMAPPED_LOW_INTERRUPT_VECTOR = 0x1018
void Remapped_Low_ISR (void)
{
     _asm goto _sqw_low_isr _endasm
}
#pragma code

// ------------------------------
// ------ Global Variables ------
// ------------------------------

// Port C PWM values and bits
// PWM value is within range [0,31] (5-bit precision)
// PWM bit indicates if a pin has PWM output enabled.
static byte PortCPWMvals[8] = {0,0,0,0,0,0,0,0};
static byte PortCPWMbits = 0x00;

// Timer 2 overflow interrupt counter
static volatile byte ntmr2ofint = 0;

// number of milliseconds passed since the start of program
static volatile ulong milliseconds = 0;

// software reset enable
static byte sr_en = 1;
// software reset tracking time
static volatile ulong sr_prev_time = 0;

// timer interrupt interval (in milliseconds)
static ulong ti_interval = 1000;

// pointers to interrupt service routines
static isr_handler isrTimer=0;
static isr_handler isrButton=0;
static isr_handler isrINT1=0;
static isr_handler isrINT2=0;

// High priority isr
#pragma interrupt _sqw_high_isr
void _sqw_high_isr(void) {

    // ------ Timer0 overflow interrupt ------
    // Timer0 is used for time keeping
    if (INTCONbits.TMR0IE && INTCONbits.TMR0IF) {
        INTCONbits.TMR0IF = 0;  // clear interrupt flag
        WriteTimer0(53717);   // write timer value
        
        // tmr0 overflow interrupts trigger at roughly 1000Hz
        // increment millisecond counter
        milliseconds ++;
    }
}

// Low priority isr
#pragma interruptlow _sqw_low_isr
void _sqw_low_isr(void) {

    // ------ Timer2 overflow interrupt ------
    // Timer2 is used for software PWM
    if (PIE1bits.TMR2IE && PIR1bits.TMR2IF) {
        PIR1bits.TMR2IF = 0;  // clear interrupt flag
        ntmr2ofint = (ntmr2ofint+1)%31;

        // if no PWM bits are set, return immediately
        if (!PortCPWMbits) return;

        // Check the PWM bits of each PORTC pin and do time sharing calculation
        if (PortCPWMbits&0x01)    LATCbits.LATC0 = (ntmr2ofint < PortCPWMvals[0]) ? 1 : 0;
        if (PortCPWMbits&0x02)    LATCbits.LATC1 = (ntmr2ofint < PortCPWMvals[1]) ? 1 : 0;
        if (PortCPWMbits&0x04)    LATCbits.LATC2 = (ntmr2ofint < PortCPWMvals[2]) ? 1 : 0;
        if (PortCPWMbits&0x08)    LATCbits.LATC3 = (ntmr2ofint < PortCPWMvals[3]) ? 1 : 0;
        if (PortCPWMbits&0x10)    LATCbits.LATC4 = (ntmr2ofint < PortCPWMvals[4]) ? 1 : 0;
        if (PortCPWMbits&0x20)    LATCbits.LATC5 = (ntmr2ofint < PortCPWMvals[5]) ? 1 : 0;
        if (PortCPWMbits&0x40)    LATCbits.LATC6 = (ntmr2ofint < PortCPWMvals[6]) ? 1 : 0;
        if (PortCPWMbits&0x80)    LATCbits.LATC7 = (ntmr2ofint < PortCPWMvals[7]) ? 1 : 0;

    }

    // ------ Timer3 overflow interrupt ------
    // Timer3 is used for custom timer interrupt and also tracking software reset
    if (PIE2bits.TMR3IE && PIR2bits.TMR3IF) {
        PIR2bits.TMR3IF = 0;    // clear interrupt flag
        WriteTimer3(53717);

        if (sr_en) {    // if software reset is enabled
            // if button is pressed for more than 3 seconds
            if (portA3 == 0) {
                if (milliseconds > sr_prev_time + 3000) {
                    Reset();
                }
            } else {
                sr_prev_time = milliseconds;
            }
        }

        // if custom timer isr is defined
        if (isrTimer) {
            // inverval match
            if ((milliseconds%ti_interval) == 0) {
                // call TimerInterrupt handler
                isrTimer();
            }
        }
    }

    // ------ INT1 interrupt ------
    // If external interrupt event happens on pinC1
    if (INTCON3bits.INT1IE && INTCON3bits.INT1IF) {
        INTCON3bits.INT1IF = 0;
        // call INT1 handler
        if(isrINT1) isrINT1();
    }

    // ------ INT2 interrupt ------
    // If external interrupt event happens on pinC2
    if (INTCON3bits.INT2IE && INTCON3bits.INT2IF) {
        INTCON3bits.INT2IF = 0;
        // call INT2 handler
        if(isrINT2) isrINT2();
    }

    // ------ PAB interrupt ------
    // If on-board button is pressed or released
    if (INTCONbits.RABIE && INTCONbits.RABIF) {
        INTCONbits.RABIF = 0;
        // call Button handler
        if(isrButton) isrButton();
        
    }
}

// ------------------------------------
// ------ Time Keeping Functions ------
// ------------------------------------
// Return milliseconds passed since the start of program
ulong millis() {
    return milliseconds;
}

void delayFor18TCY() {
    // 18 nops
    Nop();Nop();Nop();
    Nop();Nop();Nop();
    Nop();Nop();Nop();
    Nop();Nop();Nop();
    Nop();Nop();Nop();
    Nop();Nop();Nop();
}

void delayFor12TCY() {
    // 12 nops
    Nop();Nop();Nop();
    Nop();Nop();Nop();
    Nop();Nop();Nop();
    Nop();Nop();Nop();
}


// Delay specified amount of milliseconds
void delayMilliseconds(int ms) {
    while(ms!=0) {
        // delay 12,000 instructions
        // due to timer0 and timer2 interrupts
        // delay runs roughly 0.73 times slower
        // so the actual delay cycles are discounted
        Delay100TCYx(87);
        Delay10TCYx(4);
        ms --;
    }
}

// Delay specified amount of microseconds
void delayMicroseconds(int us) {
    while(us!=0) {
        delayFor12TCY();
        us --;
    }
}

// Delay specified amount of seconds
void delaySeconds(int sec) {
    while(sec!=0) {
        // delay 12,000,000 instructions
        // due to timer0 and timer2 interrupts
        // delay runs roughly 0.73 times slower
        // so the actual delay cycles are discounted
        Delay10KTCYx(0);    // delay 2,560,000 instructions
        Delay10KTCYx(0);
        Delay10KTCYx(0);
        Delay10KTCYx(106);
        sec --;
    }
}

void DelayPORXLCD() {
    delayMilliseconds(15);
}

void DelayXLCD() {
    delayMilliseconds(5);
}

// ---------------------------
// ------ Pin Functions ------
// ---------------------------
// Set a pin to digital OUTPUT mode
void setModeOutput(byte pinidx) {
    byte pidx = pinidx & 0x0f;
    if (pinidx & PORTC_BASE) {
        // set port c pin to output
        TRISC &= ~(1<<pidx);
        PortCPWMbits &= ~(1<<pidx); // disable PWM bits
    } else if (pinidx & PORTB_BASE) {
        // set port b pin to output
        TRISB &= ~(1<<pidx);
    }
    // RA3 is always input, so we do not handle PORTA here
    return;
}

// Set a pin to PWM OUTPUT mode
// Only apples to PORTC pins
void setModePWM(byte pinidx) {
    byte pidx = pinidx & 0x0f;
    // only port C pins can be set for software PWM
    if (pinidx & PORTC_BASE) {
        TRISC &= ~(1<<pidx);
        PortCPWMbits |= (1<<pidx);
        PortCPWMvals[pidx] = 0; // initial duty cycle 0
    }
    return;
}

// Set a pin to digital INPUT mode
void setModeInput(byte pinidx) {
    byte pidx = pinidx & 0x0f;
    if (pinidx & PORTC_BASE) {
        // set port c pin to output
        TRISC |= (1<<pidx);
    } else if (pinidx & PORTB_BASE) {
        // set port b pin to output
        TRISB |= (1<<pidx);
    }
    // handle ANSx bits
    // close analog input functions
    switch(pinidx) {
        case pinC0:
            ANSELbits.ANS4 = 0;
            break;
        case pinC1:
            ANSELbits.ANS5 = 0;
            break;
        case pinC2:
            ANSELbits.ANS6 = 0;
            break;
        case pinC3:
            ANSELbits.ANS7 = 0;
            break;
        case pinB4:
            ANSELHbits.ANS10 = 0;
            break;
        case pinB5:
            ANSELHbits.ANS11 = 0;
            break;
        default:
            break;
    }

    // RA3 is alway input, so no need to process PORTA
    return;
}

// Set a pin to analog INPUT (ADC) mode
// Analog input has 10-bit precision: [0,1023]
void setModeADC(byte pinidx) {

    if (pinidx & PORTADC_BASE) {
        // handle ANSx bits and TRIS bits
        switch(pinidx) {
            case pinADC4:
                ANSELbits.ANS4 = 1;
                TRISCbits.RC0 = 1;
                break;
            case pinADC5:
                ANSELbits.ANS5 = 1;
                TRISCbits.RC1 = 1;
                break;
            case pinADC6:
                ANSELbits.ANS6 = 1;
                TRISCbits.RC2 = 1;
                break;
            case pinADC7:
                ANSELbits.ANS7 = 1;
                TRISCbits.RC3 = 1;
                break;
            case pinADC10:
                ANSELHbits.ANS10 = 1;
                TRISBbits.RB4 = 1;
                break;
            case pinADC11:
                ANSELHbits.ANS11 = 1;
                TRISBbits.RB5 = 1;
                break;
            default:
                break;
        }

    }
}

// Set value to an OUTPUT pin (either digital or PWM OUTPUT)
// If the pin is digital OUTPUT, value should be 0 or 1
// If the pin is PWM OUTPUT, value should be betwee [0,31]
void setValue(byte pinidx, byte value) {
    byte pidx = pinidx & 0x0f;

    if (pinidx & PORTC_BASE) {
        if (PortCPWMbits & (1<<pidx)) {
            PortCPWMvals[pidx] = value&0x1f;
        } else {
            LATC = value ? (LATC | (1<<pidx)) : (LATC & ~(1<<pidx));
        }

    } else if (pinidx & PORTB_BASE) {
        LATB = value ? (LATB | (1<<pidx)) : (LATB & ~(1<<pidx));
    }
    // analog and RA3 pins cannot be set to a given value
    return;
}

// Read value from an INPUT pin (either a digital or analog INPUT pin)
// If the pin is digital INPUT, return value will be 0 or 1
// If the pin is analog INPUT, return value will be between [0,1023]
uint getValue(byte pinidx) {
    byte pidx = pinidx & 0x0f;
    if (pinidx & PORTC_BASE) {
        if (PortCPWMbits & (1<<pidx)) {
            return PortCPWMvals[pidx];
        } else {
            return (uint) ((PORTC>>pidx)&1);
        }
    } else if (pinidx & PORTB_BASE) {
        return (uint) ((PORTB>>pidx)&1);
    } else if (pinidx & PORTA_BASE) {
        return (uint) ((PORTA>>pidx)&1);
    } else {
        // handle adc
        SetChanADC(pidx<<3);    // according to the definition in ADCON0
        ConvertADC();
        while(BusyADC());
        return ReadADC();
    }
    return 0;
}

// Get the PWMBits of all PORTC pins
byte getPWMBits(void) {
    return PortCPWMbits;
}

// Enable internal pullup for a digital INPUT pin
// Only applies to PORTB pins
void enablePullup(byte pinidx) {
    byte pidx = pinidx & 0x0f;
    // only port b pins have internal pullup
    if (pinidx & PORTB_BASE) {
        WPUB |= (1<<pidx);
    }

}

// Diable internal pullup for a digital INPUT pin
// Only applies to PORTB pins
void disablePullup(byte pinidx) {
    byte pidx = pinidx & 0x0f;
    if (pinidx & PORTB_BASE) {
        WPUB &= ~(1<<pidx);
    }

}

// ----------------------------------
// ------ SquareWear Functions ------
// ----------------------------------
void initSquareWear() {

    // PORTC pins are initialized to digital OUTPUT by default
    TRISC = 0x00;

    // PORTB pins are initialized to digital INPUT by default
    TRISB = 0xff;
    
    // setup ADC
    // internal clock, conversion time 12 TAD
    // no interrupt, VDD-VSS voltage reference
    ADCON0 = 0b00001101;    // enabled ADC on init
    ADCON1 = 0x00;
    ADCON2 = 0b10101111;

    // Disable ADC bits
    ANSEL = 0x00;
    ANSELH = 0x00;
    
    // Set up Timer0 interrupt, used for time keeping
    // 8 bit counter, 1:1 pre-scalar, high priority
    // Timer reset value 53536, so overflow interrupt triggers at roughly
    // 12MHz / (65536-53536) = 1000Hz
    OpenTimer0(TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_1);
    INTCON2bits.TMR0IP = 1;
    WriteTimer0(53536);

    // Set up Timer2, used for software PWM
    // 8 bit counter, 1:8 post-scalar, low priority
    // TMR2 to PR2 match interrupt triggers at
    // 12MHz / 8 / 256 = 5.86kHz
    // PWM precision is 32 (5 bits), so PWM frequency is
    // 5860/32 = 183Hz
    OpenTimer2(TIMER_INT_ON & T2_PS_1_1 & T2_POST_1_8);
    IPR1bits.TMR2IP = 0;
    
    // Set up Timer3, used for tracking software reset and custom timer interrupt
    // 16-bit counter, 1:1 pre-scalar, low priority
    // Timer reset value 53536, so overflow interrupt triggers at
    // 12MHz / (65536 - 53536) = 1000Hz
    OpenTimer3(TIMER_INT_ON & T3_16BIT_RW & T3_SOURCE_INT
               & T3_PS_1_1 & T3_SYNC_EXT_OFF);
    IPR2bits.TMR3IP = 0;
    WriteTimer3(53536);

    // Timer1 is unused, disable it for now
    CloseTimer1();

    // enable interrupt priority
    RCONbits.IPEN = 1;
    // enable both low and high priority interrupts
    INTCONbits.GIEL = 1;
    INTCONbits.GIE = 1;

    // Disable PORTB internal pullup
    INTCON2bits.RABPU = 0;

}

// Return status of on-board push button: 1 -> pressed, 0 -> not pressed
bool buttonPressed() {
    if (portA3 == 0)
        return 1;
    else
        return 0;
}

// Set up custom Timer interrupt
// Parameters are interval (in milliseconds) and interrupt callback function
// For example, 500 interval means interrupt triggers at every 500 ms
void openTimerInterrupt(ulong ms, isr_handler callback) {
    isrTimer = callback;
    if (ms==0)  ms=1;
    ti_interval = ms;
}

void closeTimerInterrupt(void) {
    isrTimer = 0;
}

// Set up on-board button interrupt
// When the button is pressed or released, the callback function will be executed
void openOnBoardButtonInterrupt(isr_handler callback) {
    // Set up PORTAB pin change interrupt
    // Low priority, internal pullup on
    isrButton = callback;
    OpenPORTB(PORTB_CHANGE_INT_ON & PORTB_PULLUPS_ON);
    INTCON2bits.RABIP = 0;
    IOCA |= (1<<3);
}

// Close on-board button interrupt
void closeOnBoardButtonInterrupt(void) {
    isrButton = 0;
    IOCA &= ~(1<<3);
    ClosePORTB();
}

// Set up external interrupt
// When a rising or falling edge is detected on the interrupt pin, the callback
// function will be executed
void openExtInterrupt(byte index, byte mode, isr_handler callback) {
    switch(index) {
        case 1:
            isrINT1 = callback;
            OpenRB1INT(PORTB_CHANGE_INT_ON & mode & PORTB_INT_PRIO_LOW  & PORTB_PULLUPS_ON);
            setModeInput(pinC1);
            break;
        case 2:
            isrINT2 = callback;
            OpenRB2INT(PORTB_CHANGE_INT_ON & mode & PORTB_INT_PRIO_LOW  & PORTB_PULLUPS_ON);
            setModeInput(pinC2);
            break;
    }
}

// CLose external interrupt
void closeExtInterrupt(byte index) {
    switch(index) {
        case 1:
            isrINT1 = 0;
            CloseRB1INT();

            break;
        case 2:
            isrINT2 =0;
            CloseRB2INT();
            break;
    }
}

// Enable software reset
void enableSoftwareReset(void) {
    sr_en = 1;
    sr_prev_time = 0;
}

// Disable software reset
void disableSoftReset(void) {
    sr_en = 0;
}