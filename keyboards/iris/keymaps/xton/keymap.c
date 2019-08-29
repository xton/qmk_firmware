#include QMK_KEYBOARD_H

extern keymap_config_t keymap_config;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _QWERTY 0
#define _LOWER 3
#define _RAISE 4
#define _ADJUST 16

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE,
  ADJUST,
};

#define KC______ KC_TRNS
#define KC_XXXXX KC_NO
#define KC_LOWER LOWER
#define KC_RAISE RAISE
#define KC_RST   RESET
#define KC_ERST  EEPROM_RESET
#define KC_LRST  RGBRST
#define KC_LTOG  RGB_TOG
#define KC_LHUI  RGB_HUI
#define KC_LHUD  RGB_HUD
#define KC_LSAI  RGB_SAI
#define KC_LSAD  RGB_SAD
#define KC_LVAI  RGB_VAI
#define KC_LVAD  RGB_VAD
#define KC_LMOD  RGB_MOD
#define KC_CTLES CTL_T(KC_ESC)
#define KC_SHENT RSFT_T(KC_ENT)
#define KC_SALT  LSFT(KC_LALT)

#define KC_LTAB  LGUI(KC_LBRC) 
#define KC_NBAK  LGUI(LSFT(KC_LBRC)) 
#define KC_NFOR  LGUI(LSFT(KC_RBRC)) 
#define KC_RTAB  LGUI(KC_RBRC)

#define KC_ALEFT LALT(KC_LEFT)
#define KC_ARITE LALT(KC_RIGHT)
#define KC_AUP LALT(KC_UP)
#define KC_ADOWN LALT(KC_DOWN)

#define KC_BLON  BL_ON
#define KC_BLOF  BL_OFF
#define KC_BLUP  BL_DEC
#define KC_BLDN  BL_INC
#define KC_BLTG  BL_TOGG
#define KC_BLST  BL_STEP
#define KC_BLBR  BL_BRTG

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
        ESC,     1,     2,     3,     4,     5,                      6,     7,     8,     9,     0,  BSPC,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
        TAB,     Q,     W,     E,     R,     T,                      Y,     U,     I,     O,     P,  BSPC,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
      CTLES,     A,     S,     D,     F,     G,                      H,     J,     K,     L,  SCLN,  QUOT,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       LSFT,     Z,     X,     C,     V,     B, _____,    _____,     N,     M,  COMM,   DOT,  SLSH, SHENT,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                   LGUI, LOWER,  SALT,      SPC, RAISE,  LALT \
                              //`--------------------'  `--------------------'
  ),

  [_LOWER] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
        F12,    F1,    F2,    F3,    F4,    F5,                     F6,    F7,    F8,    F9,   F10,   F11,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       TILD,  EXLM,    AT,  HASH,   DLR,  PERC,                   CIRC,  AMPR,  ASTR,  LPRN,  RPRN,  BSPC,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
        GRV, XXXXX,  LTAB,  NBAK,  NFOR,  RTAB,                   HOME,  PGDN,  PGUP,   END, XXXXX,  PIPE,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
      _____, XXXXX, XXXXX, XXXXX, XXXXX, XXXXX, _____,    _____, XXXXX,  UNDS,  PLUS,  LCBR,  RCBR,   ENT,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                  _____, LOWER,  LALT,    _____, RAISE, _____ \
                              //`--------------------'  `--------------------'
  ),
  [_RAISE] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
        F12,    F1,    F2,    F3,    F4,    F5,                     F6,    F7,    F8,    F9,   F10,   F11,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
        GRV,     1,     2,     3,     4,     5,                      6,     7,     8,     9,     0,  BSPC,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       TILD, XXXXX, XXXXX, XXXXX, XXXXX, XXXXX,                   LEFT,  DOWN,    UP,  RGHT, XXXXX,  BSLS,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
      _____,    F1,    F2,    F3,    F4,    F5, _____,    _____, XXXXX, MINS,    EQL,  LBRC,  RBRC,   ENT,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                  _____, LOWER, _____,     LGUI, RAISE, _____ \
                              //`--------------------'  `--------------------'
  ),

  [_ADJUST] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
      XXXXX, XXXXX, XXXXX, XXXXX, XXXXX, ERST,                    BLTG,  BLUP,  BLDN,  BLBR,  BLST, XXXXX,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
      XXXXX,   RST,  LMOD, XXXXX, XXXXX, ERST,                    MNXT,  MPRV,  MFFD,  MRWD, XXXXX, XXXXX,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       LTOG,  LHUI,  LSAI,  LVAI, XXXXX, XXXXX,                  ALEFT, ADOWN,   AUP, ARITE, XXXXX, XXXXX,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
      _____,  LHUD,  LSAD,  LVAD, XXXXX, XXXXX, _____,    _____, XXXXX, XXXXX, XXXXX, XXXXX, XXXXX, XXXXX,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                  _____, LOWER, _____,    XXXXX, RAISE, XXXXX \
                              //`--------------------'  `--------------------'
  )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        set_single_persistent_default_layer(_QWERTY);
      }
      return false;
      break;
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case ADJUST:
      if (record->event.pressed) {
        layer_on(_ADJUST);
      } else {
        layer_off(_ADJUST);
      }
      return false;
      break;
  }
  return true;
}
