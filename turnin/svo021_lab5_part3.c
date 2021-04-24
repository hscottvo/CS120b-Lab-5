/*	Author: lab
 *  Partner(s) Name: Scott Vo
 *	Lab Section:21
 *	Assignment: Lab 5  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 * Demo Link: https://www.youtube.com/watch?v=VobIbybx9vE
 * 
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum light_states {light_init, light_off, light_off_wait, light_shift_left, light_shift_left_wait, left_wait_two,
	light_shift_right, light_shift_right_wait, right_wait_two, light_flash, light_flash_wait, light_reset} light_state;
unsigned char tempA = 0;
unsigned char i = 0;
unsigned char counter = 2;

void light_tick() {
	switch(light_state) {
		case light_init:
			light_state = light_off;
			PORTB = 0x00;
			break;
		case light_off:
			PORTB = 0x00;
			if ((tempA & 0x01) == 0x00) {
				light_state = light_off;
			}
			else {
				light_state = light_off_wait;
				PORTB = 0x01;
			}
			break;
		case light_off_wait:
			if ((tempA & 0x01) == 0x00) {
				light_state = light_shift_left;
			}
			else {
				light_state = light_off_wait;
				PORTB = 0x01;
			}
		case light_shift_left:
			if ((tempA & 0x01) == 0x00) {
				light_state = light_shift_left;
			} 
			else {
				light_state = light_shift_left_wait;
				PORTB = 0x01 << (2 * i);
				++i;
			}
			break;
		case light_shift_left_wait:
			if ((tempA & 0x01) == 0x00) {
				if (i <= counter) light_state = light_shift_left;
				else {
					light_state = left_wait_two;
					i = 0;
				}
			}
			else {
				light_state = light_shift_left_wait;
			}
			break;
		case left_wait_two: 
			if ((tempA & 0x01) == 0x00) {
				light_state = left_wait_two;
			}
			else {
				light_state = light_shift_right;
				PORTB = 0x20;
			}
			break;
		case light_shift_right:
			if ((tempA & 0x01) == 0x00) {
				light_state = light_shift_right;
			} 
			else {
				light_state = light_shift_right_wait;
				PORTB = 0x20 >> (2 * i);
				++i;
			}
			break;
		case light_shift_right_wait: 
			if ((tempA & 0x01) == 0x00) {
				if (i <= counter) light_state = light_shift_right;
				else {
					light_state = right_wait_two;
					i = 0;
				}
			}
			else { 
				light_state = light_shift_right_wait;
			}
			break;
		case right_wait_two:
			if ((tempA & 0x01) == 0x00) {
				light_state = right_wait_two;
			}
			else {
				light_state = light_flash;
				PORTB = 0x3F;
			}
			break;
		case light_flash: 
			if ((tempA & 0x01) == 0x00) {
				light_state = light_flash;
			}
			else {
				light_state = light_flash_wait;
				PORTB = (~PORTB) & 0x3F;
				++i;
			}
			break;
		case light_flash_wait:
			if ((tempA & 0x01) == 0x00) {
				if (i <= counter + 1) light_state = light_flash;
				else {
					light_state = light_reset;
					i = 0;
					PORTB = 0x00;
				}
			}
			else {
				light_state = light_flash_wait;
			}
			break;
		case light_reset: 
			if ((tempA & 0x01) == 0x00) {
				light_state = light_reset;
				PORTB = 0x3F;
			}
			else {
				light_state = light_off;
				i = 0;
			}
			break;
		default: 
			light_state = light_init;
			PORTB = 0x00;
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
	light_state = light_init;
    while (1) {
		tempA = ~PINA;
		light_tick();
    }
    return 1;
}
