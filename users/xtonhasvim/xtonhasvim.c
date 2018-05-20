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

#include "xtonhasvim.h"
#include "rgblight.h"
#include "math.h"

/************************************
 * helper foo
 ************************************/

#define PRESS(kc) register_code(kc)
#define RELEASE(kc) unregister_code(kc)

static void TAP(uint16_t keycode) {
    PRESS(keycode);
    RELEASE(keycode);
}

static void CMD(uint16_t keycode) {
  PRESS(KC_LGUI);
    TAP(keycode);
  RELEASE(KC_LGUI);
}

static void CTRL(uint16_t keycode) {
  PRESS(KC_LCTRL);
    TAP(keycode);
  RELEASE(KC_LCTRL);
}

static void SHIFT(uint16_t keycode) {
  PRESS(KC_LSHIFT);
    TAP(keycode);
  RELEASE(KC_LSHIFT);
}

static void ALT(uint16_t keycode) {
  PRESS(KC_LALT);
    TAP(keycode);
  RELEASE(KC_LALT);
}


static uint16_t vstate = VIM_START;
static bool yank_was_lines = false;
static bool SHIFTED = false;
static uint32_t mod_override_layer_state = 0;
static uint16_t mod_override_triggering_key = 0;

static void edit(void) { vstate = VIM_START; layer_on(_EDIT); layer_off(_CMD); }
#define EDIT edit()


static void simple_movement(uint16_t keycode) {
  switch(keycode) {
    case VIM_B:
      PRESS(KC_LALT);
        SHIFT(KC_LEFT); // select to start of this word
      RELEASE(KC_LALT);
      break;
    case VIM_E:
      PRESS(KC_LALT);
        SHIFT(KC_RIGHT); // select to end of this word
      RELEASE(KC_LALT);
      break;
    case VIM_H:
      SHIFT(KC_LEFT);
      break;
    case VIM_J:
      CMD(KC_LEFT);
      SHIFT(KC_DOWN);
      SHIFT(KC_DOWN);
      break;
    case VIM_K:
      CMD(KC_LEFT);
      TAP(KC_DOWN);
      SHIFT(KC_UP);
      SHIFT(KC_UP);
      break;
    case VIM_L:
      SHIFT(KC_RIGHT);
      break;
    case VIM_W:
      PRESS(KC_LALT);
      SHIFT(KC_RIGHT); // select to end of this word
      SHIFT(KC_RIGHT); // select to end of next word
      SHIFT(KC_LEFT);  // select to start of next word
      RELEASE(KC_LALT);
      break;
  }
}

static void comma_period(uint16_t keycode) {
  switch (keycode) {
  case VIM_COMMA:
    if (SHIFTED) {
      // indent
      CMD(KC_LBRACKET);
    } else {
      // toggle comment
      CMD(KC_SLASH);
    }
    break;
  case VIM_PERIOD:
    if (SHIFTED) {
      // outdent
      CMD(KC_RBRACKET);
    }
    break;
  }
}

__attribute__ ((weak))
bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {
  return true;
}

#define PASS_THRU process_record_keymap(keycode, record)

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  /****** FIREY_RETURN *****/
  if(record->event.pressed && keycode == FIREY_RETURN) {
    start_breath_fire();
    TAP(KC_ENT);
  }

  /****** mod passthru *****/
  if(record->event.pressed && layer_state_is(_CMD) && IS_MOD(keycode)) {
    mod_override_layer_state = layer_state;
    mod_override_triggering_key = keycode;
    // TODO: change this to track key location instead
    layer_clear();
    return PASS_THRU; // let the event fall through...
  }
  if(mod_override_layer_state && !record->event.pressed && keycode == mod_override_triggering_key) {
    layer_state_set(mod_override_layer_state);
    mod_override_layer_state = 0;
    mod_override_triggering_key = 0;
    return PASS_THRU;
  }

  if (VIM_START <= keycode && keycode <= VIM_ESC) {
    if(keycode == VIM_SHIFT) {
      SHIFTED = record->event.pressed;
      return false;
    }

    if (record->event.pressed) {
      if(keycode == VIM_START) {
        // entry from anywhere
        layer_on(_CMD);
        vstate = VIM_START;

        // reset state
        yank_was_lines = false;
        SHIFTED = false;
        mod_override_layer_state = 0;
        mod_override_triggering_key = 0;

        return false;
      }
      switch(vstate) {
        case VIM_START:
          switch(keycode){
            /*****************************
             * ground state
             *****************************/
            case VIM_A:
              if(SHIFTED) {
                // CMD(KC_RIGHT);
                CTRL(KC_E);
              } else {
                TAP(KC_RIGHT);
              }
              EDIT;
              break;
            case VIM_B:
              PRESS(KC_LALT);
              PRESS(KC_LEFT);
              break;
            case VIM_C:
              if(SHIFTED) {
                PRESS(KC_LSHIFT);
                  CMD(KC_RIGHT);
                RELEASE(KC_LSHIFT);
                CMD(KC_X);
                yank_was_lines = false;
                EDIT;
              } else {
                vstate = VIM_C;
              }
              break;
            case VIM_D:
              if(SHIFTED) {
                CTRL(KC_K);
              } else {
                vstate = VIM_D;
              }
              break;
            case VIM_E:
              PRESS(KC_LALT);
              PRESS(KC_RIGHT);
              break;
            case VIM_G:
              if(SHIFTED) {
                TAP(KC_END);
              } else {
                vstate = VIM_G;
              }
              break;
            case VIM_H:
              PRESS(KC_LEFT);
              break;
            case VIM_I:
              if(SHIFTED){
                CTRL(KC_A);
              }
              EDIT;
              break;
            case VIM_J:
              if(SHIFTED) {
                CMD(KC_RIGHT);
                TAP(KC_DEL);
              } else {
                PRESS(KC_DOWN);
              }
              break;
            case VIM_K:
              PRESS(KC_UP);
              break;
            case VIM_L:
              PRESS(KC_RIGHT);
              break;
            case VIM_O:
              if(SHIFTED) {
                CMD(KC_LEFT);
                TAP(KC_ENTER);
                TAP(KC_UP);
                EDIT;
              } else {
                CMD(KC_RIGHT);
                TAP(KC_ENTER);
                EDIT;
              }
              break;
            case VIM_P:
              if(SHIFTED) {
                CMD(KC_LEFT);
                CMD(KC_V);
              } else {
                if(yank_was_lines) {
                  CMD(KC_RIGHT);
                  TAP(KC_RIGHT);
                  CMD(KC_V);
                } else {
                  CMD(KC_V);
                }
              }
              break;
            case VIM_S:
              // s for substitute?
              if(SHIFTED) {
                CMD(KC_LEFT);
                PRESS(KC_LSHIFT);
                  CMD(KC_RIGHT);
                RELEASE(KC_LSHIFT);
                CMD(KC_X);
                yank_was_lines = false;
                EDIT;
              } else {
                SHIFT(KC_RIGHT);
                CMD(KC_X);
                yank_was_lines = false;
                EDIT;
              }
              break;
            case VIM_U:
              if(SHIFTED) {
                PRESS(KC_LSFT);
                  CMD(KC_Z);
                RELEASE(KC_LSHIFT);
              } else {
                CMD(KC_Z);
              }
              break;
            case VIM_V:
              if(SHIFTED) {
                CMD(KC_LEFT);
                SHIFT(KC_DOWN);
                vstate = VIM_VS;
              } else {
                vstate = VIM_V;
              }
              break;
            case VIM_W:
              PRESS(KC_LALT);
                TAP(KC_RIGHT);
                TAP(KC_RIGHT);
                TAP(KC_LEFT);
              RELEASE(KC_LALT);
              break;
            case VIM_X:
              // SHIFT(KC_RIGHT);
              // CMD(KC_X);
              PRESS(KC_DEL);
              break;
            case VIM_Y:
              if(SHIFTED) {
                CMD(KC_LEFT);
                SHIFT(KC_DOWN);
                CMD(KC_C);
                TAP(KC_RIGHT);
                yank_was_lines = true;
              } else {
                vstate = VIM_Y;
              }
              break;
            case VIM_COMMA:
            case VIM_PERIOD:
              comma_period(keycode);
              break;
          }
          break;
        case VIM_C:
          /*****************************
           * c-  ...for change. I never use this...
           *****************************/
          switch(keycode) {
          case VIM_B:
          case VIM_E:
          case VIM_H:
          case VIM_J:
          case VIM_K:
          case VIM_L:
          case VIM_W:
            simple_movement(keycode);
            CMD(KC_X);
            yank_was_lines = false;
            EDIT;
            break;

          case VIM_C:
            CMD(KC_LEFT);
            PRESS(KC_LSHIFT);
              CMD(KC_RIGHT);
            RELEASE(KC_LSHIFT);
            CMD(KC_X);
            yank_was_lines = false;
            EDIT;
            break;
          case VIM_I:
            vstate = VIM_CI;
            break;
          default:
            vstate = VIM_START;
            break;
          }
          break;
        case VIM_CI:
          /*****************************
           * ci-  ...change inner word
           *****************************/
          switch(keycode) {
            case VIM_W:
              ALT(KC_LEFT);
              PRESS(KC_LSHIFT);
                ALT(KC_RIGHT);
              RELEASE(KC_LSHIFT);
              CMD(KC_X);
              yank_was_lines = false;
              EDIT;
            default:
              vstate = VIM_START;
              break;
          }
          break;
        case VIM_D:
          /*****************************
           * d-  ...delete stuff
           *****************************/
          switch(keycode) {
          case VIM_B:
          case VIM_E:
          case VIM_H:
          case VIM_J:
          case VIM_K:
          case VIM_L:
          case VIM_W:
            simple_movement(keycode);
            CMD(KC_X);
            yank_was_lines = false;
            vstate = VIM_START;
            break;
          case VIM_D:
            CMD(KC_LEFT);
            SHIFT(KC_DOWN);
            CMD(KC_X);
            yank_was_lines = true;
            vstate = VIM_START;
            break;
          case VIM_I:
            vstate = VIM_DI;
            break;
          default:
            vstate = VIM_START;
            break;
          }
          break;
        case VIM_DI:
          /*****************************
           * ci-  ...delete a word... FROM THE INSIDE!
           *****************************/
          switch(keycode) {
            case VIM_W:
              ALT(KC_LEFT);
              PRESS(KC_LSHIFT);
                ALT(KC_RIGHT);
              RELEASE(KC_LSHIFT);
              CMD(KC_X);
              yank_was_lines = false;
              vstate = VIM_START;
            default:
              vstate = VIM_START;
              break;
          }
          break;
        case VIM_V:
          /*****************************
           * visual!
           *****************************/
          switch(keycode) {
            case VIM_D:
            case VIM_X:
              CMD(KC_X);
              yank_was_lines = false;
              vstate = VIM_START;
              break;
            case VIM_B:
              PRESS(KC_LALT);
              PRESS(KC_LSHIFT);
              PRESS(KC_LEFT);
              // leave open for key repeat
              break;
            case VIM_E:
              PRESS(KC_LALT);
              PRESS(KC_LSHIFT);
              PRESS(KC_RIGHT);
              // leave open for key repeat
              break;
            case VIM_H:
              PRESS(KC_LSHIFT);
              PRESS(KC_LEFT);
              break;
            case VIM_I:
              vstate = VIM_VI;
              break;
            case VIM_J:
              PRESS(KC_LSHIFT);
              PRESS(KC_DOWN);
              break;
            case VIM_K:
              PRESS(KC_LSHIFT);
              PRESS(KC_UP);
              break;
            case VIM_L:
              PRESS(KC_LSHIFT);
              PRESS(KC_RIGHT);
              break;
            case VIM_W:
              PRESS(KC_LALT);
              SHIFT(KC_RIGHT); // select to end of this word
              SHIFT(KC_RIGHT); // select to end of next word
              SHIFT(KC_LEFT);  // select to start of next word
              RELEASE(KC_LALT);
              break;
            case VIM_P:
              CMD(KC_V);
              vstate = VIM_START;
              break;
            case VIM_Y:
              CMD(KC_C);
              TAP(KC_RIGHT);
              yank_was_lines = false;
              vstate = VIM_START;
              break;
            case VIM_V:
            case VIM_ESC:
              TAP(KC_RIGHT);
              vstate = VIM_START;
              break;
            case VIM_COMMA:
            case VIM_PERIOD:
              comma_period(keycode);
              break;
            default:
              // do nothing
              break;
          }
          break;
        case VIM_VI:
          /*****************************
           * vi-  ...select a word... FROM THE INSIDE!
           *****************************/
          switch(keycode) {
            case VIM_W:
              ALT(KC_LEFT);
              PRESS(KC_LSHIFT);
                ALT(KC_RIGHT);
              RELEASE(KC_LSHIFT);
              vstate = VIM_V;
            default:
              // ignore
              vstate = VIM_V;
              break;
          }
          break;
        case VIM_VS:
          /*****************************
           * visual line
           *****************************/
          switch(keycode) {
            case VIM_D:
            case VIM_X:
              CMD(KC_X);
              yank_was_lines = true;
              vstate = VIM_START;
              break;
            case VIM_J:
              PRESS(KC_LSHIFT);
              PRESS(KC_DOWN);
              break;
            case VIM_K:
              PRESS(KC_LSHIFT);
              PRESS(KC_UP);
              break;
            case VIM_Y:
              CMD(KC_C);
              yank_was_lines = true;
              TAP(KC_RIGHT);
              vstate = VIM_START;
              break;
            case VIM_P:
              CMD(KC_V);
              vstate = VIM_START;
              break;
            case VIM_V:
            case VIM_ESC:
              TAP(KC_RIGHT);
              vstate = VIM_START;
              break;
            case VIM_COMMA:
            case VIM_PERIOD:
              comma_period(keycode);
              break;
            default:
              // do nothing
              break;
          }
          break;
        case VIM_G:
          /*****************************
           * gg, and a grab-bag of other macros i find useful
           *****************************/
          switch(keycode) {
            case VIM_G:
              TAP(KC_HOME);
              break;
            // codes b
            case VIM_H:
              CTRL(KC_A);
              break;
            case VIM_J:
              PRESS(KC_PGDN);
              break;
            case VIM_K:
              PRESS(KC_PGUP);
              break;
            case VIM_L:
              CTRL(KC_E);
              break;
            default:
              // do nothing
              break;
          }
          vstate = VIM_START;
          break;
        case VIM_Y:
          /*****************************
           * yoink!
           *****************************/
          switch(keycode) {
          case VIM_B:
          case VIM_E:
          case VIM_H:
          case VIM_J:
          case VIM_K:
          case VIM_L:
          case VIM_W:
            simple_movement(keycode);
            CMD(KC_C);
            TAP(KC_RIGHT);
            yank_was_lines = false;
            break;
          case VIM_Y:
            CMD(KC_LEFT);
            SHIFT(KC_DOWN);
            CMD(KC_C);
            TAP(KC_RIGHT);
            yank_was_lines = true;
            break;
          default:
            // NOTHING
            break;
          }
          vstate = VIM_START;
          break;
        }
    } else {
      /************************
       * key release events
       ************************/
      clear_keyboard();
    }
    return false;
  } else {
    return PASS_THRU;
  }
}


__attribute__ ((weak))
void matrix_scan_keymap(void) {
  // override me, if you want.
  return;
}



#define ABSDIFF(a,b) ((a)>(b)?(a)-(b):(b)-(a))
#define MIN(a,b) ((a)>(b)?(b):(a))
#define MAX(a,b) ((a)<(b)?(b):(a))

#define FADE_BACK_TIME 500
#define BREATH_FIRE_TIME 800
#define ANIMATION_STEP_INTERVAL 20
#define POWER_KEY_OFFSET (RGBLED_NUM / 2)
#define SPACE_OFFSET_MAX (RGBLED_NUM / 2)


uint8_t user_rgb_mode = 0;
LED_TYPE last_led_color = {0};
uint16_t effect_start_timer = 0;

void start_breath_fire(void) {
  user_rgb_mode = BREATH_FIRE;
  effect_start_timer = timer_read();
  last_led_color = led[9];
}

/** 0---max
 *  [___]
 *  [__/]
 *  [_/\]
 *  [/\_]
 *  [\__]
 *  [___]
 **/

/** plain blackbody
  { 0x38, 0xff, 0x00 },
  { 0x7e, 0xff, 0x00 },
  { 0xa5, 0xff, 0x4f },
  { 0xc1, 0xff, 0x84 },
  { 0xd5, 0xff, 0xad },
  { 0xe4, 0xff, 0xce },
  { 0xf0, 0xff, 0xe9 },
  { 0xf9, 0xfe, 0xff }
*/

/** hand-tuned
  { 0x20, 0xff, 0x00 },
  // hold
  { 0x20, 0xff, 0x00 },
  { 0x38, 0xff, 0x00 },
  { 0x7e, 0xff, 0x20 },
  { 0xa5, 0xff, 0x20 },
  { 0xc1, 0xff, 0x4f },
  { 0xd5, 0xff, 0x84 },
  // { 0xf0, 0xff, 0xe9 },
  // { 0xf9, 0xfe, 0xff }
 *
 * */



/** black body radiation curve w/ value normalized to 1 */
/* laid out as g,r,b for some reason? */
static LED_TYPE black_body_hs[] = {
  { 0x00, 0x00, 0x00 },
  { 0x00, 0x66, 0x02 },
  { 0x2f, 0xc5, 0x0a },
  { 0x5d, 0xff, 0x13 },
  { 0x7d, 0xff, 0x13 },
  { 0x9e, 0xff, 0x13 },
  { 0xd0, 0xff, 0x8c },
  { 0xe9, 0xff, 0x67 },
  { 0xe9, 0xff, 0x67 },
  { 0xe9, 0xff, 0x67 }
};

void set_color_for_offsets(uint16_t time_offset, uint16_t space_offset, LED_TYPE *target_led) {
  // LED_TYPE target = {0xFF, 00, 00};
  float time_progress = (float)time_offset / BREATH_FIRE_TIME -0.0;
  float space_progress = (float)space_offset / SPACE_OFFSET_MAX;
  float progress = time_progress * 7.0 - space_progress;
  if(progress > 1.0) {
    progress -= 1.0;
    progress /= 2.0;
    progress = 1.0 - progress;
    // progress = 2.0 - progress;
  }
  // progress = MAX(0,MIN(1.0,progress));
  progress = MAX(0.0,progress);
  progress *= progress; // squared!

  float flidx = progress * (sizeof(black_body_hs)/sizeof(*black_body_hs) - 1);
  LED_TYPE lower = black_body_hs[(uint8_t)floor(flidx)];
  float mix = 1.0 - (flidx - floor(flidx));
  LED_TYPE higher = black_body_hs[(uint8_t)ceil(flidx)];

  target_led->r = /* progress * */ (mix * lower.r + (1.0 - mix) * higher.r);
  target_led->g = /* progress * */ (mix * lower.g + (1.0 - mix) * higher.g);
  target_led->b = /* progress * */ (mix * lower.b + (1.0 - mix) * higher.b);

  if(target_led == &led[3]) xprintf("progress %u = %u : %u [%X,%X,%X] (%u) -- %u, %u\n",
      (uint16_t)(100*progress),
      (uint16_t)(100*time_progress),
      (uint16_t)(100*space_progress),
      (uint16_t)target_led->r,
      (uint16_t)target_led->g,
      (uint16_t)target_led->b,
      time_offset,
      (uint16_t)(100*flidx),
      (uint16_t)(100*mix)
    );
}

void rgb_mode_breath_fire(void) {
  static uint16_t last_timer = 0;
  if(!last_timer) last_timer = timer_read();
  uint16_t this_timer = timer_read();

  // too soon. don't spam updates
  if(this_timer - last_timer < ANIMATION_STEP_INTERVAL) return;

  uint16_t elapsed = this_timer - effect_start_timer;
  // xprintf("breath %d\n", elapsed);

  last_timer = this_timer;
  if(elapsed >= BREATH_FIRE_TIME) {
    // complete
    user_rgb_mode = FADE_BACK;
    effect_start_timer = this_timer;
  } else {
    // linear fade
    for(uint16_t i = 0; i < RGBLED_NUM; i++) {
      uint16_t space_offset = ABSDIFF(i,POWER_KEY_OFFSET);
      if(space_offset > SPACE_OFFSET_MAX) space_offset = RGBLED_NUM - space_offset;

      set_color_for_offsets(elapsed, space_offset, &led[i]);
    }
    rgblight_set();
  }
}

void rgb_mode_fade_back(void) {
  static uint16_t last_timer = 0;
  if(!last_timer) last_timer = timer_read();
  uint16_t this_timer = timer_read();

  // too soon. don't spam updates
  if(this_timer - last_timer < ANIMATION_STEP_INTERVAL) return;

  uint16_t elapsed = this_timer - effect_start_timer;
  xprintf("fade %d\n", elapsed);

  last_timer = this_timer;
  if(elapsed >= FADE_BACK_TIME) {
    // complete
    rgblight_setrgb(last_led_color.r, last_led_color.g, last_led_color.b);
    user_rgb_mode = 0;
  } else {
    // linear fade
    rgblight_setrgb(
      (uint8_t)(((uint32_t)last_led_color.r) * elapsed / FADE_BACK_TIME),
      (uint8_t)(((uint32_t)last_led_color.g) * elapsed / FADE_BACK_TIME),
      (uint8_t)(((uint32_t)last_led_color.b) * elapsed / FADE_BACK_TIME));
  }
}


void matrix_scan_user(void) {
  switch (user_rgb_mode) {
    case BREATH_FIRE:
      rgb_mode_breath_fire();
      break;
    case FADE_BACK:
      rgb_mode_fade_back();
      break;
  }
}
