#include QMK_KEYBOARD_H
#include "xtonhasvim.h"

extern keymap_config_t keymap_config;

// Fillers to make layering more clear

#define ____ KC_TRNS

#define SFT_ESC  SFT_T(KC_ESC)
#define CTL_BSPC CTL_T(KC_BSPC)
#define ALT_SPC  ALT_T(KC_SPC)
#define SFT_ENT  SFT_T(KC_ENT)

#define KC_ML KC_MS_LEFT
#define KC_MR KC_MS_RIGHT
#define KC_MU KC_MS_UP
#define KC_MD KC_MS_DOWN
#define KC_MB1 KC_MS_BTN1
#define KC_MB2 KC_MS_BTN1

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
                    KC_LALT,KC_LGUI,                                            KC_RGUI,KC_RALT,                    \
                                    MO(_LOWER),  MO(_MOVE),             KC_SPC, MO(_RAISE),                                     \
                                    MO(_MOVE), LSFT(KC_LALT),            VIM_START, MO(_MOVE),                                    \
                                    KC_LALT,KC_LGUI,             KC_RGUI,KC_RALT                                    \
),

[_LOWER] = LAYOUT_xton_wired_wrong(
  KC_TILD, KC_F1,       KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_UNDS,    KC_PLUS, KC_LCBR, KC_RCBR,     KC_BSPC, \
  KC_DEL,  KC_EXLM,     KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR,    KC_ASTR, KC_LPRN, KC_RPRN,     KC_PIPE, \
  _______, KC_F7,       KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  X_____X,    X_____X, X_____X, X_____X,     FIREY_RETURN, \
                                   _______,  _______,             _______, MO(_RAISE),                                     \
                                            _______,_______,    _______,_______,                                   \
                                            _______,_______,    _______,_______,                                   \
                                            RESET,_______,    _______,_______                                    \
),

[_RAISE] = LAYOUT_xton_wired_wrong(
  KC_GRV,  X_____X,     X_____X, X_____X, X_____X,    X_____X, X_____X, KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC,     KC_BSPC, \
  KC_DEL,  KC_1,        KC_2,    KC_3,    KC_4,       KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,        KC_BSLS, \
  _______, X_____X,     X_____X, X_____X, X_____X,    X_____X, X_____X, X_____X, X_____X, X_____X, X_____X,     FIREY_RETURN, \
                                    MO(_LOWER),  _______,             _______, _______,                                     \
                                            _______,_______,    _______,_______,                                   \
                                            _______,_______,    _______,_______,                                   \
                                            _______,_______,    _______,RESET                                    \
),
[_ADJUST] = LAYOUT_xton_wired_wrong(
  X_____X,  RESET,   DEBUG,   X_____X, X_____X, X_____X, X_____X, X_____X, X_____X, X_____X, RGB_TOG,     RGB_MODE_PLAIN, \
  X_____X,  X_____X, X_____X, X_____X, X_____X, X_____X, X_____X, X_____X, X_____X, X_____X, X_____X,     RGB_MODE_REVERSE, \
  X_____X,  X_____X, X_____X, X_____X, X_____X, X_____X, X_____X, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY,     RGB_MODE_FORWARD, \
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
