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

#include QMK_KEYBOARD_H
#include QMK_KEYBOARD_CONFIG_H
#ifdef TRACKBALL_ENABLED


#include <mousekey.h>
#include "xtonhasvim.h"
#include "trackball.h"
#include "wait.h"

#ifdef __arm__
#include "hal.h"
#include "hal_pal.h"
//#include "stm32_registry.h"
#endif /* __arm__ */


static report_mouse_t mouse_report = {0};
uint8_t tb_ball_mode = TB_MODE_MOUSE;

#define TAP(kc) do { register_code(kc); unregister_code(kc); } while (0)
#define wait_between_moves 1

#define M_CONCAT_(a,b) a ## b
#define M_CONCAT(a,b) M_CONCAT_(a,b)
static uint32_t ipins[] = { 
  M_CONCAT(TB_PAD_UP,TB_LINE_UP),
  M_CONCAT(TB_PAD_DN,TB_LINE_DN),
  M_CONCAT(TB_PAD_LT,TB_LINE_LT),
  M_CONCAT(TB_PAD_RT,TB_LINE_RT) };
static uint32_t ipins_were[4] = { 0, 0, 0, 0 };

static uint32_t deltas[4] = {0, 0, 0, 0};
#define ddy deltas[0]
#define diy deltas[1]
#define ddx deltas[2]
#define dix deltas[3]
#define dx (dix - ddx)
#define dy (diy - ddy)

/** time since last movement in that direction */
static int32_t iix = 0;
static int32_t idx = 0;
static int32_t iiy = 0;
static int32_t idy = 0;
static int32_t since_last = 0;
static bool has_moved = false;

#ifdef TB_USE_MATRIX_ROW
static void poll_column_pins(void){
  writePinHigh(TB_MATRIX_ROW);
  wait_us(2000);
  for(int i = 0; i < 4; i++){ 
    uint16_t on = readPin(ipins[i]);
    if(on != ipins_were[i]) {
      xprintf("OH SHIRT - %d -> %d\n",i, deltas[i]);
      deltas[i]++;	
      has_moved=true;
    }
    ipins_were[i] = on;
  }
  writePinLow(TB_MATRIX_ROW);
  wait_us(2000);

}
#else 
/** poll the various pins for differences in voltages. 
 * increment the appropriate delta if any is found 
 */
static void poll_pins(void) {
  for(int i = 0; i < 4; i++){ 
    uint16_t on = readPin(ipins[i]);
    if(on != ipins_were[i]) {
      xprintf("OH SHIT - %d -> %d\n",i, deltas[i]);
      deltas[i]++;	
      has_moved=true;
    }
    ipins_were[i] = on;
  }
}
#endif /* TB_USE_MATRIX_ROW */

#ifdef TB_INTERRUPT_ENABLED

#ifdef __arm__
EXTConfig extConfig = {{{0}}};

static void ballMoved(EXTDriver *extp, expchannel_t channel) {
  switch(channel) {
    case TB_LINE_UP: ddy++; break;
    case TB_LINE_DN: diy++; break;
    case TB_LINE_LT: ddx++; break;
    case TB_LINE_RT: dix++; break;
  }
  has_moved=true;
}
#endif /* __arm__ */
#ifdef __AVR__


ISR(PCINT0_vect){
  poll_pins();
}

#endif /* __AVR__ */

#endif /* TB_INTERRUPT_ENABLED */

void matrix_init_trackball(void) {
#ifdef TB_USE_MATRIX_ROW
  setPinOutput(TB_MATRIX_ROW);
#endif

#ifdef TB_INTERRUPT_ENABLED
#ifdef __arm__

	osalSysLock();

#define C_GPIO(x) GPIO ## x
#define C_EXT_MODE_GPIO(x) EXT_MODE_GPIO ## x

#define BALL_PIN(PAD,LINE) palSetGroupMode(C_GPIO(PAD), PAL_PORT_BIT(LINE), 0, PAL_MODE_INPUT); \
		extConfig.channels[LINE].mode = EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART | C_EXT_MODE_GPIO(PAD); \
		extConfig.channels[LINE].cb = ballMoved
  BALL_PIN(TB_PAD_UP,TB_LINE_UP);
  BALL_PIN(TB_PAD_DN,TB_LINE_DN);
  BALL_PIN(TB_PAD_LT,TB_LINE_LT);
  BALL_PIN(TB_PAD_RT,TB_LINE_RT);

	extStart(&EXTD1, &extConfig);

	osalSysUnlock();

  /* quelling warning of unused */
  (void)ipins;
  (void)ipins_were;
#endif  /* __arm__ */
#ifdef __AVR__
  /** this assumes the PC interrupts (B pad) */

  // it's not entirely clear that this does anything...
	for(int i = 0; i < 4; i++){
		setPinInputHigh(ipins[i]);
	}
	// enable interrupts
	SREG |= (0x1 << 7);
	// mask out just our target pins
#define PC_(n) PCINT ## n
#define PC(n) PC_(n)
	PCMSK0 |= ((1 << PC(TB_LINE_UP)) | (1 << PC(TB_LINE_DN)) | (1 << PC(TB_LINE_LT)) | (1 << PC(TB_LINE_RT))); 
	// turn on PC int
	PCICR |= (1 << PCIE0);
#endif /* __AVR__ */
#endif /* TB_INTERRUPT_ENABLED */
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

void matrix_scan_trackball(void) {
#ifndef TB_INTERRUPT_ENABLED
#ifdef TB_USE_MATRIX_ROW
  poll_column_pins();
#else
  poll_pins();
#endif
#endif 
  if(has_moved && since_last > wait_between_moves) {
    xprintf("%d, %d [%d]\n", (int)dx, (int)dy, (int)since_last);
    if(tb_ball_mode == TB_MODE_ARROW){
      /** arrow keys */
      for(int i = 0; i < ddy; i++) TAP(KC_UP);
      for(int i = 0; i < diy; i++) TAP(KC_DOWN);
      for(int i = 0; i < ddx; i++) TAP(KC_LEFT);
      for(int i = 0; i < dix; i++) TAP(KC_RIGHT);
    } else if(tb_ball_mode == TB_MODE_SCROLL) {
      /** scroll wheels */
      mouse_report.h = dx;
      mouse_report.v = dy;
      mouse_report.x = mouse_report.y = 0;
      host_mouse_send(&mouse_report);
    } else if(tb_ball_mode == TB_MODE_MOUSE) {
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
    dix=ddx=diy=ddy=0;
    since_last = 0;
    has_moved = false;
  } 
  // these are regular enough to use for timing
  since_last++;
  iix++;
  idx++;
  iiy++;
  idy++;
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

#else /* TRACKBALL_ENABLED */

/** stubs for when trackball is off */
void matrix_scan_trackball(void) {}
void matrix_init_trackball(void) {}
bool process_record_trackball(uint16_t keycode, keyrecord_t *record) { return true; }

#endif /* TRACKBALL_ENABLED */
