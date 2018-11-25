/* Copyright 2018 Christon DeWan
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
#include <mousekey.h>
#include "xtonhasvim.h"
#include "action_layer.h"
#include "muse.h"
#include "hal.h"
#include "hal_pal.h"
#include "stm32_registry.h"


extern keymap_config_t keymap_config;

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
[_QWERTY] = LAYOUT_planck_grid(
    KC_TAB,         KC_Q,      KC_W,    KC_E,    KC_R,         KC_T,    KC_Y,   KC_U,       KC_I,    KC_O,    KC_P,              KC_BSPC, \
    LCTL_T(KC_ESC), KC_A,      KC_S,    KC_D,    KC_F,         KC_G,    KC_H,   KC_J,       KC_K,    KC_L,    LT(_MOVE,KC_SCLN), KC_QUOT, \
    KC_LSFT,        KC_Z,      KC_X,    KC_C,    KC_V,         KC_B,    KC_N,   KC_M,       KC_COMM, KC_DOT,  KC_SLSH,           RSFT_T(KC_ENT) , \
    LSFT(KC_LALT),  MO(_MOVE), KC_LALT, KC_LGUI, MO(_LOWER),   MO(_MOVE),  KC_SPC, MO(_RAISE), KC_RGUI, KC_RALT, MO(_MOVE),         VIM_START \
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
[_LOWER] = LAYOUT_planck_grid( \
  KC_TILD, KC_F1,       KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_UNDS,    KC_PLUS, KC_LCBR, KC_RCBR,     KC_BSPC, \
  KC_DEL,  KC_EXLM,     KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR,    KC_ASTR, KC_LPRN, KC_RPRN,     KC_PIPE, \
  _______, KC_F7,       KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  X_____X,    X_____X, X_____X, X_____X,     KC_ENT, \
  RESET,   TO(_QWERTY), _______, _______, _______, KC_MS_BTN2, KC_MS_BTN1, MO(_RAISE), _______, _______, TO(_QWERTY), X_____X \
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
[_RAISE] = LAYOUT_planck_grid( \
  KC_GRV,  X_____X,     X_____X, X_____X, X_____X,    X_____X, X_____X, KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC,     KC_BSPC, \
  KC_DEL,  KC_1,        KC_2,    KC_3,    KC_4,       KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,        KC_BSLS, \
  _______, X_____X,     X_____X, X_____X, X_____X,    X_____X, X_____X, X_____X, X_____X, X_____X, X_____X,     KC_ENT, \
  X_____X, TO(_QWERTY), _______, _______, MO(_LOWER), KC_MS_BTN2, KC_MS_BTN1, _______, _______, _______, TO(_QWERTY), RESET \
),

/* Adjust (Lower + Raise)
 * ,-----------------------------------------------------------------------------------.
 * |      | Reset|      |      |      |      |      |      |      |      |      |  Del |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |      |      |      |Aud on|Audoff|AGnorm|AGswap|Qwerty|Colemk|Dvorak|Plover|      |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |Voice-|Voice+|Mus on|Musoff|MIDIon|MIDIof|      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_ADJUST] = LAYOUT_planck_grid(
    X_____X, RESET,   DEBUG,    RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, X_____X ,
    X_____X, X_____X, MU_MOD,  AU_ON,   AU_OFF,  X_____X, KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT, KC_MPLY,  X_____X,
    X_____X, MUV_DE,  MUV_IN,  MU_ON,   MU_OFF,  X_____X, X_____X, TERM_ON, TERM_OFF, X_____X, X_____X, X_____X,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
),

/* movement layer (hold semicolon) */
[_MOVE] = LAYOUT_planck_grid( \
  TO(_QWERTY), X_____X, X_____X,       X_____X,             X_____X,             X_____X,       KC_HOME, KC_PGDN, KC_PGUP, KC_END,  X_____X, X_____X, \
  _______,     X_____X, LGUI(KC_LBRC), LGUI(LSFT(KC_LBRC)), LGUI(LSFT(KC_RBRC)), LGUI(KC_RBRC), KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______, X_____X, \
  _______,     X_____X, X_____X,       X_____X,             X_____X,             X_____X,       X_____X, X_____X, X_____X, X_____X, X_____X, _______, \
  _______,     _______, _______,       _______,             _______,             X_____X,       X_____X, _______, _______, _______, _______, X_____X \
),


/* vim command layer.
 */
[_CMD] = LAYOUT_planck_grid( \
  X_____X,   X_____X,     VIM_W,   VIM_E,   X_____X, X_____X, VIM_Y,   VIM_U,   VIM_I,     VIM_O,      VIM_P,       X_____X, \
  VIM_ESC,   VIM_A,       VIM_S,   VIM_D,   X_____X, VIM_G,   VIM_H,   VIM_J,   VIM_K,     VIM_L,      X_____X,     X_____X, \
  VIM_SHIFT, X_____X,     VIM_X,   VIM_C,   VIM_V,   VIM_B,   X_____X, X_____X, VIM_COMMA, VIM_PERIOD, X_____X,     VIM_SHIFT, \
  _______,   TO(_QWERTY), _______, _______, X_____X, X_____X, X_____X, X_____X, _______,   _______,    TO(_QWERTY), X_____X \
)

};

uint32_t layer_state_set_user(uint32_t state) {
  return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}


bool muse_mode = false;
uint8_t last_muse_note = 0;
uint16_t muse_counter = 0;
uint8_t muse_offset = 70;
uint16_t muse_tempo = 50;

void encoder_update(bool clockwise) {
  if (muse_mode) {
    if (IS_LAYER_ON(_RAISE)) {
      if (clockwise) {
        muse_offset++;
      } else {
        muse_offset--;
      }
    } else {
      if (clockwise) {
        muse_tempo+=1;
      } else {
        muse_tempo-=1;
      }
    }
  } else {
    if (clockwise) {
      register_code(KC_PGDN);
      unregister_code(KC_PGDN);
    } else {
      register_code(KC_PGUP);
      unregister_code(KC_PGUP);
    }
  }
}

#define TAP(kc) do { register_code(kc); unregister_code(kc); } while (0)
#define pin_count 4
uint32_t pins_were[pin_count] = { 0, 0, 0, 0 };
uint32_t pins[pin_count] = { B8, B3, B4, B5 };
uint32_t pins_kc[pin_count] = { KC_UP, KC_DOWN, KC_LEFT, KC_RIGHT };
#define mouse_step 20

EXTConfig extConfig = {{{0}}};
uint32_t ext_int_lines[] = { 8, 3, 4, 5 };
/* uint32_t ext_int_lines[] = { }; */
#define ext_int_lines_count sizeof(ext_int_lines) / sizeof(uint32_t)
static int32_t dx = 0;
static int32_t dy = 0;

void ballMoved(EXTDriver *extp, expchannel_t channel) {
  switch(channel) {
    case 8: dy--; break;
    case 3: dy++; break;
    case 4: dx--; break;
    case 5: dx++; break;
  }
}

static report_mouse_t mouse_report = {0};

void matrix_init_user() {

	osalSysLock();

	/* // map line 2 to wire B3 */
	palSetGroupMode(GPIOB, PAL_PORT_BIT(8), 0, PAL_MODE_INPUT);
	palSetGroupMode(GPIOB, PAL_PORT_BIT(3), 0, PAL_MODE_INPUT);
	palSetGroupMode(GPIOB, PAL_PORT_BIT(4), 0, PAL_MODE_INPUT);
	palSetGroupMode(GPIOB, PAL_PORT_BIT(5), 0, PAL_MODE_INPUT);

	for(int i = 0; i < ext_int_lines_count; i++){
		extConfig.channels[ext_int_lines[i]].mode = 
			EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOB;
		extConfig.channels[ext_int_lines[i]].cb = ballMoved;
	}


	extStart(&EXTD1, &extConfig);

	osalSysUnlock();
}

uint32_t since_last = 0;
#define wait_between_moves 0
static int32_t iix = 0;
static int32_t idx = 0;
static int32_t iiy = 0;
static int32_t idy = 0;


int8_t scale_mouse_delta(int32_t d, uint32_t sl) {
  int32_t scale = 10;
  if(sl < 20) scale = 100;
  else if (sl < 50) scale = 50;
  else if (sl < 100) scale = 30;
  else if (sl < 200) scale = 15;
  uint32_t dd = scale*d*d;
  if(dd > 127) dd = 127;
  return dd*(d>0?1:-1);
}

void matrix_scan_user(void) {
  
  if((dx || dy) && since_last > wait_between_moves) {
    xprintf("%d, %d [%d]\n", (int)dx, (int)dy, (int)since_last);
    if(IS_LAYER_ON(_LOWER)){
      if(dx > 0) {
        for(int i = 0; i < dx; i++) TAP(KC_RIGHT);
      } else {
        dx = -dx;
        for(int i = 0; i < dx; i++) TAP(KC_LEFT);
      }
      if(dy > 0) {
        for(int i = 0; i < dy; i++) TAP(KC_DOWN);
      } else {
        dy = -dy;
        for(int i = 0; i < dy; i++) TAP(KC_UP);
      }
    } else if(IS_LAYER_ON(_RAISE)) {
      mouse_report.h = dx;
      mouse_report.v = dy;
      mouse_report.x = mouse_report.y = 0;
      host_mouse_send(&mouse_report);
    } else {
      if(dx > 0) {
        mouse_report.x = scale_mouse_delta(dx, iix);
        iix = 0;
      } else if(dx < 0) {
        mouse_report.x = scale_mouse_delta(dx, idx);
        idx = 0;
      } else mouse_report.x = 0;

      if(dy > 0) {
        mouse_report.y = scale_mouse_delta(dy, iiy);
        iiy = 0;
      } else if(dy < 0) {
        mouse_report.y = scale_mouse_delta(dy, idy);
        idy = 0;
      } else mouse_report.y = 0;
      mouse_report.v = mouse_report.h = 0;
      host_mouse_send(&mouse_report);
    }
    dx = 0;
    dy = 0;
    since_last = 0;
  } 
  // these are regular enough to use for timing
  since_last++;
  iix++;
  idx++;
  iiy++;
  idy++;
    
  #ifdef AUDIO_ENABLE
    if (muse_mode) {
      if (muse_counter == 0) {
        uint8_t muse_note = muse_offset + SCALE[muse_clock_pulse()];
        if (muse_note != last_muse_note) {
          stop_note(compute_freq_for_midi_note(last_muse_note));
          play_note(compute_freq_for_midi_note(muse_note), 0xF);
          last_muse_note = muse_note;
        }
      }
      muse_counter = (muse_counter + 1) % muse_tempo;
    }
  #endif
}
