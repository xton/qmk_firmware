#include "baller.h"
#include <print.h>
#include <mousekey.h>
#include <util/delay.h>
#include <string.h>
#define delay(us)  _delay_us(us) 

static report_mouse_t mouse_report = {};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* {{ KC_A, KC_B, KC_C, KC_D }} */
    [0] = {{ KC_NO, KC_NO, KC_NO, KC_NO }},
    [1] = {{ KC_NO, KC_NO, KC_NO, KC_NO }}
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

uint8_t vim_cmd_layer(void) { return 1; }

#define pin_count 4
uint16_t pins_were[pin_count] = { 0, 0, 0, 0 };
uint16_t pins[pin_count] = { D1, D0, D4, C6 };
uint16_t pins_kc[pin_count] = { KC_UP, KC_DOWN, KC_LEFT, KC_RIGHT };
#define mouse_step 20
#define ipin_count 4
uint16_t ipins[ipin_count] = { B5, B7, B6, B0 };
uint16_t ipins_were[ipin_count] = {0};
uint32_t deltas[ipin_count] = {0};
bool has_moved = false;

void matrix_init_keymap(void) {
	for(int i = 0; i < pin_count; i++){
		setPinInputHigh(pins[i]);
	}
	// enable interrupts
	SREG |= (0x1 << 7);
	// mask out just B5, B7
	PCMSK0 |= ((1 << PCINT7) | (1 << PCINT5) | (1 << PCINT6) | (1 << PCINT0)); 
	// turn on PC int
	PCICR |= (1 << PCIE0);

}
#define TAP(kc) do { register_code(kc); unregister_code(kc); } while (0)

void matrix_scan_keymap(void) {
	if(has_moved){
		mouse_report.y += mouse_step*deltas[2];
		mouse_report.y -= mouse_step*deltas[1];
		mouse_report.x += mouse_step*deltas[3];
		mouse_report.x -= mouse_step*deltas[0];
		host_mouse_send(&mouse_report);
		memset(&deltas, 0, sizeof(*deltas)*ipin_count);
		mouse_report.x = 0;
		mouse_report.y = 0;
		has_moved = false;
	}
	/* case 0: */ 
	/* 	// up */
	/* 	 mouse_report.y = -mouse_step; */
	/* 	 break; */
	/* case 1: */ 
	/* 	// up */
	/* 	 mouse_report.y = mouse_step; */
	/* 	 break; */
	/* case 2: */ 
	/* 	// up */
	/* 	 mouse_report.x = -mouse_step; */
	/* 	 break; */
	/* case 3: */ 
	/* 	// up */
	/* 	 mouse_report.x = mouse_step; */
	/* 	 break; */
	/* for(int i = 1; i < ipin_count; i++){ */ 
		
	/* } */
	/* for(int i = 0; i < pin_count; i++){ */
	/* 	uint16_t on = readPin(pins[i]); */
	/* 	if(on != pins_were[i]) { */
/* #if 1 */
	/* 		/1* xprintf("I changed: %d - %d\n",i, on); *1/ */
	/* 		/1* register_code(pins_kc[i]); *1/ */
	/* 		/1* unregister_code(pins_kc[i]); *1/ */
	/* 		switch(i){ */
	/* 			case 0: */ 
	/* 				// up */
	/* 				 mouse_report.y = -mouse_step; */
	/* 				 break; */
	/* 			case 1: */ 
	/* 				// up */
	/* 				 mouse_report.y = mouse_step; */
	/* 				 break; */
	/* 			case 2: */ 
	/* 				// up */
	/* 				 mouse_report.x = -mouse_step; */
	/* 				 break; */
	/* 			case 3: */ 
	/* 				// up */
	/* 				 mouse_report.x = mouse_step; */
	/* 				 break; */
	/* 		} */
	/* 		host_mouse_send(&mouse_report); */
	/* 		mouse_report.x = 0; */
	/* 		mouse_report.y = 0; */
	/* 		host_mouse_send(&mouse_report); */
/* #else */ 
	/* 		xprintf("YAY: %d\n",i); */
	/* 		xprintf("delta0: %d\n",deltas[0]); */
	/* 		xprintf("delta1: %d\n",deltas[1]); */
	/* 		/1* TAP(pins_kc[i]); *1/ */
/* #endif */
	/* 	} */
	/* 	pins_were[i] = on; */

	/* } */
	/* xprintf("%d,%d,%d,%d\n",pins[0],pins[1],pins[2],pins[3]); */
	/* delay(1000); */
}



ISR(PCINT0_vect){
  for(int i = 0; i < ipin_count; i++){ 
	uint16_t on = readPin(ipins[i]);
	if(on != ipins_were[i]) {
		  /* xprintf("OH SHIT - %d -> %d\n",i, deltas[i]); */
		deltas[i]++;	
		has_moved=true;
	}
	ipins_were[i] = on;

  }

}
