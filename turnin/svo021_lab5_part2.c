/*	Author: lab
 *  Partner(s) Name: Scott Vo
 *	Lab Section:21
 *	Assignment: Lab 5  Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum counter_states {counter_init, counter_reset, counter_wait, counter_dec, counter_dec_wait, counter_inc, counter_inc_wait} counter_state;
unsigned char tempA = 0;

void counter_tick() {
	switch(counter_state) {
		case counter_init:
			counter_state = counter_wait;
			PORTC = 0x07;
			break;
		case counter_reset:
			if ((tempA & 0x03) == 0x03) {
				counter_state = counter_reset;
			}
			else {
				counter_state = counter_wait;
			}
		case counter_wait:
			if ((tempA & 0x03) == 0x00) {
				counter_state = counter_wait;
			} 
			else if ((tempA & 0x03) == 0x01){
				counter_state = counter_inc;
			} 
			else if ((tempA & 0x03) == 0x02) {
				counter_state = counter_dec;
			}
			else {
				counter_state = counter_reset;
			}
			break;
		case counter_dec:
			if ((tempA & 0x03) == 0x03) {
				counter_state = counter_reset;
			} 
			else {
				counter_state = counter_dec_wait;
			}
			break;
		case counter_dec_wait: 
			if ((tempA & 0x03) == 0x03) {
				counter_state = counter_reset;
			}
			else if ((tempA & 0x03) == 0x02) {
				counter_state = counter_dec_wait;
			}
			else {
				counter_state = counter_wait;
			}
			break;
		case counter_inc: 
			if ((tempA & 0x03) == 0x03) {
				counter_state = counter_reset;
			}
			else {
				counter_state = counter_inc_wait;
			}
			break;
		case counter_inc_wait: 
			if ((tempA & 0x03) == 0x03) {
				counter_state = counter_reset;
			}
			else if ((tempA & 0x03) == 0x01) {
				counter_state = counter_inc_wait;
			}
			else {
				counter_state = counter_wait;
			}
			break;
		default: 
			counter_state = counter_init;
			break;
	}
	switch(counter_state) {
		case counter_init: 
			PORTC = 0x07; 
			break;
		case counter_reset:
			PORTC = 0x00;
			break;
		case counter_wait:
			break;
		case counter_dec:
			if (PORTC > 0x00) {
				PORTC = PORTC - 1;
			}
			break;
		case counter_dec_wait: 
			break;
		case counter_inc: 
			if (PORTC < 0x09) {
				PORTC = PORTC + 1;
			}
			break;
		default: 
			// PORTC = 0x07;
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
    /* Insert your solution below */
    while (1) {
		tempA = ~PINA;
		counter_tick();
    }
    return 1;
}
