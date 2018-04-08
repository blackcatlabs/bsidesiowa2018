/* 
 * This file is part of the B-Sides Iowa 2018 badge 
 * <https://github.com/blackcatlabs/bsidesiowa2018>
 * Copyright (c) 2018 B-Sides Iowa.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */


#include <avr/sleep.h>
#include <avr/io.h>
#define nop() asm volatile("nop")

// ==================================================================================
//                ATtiny85
//              +--------+
//            --+ o  Vcc +------------
//  LED - PB3 --+        +-- PB2 - LED
//  LED - PB4 --+        +-- PB1 - RST
//  ------------+ GND    +-- PB0 - LED
//              +--------+
//
// If you are new to AVR microcontrollers, the following cheat sheet may be helpful:
//  PORTB :  Port B Data Register
//  PBn   :  Port B, Pin N
//  DDRB  :  Data direction register 
//
// For additional information, please refer to the ATtiny85 datasheet: 
// <https://www.mouser.com/ds/2/268/Atmel-2586-AVR-8-bit-Microcontroller-ATtiny25-ATti-1066528.pdf>
//
// Note: This software has been developed with novice readability in mind. Experts are 
//       encouraged to make modifications to preserve code space.  Familiarizing yourself  
//       with the PortB registers and their corresponding register bit effects can serve  
//       to conserve overall code space.    
// ==================================================================================

// Define the I/O ports to be used for the LEDs
#define LED_PIN0 PB0
#define LED_PIN1 PB2
#define LED_PIN2 PB3
#define LED_PIN3 PB4
#define RESET    PB1

boolean leds_on = true;

// Interrupt service routine
ISR(INT0_vect) {
   sleep_disable();
   leds_on = true;
}

// Setup runs once; this initializes the badge
void setup() {
   // Set the reset port pin as an input
   // All other pins will act as LED output
   digitalWrite(RESET, HIGH);
   pinMode(PB1, INPUT_PULLUP);
   
   pinMode(LED_PIN0, OUTPUT);
   pinMode(LED_PIN1, OUTPUT);
   pinMode(LED_PIN2, OUTPUT);
   pinMode(LED_PIN3, OUTPUT);

   // To conserve battery power, set to sleep mode
   sleep();
}

// This is the main routine of the program
void loop() {

   if (leds_on)
   {
      // Disable global interrupts
      cli();
      leds_on = false;
   }

   // Check for reset
   // If reset is active, do something special to sanity check
   if (!(digitalRead(RESET)))
   {
      charlieplexpin(5);
      delay(10);
      leds_off();
      delay(10);
      charlieplexpin(11);
      delay(200);
      sleep();
   }
   else
   {
      animate_leds();
   }
}

// Set the microcontroller into the sleep state 
// System wakes up when watchdog times out
void sleep()
{
   leds_off();

   // Disable the watchdog by performing the following steps:
   // Clear watchdog reset flag in the MCU status register 
   MCUSR &= ~(1 << WDRF);
   // Enable the watchdog timer
   WDTCR |= (1 << WDCE) | (1 << WDE);
   // Turn off the watchdog timer
   WDTCR = 0x00;

   // Turn off the ADC to save further power
   ADCSRA &= ~(1 << ADEN); 

   // Enable external interrupts on pin change
   GIMSK |= (1 << INT0);
   GIMSK |= (1 << PCIE);

   // Set sleep mode to conserve power
   set_sleep_mode(SLEEP_MODE_PWR_DOWN);
   sleep_enable();
   // Enable system interrupts
   sei(); 
   // System actually sleeps here
   sleep_cpu();
   
   // System continues here on interrupt
   sleep_disable();
}

void animate_leds()
{
   // Light clockwise
   for (int i = 0; i < 12; i++) {
      charlieloop(i);
   }

   // Light counterclockwise
   for (int i = 11; i >= 0; i--) {
      charlieloop(i);
   }

   // Blink top last time
   charlieloop(11);
}

// The ATtiny has relatively few I/O pins, so we drive the LEDs using a
// technique knoqn as charlieplexing.  The method uses tri-state logic
// For more info: <https://en.wikipedia.org/wiki/Charlieplexing>
void charlieloop(int pin)
{
   for (int j=0; j<4096; j++)
   {
      // This should ideally be cleaned up with better timing, but
      // it works for the moment...  Credits to darko for the PWM 
      charlieplexpin(pin);
      nop();
      nop();
      nop();
      nop();
      leds_off();
      nop();
      nop();
      nop();
      nop();
      nop();
      nop();
      nop();
      nop();
      nop();
      nop();
   }
}

void leds_off()
{
   DDRB &= ~((1 << LED_PIN0) | (1 << LED_PIN1) | (1 << LED_PIN2) | (1 << LED_PIN3));
   PORTB &= ~((1 << LED_PIN0) | (1 << LED_PIN1) | (1 << LED_PIN2) | (1 << LED_PIN3));
}

void  charlieplexpin(int id) 
{
   leds_off();

   switch (id) {
     case 0: 
      DDRB |= (1 << LED_PIN0) | (1 << LED_PIN1); 
      PORTB |= (1 << LED_PIN0);  // HIGH
      // Equivalent to digitalWrite(LED_PIN0, HIGH) 
      // We alter the registers directly for the sake of speed 
      // and a smoother animation
      PORTB &= ~(1 << LED_PIN1); // LOW
      break;
    case 1: 
      DDRB |= (1 << LED_PIN0) | (1 << LED_PIN1); 
      PORTB |= (1 << LED_PIN1);  // HIGH
      PORTB &= ~(1 << LED_PIN0); // LOW
      break;
    case 2: 
      DDRB |= (1 << LED_PIN1) | (1 << LED_PIN2); 
      PORTB |= (1 << LED_PIN1);  // HIGH
      PORTB &= ~(1 << LED_PIN2); // LOW
      break;
    case 3: 
      DDRB |= (1 << LED_PIN1) | (1 << LED_PIN2); 
      PORTB |= (1 << LED_PIN2);  // HIGH
      PORTB &= ~(1 << LED_PIN1); // LOW
      break;
    case 4: 
      DDRB |= (1 << LED_PIN2) | (1 << PB4); 
      PORTB |= (1 << LED_PIN2);  // HIGH
      PORTB &= ~(1 << PB4);      // LOW
      break;
    case 5: 
      DDRB |= (1 << LED_PIN2) | (1 << PB4); 
      PORTB |= (1 << PB4);       // HIGH
      PORTB &= ~(1 << LED_PIN2); // LOW
      break;
    case 6: 
      DDRB |= (1 << LED_PIN0) | (1 << LED_PIN2); 
      PORTB |= (1 << LED_PIN0);  // HIGH
      PORTB &= ~(1 << LED_PIN2); // LOW
      break;
    case 7: 
      DDRB |= (1 << LED_PIN0) | (1 << LED_PIN2); 
      PORTB |= (1 << LED_PIN2);  // HIGH
      PORTB &= ~(1 << LED_PIN0); // LOW
      break;
    case 8: 
      DDRB |= (1 << LED_PIN1) | (1 << LED_PIN3); 
      PORTB |= (1 << LED_PIN1);  // HIGH
      PORTB &= ~(1 << LED_PIN3); // LOW
      break;
    case 9: 
      DDRB |= (1 << LED_PIN1) | (1 << LED_PIN3); 
      PORTB |= (1 << LED_PIN3);  // HIGH
      PORTB &= ~(1 << LED_PIN1); // LOW
      break;
    case 10: 
      DDRB |= (1 << LED_PIN0) | (1 << LED_PIN3); 
      PORTB |= (1 << LED_PIN0);  // HIGH
      PORTB &= ~(1 << LED_PIN3); // LOW
      break;
    case 11: 
      DDRB |= (1 << LED_PIN0) | (1 << LED_PIN3); 
      PORTB |= (1 << LED_PIN3);  // HIGH
      PORTB &= ~(1 << LED_PIN0); // LOW
      break;
   }
}
