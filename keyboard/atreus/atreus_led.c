#include "atreus_led.h"

void cycle_leds() {
    led_1_on();
    _delay_ms(75);
    led_1_off();
    led_2_on();
    _delay_ms(75);
    led_2_off();
    led_3_on();
    _delay_ms(75);
    led_3_off();
}

void init_leds() {
    // Set LED pins as ouput
    DDRB |= (1<<5|1<<6);
    DDRD |= (1<<7);

    // Setup PWM for LED pins
    // B5: OC1A, D7: OC4D, B6: OC1B

    // Setup fast PWM for OC1A & OC1B
    TCCR1A = 0b10101001;
    TCCR1B = 0b00001001;

    // Setup fast PWM for OC4D
    TCCR4A = 0b00000000;
    TCCR4B = 0b00000001;
    TCCR4C = 0b00001001;
    TCCR4D = 0b00000000;

    // Set brightness of LEDs
    led_1_set_brightness(INDICATOR_LED_BRIGHTNESS);
    led_2_set_brightness(INDICATOR_LED_BRIGHTNESS);
    led_3_set_brightness(INDICATOR_LED_BRIGHTNESS);

    // Initialize with off state
    led_1_off();
    led_2_off();
    led_3_off();
}