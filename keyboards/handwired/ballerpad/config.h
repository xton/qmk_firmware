/*
Copyright 2015 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CONFIG_H
#define CONFIG_H

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x6464
#define DEVICE_VER      0x0001
#define MANUFACTURER    none
#define PRODUCT         ballerpad
#define DESCRIPTION     test matrix for integrated trackball

/* key matrix size */
#define MATRIX_ROWS 2
#define MATRIX_COLS 4

#define MATRIX_COL_PINS {  F4, F5, F6, F7 }
#define MATRIX_ROW_PINS {  C6, D4 } // D0
#define UNUSED_PINS

/* define if matrix has ghost */
//#define MATRIX_HAS_GHOST

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE    5

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
/* #define LOCKING_SUPPORT_ENABLE */
/* Locking resynchronize hack */
/* #define LOCKING_RESYNC_ENABLE */

/* key combination for command */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)) \
)



/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG

/* disable print */
//#define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT
//#define NO_ACTION_MACRO
//#define NO_ACTION_FUNCTION


// this thing doesn't use mousekeys quite like qmk expects...
/* #define MOUSEKEY_DELAY             0 */
/* #define MOUSEKEY_INTERVAL          1 */
/* #define MOUSEKEY_MAX_SPEED         10 */
/* #define MOUSEKEY_TIME_TO_MAX       0 */


#endif
