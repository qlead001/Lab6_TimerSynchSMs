/*	Author: Quinn Leader qlead001@ucr.edu
 *  Partner(s) Name: NA
 *	Lab Section: 026
 *	Assignment: Lab 6  Exercise 3
 *	Exercise Description: Lab 5 Exercise 2 debounced and repeat button press every second
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0; // TimerISR sets to 1, user code sets to 0

unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1 ms ticks

void TimerOn() {
    // See https://docs.google.com/document/d/151t_PpK_cezrEfeSML2oEYGC0BedWDxp3f0Avicnptc/
    TCCR1B = 0x0B;
    OCR1A = 125;
    TIMSK1 = 0x02;
    TCNT1 = 0;
    _avr_timer_cntcurr = _avr_timer_M;
    SREG |= 0x80;
}

void TimerOff() {
    TCCR1B = 0x00;
}

void TimerISR () {
    TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
    _avr_timer_cntcurr--;
    if (_avr_timer_cntcurr == 0) {
        TimerISR();
        _avr_timer_cntcurr = _avr_timer_M;
    }
}

void TimerSet(unsigned long M) {
    _avr_timer_M = M;
    _avr_timer_cntcurr = _avr_timer_M;
}

enum States {
    Start,
    Press,
    Release,
} state;

unsigned char count, held;

void Tick() {
    switch(state) { // Transitions
        case Start:
            state = Press;
            count = 7;
            break;
        case Press:
            held = 0;
            if ((~PINA)&0x03) {
                state = Release;
                if (((~PINA)&0x03) == 0x03) count = 0;
                else if (((~PINA)&0x03) == 0x01 && count < 9) count++;
                else if (((~PINA)&0x03) == 0x02 && count > 0) count--;
            }
            break;
        case Release:
            if (((~PINA)&0x03) == 0x03) count = 0;
            if (!((~PINA)&0x03)) state = Press;
            else if (held >= 10) {
                held = 0; 
                if (((~PINA)&0x03) == 0x03) count = 0;
                else if (((~PINA)&0x03) == 0x01 && count < 9) count++;
                else if (((~PINA)&0x03) == 0x02 && count > 0) count--;
            } else held++;
            break;
        default:
            state = Start;
            break;
    } // Transitions

    switch(state) { // State Actions
        case Start:
            break;
        default:
            break;
    } // State Actions

    PORTB = count;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
    TimerSet(10);
    TimerOn();
    state = Start;
    while (1) {
        Tick();
        while (!TimerFlag);
        TimerFlag = 0;
    }
    return 1;
}
