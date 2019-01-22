#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "hal.h"
#include "timer.h"
#include "wait.h"
#include "printf.h"
#include "backlight.h"
#include "matrix.h"
#include "action.h"
#include "keycode.h"
#include <string.h>

/*
 * cols: { B12, B11, B10, B9, B15, B14, B13, B8, A0, A1, A2, A15, A8, A7, A6, A5, A4 }
 * rows: { B0, B1, B2, B3, B4, B5 }
 */
/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_COLS];
static bool debouncing = false;
static uint16_t debouncing_time = 0;


__attribute__ ((weak))
void matrix_init_user(void) {}

__attribute__ ((weak))
void matrix_scan_user(void) {}

__attribute__ ((weak))
void matrix_init_kb(void) {
  matrix_init_user();
}

__attribute__ ((weak))
void matrix_scan_kb(void) {
  matrix_scan_user();
}

void matrix_init(void) {
    printf("matrix init\n");
    /* debug_matrix = true; */

    // actual matrix setup
    
    /* cols: { B12, B11, B10, B9, B15, B14, B13, B8, A0, A1, A2, A15, A8, A7, A6, A5, A4 }  */
    palSetPadMode(GPIOB, 12, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOB, 11,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOB, 10,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOB, 9, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOB, 15, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOB, 14, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOB, 13, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOB, 8,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOA, 0,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOA, 1,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOA, 2,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOA, 15,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOA, 8,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOA, 7,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOA, 6,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOA, 5,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOA, 4,  PAL_MODE_OUTPUT_PUSHPULL);

    /* rows: { B0, B1, B2, B3, B4, B5 } */
    palSetPadMode(GPIOB, 0, PAL_MODE_INPUT_PULLDOWN);
    palSetPadMode(GPIOB, 1, PAL_MODE_INPUT_PULLDOWN);
    palSetPadMode(GPIOB, 2,  PAL_MODE_INPUT_PULLDOWN);
    palSetPadMode(GPIOB, 3,  PAL_MODE_INPUT_PULLDOWN);
    palSetPadMode(GPIOB, 4,  PAL_MODE_INPUT_PULLDOWN);
    palSetPadMode(GPIOB, 5,  PAL_MODE_INPUT_PULLDOWN);


    memset(matrix, 0, MATRIX_ROWS * sizeof(matrix_row_t));
    memset(matrix_debouncing, 0, MATRIX_COLS * sizeof(matrix_row_t));


    matrix_init_quantum();
}


uint8_t matrix_scan(void) {

    // actual matrix
    for (int col = 0; col < MATRIX_COLS; col++) {
        matrix_row_t data = 0;

        // strobe col 
        /* cols: { B12, B11, B10, B9, B15, B14, B13, B8, A0, A1, A2, A15, A8, A7, A6, A5, A4 }  */
        switch (col) {
            case 0:  palSetPad(GPIOB, 12); break;
            case 1:  palSetPad(GPIOB, 11); break;
            case 2:  palSetPad(GPIOB, 10); break;
            case 3:  palSetPad(GPIOB, 9); break;
            case 4:  palSetPad(GPIOB, 15); break;
            case 5:  palSetPad(GPIOB, 14); break;
            case 6:  palSetPad(GPIOB, 13); break;
            case 7:  palSetPad(GPIOB, 8); break;
            case 8:  palSetPad(GPIOA, 0); break;
            case 9:  palSetPad(GPIOA, 1); break;
            case 10: palSetPad(GPIOA, 2); break;
            case 11: palSetPad(GPIOA, 15); break;
            case 12: palSetPad(GPIOA, 8); break;
            case 13: palSetPad(GPIOA, 7); break;
            case 14: palSetPad(GPIOA, 6); break;
            case 15: palSetPad(GPIOA, 5); break;
            case 16: palSetPad(GPIOA, 4); break;
        }

        // need wait to settle pin state
        wait_us(20);

        // read row data
        /* rows: { B0, B1, B2, B3, B4, B5 } */
        data = (
            (palReadPad(GPIOB, 5) << 0 ) |
            (palReadPad(GPIOB, 4) << 1 ) |
            (palReadPad(GPIOB, 3) << 2 ) |
            (palReadPad(GPIOB, 2) << 3 ) |
            (palReadPad(GPIOB, 1) << 4 ) |
            (palReadPad(GPIOB, 0) << 5 )
        );

        // unstrobe  col { B11, B10, B2, B1, A7, B0 }
        switch (col) {
            case 0:  palClearPad(GPIOB, 12); break;
            case 1:  palClearPad(GPIOB, 11); break;
            case 2:  palClearPad(GPIOB, 10); break;
            case 3:  palClearPad(GPIOB, 9); break;
            case 4:  palClearPad(GPIOB, 15); break;
            case 5:  palClearPad(GPIOB, 14); break;
            case 6:  palClearPad(GPIOB, 13); break;
            case 7:  palClearPad(GPIOB, 8); break;
            case 8:  palClearPad(GPIOA, 0); break;
            case 9:  palClearPad(GPIOA, 1); break;
            case 10: palClearPad(GPIOA, 2); break;
            case 11: palClearPad(GPIOA, 15); break;
            case 12: palClearPad(GPIOA, 8); break;
            case 13: palClearPad(GPIOA, 7); break;
            case 14: palClearPad(GPIOA, 6); break;
            case 15: palClearPad(GPIOA, 5); break;
            case 16: palClearPad(GPIOA, 4); break;
        }

        if (matrix_debouncing[col] != data) {
            matrix_debouncing[col] = data;
            debouncing = true;
            debouncing_time = timer_read();
        }
    }

    if (debouncing && timer_elapsed(debouncing_time) > DEBOUNCE) {
        for (int row = 0; row < MATRIX_ROWS; row++) {
            matrix[row] = 0;
            for (int col = 0; col < MATRIX_COLS; col++) {
                matrix[row] |= ((matrix_debouncing[col] & (1 << row) ? 1 : 0) << col);
            }
        }
        debouncing = false;
    }

    matrix_scan_quantum();

    return 1;
}

bool matrix_is_on(uint8_t row, uint8_t col) {
    return (matrix[row] & (1<<col));
}

matrix_row_t matrix_get_row(uint8_t row) {
    return matrix[row];
}

void matrix_print(void) {
    printf("\nr/c 01234567\n");
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        printf("%X0: ", row);
        matrix_row_t data = matrix_get_row(row);
        for (int col = 0; col < MATRIX_COLS; col++) {
            if (data & (1<<col))
                printf("1");
            else
                printf("0");
        }
        printf("\n");
    }
}
