// Description: This file implements functions that utilize the timers
//----------------------------------------------------------------------//

#include "timer.h"

/* Initialize timer 0, you should not turn the timer on here.
* You will need to use CTC mode */
void initTimer0(){
    // set mode CTC (010)

    TCCR0A &= ~(1 << WGM00);
    TCCR0A |= (1 << WGM01);
    TCCR0B &= ~(1 << WGM02);

    // set prescaler (this set to 1 or 001)
    TCCR0B|=(1 << CS00);
    TCCR0B&=~(1 << CS01);
    TCCR0B&=~(1 << CS02);

    // output compare reg
    OCR0A=15; // 1 us 
    //OCR0B=250;
    
    //TIMSK0 |= (1 << OCIE0B) // interrupt flag in ctc mode
}

// this function provides a delay in milliseconds
void delayMs(unsigned int delay){
    unsigned int count = 0;

    while (count < delay){
        delayUs(1000); // delay for 1 ms
        count++;
    } 
}

void delayUs(unsigned int delay){
    unsigned int count = 0;

    // start at 0 for CTC mode
    TCNT0 = 0;
    TIFR0 |= (1 << OCF0A); // reset flag   

    while (count < delay){
        while (!(TIFR0 & (1 << OCF0A)));
        count++;
        TIFR0 |= (1 << OCF0A); // reset flag
    }
}
