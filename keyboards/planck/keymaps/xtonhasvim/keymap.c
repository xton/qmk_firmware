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
  RESET,   TO(_QWERTY), _______, _______, _______, _______, _______, MO(_RAISE), _______, _______, TO(_QWERTY), X_____X \
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
  X_____X, TO(_QWERTY), _______, _______, MO(_LOWER), _______, _______, _______, _______, _______, TO(_QWERTY), RESET \
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

uint32_t did_happen = 0;

void okay_yeah(void) {
	did_happen += 1;
}

// linker should place this where it needs to go...
void EIC_3_Handler(void){ 
	did_happen += 1;
	// assume that's what called us. clear it....
	EXTI->PR &= ~EXTI_PR_PR3;
}

ISR(STM32_EXTI_LINE0_HANDLER) {
	xprintf("fuck yeah")
	did_happen += 1;
	// assume that's what called us. clear it....
	EXTI->PR &= ~EXTI_PR_PR3;
}
/* ISR(Vector5C){ */ 
/* 	did_happen += 2; */
/* 	// assume that's what called us. clear it.... */
/* 	EXTI->PR &= ~EXTI_PR_PR3; */
/* } */
/* ISR(Vector58){ */ 
/* 	did_happen += 4; */
/* 	// assume that's what called us. clear it.... */
/* 	EXTI->PR &= ~EXTI_PR_PR3; */
/* } */
/* ISR(Vector60){ */ 
/* 	did_happen += 4; */
/* 	// assume that's what called us. clear it.... */
/* 	EXTI->PR &= ~EXTI_PR_PR3; */
/* } */

void matrix_init_user() {
	for(int i = 0; i < pin_count; i++){
		setPinInputHigh(pins[i]);
	}
	// this doesn't work
	/* palLineEnableEventI(PAL_LINE(GPIOB, 4), PAL_EVENT_MODE_BOTH_EDGES, okay_yeah); */
	
	// enable input on this pin, required to receive interrupts
	palSetPadMode(GPIOB, 3, PAL_MODE_INPUT_PULLUP);

	// map line 2 to wire B3
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PB;
	// enable rising edge for line 2
	EXTI->RTSR |= EXTI_RTSR_TR3;
	// and falling edge
	EXTI->FTSR |= EXTI_FTSR_TR3;
	// enable this interrupt
	EXTI->IMR |= EXTI_IMR_MR3;

	// copypasta
	NVIC_DisableIRQ(EXTI0_IRQn);
	NVIC_ClearPendingIRQ(EXTI0_IRQn);
	NVIC_SetPriority(EXTI0_IRQn, 0);
	NVIC_EnableIRQ(EXTI0_IRQn);

}

void matrix_scan_user(void) {
  if(did_happen){
	xprintf("got to %d\n");
	did_happen = 0;
  }
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
	for(int i = 0; i < pin_count; i++){
		uint16_t on = readPin(pins[i]);
		if(on != pins_were[i]) {
			TAP(pins_kc[i]);
		}
		pins_were[i] = on;

	}
}
