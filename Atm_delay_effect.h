#pragma once

#include <Automaton.h>

class Atm_delay_effect: public Machine {

 public:
  enum { OFF, DISPLAY_1, ON, SEND_1, SEND_2, MIX, TIME_1, FEEDBACK_1, TIME_2, FEEDBACK_2, TIME_3, FEEDBACK_3, TIME_4, FEEDBACK_4 }; // STATES
  enum { EVT_BTN_1, EVT_BTN_2, EVT_ENC_UP_1, EVT_ENC_DOWN_1, EVT_ENC_UP_2, EVT_ENC_DOWN_2, EVT_TIMER_1, EVT_TIMER_2, ELSE }; // EVENTS
  Atm_delay_effect( void ) : Machine() {};
  Atm_delay_effect& begin( void );
  Atm_delay_effect& trace( Stream & stream );
  Atm_delay_effect& trigger( int event );
  int state( void );
  Atm_delay_effect& btn_1( void );
  Atm_delay_effect& btn_2( void );
  Atm_delay_effect& enc_up_1( void );
  Atm_delay_effect& enc_down_1( void );
  Atm_delay_effect& enc_up_2( void );
  Atm_delay_effect& enc_down_2( void );

 private:
  enum { ENT_OFF, ENT_DISPLAY, EXIT_DISPLAY, ENT_HOME }; // ACTIONS
  int event( int id ); 
  void action( int id );
  atm_timer_millis display_timer; 
  atm_timer_millis param_timer;

};
