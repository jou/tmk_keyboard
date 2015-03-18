#ifndef ATREUS_LED_H
#define ATREUS_LED_H 

#include <avr/io.h>
#include <util/delay.h>

// LEDs: B5 D7 B6

#define INDICATOR_LED_BRIGHTNESS 10

inline void led_1_on(void)  { DDRB |=  (1<<5); PORTB |=  (1<<5); }
inline void led_1_off(void) { DDRB &= ~(1<<5); PORTB &= ~(1<<5); }
inline void led_1_set_brightness(uint8_t n) { OCR1B = n; } 

inline void led_2_on(void)  { DDRD |=  (1<<7); PORTD |=  (1<<7); }
inline void led_2_off(void) { DDRD &= ~(1<<7); PORTD &= ~(1<<7); }
inline void led_2_set_brightness(uint8_t n) { OCR4D = n; } 

inline void led_3_on(void)  { DDRB |=  (1<<6); PORTB |=  (1<<6); }
inline void led_3_off(void) { DDRB &= ~(1<<6); PORTB &= ~(1<<6); }
inline void led_3_set_brightness(uint8_t n) { OCR1A = n; } 

void cycle_leds();
void init_leds();

#endif
