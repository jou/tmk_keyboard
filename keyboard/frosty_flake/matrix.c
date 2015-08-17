/*
Copyright 2014 Ralf Schmitt <ralf@bunkertor.net>

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

#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"

#ifndef DEBOUNCE
#   define DEBOUNCE 0
#endif
static uint8_t debouncing = DEBOUNCE;

static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

static uint8_t read_rows(void);
static void init_rows(void);
static void unselect_cols(void);
static void select_col(uint8_t col);

inline uint8_t matrix_rows(void)
{
    return MATRIX_ROWS;
}

inline uint8_t matrix_cols(void)
{
    return MATRIX_COLS;
}

void matrix_init(void)
{
    unselect_cols();
    init_rows();
    for (uint8_t i=0; i < MATRIX_ROWS; i++)  {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }
}

uint8_t matrix_scan(void)
{
    for (uint8_t col = 0; col < MATRIX_COLS; col++) {
        select_col(col);
        _delay_us(3);
        uint8_t rows = read_rows();
        for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
            bool prev_bit = matrix_debouncing[row] & ((matrix_row_t)1<<col);
            bool curr_bit = rows & (1<<row);
            if (prev_bit != curr_bit) {
                matrix_debouncing[row] ^= ((matrix_row_t)1<<col);
                debouncing = DEBOUNCE;
            }
        }
        unselect_cols();
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

inline bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & ((matrix_row_t)1<<col));
}

inline matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

void matrix_print(void)
{
    print("\nr/c 0123456789ABCDEF\n");
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        xprintf("%02X: %032lb\n", row, bitrev32(matrix_get_row(row)));
    }
}

uint8_t matrix_key_count(void)
{
    uint8_t count = 0;
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        count += bitpop32(matrix[i]);
    }
    return count;
}

/* Row pin configuration
 *
 * Revision 20140521
 * -----------------
 * row: 0    1    2    3    4    5    6    7
 * pin: PC7  PB5  PB4  PB6  PB1  PB0  PB3  PB2
 *
 * Revision 20130602
 * -----------------
 * row: 0    1    2    3    4    5    6    7
 * pin: PC2  PD0  PD1  PC7  PD5  PD6  PD2  PD4
 */
static void init_rows(void)
{
#if HW_REVISION == 20140521
    DDRC &= ~0b10000000;
    DDRB &= ~0b01111111;
    PORTC |= 0b10000000;
    PORTB |= 0b01111111;
#elif HW_REVISION == 20130602
    DDRC &= ~0b10000100;
    DDRD &= ~0b01110111;
    PORTC |= 0b10000100;
    PORTD |= 0b01110111;
#endif
}

static uint8_t read_rows(void)
{
#if HW_REVISION == 20140521
    return (PINC&(1<<7) ? 0 : (1<<0)) |
           (PINB&(1<<5) ? 0 : (1<<1)) |
           (PINB&(1<<4) ? 0 : (1<<2)) |
           (PINB&(1<<6) ? 0 : (1<<3)) |
           (PINB&(1<<1) ? 0 : (1<<4)) |
           (PINB&(1<<0) ? 0 : (1<<5)) |
           (PINB&(1<<3) ? 0 : (1<<6)) |
           (PINB&(1<<2) ? 0 : (1<<7));
#elif HW_REVISION == 20130602
    return (PINC&(1<<2) ? 0 : (1<<0)) |
           (PIND&(1<<0) ? 0 : (1<<1)) |
           (PIND&(1<<1) ? 0 : (1<<2)) |
           (PINC&(1<<7) ? 0 : (1<<3)) |
           (PIND&(1<<5) ? 0 : (1<<4)) |
           (PIND&(1<<6) ? 0 : (1<<5)) |
           (PIND&(1<<2) ? 0 : (1<<6)) |
           (PIND&(1<<4) ? 0 : (1<<7));
#endif
}

/*  These columns uses two 74HC42 4 to 10 bit demultiplexers (low active).
 *
 *  Revision 20140521
 *  -----------------
 *   COL PD0 PD1 PD2 PD3 PD4 PD5 PD6 PD7
 *    0   1   1   0   0   0   1   0   0 
 *    1   0   0   0   1   1   1   1   0 
 *    2   1   1   0   0   1   0   0   0 
 *    3   1   0   0   1   0   1   1   0 
 *    4   1   1   0   1   0   0   0   0 
 *    5   1   1   0   1   1   1   0   0 
 *    6   0   0   0   1   0   1   1   0 
 *    7   0   0   0   0   0   1   1   0 
 *    8   0   0   0   0   1   1   1   0 
 *    9   1   0   0   0   0   1   1   0 
 *   10   1   0   0   0   1   1   1   0 
 *   11   0   1   0   0   0   1   1   0 
 *   12   1   0   0   1   1   1   1   0 
 *   13   0   1   0   1   0   1   1   0 
 *   14   1   1   0   0   0   0   1   0 
 *   15   1   1   0   1   1   0   0   0 
 *   16   1   1   0   1   0   1   0   0 
 *   17   1   1   0   0   1   1   0   0 
 *
 *  Revision 20130602
 *  -----------------
 *   COL PB0 PB1 PB2 PB3 PB4 PB5 PB6 PB7
 *    0   0   0   1   1   1   1   0   0
 *    1   0   1   1   0   0   1   1   0
 *    2   0   0   0   1   1   1   0   0
 *    3   0   1   0   0   1   1   1   0
 *    4   0   1   1   1   1   0   0   0
 *    5   0   1   1   1   1   1   0   0
 *    6   0   1   0   0   0   1   1   0
 *    7   0   0   0   0   0   1   1   0
 *    8   0   0   1   0   0   1   1   0
 *    9   0   0   0   1   0   1   1   0
 *   10   0   1   0   1   0   1   1   0
 *   11   0   0   1   1   0   1   1   0
 *   12   0   1   1   1   0   1   1   0
 *   13   0   0   0   0   1   1   1   0
 *   14   0   0   0   1   1   0   1   0
 *   15   0   1   0   1   1   1   0   0
 *   16   0   1   0   1   1   0   0   0
 *   17   0   0   0   1   1   0   0   0
 */
static void unselect_cols(void)
{
#if HW_REVISION == 20140521
    DDRD  |=  0b01111011;
    PORTD &= ~0b01111011;
#elif HW_REVISION == 20130602
    DDRB  |=  0b01111110;
    PORTB &= ~0b01111110;
#endif
}

static void select_col(uint8_t col)
{
#if HW_REVISION == 20140521
    switch (col) {
        case 0:
            PORTD |= (1<<0) | (1<<1) | (1<<5);
            break;
        case 1:
            PORTD |= (1<<3) | (1<<4) | (1<<5) | (1<<6);
            break;
        case 2:
            PORTD |= (1<<0) | (1<<1) | (1<<4);
            break;
        case 3:
            PORTD |= (1<<0) | (1<<3) | (1<<5) | (1<<6);
            break;
        case 4:
            PORTD |= (1<<0) | (1<<1) | (1<<3);
            break;
        case 5:
            PORTD |= (1<<0) | (1<<1) | (1<<3) | (1<<4) | (1<<5);
            break;
        case 6:
            PORTD |= (1<<3) | (1<<5) | (1<<6);
            break;
        case 7:
            PORTD |= (1<<5) | (1<<6);
            break;
        case 8:
            PORTD |= (1<<4) | (1<<5) | (1<<6);
            break;
        case 9:
            PORTD |= (1<<0) | (1<<5) | (1<<6);
            break;
        case 10:
            PORTD |= (1<<0) | (1<<4) | (1<<5) | (1<<6);
            break;
        case 11:
            PORTD |= (1<<1) | (1<<5) | (1<<6);
            break;
        case 12:
            PORTD |= (1<<0) | (1<<3) | (1<<4) | (1<<5) | (1<<6);
            break;
        case 13:
            PORTD |= (1<<1) | (1<<3) | (1<<5) | (1<<6);
            break;
        case 14:
            PORTD |= (1<<0) | (1<<1) | (1<<6);
            break;
        case 15:
            PORTD |= (1<<0) | (1<<1) | (1<<3) | (1<<4);
            break;
        case 16:
            PORTD |= (1<<0) | (1<<1) | (1<<3) | (1<<5);
            break;
        case 17:
            PORTD |= (1<<0) | (1<<1) | (1<<4) | (1<<5);
            break;
    }
#elif HW_REVISION == 20130602
    switch (col) {
        case 0:
            PORTB |= (1<<2) | (1<<3) | (1<<4) | (1<<5);
            break;
        case 1:
            PORTB |= (1<<1) | (1<<2) | (1<<5) | (1<<6);
            break;
        case 2:
            PORTB |= (1<<3) | (1<<4) | (1<<5);
            break;
        case 3:
            PORTB |= (1<<1) | (1<<4) | (1<<5) | (1<<6);
            break;
        case 4:
            PORTB |= (1<<1) | (1<<2) | (1<<3) | (1<<4);
            break;
        case 5:
            PORTB |= (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5);
            break;
        case 6:
            PORTB |= (1<<1) | (1<<5) | (1<<6);
            break;
        case 7:
            PORTB |= (1<<5) | (1<<6);
            break;
        case 8:
            PORTB |= (1<<2) | (1<<5) | (1<<6);
            break;
        case 9:
            PORTB |= (1<<3) | (1<<5) | (1<<6);
            break;
        case 10:
            PORTB |= (1<<1) | (1<<3) | (1<<5) | (1<<6);
            break;
        case 11:
            PORTB |= (1<<2) | (1<<3) | (1<<5) | (1<<6);
            break;
        case 12:
            PORTB |= (1<<1) | (1<<2) | (1<<3) | (1<<5) | (1<<6);
            break;
        case 13:
            PORTB |= (1<<4) | (1<<5) | (1<<6);
            break;
        case 14:
            PORTB |= (1<<3) | (1<<4) | (1<<6);
            break;
        case 15:
            PORTB |= (1<<1) | (1<<3) | (1<<4) | (1<<5);
            break;
        case 16:
            PORTB |= (1<<1) | (1<<3) | (1<<4);
            break;
        case 17:
            PORTB |= (1<<3) | (1<<4);
            break;
    }
#endif
}
