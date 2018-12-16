#include "ballerpad.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = {
      { KC_A, KC_B, KC_C, KC_E },
      { KC_F, KC_G, KC_H, KC_I }
      /* { KC_J, KC_K, KC_L, KC_M } */
    },
    [1] = {
      { KC_NO, KC_NO, KC_NO, KC_NO },
      { KC_NO, KC_NO, KC_NO, KC_NO }
      /* { KC_NO, KC_NO, KC_NO, KC_NO } */
    }
};


uint8_t vim_cmd_layer(void) { return 1; }

