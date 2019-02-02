#include "ballerpad.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = {
      { RGB_SAD, RGB_SAI, RGB_VAI, RGB_VAD },
      { RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD }
      /* { KC_J, KC_K, KC_L, KC_M } */
    },
    [1] = {
      { KC_NO, KC_NO, KC_NO, KC_NO },
      { KC_NO, KC_NO, KC_NO, KC_NO }
      /* { KC_NO, KC_NO, KC_NO, KC_NO } */
    }
};


uint8_t vim_cmd_layer(void) { return 1; }

