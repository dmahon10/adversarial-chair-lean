#include <avr/io.h>
#include <util/delay.h>

// DDR = in or out
// PORT = high or low for output
// PIN = is pin recieving or not (for conditionals)

#define output(directions,pin) (directions |= pin) // set port direction for output
#define set(port,pin) (port |= pin) // set port pin
#define clear(port,pin) (port &= (~pin)) // clear port pin (~ complement - flips bits)
#define pin_test(pins,pin) (pins & pin) // test for port pin
#define bit_test(byte,bit) (byte & (1 << bit)) // test for bit set

#define on_delay() _delay_us(3) // PWM on time
#define fast_off_delay() _delay_us(1) // PWM fast off time
#define medium_off_delay() _delay_us(3) // PWM medium off time
#define slow_off_delay() _delay_us(5) // PWM slow off time
#define PWM_count 20000 // number of PWM cycles
#define cycle_count 5 // number of speed cycles

#define bridge_port PORTA // H-bridge port
#define bridge_direction DDRA // H-bridge direction
#define IN1 (1 << PA3) // IN1 (setting as input pin)
#define IN2 (1 << PA2) // IN2 (setting as input pin)

int main(void) {
   //
   // main
   //
   static uint16_t count;
   static uint8_t cycle;
   //
   // set clock divider to /1
   // 

   // Set internal RC 8MHz clock for  (this comes as default)


   // CKSEL is shipped set to CKSEL 0..3 = 0010, this is the internal 8MHz RC oscillator
   // CLKPR is shipped set to 
   //
   // CKSEL |= 00000010
   //
   CLKPR = (1 << CLKPCE); // Change this bit of the Clock prescale register to 1 to enable changes to first 4 bits
                        // first four (0-3) bits comprise the division factor

   // CLock Prescale Register (8 bits)
   // CLocK Prescaler Change Enable (7th bit)
   // bits 4-6 are always 0
   // CLocK Prescaler Select (first 4 bits = lower nibble)

   CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);
   //
   // initialize H-bridge pins
   //

   // DDR = in or out
   // PORT = high or low for output
   // PIN = is pin recieving or not (for conditionals)


   clear(bridge_port, IN1);   // Make the pin not SEND anything 
                              // PORTA == 00000000 &= ~00001000 => 00000000 &= 11110111 => 00000000
                            // PORTA == 01100000 &= ~00001000 => 01100000 &= 11110111 => 01100000 (now sending out on those pins, problem?)
                           // PORTA == 01100000 &= ~01101000 => 01100000 &= 10010111 => 01100000
                           // This is for making the PIN on the port not send a HIGH! Invert the pin first so you preserve what the PORT was set to and change only the pin)
   output(bridge_direction, IN1);// Make the pin an output 
                                 // DDRA == 00000000 |= 00001000 => 000010000 (set PA3 as an output pin)
                                 
                                 // make sure clear first, so that when it becomes output, it is not sending anything//

   
   //Repeat for IN2 (PA2)
   clear(bridge_port, IN2);
   output(bridge_direction, IN2);
   //
   // main loop
   //
   while (1) {
      for (cycle = 0; cycle < cycle_count; ++cycle) { // 5 times
         //
         // turn forward slow
         //
         clear(bridge_port, IN1); // Make sure PA3 not sending to IN1
         set(bridge_port, IN2); // Make PA2 send to IN2

         for (count = 0; count < PWM_count; ++count) { // Do 20000 times... why??
            set(bridge_port, IN2); // Make PA2 send to IN2 (motor on)
            on_delay(); // wait for 3 microseconds
            clear(bridge_port, IN2); // Turn off PA2
            slow_off_delay(); // for 5 microseconds
            }
         //
         // turn reverse slow
         //
         clear(bridge_port, IN2); //Turn off PA2
         set(bridge_port, IN1); // Turn on  PA3

         for (count = 0; count < PWM_count; ++count) { //200000 times
            set(bridge_port, IN1); // PA3 ON
            on_delay(); // 3 microsec
            clear(bridge_port, IN1); // PA3 Off
            slow_off_delay(); // 5 micro secs
            }
         }
      for (cycle = 0; cycle < cycle_count; ++cycle) {
         //
         // turn forward medium
         //
         clear(bridge_port, IN1); // PA3 off
         set(bridge_port, IN2); // PA2 on
         for (count = 0; count < PWM_count; ++count) {
            set(bridge_port, IN2); // PA2 on
            on_delay(); // 3 micro
            clear(bridge_port, IN2); // PA2 off
            medium_off_delay(); // for 3 micro secs
            }
         //
         // turn reverse medium
         //
         clear(bridge_port, IN2); // PA2 off
         set(bridge_port, IN1); // PA3 on
         for (count = 0; count < PWM_count; ++count) { //20000
            set(bridge_port, IN1); // PA2 on
            on_delay(); // 3 us
            clear(bridge_port, IN1); // PA2 off
            medium_off_delay(); // 3 us
            }
         }

         // Why turn on before loop and the on in loop?? Why turn at end of loop and off again before next loop?

      for (cycle = 0; cycle < cycle_count; ++cycle) {
         //
         // turn forward fast
         //
         clear(bridge_port, IN1);
         set(bridge_port, IN2);
         for (count = 0; count < PWM_count; ++count) {
            set(bridge_port, IN2);
            on_delay();
            clear(bridge_port, IN2);
            fast_off_delay();
            }
         //
         // turn reverse fast
         //
         clear(bridge_port, IN2);
         set(bridge_port, IN1);
         for (count = 0; count < PWM_count; ++count) {
            set(bridge_port, IN1);
            on_delay();
            clear(bridge_port, IN1);
            fast_off_delay();
            }
         }
      }
   }