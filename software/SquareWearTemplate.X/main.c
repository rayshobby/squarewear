#include "SquareWear.h"

void main(void) {
    initSquareWear();       // must call initialization function first
    setModeOutput(pinC7);   // set pinC7 as digital OUTPUT pin
    while(1) {
        latC7 = !latC7 ;    // toggle pin C7 value
                            // equivalent to but faster than
                            // setValue(pinC7, 1-getValue(pinC7))
        delayMilliseconds(500);
    }
}
