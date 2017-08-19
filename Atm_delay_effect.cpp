#include "Atm_delay_effect.h"
#include "audio_system.h"
#include "Atm_sev_seg.h"

/* Add optional parameters for the state machine to begin()
 * Add extra initialization code
 */

int effect_select;
int param_select;
int send_1_level;
int send_2_level;
int mix_level;
int time_1_level;
int fb_1_level;

extern Atm_sev_seg displayMain;

Atm_delay_effect& Atm_delay_effect::begin() {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*               	ON_ENTER  ON_LOOP  ON_EXIT  EVT_BTN_1  EVT_BTN_2  EVT_ENC_UP_1  EVT_ENC_DOWN_1  EVT_ENC_UP_2  EVT_ENC_DOWN_2  	EVT_TIMER_1		EVT_TIMER_2		ELSE */
    /*        OFF */     ENT_OFF,   -1,      -1,   		-1,     	-1,  		-1,          	-1,     		-1,         	-1,   		-1,             -1,   		-1,
    /*       HOME */     ENT_HOME,  -1,      -1,   DISPLAY_1,     DISPLAY_1,  DISPLAY_1,          DISPLAY_1,     	DISPLAY_1,         DISPLAY_1,   	-1,             -1,   		-1,   
	/*    DISPLAY_1 */   ENT_DISPLAY, -1,      -1,        -1,        -1,           -1,             -1,           -1,             -1,   	EXIT_DISPLAY,       -1,   		-1,	  
    /*         ON */       -1,      -1,      -1,        -1,        -1,           -1,             -1,           -1,             -1,   		-1,             -1,   		-1,
    /*     SEND_1 */       -1,      -1,      -1,        -1,        -1,           -1,             -1,           -1,             -1,   		-1,             -1,   		-1,
    /*     SEND_2 */       -1,      -1,      -1,        -1,        -1,           -1,             -1,           -1,             -1,   		-1,             -1,   		-1,
    /*        MIX */       -1,      -1,      -1,        -1,        -1,           -1,             -1,           -1,             -1,   		-1,             -1,   		-1,
    /*     TIME_1 */       -1,      -1,      -1,        -1,        -1,           -1,             -1,           -1,             -1,   		-1,             -1,   		-1,
    /* FEEDBACK_1 */       -1,      -1,      -1,        -1,        -1,           -1,             -1,           -1,             -1,   		-1,             -1,   		-1,
    /*     TIME_2 */       -1,      -1,      -1,        -1,        -1,           -1,             -1,           -1,             -1,   		-1,             -1,   		-1,
    /* FEEDBACK_2 */       -1,      -1,      -1,        -1,        -1,           -1,             -1,           -1,             -1,   		-1,             -1,   		-1,
    /*     TIME_3 */       -1,      -1,      -1,        -1,        -1,           -1,             -1,           -1,             -1,   		-1,             -1,   		-1,
    /* FEEDBACK_3 */       -1,      -1,      -1,        -1,        -1,           -1,             -1,           -1,             -1,   		-1,             -1,   		-1,
    /*     TIME_4 */       -1,      -1,      -1,        -1,        -1,           -1,             -1,           -1,             -1,   		-1,             -1,   		-1,
    /* FEEDBACK_4 */       -1,      -1,      -1,        -1,        -1,           -1,             -1,           -1,             -1,	   		-1,             -1,   		-1,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  mixer3.gain(0, 0.0);
  mixer3.gain(1, 0.0);
  mixer9.gain(1, 0.0);
  return *this;
  //display_timer.set(100);
  //param_timer.set(2000);          
}

/* Add C++ code for each internally handled event (input) 
 * The code must return 1 to trigger the event
 */

int Atm_delay_effect::event( int id ) {
  switch ( id ) {
    case EVT_BTN_1:
      return 0;
    case EVT_BTN_2:
      return 0;
    case EVT_ENC_UP_1:
      return 0;
    case EVT_ENC_DOWN_1:
      return 0;
    case EVT_ENC_UP_2:
      return 0;
    case EVT_ENC_DOWN_2:
      return 0;
    case EVT_TIMER_1:
      return display_timer.expired( this );
	case EVT_TIMER_2:
	  return param_timer.expired( this );
  }
  return 0;
}

/* Add C++ code for each action
 * This generates the 'output' for the state machine
 */

void Atm_delay_effect::action( int id ) {
  switch ( id ) {
	case ENT_OFF:
      mixer3.gain(0, 0.0);
      mixer3.gain(1, 0.0);
      mixer9.gain(1, 0.0);
	  effect_select = 0;
	  return;
  	case ENT_DISPLAY:
	  display_timer.set(100);
	  
  	  return;
   	case EXIT_DISPLAY:
		if ( param_select == 1){
			trigger(SEND_1);
		}
      return;
    case ENT_HOME:
  	  	display_timer.set(100);
		return;
  }
}

/* Optionally override the default trigger() method
 * Control how your machine processes triggers
 */

Atm_delay_effect& Atm_delay_effect::trigger( int event ) {
  Machine::trigger( event );
  return *this;
}

/* Optionally override the default state() method
 * Control what the machine returns when another process requests its state
 */

int Atm_delay_effect::state( void ) {
  return Machine::state();
}

/* Nothing customizable below this line                          
 ************************************************************************************************
*/

/* Public event methods
 *
 */

Atm_delay_effect& Atm_delay_effect::btn_1() {
  trigger( EVT_BTN_1 );
  return *this;
}

Atm_delay_effect& Atm_delay_effect::btn_2() {
  trigger( EVT_BTN_2 );
  return *this;
}

Atm_delay_effect& Atm_delay_effect::enc_up_1() {
  trigger( EVT_ENC_UP_1 );
  return *this;
}

Atm_delay_effect& Atm_delay_effect::enc_down_1() {
  trigger( EVT_ENC_DOWN_1 );
  return *this;
}

Atm_delay_effect& Atm_delay_effect::enc_up_2() {
  trigger( EVT_ENC_UP_2 );
  return *this;
}

Atm_delay_effect& Atm_delay_effect::enc_down_2() {
  trigger( EVT_ENC_DOWN_2 );
  return *this;
}

/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

Atm_delay_effect& Atm_delay_effect::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "DELAY_EFFECT\0EVT_BTN_1\0EVT_BTN_2\0EVT_ENC_UP_1\0EVT_ENC_DOWN_1\0EVT_ENC_UP_2\0EVT_ENC_DOWN_2\0ELSE\0OFF\0ON\0SEND_1\0SEND_2\0MIX\0TIME_1\0FEEDBACK_1\0TIME_2\0FEEDBACK_2\0TIME_3\0FEEDBACK_3\0TIME_4\0FEEDBACK_4" );
  return *this;
}



