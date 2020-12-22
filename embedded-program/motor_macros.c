#include <avr/io.h>

#define clear(port,pin) (port &= (~pin)) // Pin to 0
#define set(port,pin) (port |= pin) // Pin to 1
#define receiving(pinport,pin) ((pinport & pin) == pin) // Check if input pin receiving high signal
#define off(port,pin) ((port & pin) == 0) // Check if output pin not sending high signal
#define A2 (1 << PA2) 
#define A3 (1 << PA3) 
#define B0 (1 << PINB0)
#define B1 (1 << PINB1)

int main(void) {

	// Enable change of prescale register and set prescale to divide by 1 (makes timer run at 8MHz)
	CLKPR = (1 << CLKPCE); 
	CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);

	// Clear PA2 and PA3 output signals then set as output pins
	clear(PORTA, A2);
	set(DDRA, A2);
	clear(PORTA, A3);
	set(DDRA, A3);

	while(1) {

		// PB0 turns on PA2
		// PB1 turns on PA3
		
		// While PB0 is receiving AND PA3 is off
		while (receiving(PINB, B0) && off(PORTA, A3)) {

			// PA2 on
			set(PORTA, A2);
		}

		// PA2 off
		clear(PORTA, A2);

		// While PB1 is receiving AND PA2 is off
		while (receiving(PINB, B1) && off(PORTA, A2)) {

			// PA3 on
			set(PORTA, A3);
		}

		// PA3 off
		clear(PORTA, A3);
	}
}