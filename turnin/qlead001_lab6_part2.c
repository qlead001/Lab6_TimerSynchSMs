/*	Author: Quinn Leader qlead001@ucr.edu
 *  Partner(s) Name: NA
 *	Lab Section: 026
 *	Assignment: Lab 6  Exercise 2
 *	Exercise Description: Blink three LEDs in sequence and stop when button pressed
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
    One,
    Two,
    Three,
    Stop,
    WaitButton,
    WaitButton2,
} state;

unsigned char count = 0;

void Tick() {
    switch (state) {
        case Start:
            state = One;
            break;
        case One:
            if ((~PINA)&0x01) state = Stop;
            else if (count < 30) count++;
            else {
                state = Two;
                count = 0;
            }
            break;
        case Two:
            if ((~PINA)&0x01) state = Stop;
            else if (count < 30) count++;
            else {
                state = Three;
                count = 0;
            }
            break;
        case Three:
            if ((~PINA)&0x01) state = Stop;
            else if (count < 30) count++;
            else {
                state = One;
                count = 0;
            }
            break;
        case Stop:
            if (!((~PINA)&0x01)) state = WaitButton;
            count = 0;
            break;
        case WaitButton:
            if ((~PINA)&0x01) state = WaitButton2;
            break;
        case WaitButton2:
            if (!((~PINA)&0x01)) state = One;
            break;
        default:
            state = Start;
            break;
    }
    switch (state) {
        case Start:
            break;
        case One:
            PORTB = 1;
            break;
        case Two:
            PORTB = 2;
            break;
        case Three:
            PORTB = 4;
            break;
        default:
            break;
    }
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
    TimerSet(1);
    TimerOn();
    state = Start;
    while (1) {
        Tick();
        while (!TimerFlag);
        TimerFlag = 0;
    }
    return 1;
}
