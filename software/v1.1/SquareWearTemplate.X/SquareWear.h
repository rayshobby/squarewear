#ifndef _SQW_H
#define _SQW_H

// Include files. Requires Microchip C18 compiler
// and peripheral library (comes with C18 installation)
#include <p18cxxx.h>
#include <adc.h>
#include <usart.h>
#include <delays.h>
#include <timers.h>
#include <portb.h>
#include <pwm.h>
#include <portb.h>
#include <i2c.h>
#include <EEP.h>

// -------------------------------
// ------ Frequency Defines ------
// -------------------------------
#define CPU_FREQ  48000000      // 48MHz CPU clock (12MHz crystal with 4xPLL)
#define BAUD_RATE 57600         // USART baud rate: 57.6kHz
#define SPBRG_LOWVAL (CPU_FREQ/16/BAUD_RATE - 1)    // USART SPBRG value

// --------------------------
// ------ Type Defines ------
// --------------------------
typedef unsigned char bool;
typedef unsigned char byte;
typedef unsigned int  uint;
typedef unsigned long ulong;
typedef void (*isr_handler)(void);

// -------------------------
// ------ Pin Defines ------
// -------------------------
#define PORTA_BASE      0x10
#define PORTB_BASE      0x20
#define PORTC_BASE      0x40
#define PORTADC_BASE    0x80

// Pin mode defines
/* OUTPUT/INPUT: can be set on any general I/O pins C0-C7 and B4-B7
 * PWM (output): can be set on any portC pins C0-C7
 * ADC (input) : can be set on any analog pins A4-A7, A10, and A11
 * */
// Pin mode functions
void setModeOutput(byte pinname);    // set as digital output
void setModePWM   (byte pinname);    // set as pwm output
void setModeInput (byte pinname);    // set as digital input
void setModeADC   (byte pinname);    // set as analog input

// Set pin value. If a pin is digital output, value should be 0 or 1.
// If a pin is PWM output, value can range from 0 to 31 (5-bit)
void setValue(byte pinname, byte value);

// Get pin value. If a pin is digital input, return is 0 or 1.
// If a pin is analog input, return is between 0 to 1023
uint getValue(byte pinname);

// Get PWM bits (portC)
// A bit value of 1 means the pin is set as PWM output
byte getPWMBits(void);

// Enable/disable internal pullup on input pins.
// Only available for portB pins:B4-B7
void enablePullup(byte pinname);
void disablePullup(byte pinname);

// Digital pin names
#define pinA3       (PORTA_BASE+3)
#define pinB4       (PORTB_BASE+4)
#define pinB5       (PORTB_BASE+5)
#define pinB6       (PORTB_BASE+6)
#define pinB7       (PORTB_BASE+7)
#define pinC0       (PORTC_BASE+0)
#define pinC1       (PORTC_BASE+1)
#define pinC2       (PORTC_BASE+2)
#define pinC3       (PORTC_BASE+3)
#define pinC4       (PORTC_BASE+4)
#define pinC5       (PORTC_BASE+5)
#define pinC6       (PORTC_BASE+6)
#define pinC7       (PORTC_BASE+7)

// Analog pin names
#define pinADC4     (PORTADC_BASE+4)  // shared with pinC0
#define pinADC5     (PORTADC_BASE+5)  // shared with pinC1
#define pinADC6     (PORTADC_BASE+6)  // shared with pinC2
#define pinADC7     (PORTADC_BASE+7)  // shared with pinC3
#define pinADC10    (PORTADC_BASE+10) // shared with pinB4
#define pinADC11    (PORTADC_BASE+11) // shared with pinB5

// Digital pin aliases
// Used for direct pin access
// Do not use these for read-modify-write operations. Use 'lat' aliases instead
#define portA3      PORTAbits.RA3        // RA3 is read only
#define portB4      PORTBbits.RB4
#define portB5      PORTBbits.RB5
#define portB6      PORTBbits.RB6
#define portB7      PORTBbits.RB7
#define portC0      PORTCbits.RC0
#define portC1      PORTCbits.RC1
#define portC2      PORTCbits.RC2
#define portC3      PORTCbits.RC3
#define portC4      PORTCbits.RC4
#define portC5      PORTCbits.RC5
#define portC6      PORTCbits.RC6
#define portC7      PORTCbits.RC7

// Digital pin aliases
// Used for direct read-modify-write access
#define latB4       LATBbits.LATB4
#define latB5       LATBbits.LATB5
#define latB6       LATBbits.LATB6
#define latB7       LATBbits.LATB7
#define latC0       LATCbits.LATC0
#define latC1       LATCbits.LATC1
#define latC2       LATCbits.LATC2
#define latC3       LATCbits.LATC3
#define latC4       LATCbits.LATC4
#define latC5       LATCbits.LATC5
#define latC6       LATCbits.LATC6
#define latC7       LATCbits.LATC7

// External interrupt mode: trigger on rising or falling edge
#define RISING      RISING_EDGE_INT
#define FALLING     FALLING_EDGE_INT

// ----------------------------------
// ------ USB Serial Functions ------
// ----------------------------------
// Serial input/output through SquareWear's built-in USB connector
void openUSBSerial(void);                       // must call this function first
void putnUSBSerial(char *string, byte len);     // write a string of size n
void putsUSBSerial(char *string);               // write a string
void putrsUSBSerial(const rom char *string);    // write a constant string
byte getUSBSerial(char *string, byte len);      // read a string

// SquareWear's USB serial is based on polling
// You must call this function as often as possible to perform USB tasks
void pollUSBSerial(void);

// -----------------------------
// ------ USART Functions ------
// -----------------------------
// If you have an external USB serial cable, you can use USART for
// serial tasks. This will save you program memory space compared to USB serial

// Open USART with interrupts off, 57600 baud rate
#define openUSART() {OpenUSART(USART_TX_INT_OFF & \\
                          USART_RX_INT_OFF & \\
                          USART_ASYNCH_MODE & \\
                          USART_EIGHT_BIT & \\
                          USART_CONT_RX & \\
                          USART_BRGH_HIGH, \\
                          SPBRG_LOWVAL); delayMilliseconds(100);}

// Close USART
#define closeUSART CloseUSART

// ------------------------------------
// ------ Hardware PWM Functions ------
// ------------------------------------
// Hardware PWM has higher precision compared to software PWM.
// The only supported hardware PWM pin is pinC5.

// Start Hardware PWM on C5 with 11.8kHz PWM frequency
#define openHardwarePWM()     {OpenPWM1(0xff);SetOutputPWM1(SINGLE_OUT, PWM_MODE_1);}

// Set PWM duty cycle, 10-bit precision (1023 max)
#define setHardwarePWMduty(duty)  SetDCPWM1(duty)

#define closeHardwarePWM    ClosePWM1


// -----------------------------
// ------ Delay Functions ------
// -----------------------------
void delayMilliseconds(int ms);     // delay specified amount of milliseconds
void delayMicroseconds(int us);     // delay specified amount of microseconds
void delaySeconds(int sec);         // delay specified amount of seconds
void delayFor18TCY();
void delayFor12TCY();

// ---------------------------------
// ------ Interrupt Functions ------
// ---------------------------------
// Custom timer interrupt.
// Parameters are trigger interval (in milliseconds) and callback function.
void openTimerInterrupt(ulong milliseconds, isr_handler callback);
void closeTimerInterrupt(void);

// External interrupts INT1 (pinC1) and INT2 (pinC2).
// Parameters are interrupt index (1 or 2), mode (RISING or FALLING),
// and callback function.
void openExtInterrupt(byte index, byte mode, isr_handler callback);
void closeExtInterrupt(byte index);

// On-board button interrupt.
// Triggers on button status change (press or release)
// Parameter is callback function.
void openOnBoardButtonInterrupt(isr_handler callback);
void closeOnBoardButtonInterrupt(void);

// Software reset (press on-board button more than 5 seconds to trigger reset
// and enter programming mode). Default on.
void openSoftwareReset(void);
void closeSoftwareReset(void);

// ------------------------------
// ------ EEPROM functions ------
// ------------------------------
// PIC18F14K50 has 256 bytes of internal EEPROM
#define writeEEPROM Write_b_eep
#define readEEPROM  Read_b_eep
#define busyEEPROM  Busy_eep

// ------------------------------
// ------ Sleep Functions -------
// ------------------------------
/* During sleep, CPU will only wake up on interrupts or reset
 * In idleSleep, CPU clock continues running, timing and timer interrupt work as usual
 * In deepSleep, CPU clock stops running, timing and timer interrupt stop working
 *   the only way to wake up CPU is through button interrupt or external interrupt.
 * */
#define idleSleep() {OSCCONbits.IDLEN = 1; Sleep(); }
#define deepSleep() {OSCCONbits.IDLEN = 0; Sleep(); }

// ----------------------------------
// ------ SquareWear Functions ------
// ----------------------------------
void initSquareWear(void);   // must call this function at the beginning of the program
bool buttonPressed(void);    // detect whether on-board button is pressed
ulong millis(void);          // return milliseconds since the start of the program

#endif // _SQW_H