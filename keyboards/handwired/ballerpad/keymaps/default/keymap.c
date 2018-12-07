#include "ballerpad.h"
#include <print.h>
#include <mousekey.h>

/* static report_mouse_t mouse_report = {}; */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* {{ KC_A, KC_B, KC_C, KC_D }} */
    {{ KC_MS_BTN1 }}
};

/* bool process_record_user(uint16_t keycode, keyrecord_t *record) { */
/* 	// up or down, just send one key event */	
/* 	xprintf("oh hai %d\n", keycode); */
/* 	/1* print(keycode); *1/ */
/* 	/1* print(record->event.pressed); *1/ */
/* 	register_code(keycode); */
/* 	unregister_code(keycode); */
/* 	return false; */
/* } */


#define pin_count 4
uint16_t pins_were[pin_count] = { 0, 0, 0, 0 };
uint16_t pins[pin_count] = { B8, B3, B4, B5 };
uint16_t pins_kc[pin_count] = { KC_UP, KC_DOWN, KC_LEFT, KC_RIGHT };
#define mouse_step 20

void matrix_init_user() {
	for(int i = 0; i < pin_count; i++){
		setPinInputHigh(pins[i]);
	}

}
#define TAP(kc) do { register_code(kc); unregister_code(kc); } while (0)

void matrix_scan_user() {
	for(int i = 0; i < pin_count; i++){
		uint16_t on = readPin(pins[i]);
		if(on != pins_were[i]) {
#if 0
			/* xprintf("I changed: %d - %d\n",i, on); */
			/* register_code(pins_kc[i]); */
			/* unregister_code(pins_kc[i]); */
			switch(i){
				case 0: 
					// up
					 mouse_report.y = -mouse_step;
					 break;
				case 1: 
					// up
					 mouse_report.y = mouse_step;
					 break;
				case 2: 
					// up
					 mouse_report.x = -mouse_step;
					 break;
				case 3: 
					// up
					 mouse_report.x = mouse_step;
					 break;
			}
			host_mouse_send(&mouse_report);
			mouse_report.x = 0;
			mouse_report.y = 0;
			host_mouse_send(&mouse_report);
#else 
			TAP(pins_kc[i]);
#endif
		}
		pins_were[i] = on;

	}
}
