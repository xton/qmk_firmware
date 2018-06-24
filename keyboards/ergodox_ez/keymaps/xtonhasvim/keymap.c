// Netable differences vs. the default firmware for the ErgoDox EZ:
// 1. The Cmd key is now on the right side, making Cmd+Space easier.
// 2. The media keys work on OSX (But not on Windows).

#include QMK_KEYBOARD_H
#include "action_layer.h"
#include "xtonhasvim.h"

/************************************
 * states
 ************************************/

enum layers {
  _QWERTY,
  _LOWER,
  _RAISE,
  _ADJUST,
  _MOVE,
  _MOUSE,
  _CMD
};

extern uint8_t vim_cmd_layer(void) { return _CMD; }

enum keymap_keycodes {
  RAISE = VIM_SAFE_RANGE,
  LOWER
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: Basic layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |   `    |   1  |   2  |   3  |   4  |   5  |      |           |      |   6  |   7  |   8  |   9  |   0  | Bksp   |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * | Tab    |   Q  |   W  |   E  |   R  |   T  |      |           |      |   Y  |   U  |   I  |   O  |   P  | Bksp   |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |Ctrl/Esc|   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  | ;/mv |   '    |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * | LShift |Z/Ctrl|   X  |   C  |   V  |   B  |      |           |      |   N  |   M  |   ,  |   .  |   /  |rsht/rtn|
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |  Alt | Gui  | Lower/Spc |                              | Raise/Spc| Gui  |  Alt |      |      |
 *   `---------------------------------------'                               `-------------------------------------'
 *                                        ,-------------.       ,---------------.
 *                                        |      |      |       |      |        |
 *                                 ,------|------|------|       |------+--------+------.
 *                                 |      |      |      |       |      |        |      |
 *                                 |AltShf|      |------|       |------|        |  Vim |
 *                                 |      |      |      |       |      |        |      |
 *                                 `--------------------'       `----------------------'
 */
// If it accepts an argument (i.e, is a function), it doesn't need KC_.
// Otherwise, it needs KC_*
[_QWERTY] = LAYOUT_ergodox(  // layer 0 : default
        // left hand
        KC_GRV,         KC_1,         KC_2,   KC_3,   KC_4,   KC_5,   X_____X,
        KC_TAB,         KC_Q,         KC_W,   KC_E,   KC_R,   KC_T,   X_____X,
        LCTL_T(KC_ESC), KC_A,         KC_S,   KC_D,   KC_F,   KC_G,
        KC_LSFT,        KC_Z,         KC_X,   KC_C,   KC_V,   KC_B,   X_____X,
           X_____X,  X_____X,      KC_LALT, KC_LGUI,  LOWER,
                                              X_____X,  X_____X,
                                                        X_____X,
                                  LSFT(KC_LALT),X_____X,X_____X,
        // right hand
             X_____X, KC_6,   KC_7,   KC_8,   KC_9,   KC_0,              KC_BSPC,
             X_____X, KC_Y,   KC_U,   KC_I,   KC_O,   KC_P,              KC_BSPC,
                      KC_H,   KC_J,   KC_K,   KC_L,   LT(_MOVE,KC_SCLN), KC_QUOT,
             X_____X, KC_N,   KC_M,   KC_COMM,KC_DOT, KC_SLSH,           RSFT_T(KC_ENT),
                                  RAISE,  KC_LGUI,KC_LALT,X_____X,  X_____X,
             X_____X, X_____X,
             X_____X,
             X_____X, X_____X, VIM_START
    ),
/* Keymap 1: Symbol Layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |  F1  |  F2  |  F3  |  F4  |  F5  |      |           |      |  F6  |  F7  |  F8  |  F9  |  F10 |   F11  |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |    ~   |      |      |      |      |      |      |           |      |      |  _   |   +  |   {  |   }  |  bksp  |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |   del  |   !  |   @  |   #  |   $  |   %  |------|           |------|   ^  |   &  |   *  |   (  |   )  |   |    |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |  BOOM  |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |      |      |                                       |      |    . |      |      |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 |      |      |------|       |------|      |      |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
// LOWER
[_LOWER] = LAYOUT_ergodox(
       // left hand
       X_____X,KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  X_____X,
       KC_TILD,X_____X,X_____X,X_____X,X_____X,X_____X,X_____X,
       KC_DEL, KC_EXLM,KC_AT,  KC_HASH,KC_DLR, KC_PERC,
       _______,X_____X,X_____X,X_____X,X_____X,X_____X,X_____X,
       _______,_______,_______,_______,_______,
                                       X_____X,X_____X,
                                               X_____X,
                               X_____X,X_____X,X_____X,
       // right hand
       X_____X, KC_F6,   KC_F7,  KC_F8,   KC_F9,   KC_F10,  KC_F11,
       X_____X, X_____X, KC_UNDS, KC_PLUS, KC_LCBR,KC_RCBR, KC_BSPC,
                KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN,KC_RPRN, KC_PIPE,
       X_____X, X_____X, X_____X, X_____X, X_____X,X_____X, FIREY_RETURN,
                         _______, _______, _______,_______, _______,
       X_____X, X_____X,
       X_____X,
       X_____X, X_____X, X_____X
),
/* Keymap 2: Media and mouse keys
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |      |      | MsUp |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |MsLeft|MsDown|MsRght|      |------|           |------|      |      |      |      |      |  Play  |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      |      | Prev | Next |      |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      | Lclk | Rclk |                                       |VolUp |VolDn | Mute |      |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |Brwser|
 *                                 |      |      |------|       |------|      |Back  |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
// MEDIA AND MOUSE
[MDIA] = LAYOUT_ergodox(
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_MS_U, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_MS_L, KC_MS_D, KC_MS_R, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_BTN1, KC_BTN2,
                                           KC_TRNS, KC_TRNS,
                                                    KC_TRNS,
                                  KC_TRNS, KC_TRNS, KC_TRNS,
    // right hand
       KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                 KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MPLY,
       KC_TRNS,  KC_TRNS, KC_TRNS, KC_MPRV, KC_MNXT, KC_TRNS, KC_TRNS,
                          KC_VOLU, KC_VOLD, KC_MUTE, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS,
       KC_TRNS,
       KC_TRNS, KC_TRNS, KC_WBAK
),
};


// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {

};

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {

  uint8_t layer = biton32(layer_state);

  ergodox_board_led_off();
  ergodox_right_led_1_off();
  ergodox_right_led_2_off();
  ergodox_right_led_3_off();
  switch (layer) {
    // TODO: Make this relevant to the ErgoDox EZ.
    case SYMB:
      ergodox_right_led_1_on();
      break;
    case MDIA:
      ergodox_right_led_2_on();
      break;
    default:
      // none
      break;
  }

};
