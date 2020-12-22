#include <avr/io.h>

int main(void) {

	// Set up clocking (default set to 8MHz internal oscillator)
	// Enable change of prescale register
	CLKPR = (1 << CLKPCE); 

	// Prescale register default set to divide clock frequency by 8 (need it to be 1)
	// Set prescale to divide by 1 (Timer running at 8MHz)
	CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);

	// Set up output pins (all pins input by default)
	// Clear PA3's output signal value before making it an output pin (don't want signal sent when it is set to output)
	PORTA &= ~(1 << PA3);

	// Make PA3 an output
	DDRA |= (1 << PA3);

	// Clear PA2 output signal value
	PORTA &= ~(1 << PA2);

	// Make PA2 an output
	DDRA |= (1 << PA2);

	while(1) {

		// PB0 turns on PA2
		// PB1 turns on PA3
		
		// While PB0 is receiving AND PA3 is off
		while ( ((PINB & (1 << PINB0)) == (1 << PINB0)) && (PORTA & (1 << PA3) == 0) ) {

			// Turn on PA2
			PORTA |= (1 << PA2);
		}

		// Turn off PA2
		PORTA &= ~(1 << PA2);

		// While PB1 is receiving AND PA2 is off
		while ( ((PINB & (1 << PINB1)) == (1 << PINB1)) && (PORTA & (1 << PA2) == 0) ) {

			// Turn off PA2
			PORTA &= ~(1 << PA2);

			// Turn on PA3
			PORTA |= (1 << PA3);
		}

		// Turn off PA3
		PORTA &= ~(1 << PA3);
	}
}