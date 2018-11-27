 /* Copyright 2015-2018 Christon DeWan
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

#ifdef TRACKBALL_ENABLED

#include <mousekey.h>
#include "xtonhasvim.h"

#ifdef __ARM__
#include "hal.h"
#include "hal_pal.h"
//#include "stm32_registry.h"
#endif /* __ARM__ */


static report_mouse_t mouse_report = {0};

#define TAP(kc) do { register_code(kc); unregister_code(kc); } while (0)

uint32_t pins_were[pin_count] = { 0, 0, 0, 0 };


static int32_t dx = 0;
static int32_t dy = 0;
static int32_t iix = 0;
static int32_t idx = 0;
static int32_t iiy = 0;
static int32_t idy = 0;

#ifdef __ARM__
EXTConfig extConfig = {{{0}}};
uint32_t ext_int_lines[] = { TB_LINE_UP, TB_LINE_DN, TB_LINE_LT, TB_LINE_RT };


void ballMoved(EXTDriver *extp, expchannel_t channel) {
  switch(channel) {
    case TB_LINE_UP: dy--; break;
    case TB_LINE_DN: dy++; break;
    case TB_LINE_LT: dx--; break;
    case TB_LINE_RT: dx++; break;
  }
}
#endif /* __ARM__ */

void matrix_init_trackball(void) {

#ifdef __ARM__

	osalSysLock();

	/* // map line 2 to wire B3 */
  // TODO: may need to make a macro out of this
	palSetGroupMode(GPIO ## TB_PAD_UP, PAL_PORT_BIT(TB_LINE_UP), 0, PAL_MODE_INPUT);
	palSetGroupMode(GPIO ## TB_PAD_DN, PAL_PORT_BIT(TB_LINE_DN), 0, PAL_MODE_INPUT);
	palSetGroupMode(GPIO ## TB_PAD_LT, PAL_PORT_BIT(TB_LINE_LT), 0, PAL_MODE_INPUT);
	palSetGroupMode(GPIO ## TB_PAD_RT, PAL_PORT_BIT(TB_LINE_RT), 0, PAL_MODE_INPUT);

	for(int i = 0; i < 4; i++){
		extConfig.channels[ext_int_lines[i]].mode = 
			EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOB;
		extConfig.channels[ext_int_lines[i]].cb = ballMoved;
	}


	extStart(&EXTD1, &extConfig);

	osalSysUnlock();
#endif /* __ARM__ */
}


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
#endif /* arm */

void matrix_scan_trackball(void) {
  
#ifdef __ARM__

  if((dx || dy) && since_last > wait_between_moves) {
    xprintf("%d, %d [%d]\n", (int)dx, (int)dy, (int)since_last);
    if(IS_LAYER_ON(_LOWER)){
      /** arrow keys */
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
      /** scroll wheels */
      mouse_report.h = dx;
      mouse_report.v = dy;
      mouse_report.x = mouse_report.y = 0;
      host_mouse_send(&mouse_report);
    } else {
      /** mouse movement */
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
#endif /* arm */
}

/** need to reflect mouse buttons in our own mouse tracking too */
bool process_record_trackball(uint16_t keycode, keyrecord_t *record) {
  if(keycode == KC_MS_BTN1) {
    if(record->event.pressed) mouse_report.buttons |= MOUSE_BTN1;
    else mouse_report.buttons &= ~MOUSE_BTN1;
  }
  if(keycode == KC_MS_BTN2) {
    if(record->event.pressed) mouse_report.buttons |= MOUSE_BTN2;
    else mouse_report.buttons &= ~MOUSE_BTN2;
  }
  if(keycode == KC_MS_BTN3) {
    if(record->event.pressed) mouse_report.buttons |= MOUSE_BTN3;
    else mouse_report.buttons &= ~MOUSE_BTN3;
  }
  // and let this fall through for the normal mousekeys mechanism to handle
  return true;
}


#else 

/**
void matrix_scan_trackball(void) {}
void matrix_init_trackball(void) {}
bool process_record_trackball(uint16_t keycode, keyrecord_t *record) { return true; }

#endif /* TRACKBALL_ENABLED */
