#ifndef CONFIG_USER_H
#define CONFIG_USER_H

#include "config_common.h"

#ifdef AUDIO_ENABLE
    #define STARTUP_SONG SONG(PLANCK_SOUND)
    // #define STARTUP_SONG SONG(NO_SOUND)

    #define DEFAULT_LAYER_SONGS { SONG(QWERTY_SOUND), \
                                  SONG(COLEMAK_SOUND), \
                                  SONG(DVORAK_SOUND) \
                                }
#endif

/*
 * MIDI options
 */

/* Prevent use of disabled MIDI features in the keymap */
//#define MIDI_ENABLE_STRICT 1

/* enable basic MIDI features:
   - MIDI notes can be sent when in Music mode is on
*/

#define MIDI_BASIC

/* enable advanced MIDI features:
   - MIDI notes can be added to the keymap
   - Octave shift and transpose
   - Virtual sustain, portamento, and modulation wheel
   - etc.
*/
//#define MIDI_ADVANCED

/* override number of MIDI tone keycodes (each octave adds 12 keycodes and allocates 12 bytes) */
//#define MIDI_TONE_KEYCODE_OCTAVES 2

// Most tactile encoders have detents every 4 stages
#define ENCODER_RESOLUTION 4


/* #ifdef __ARM__ */
/* #define TRACKBALL_ENABLED */

/* // the line for each direction (numeric part of pin) */
/* #define TB_LINE_UP 8 */
/* #define TB_LINE_DN 3 */
/* #define TB_LINE_LT 4 */
/* #define TB_LINE_RT 5 */

/* //the pad for each direction (a letter) */
/* #define TB_PAD_UP B */
/* #define TB_PAD_DN B */
/* #define TB_PAD_LT B */
/* #define TB_PAD_RT B */

/* #endif */

#endif
