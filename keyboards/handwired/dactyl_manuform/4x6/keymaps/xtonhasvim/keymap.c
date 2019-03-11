#include QMK_KEYBOARD_H
#include "xtonhasvim.h"

extern keymap_config_t keymap_config;

// Fillers to make layering more clear

#define ____ KC_TRNS

enum layers {
  _QWERTY,
  _LOWER,
  _RAISE,
  _ADJUST,
  _MOVE,
  _CMD
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Base (qwerty)
 * +-----------------------------------------+                             +-----------------------------------------+
 * | ESC  |   q  |   w  |   e  |   r  |   t  |                             |   y  |   u  |   i  |   o  |   p  |      |
 * |------+------+------+------+------+------|                             |------+------+------+------+------+------|
 * | TAB  |   a  |   s  |   d  |   f  |   g  |                             |   h  |   j  |   k  |   l  |   ;  |      |
 * |------+------+------+------+------+------|                             |------+------+------+------+------+------|
 * | SHFT |   z  |   x  |   c  |   v  |   b  |                             |   n  |   m  |   ,  |   .  |   /  |      |
 * +------+------+------+------+-------------+                             +-------------+------+------+------+------+
 *               |  [   |   ]  |                                                         |      |      |
 *               +-------------+-------------+                             +-------------+-------------+
 *                             |      |      |                             |      |      |
 *                             |------+------|                             |------+------|
 *                             |      |      |                             |      |      |
 *                             +-------------+                             +-------------+
 *                                           +-------------+ +-------------+
 *                                           |      |      | |      |      |
 *                                           |------+------| |------+------|
 *                                           |      |      | |      |      |
 *                                           +-------------+ +-------------+
 */

[_QWERTY] = LAYOUT_xton_wired_wrong( \
    KC_TAB,  KC_Q,  KC_W,   KC_E,   KC_R,   KC_T,               KC_Y,   KC_U,   KC_I,   KC_O,   KC_P,   KC_BSPC,   \
    LCTL_T(KC_ESC),  KC_A,  KC_S,   KC_D,   KC_F,   KC_G,               KC_H,   KC_J,   KC_K,   KC_L,   LT(_MOVE,KC_SCLN),KC_QUOT,   \
    KC_LSFT, KC_Z,  KC_X,   KC_C,   KC_V,   KC_B,               KC_N,   KC_M,   KC_COMM,KC_DOT, KC_SLSH,RSFT_T(KC_ENT), \
                    X_____X, KC_LGUI,                                             KC_RALT, X_____X,                    \
                                    MO(_LOWER), LSFT(KC_LALT),   KC_SPC, MO(_RAISE),                                \
                                    MO(_MOVE), KC_LGUI,    KC_LALT, VIM_START,                                 \
                                    KC_LALT,KC_LGUI,             KC_RGUI,KC_RALT                                    \
),

[_LOWER] = LAYOUT_xton_wired_wrong(
  KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR,    KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC, \
  KC_DEL,  X_____X, LGUI(KC_LBRC), LGUI(LSFT(KC_LBRC)), LGUI(LSFT(KC_RBRC)), LGUI(KC_RBRC), KC_HOME, KC_PGDN, KC_PGUP, KC_END, X_____X,     KC_PIPE, \
  _______, X_____X, X_____X, X_____X, X_____X, X_____X, X_____X,  KC_UNDS,   KC_PLUS, KC_LCBR, KC_RCBR,     FIREY_RETURN, \
                                   _______,  _______,             _______, _______,                                     \
                                            _______,_______,    _______,_______,                                   \
                                            _______, KC_LALT,    _______,_______,                                   \
                                            RESET,  _______,    _______,_______                                    \
),

[_RAISE] = LAYOUT_xton_wired_wrong(
  KC_GRV, KC_1,        KC_2,    KC_3,    KC_4,       KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,        KC_BSPC,  \
  KC_DEL,  X_____X,     X_____X, X_____X, X_____X,    X_____X, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT,X_____X,   KC_BSLS,\
  _______, X_____X,     X_____X, X_____X, X_____X,    X_____X, X_____X, KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC,      FIREY_RETURN, \
                                      _______, _______,             _______, _______,                                     \
                                            _______,_______,    _______,_______,                                   \
                                            _______,_______,    KC_LGUI,_______,                                   \
                                            _______,_______,    _______,RESET                                    \
),
[_ADJUST] = LAYOUT_xton_wired_wrong(
  X_____X,  RESET,   DEBUG,    RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD,  RGB_MODE_PLAIN, \
  X_____X,  X_____X, X_____X, X_____X, X_____X, X_____X, LALT(KC_LEFT), LALT(KC_DOWN), LALT(KC_UP), LALT(KC_RIGHT), X_____X,     RGB_MODE_REVERSE, \
  _______,  X_____X, X_____X, X_____X, X_____X, X_____X, X_____X, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY,     RGB_MODE_FORWARD, \
                                    _______,  _______,             _______, _______,                                     \
                                            _______,_______,    _______,_______,                                   \
                                            _______,_______,    _______,_______,                                   \
                                            _______,_______,    _______,_______                                    \
),
[_MOVE] = LAYOUT_xton_wired_wrong(
  TO(_QWERTY), X_____X, X_____X,       X_____X,             X_____X,             X_____X,       KC_HOME, KC_PGDN, KC_PGUP, KC_END,  X_____X, X_____X, \
  _______,     X_____X, LGUI(KC_LBRC), LGUI(LSFT(KC_LBRC)), LGUI(LSFT(KC_RBRC)), LGUI(KC_RBRC), KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______, X_____X, \
  _______,     X_____X, X_____X,       X_____X,             X_____X,             X_____X,       X_____X, X_____X, X_____X, X_____X, X_____X, _______, \
                                   _______,  X_____X,             X_____X, _______,                                     \
                                            _______,_______,    _______,_______,                                   \
                                            TO(_QWERTY),_______,    _______,_______,                                   \
                                            _______,_______,    _______,_______                                    \
),
[_CMD] = LAYOUT_xton_wired_wrong(
  X_____X,   X_____X,     VIM_W,   VIM_E,   X_____X, X_____X, VIM_Y,   VIM_U,   VIM_I,     VIM_O,      VIM_P,       X_____X, \
  VIM_ESC,   VIM_A,       VIM_S,   VIM_D,   X_____X, VIM_G,   VIM_H,   VIM_J,   VIM_K,     VIM_L,      X_____X,     X_____X, \
  VIM_SHIFT, X_____X,     VIM_X,   VIM_C,   VIM_V,   VIM_B,   X_____X, X_____X, VIM_COMMA, VIM_PERIOD, X_____X,     VIM_SHIFT, \
                                    X_____X,  X_____X,             X_____X, X_____X,                                     \
                                            _______,_______,    _______,_______,                                   \
                                            TO(_QWERTY),_______,    _______,_______,                                   \
                                            _______,_______,    _______,_______                                    \
)
};

extern uint8_t vim_cmd_layer(void) { return _CMD; }
uint32_t layer_state_set_user(uint32_t state) {
  return update_tri_layer_state(state, _RAISE, _LOWER, _ADJUST);
}


void matrix_slave_scan_user(void) {
  #if defined(RGBLIGHT_ANIMATIONS) & defined(RGBLIGHT_ENABLE)
  // some WIERD behavior happens with this on
//    rgblight_task();
  #endif
}

