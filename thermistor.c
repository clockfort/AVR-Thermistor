/* Thermistor reader program for AVR chips
*  By Chris Lockfort (clockfort@csh.rit.removethispartforspam.edu)
* 
*  Basic Schematic:
*
*  GND --- 10K ohm normal resistor ---|--- 10K ohm thermistor ---- VCC
*  				      |
*  				Microcontroller ADC
*
* I haven't back-of-the-napkin'd the math, but I believe you might want to 
* use a lower-tolerance resistor than you would normally care about.
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
#include <math.h>
#include "thermistor.h"
#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))

float temperature = 0.0;

int main(void)
{
	CPU_PRESCALE(0); //16 MHz clock
	initialize_analog_inputs();
	while(1){
		take_temperature_reading();
	}
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

void take_temperature_reading(){
	float reading = read_analog_input();
	// I apologize for the following code, it's based off of an Arduino Playground example
	// It seems to mumble something about being a 10K-ohm specific version of a Steinhart-Hart Thermistor Equation :-)
	reading = log(((10240000/reading) - 10000)); 
	reading = 1 / (0.001129148 + (0.000234125 * reading) + (0.0000000876741 * reading * reading * reading));
	reading -= 273.15; //K to C
	reading = (reading * 9.0)/ 5.0 + 32.0; // C to F
	temperature = reading;
}
