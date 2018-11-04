#include "baller.h"
#include <print.h>

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* {{ KC_A, KC_B, KC_C, KC_D }} */
    {{ KC_A}}, {{ KC_B}},{{ KC_C}},{{ KC_D }}
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	// up or down, just send one key event	
	xprintf("oh hai %d\n", keycode);
	/* print(keycode); */
	/* print(record->event.pressed); */
	register_code(keycode);
	unregister_code(keycode);
	return false;
}
