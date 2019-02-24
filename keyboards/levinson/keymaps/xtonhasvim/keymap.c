 /* Copyright 2015-2017 Christon DeWan
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "xtonhasvim.h"
#include "fancylighting.h"

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

/************************************
 * keymaps!
 ************************************/

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Qwerty
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Ctrl*|   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;* |  '   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | chkwm| Move | Alt  | GUI  |Lower |     Space   |Raise | SPC  | GUI  | Move | Vim  |
 * `-----------------------------------------------------------------------------------'
 *
 * - Ctrl acts as Esc when tapped.
 * - Holding ; switches to movement layer.
 */
[_QWERTY] = LAYOUT_ortho_4x12( \
    KC_TAB,         KC_Q,      KC_W,    KC_E,    KC_R,         KC_T,    KC_Y,   KC_U,       KC_I,    KC_O,    KC_P,      KC_BSPC, \
    LCTL_T(KC_ESC), KC_A,      KC_S,    KC_D,    KC_F,         KC_G,    KC_H,   KC_J,       KC_K,    KC_L,    KC_SCLN,   KC_QUOT, \
    KC_LSFT,        KC_Z,      KC_X,    KC_C,    KC_V,         KC_B,    KC_N,   KC_M,       KC_COMM, KC_DOT,  KC_SLSH,   RSFT_T(KC_ENT) , \
    X_____X,  X_____X, X_____X, KC_LGUI, MO(_LOWER), LSFT(KC_LALT),  KC_SPC, MO(_RAISE), KC_RALT, X_____X, X_____X, VIM_START \
),

/* Lower
 * ,-----------------------------------------------------------------------------------.
 * |  ~   |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |   _  |   +  |   {  |   }  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |  Del |   !  |   @  |   #  |   $  |   %  |   ^  |   &  |   *  |   (  |   )  |  |   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |      | Next | Vol- | Vol+ | Play |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      | Bail |      |      |      |      |      |Raise |      |      | Bail |      |
 * `-----------------------------------------------------------------------------------'
 */
[_LOWER] = LAYOUT_ortho_4x12( \
  KC_TILD, KC_EXLM,  KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR,    KC_ASTR, KC_LPRN, KC_RPRN,     KC_BSPC, \
  X_____X,  X_____X,  LGUI(KC_LBRC), LGUI(LSFT(KC_LBRC)), LGUI(LSFT(KC_RBRC)), LGUI(KC_RBRC),  KC_HOME, KC_PGDN, KC_PGUP, KC_END, X_____X,     KC_PIPE, \
  _______, X_____X,  X_____X, X_____X,  X_____X, X_____X, X_____X,   KC_UNDS,    KC_PLUS, KC_LCBR, KC_RCBR,     KC_ENT, \
  _______, _______, _______, _______, _______, KC_LALT, _______, MO(_RAISE), _______, _______, _______, X_____X \
),

/* Raise
 * ,-----------------------------------------------------------------------------------.
 * |   `  |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |   -  |   =  |   [  |   ]  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |  Del |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  |  \   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |      | Next | Vol- | Vol+ | Play |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      | Bail |      |      |Lower |      |      |      |      |      | Bail |      |
 * `-----------------------------------------------------------------------------------'
 */
[_RAISE] = LAYOUT_ortho_4x12( \
  KC_GRV,  KC_1,        KC_2,    KC_3,    KC_4,       KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,        KC_BSPC, \
  X_____X,  X_____X,     X_____X, X_____X, X_____X,    X_____X, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, X_____X,     KC_BSLS, \
  _______, X_____X,     X_____X, X_____X, X_____X,    X_____X, X_____X, KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC,     KC_ENT, \
  _______, _______, _______, _______, MO(_LOWER), _______, KC_LGUI, _______, _______, _______, _______, _______ \
),


/* Adjust (Lower + Raise)
 * ,-------------------------------------------------------------------------------------.
 * |BL Raise|      |      |      |      |      |      |      |      |      |      |      |
 * |--------+------+------+------+------+-------------+------+------+------+------+------|
 * |BL Lower|      |      |      |      |      |      |      |      |      |      |      |
 * |--------+------+------+------+------+------|------+------+------+------+------+------|
 * |BL STEP |      |      |      |      |      |      | Next | Vol- | Vol+ | Play |      |
 * |--------+------+------+------+------+------+------+------+------+------+------+------|
 * |Backlite| Bail |      |      |      |      |      |      |      |      | Bail |      |
 * `-------------------------------------------------------------------------------------'
 */
[_ADJUST] = LAYOUT_ortho_4x12( \
    X_____X, RESET,   DEBUG,    RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, X_____X ,
    X_____X, X_____X, MU_MOD,  AU_ON,   AU_OFF,  X_____X, LALT(KC_LEFT), LALT(KC_DOWN), LALT(KC_UP), LALT(KC_RGHT), X_____X, X_____X,
    _______, MUV_DE,  MUV_IN,  MU_ON,   MU_OFF,  X_____X, X_____X, X_____X, X_____X, X_____X, X_____X, X_____X,
    _______, _______, _______, _______, _______, _______, X_____X, _______, X_____X, _______, _______, _______
),


/* movement layer (hold semicolon) */
[_MOVE] = LAYOUT_ortho_4x12( \
  TO(_QWERTY), X_____X, X_____X,       X_____X,             X_____X,             X_____X,       KC_HOME, KC_PGDN, KC_PGUP, KC_END,  X_____X, X_____X, \
  _______,     X_____X, LGUI(KC_LBRC), LGUI(LSFT(KC_LBRC)), LGUI(LSFT(KC_RBRC)), LGUI(KC_RBRC), KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______, X_____X, \
  _______,     X_____X, X_____X,       X_____X,             X_____X,             X_____X,       X_____X, X_____X, X_____X, X_____X, X_____X, _______, \
  _______,     _______, _______,       _______,             _______,             X_____X,       X_____X, _______, _______, _______, _______, X_____X \
),

/* mouse layer
 */
[_MOUSE] = LAYOUT_ortho_4x12( \
  TO(_QWERTY), X_____X,     X_____X,    KC_MS_UP,   X_____X,     X_____X, KC_MS_WH_LEFT, KC_MS_WH_DOWN, KC_MS_WH_UP, KC_MS_WH_RIGHT, X_____X,     X_____X, \
  _______,     X_____X,     KC_MS_LEFT, KC_MS_DOWN, KC_MS_RIGHT, X_____X, X_____X,       KC_MS_BTN1,    KC_MS_BTN2,  KC_MS_BTN3,     X_____X,     X_____X, \
  _______,     X_____X,     X_____X,    X_____X,    X_____X,     X_____X, X_____X,       X_____X,       X_____X,     X_____X,        X_____X,     _______, \
  _______,     TO(_QWERTY), _______,    _______,    _______,     X_____X, X_____X,       _______,       _______,     _______,        TO(_QWERTY), X_____X \
),

/* vim command layer.
 */
[_CMD] = LAYOUT_ortho_4x12( \
  X_____X,   X_____X,     VIM_W,   VIM_E,   X_____X, X_____X, VIM_Y,   VIM_U,   VIM_I,     VIM_O,      VIM_P,       X_____X, \
  VIM_ESC,   VIM_A,       VIM_S,   VIM_D,   X_____X, VIM_G,   VIM_H,   VIM_J,   VIM_K,     VIM_L,      X_____X,     X_____X, \
  VIM_SHIFT, X_____X,     VIM_X,   VIM_C,   VIM_V,   VIM_B,   X_____X, X_____X, VIM_COMMA, VIM_PERIOD, X_____X,     VIM_SHIFT, \
  _______,   TO(_QWERTY), _______, _______, X_____X, X_____X, X_____X, X_____X, _______,   _______,    TO(_QWERTY), X_____X \
)

};



#define C_RED 0xFF, 0x00, 0x00
#define C_GRN 0x00, 0xFF, 0x00
#define C_BLU 0x00, 0x00, 0xFF

#define C_YAN 0x00, 0xFF, 0xFF
#define C_PRP 0x7A, 0x00, 0xFF
#define C_ORG 0xFF, 0x93, 0x00

void rgbflag(uint8_t r, uint8_t g, uint8_t b, uint8_t rr, uint8_t gg, uint8_t bb) {
  LED_TYPE *target_led = user_rgb_mode ? shadowed_led : led;
  for (int i = 0; i < RGBLED_NUM; i++)  {
    switch (i) {
    case 6: case 7:
      target_led[i].r = r;
      target_led[i].g = g;
      target_led[i].b = b;
      break;
    case 4: case 5:
      target_led[i].r = rr;
      target_led[i].g = gg;
      target_led[i].b = bb;
      break;
    default:
      target_led[i].r = 0;
      target_led[i].g = 0;
      target_led[i].b = 0;
      break;
    }
  }
  rgblight_set();
}

void set_state_leds(void) {
  if (rgblight_get_mode() == 1) {
    switch (biton32(layer_state)) {
    case _RAISE:
      rgbflag(C_BLU, C_GRN);
      break;
    case _LOWER:
      rgbflag(C_BLU, C_RED);
      break;
    case _ADJUST:
      rgbflag(C_BLU, C_PRP);
      break;
    case _MOVE:
      rgbflag(C_RED, C_PRP);
      break;
    case _MOUSE:
      rgbflag(C_RED, C_GRN);
      break;
    case _CMD:
      switch(vstate) {
        case VIM_V:
        case VIM_VI:
        case VIM_VS:
          rgbflag(C_GRN, C_YAN);
          break;
        case VIM_C:
        case VIM_CI:
          rgbflag(C_GRN, C_ORG);
          break;
        case VIM_D:
        case VIM_DI:
          rgbflag(C_GRN, C_RED);
          break;
        case VIM_G:
          rgbflag(C_GRN, C_BLU);
          break;
        case VIM_Y:
          rgbflag(C_GRN, C_PRP);
          break;
        case VIM_START:
        default:
          rgbflag(C_GRN, C_GRN);
          break;
      }
      break;
    default: //  for any other layers, or the default layer
      rgbflag(C_YAN, C_YAN);
      break;
    }
  }
}

uint32_t layer_state_set_user(uint32_t state) {
  return update_tri_layer_state(state, _RAISE, _LOWER, _ADJUST);
}
