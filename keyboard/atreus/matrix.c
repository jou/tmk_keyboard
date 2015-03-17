/*
Copyright 2012 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * scan matrix
 */
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"

#ifdef BUILD_JOU
#include "atreus_led.h"
#include "action_layer.h"
#endif

#ifndef DEBOUNCE
#   define DEBOUNCE	5
#endif
static uint8_t debouncing = DEBOUNCE;

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

static matrix_row_t read_cols(void);
static void init_cols(void);
static void unselect_rows(void);
static void select_row(uint8_t row);


inline
uint8_t matrix_rows(void)
{
    return MATRIX_ROWS;
}

inline
uint8_t matrix_cols(void)
{
    return MATRIX_COLS;
}

void matrix_init(void)
{
    // initialize row and col
    unselect_rows();
    init_cols();

#ifdef BUILD_JOU
    init_leds();
    cycle_leds();
#endif

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }
}

uint8_t matrix_scan(void)
{
#ifdef BUILD_JOU
    uint8_t layer = biton32(layer_state);

    led_1_off();
    led_2_off();
    led_3_off();

    switch (layer) {
        case 1:
            led_1_on();
            break;
        case 2: 
            led_2_on();
            break;
        case 3: 
            led_3_on();
            break;
        default:
            // none
            break;
    }
#endif

    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        select_row(i);
        _delay_us(50);  // without this wait read unstable value.
        matrix_row_t cols = read_cols();
        if (matrix_debouncing[i] != cols) {
            matrix_debouncing[i] = cols;
            if (debouncing) {
                debug("bounce!: "); debug_hex(debouncing); debug("\n");
            }
            debouncing = DEBOUNCE;
        }
        unselect_rows();
    }

    if (debouncing) {
        if (--debouncing) {
            _delay_ms(1);
        } else {
            for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
                matrix[i] = matrix_debouncing[i];
            }
        }
    }

    return 1;
}

bool matrix_is_modified(void)
{
    if (debouncing) return false;
    return true;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & ((matrix_row_t)1<<col));
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

void matrix_print(void)
{
    print("\nr/c 0123456789ABCDEF\n");
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        phex(row); print(": ");
        pbin_reverse16(matrix_get_row(row));
        print("\n");
    }
}

uint8_t matrix_key_count(void)
{
    uint8_t count = 0;
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        count += bitpop16(matrix[i]);
    }
    return count;
}

/* Column pin configuration
 * col: 0   1   2   3   4   5   6   7   8   9   10
 * pin: B7  D6  F7  F6  B6  D4  E6  B4  B5  C6  D7
 * pin: E6  C6  D4  D0  D3  B3  F4  F5  F6  F7  B1 (jou)
 */
static void  init_cols(void)
{
#ifdef BUILD_JOU
    // Input with pull-up(DDR:0, PORT:1)
    DDRE  &= ~(0b01000000);
    PORTE |=  (0b01000000);

    DDRC  &= ~(0b01000000);
    PORTC |=  (0b01000000);

    DDRD  &= ~(0b00011001);
    PORTD |=  (0b00011001);

    DDRB  &= ~(0b00001010);
    PORTB |=  (0b00001010);

    DDRF  &= ~(0b11110000);
    PORTF |=  (0b11110000);
#else
    DDRB = DDRC = DDRE = DDRF = 0; // columns
    PORTB = PORTC = PORTE = PORTF = 255; // pullup resistors on inputs
    DDRD = 15; // rows (1 2 4 8) high and columns (16 32 64 128) low
    PORTD = 15;
#endif
}

static matrix_row_t read_cols(void)
{
#ifdef TEENSY
    return (PINF&(1<<6) ? 0 : (1<<0)) |
           (PINF&(1<<5) ? 0 : (1<<1)) |
           (PINF&(1<<4) ? 0 : (1<<2)) |
           (PINB&(1<<7) ? 0 : (1<<3)) |
           (PINB&(1<<6) ? 0 : (1<<4)) |
           (PINB&(1<<5) ? 0 : (1<<5)) |
           (PINB&(1<<4) ? 0 : (1<<6)) |
           (PINB&(1<<3) ? 0 : (1<<7)) |
           (PINB&(1<<2) ? 0 : (1<<8)) |
           (PINB&(1<<1) ? 0 : (1<<9)) |
           (PINB&(1<<0) ? 0 : (1<<10)) ;
#elif defined(BUILD_JOU)
    return (PINE&(1<<6) ? 0 : (1<<0)) |
           (PINC&(1<<6) ? 0 : (1<<1)) |
           (PIND&(1<<4) ? 0 : (1<<2)) |
           (PIND&(1<<0) ? 0 : (1<<3)) |
           (PIND&(1<<3) ? 0 : (1<<4)) |
           (PINB&(1<<3) ? 0 : (1<<5)) |
           (PINF&(1<<4) ? 0 : (1<<6)) |
           (PINF&(1<<5) ? 0 : (1<<7)) |
           (PINF&(1<<6) ? 0 : (1<<8)) |
           (PINF&(1<<7) ? 0 : (1<<9)) |
           (PINB&(1<<1) ? 0 : (1<<10)) ;
#else
    return (PINB&(1<<7) ? 0 : (1<<0)) |
           (PIND&(1<<6) ? 0 : (1<<1)) |
           (PINF&(1<<7) ? 0 : (1<<2)) |
           (PINF&(1<<6) ? 0 : (1<<3)) |
           (PINB&(1<<6) ? 0 : (1<<4)) |
           (PIND&(1<<4) ? 0 : (1<<5)) |
           (PINE&(1<<6) ? 0 : (1<<6)) |
           (PINB&(1<<4) ? 0 : (1<<7)) |
           (PINB&(1<<5) ? 0 : (1<<8)) |
           (PINC&(1<<6) ? 0 : (1<<9)) |
           (PIND&(1<<7) ? 0 : (1<<10)) ;
#endif
}

/* Row pin configuration
 * row: 0   1   2   3
 * pin: D0  D1  D3  D2    (a-star micro)
 * pin: D0  D1  D2  D3    (teensy2)
 * pin: B4  D5  B0  B2    (jou)
 */
static void unselect_rows(void)
{
#ifdef BUILD_JOU
    // Hi-Z(DDR:0, PORT:0) to unselect
    DDRB  &= ~(0b00010101);
    PORTB &= ~(0b00010101);

    DDRD  &= ~(0b00100000);
    PORTD &= ~(0b00100000);
#else
    PORTD = 15;
#endif
}

#define ROW_COUNT 4
#ifdef TEENSY
int rows[ROW_COUNT] = {0, 1, 2, 3};
#else
int rows[ROW_COUNT] = {0, 1, 3, 2};
#endif

static void select_row(uint8_t row)
{
#ifdef BUILD_JOU
    // Output low(DDR:1, PORT:0) to select
    switch (row) {
        case 0:
            DDRB  |=  (1<<4);
            PORTB &= ~(1<<4);
            break;
        case 1:
            DDRD  |=  (1<<5);
            PORTD &= ~(1<<5);
            break;
        case 2:
            DDRB  |=  (1<<0);
            PORTB &= ~(1<<0);
            break;
        case 3:
            DDRB  |=  (1<<2);
            PORTB &= ~(1<<2);
            break;
    }
#else
  PORTD = (char)(~(1 << rows[row]));
#endif
}
