/* Thermistor reader program for AVR chips
*  By Chris Lockfort (clockfort@csh.rit.removethispartforspam.edu)
* 
*  Basic Schematic:
*
*  GND --- 10K ohm normal resistor ---|--- 10K ohm thermistor ---- VCC
*  				      |
*  				Microcontroller ADC
*
* Portions of this code are borrowed from Paul J Stoffregen's sample ADC code,
* many thanks to him for providing readable examples.
*/

/*
* Copyright (c) 2010  Chris Lockfort
*
* Permission to use, copy, modify, and distribute this software for any
* purpose with or without fee is hereby granted, provided that the above
* copyright notice and this permission notice appear in all copies.
*
* THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
* WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
* ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
* WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
* ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include "thermistor.h"

#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))

int main(void)
{
	CPU_PRESCALE(0); //16 MHz clock
	initialize_analog_inputs();
	read_analog_input();
	
}

void initialize_analog_inputs(){
        ADCSRA = (1<<ADEN) | ADC_PRESCALER; //enable ADC
        ADCSRB = (1<<ADHSM); // high speed mode
        ADMUX = aref;
}

int16_t read_analog_input(){
        ADCSRA = (1<<ADEN) | ADC_PRESCALER | (1<<ADSC);
        while (ADCSRA & (1<<ADSC)) ; //block until ready
        uint8_t lower_byte = ADCL;
        int16_t ret_val = ADCH << 8;
        ret_val |= lower_byte;
        return ret_val;
}
