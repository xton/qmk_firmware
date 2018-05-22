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
#include "color.h"

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


uint16_t vstate = VIM_START;
static bool yank_was_lines = false;
static bool SHIFTED = false;
static uint32_t mod_override_layer_state = 0;
static uint16_t mod_override_triggering_key = 0;
void start_firey_return(void);

static void edit(void) { vstate = VIM_START; layer_clear(); }
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

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  /* keymap gets first whack */
  if(!process_record_keymap(keycode, record)) return false;

  /****** FIREY_RETURN *****/
  if(record->event.pressed && keycode == FIREY_RETURN) {
    start_firey_return();
    TAP(KC_ENT);
  }

  /****** mod passthru *****/
  if(record->event.pressed && layer_state_is(vim_cmd_layer()) && (IS_MOD(keycode) || keycode == LSFT(KC_LALT))) {
    mod_override_layer_state = layer_state;
    mod_override_triggering_key = keycode;
    // TODO: change this to track key location instead
    layer_clear();
    return true; // let the event fall through...
  }
  if(mod_override_layer_state && !record->event.pressed && keycode == mod_override_triggering_key) {
    layer_state_set(mod_override_layer_state);
    mod_override_layer_state = 0;
    mod_override_triggering_key = 0;
    return true;
  }

  if (VIM_START <= keycode && keycode <= VIM_ESC) {
    if(keycode == VIM_SHIFT) {
      SHIFTED = record->event.pressed;
      return false;
    }

    if (record->event.pressed) {
      if(keycode == VIM_START) {
        // entry from anywhere
        layer_on(vim_cmd_layer());
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
    return true;
  }
}

__attribute__ ((weak))
void matrix_scan_keymap(void) {
  // override me, if you want.
  return;
}

#define ABSDIFF(a,b) ((a)>(b)?(a)-(b):(b)-(a))
#ifdef MAX
#undef MAX
#endif
#ifdef MIN
#undef MIN
#endif
#define MIN(a,b) ((a)>(b)?(b):(a))
#define MAX(a,b) ((a)<(b)?(b):(a))

#define FADE_BACK_TIME 500
#define BREATH_FIRE_TIME 1000
#define ANIMATION_STEP_INTERVAL 20
#define POWER_KEY_OFFSET (RGBLED_NUM / 2)
#define SPACE_OFFSET_MAX (RGBLED_NUM / 2)


uint8_t user_rgb_mode = 0;
uint16_t effect_start_timer = 0;
LED_TYPE shadowed_led[RGBLED_NUM] = {0};

void start_firey_return(void) {
  user_rgb_mode = BREATH_FIRE;
  effect_start_timer = timer_read();
  for(uint8_t i = 0; i < RGBLED_NUM; i++) {
    shadowed_led[i] = led[i];
  }
}

/** 0---max
 *  [___]
 *  [__/]
 *  [_/\]
 *  [/\_]
 *  [\__]
 *  [___]
 **/

// #define FIRE_GRADIENT
#ifdef FIRE_GRADIENT
static LED_TYPE firey_gradient[] = {
  { .r = 0x00, .g = 0x00, .b = 0x00 },
  { .r = 0x66, .g = 0x00, .b = 0x02 },
  { .r = 0xc5, .g = 0x2f, .b = 0x0a },
  { .r = 0xff, .g = 0x5d, .b = 0x13 },
  { .r = 0xff, .g = 0x7d, .b = 0x13 },
  { .r = 0xff, .g = 0x9e, .b = 0x13 },
  { .r = 0xff, .g = 0xd0, .b = 0x8c },
  { .r = 0xff, .g = 0xe9, .b = 0x67 },
  { .r = 0xff, .g = 0xe9, .b = 0x67 },
  { .r = 0xff, .g = 0xe9, .b = 0x67 }
};
#endif

void set_color_for_offsets(uint16_t time_offset, uint16_t space_offset, uint8_t idx) {
  float time_progress = (float)time_offset / BREATH_FIRE_TIME;
  float space_progress = (float)space_offset / SPACE_OFFSET_MAX;
  float progress = time_progress * 5.0 - space_progress;
  if(progress > 1.0) {
    progress -= 1.0;
    progress /= 4.0;
    progress = 1.0 - progress;
  }
  progress = MAX(0.0,progress);
  progress *= progress; // squared!

  float alpha = (time_progress + 0.1) * 7.0 - space_progress;
  alpha = MIN(1.0, alpha*alpha);

#ifdef FIRE_GRADIENT

  float flidx = progress * (sizeof(firey_gradient)/sizeof(*firey_gradient) - 1);
  LED_TYPE lower = firey_gradient[(uint8_t)floor(flidx)];
  float mix = 1.0 - (flidx - floor(flidx));
  LED_TYPE higher = firey_gradient[(uint8_t)ceil(flidx)];

  led[idx].r = alpha * (mix * lower.r + (1.0 - mix) * higher.r) + ( 1.0 - alpha) * shadowed_led[idx].r;
  led[idx].g = alpha * (mix * lower.g + (1.0 - mix) * higher.g) + ( 1.0 - alpha) * shadowed_led[idx].g;
  led[idx].b = alpha * (mix * lower.b + (1.0 - mix) * higher.b) + ( 1.0 - alpha) * shadowed_led[idx].b;
#else
  LED_TYPE px[1] = {0};
  sethsv((uint16_t)(fmod(time_progress * 1.5 + space_progress,1.0)*360), 255, (uint8_t)(progress*255),&px[0]);
  led[idx].r = alpha * px[0].r + ( 1.0 - alpha) * shadowed_led[idx].r;
  led[idx].g = alpha * px[0].g + ( 1.0 - alpha) * shadowed_led[idx].g;
  led[idx].b = alpha * px[0].b + ( 1.0 - alpha) * shadowed_led[idx].b;
#endif
}

void rgb_mode_breath_fire(void) {
  static uint16_t last_timer = 0;
  if(!last_timer) last_timer = timer_read();
  uint16_t this_timer = timer_read();

  // too soon. don't spam updates
  if(this_timer - last_timer < ANIMATION_STEP_INTERVAL) return;

  uint16_t elapsed = this_timer - effect_start_timer;

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

      set_color_for_offsets(elapsed, space_offset, i);
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

  last_timer = this_timer;
  float progress = (float)elapsed / FADE_BACK_TIME;
  progress = MIN(1.0,progress);

  for(uint8_t i = 0; i < RGBLED_NUM; i++) {
    led[i].r = shadowed_led[i].r * progress;
    led[i].g = shadowed_led[i].g * progress;
    led[i].b = shadowed_led[i].b * progress;
  }
  rgblight_set();

  if(elapsed >= FADE_BACK_TIME) user_rgb_mode = 0;
}

/** called when layer state or vstate has changed */
__attribute__ ((weak))
void set_state_leds(void) {
  return;
}

void matrix_scan_user(void) {
  static uint32_t last_layer = 0;
  static uint32_t last_vstate = 0;
  if(last_layer != layer_state || last_vstate != vstate) set_state_leds();
  last_layer = layer_state;
  last_vstate = vstate;

  switch (user_rgb_mode) {
    case BREATH_FIRE:
      rgb_mode_breath_fire();
      break;
    case FADE_BACK:
      rgb_mode_fade_back();
      break;
  }
  matrix_scan_keymap();
}
