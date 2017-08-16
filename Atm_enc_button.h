#pragma once

#include <Automaton.h>

class Atm_enc_button: public Machine {

 public:
  enum { IDLE, COUNT_UP}; // STATES
  enum { EVT_BTN_1, ELSE }; // EVENTS
  Atm_enc_button( void ) : Machine() {};
  Atm_enc_button& begin( int encButton );
  Atm_enc_button& trace( Stream & stream );
  Atm_enc_button& trigger( int event );
  Atm_enc_button& count( void );
  int state( void );
  Atm_enc_button& btn_1( void );

  
 private:
  enum { ENT_COUNT_UP }; // ACTIONS
  int event( int id ); 
  void action( int id ); 
  int encBtn;
};


