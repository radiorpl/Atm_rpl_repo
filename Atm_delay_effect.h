#pragma once

#include <Automaton.h>

class Atm_delay_effect: public Machine {

 public:
  enum { CONTROL, ENC_UP, ENC_DOWN, BTN_1 }; // STATES
  enum { EVT_CONTROL, EVT_ENC_UP, EVT_ENC_DOWN, EVT_BTN_1, ELSE }; // EVENTS
  Atm_delay_effect( void ) : Machine(){};
  Atm_delay_effect& begin( int param_con );
  Atm_delay_effect& trace( Stream & stream );
  Atm_delay_effect& trigger( int event );
  Atm_delay_effect& setLevel( void );
  Atm_delay_effect& off( void );
  Atm_delay_effect& btn1( void );
  Atm_delay_effect& encoderUp( void );
  Atm_delay_effect& encoderDown( void );  
  int state( void );
 
 private:
  enum { ENT_CONTROL, ENT_ENC_UP, ENT_ENC_DOWN, ENT_BTN_1 }; // ACTIONS
  int event( int id ); 
  void action( int id );
  int param_control;
  int param_position;
  int display_delay;
  float param_array [34]{
	0.0, 0.03, 0.06, 0.09, 0.12, 0.15, 0.18, 0.21, 0.24, 0.27, 0.30, 0.33, 0.36, 0.39, 0.42, 0.45, 0.48, 0.51, 0.54, 0.57, 0.60, 0.63, 0.66, 0.69, 0.72, 0.75, 0.78, 0.81, 0.84, 0.87, 0.90, 0.93, 0.96, 0.99
  };
  int speed_array [13]{
  	 0, 27, 35, 54, 71, 80, 107, 143, 160, 214, 286, 321, 425
  };
  
};
