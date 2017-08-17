#pragma once
/*
Works controlling master vol along with Atm_wav_1_loop_r2
*/

#include <Automaton.h>
#include <Audio.h>

class Atm_master_vol : public Machine {
	public:
		enum { START_UP, HOME, DISPLAY_1, VOL_0, VOL_1, VOL_2, VOL_3, VOL_4, VOL_5, VOL_6, VOL_7, VOL_8, VOL_9, VOL_10, VOL_11, VOL_12, VOL_13, VOL_14, VOL_15, VOL_16, VOL_17, VOL_18, VOL_19, VOL_20, VOL_21, VOL_22, VOL_23, VOL_24, VOL_25, VOL_26, VOL_27, VOL_28, VOL_29, VOL_30, VOL_31, VOL_32, VOL_33 }; //STATES
		enum { EVT_ENC_DOWN, EVT_ENC_UP, EVT_BTN_1, EVT_TIMER_1, EVT_TIMER_2, EVT_VOL_0, EVT_VOL_1, EVT_VOL_2, EVT_VOL_3, EVT_VOL_4, EVT_VOL_5, EVT_VOL_6, EVT_VOL_7, EVT_VOL_8, EVT_VOL_9, EVT_VOL_10, EVT_VOL_11, EVT_VOL_12, EVT_VOL_13, EVT_VOL_14, EVT_VOL_15, EVT_VOL_16, EVT_VOL_17, EVT_VOL_18, EVT_VOL_19, EVT_VOL_20, EVT_VOL_21, EVT_VOL_22, EVT_VOL_23, EVT_VOL_24, EVT_VOL_25, EVT_VOL_26, EVT_VOL_27, EVT_VOL_28, EVT_VOL_29, EVT_VOL_30, EVT_VOL_31, EVT_VOL_32, EVT_VOL_33, ELSE }; //EVENTS
		//enum {WAV_START, WAV_STOP}; //ACTIONS
		
		Atm_master_vol( void ) : Machine(){};
		Atm_master_vol& begin( int volCon );
		Atm_master_vol& setVolume( float volume_gain ); 
		Atm_master_vol& trace( Stream& stream );
		Atm_master_vol& onPress( Machine& machine, int event = 0 );
		Atm_master_vol& onPress( atm_cb_push_t callback, int idx = 0 ); 
		Atm_master_vol& trigger( int event );
		int state( void );
		//atm_counter controlCounter; 
		//Atm_wav_1& volume( int volume );
		
		
	private:
     	enum { ENT_HOME, ENT_DISPLAY, EXIT_DISPLAY, ENT_VOL_0, ENT_VOL_1, ENT_VOL_2, ENT_VOL_3, ENT_VOL_4, ENT_VOL_5, ENT_VOL_6, ENT_VOL_7, ENT_VOL_8, ENT_VOL_9,ENT_VOL_10, ENT_VOL_11, ENT_VOL_12, ENT_VOL_13, ENT_VOL_14, ENT_VOL_15, ENT_VOL_16, ENT_VOL_17, ENT_VOL_18, ENT_VOL_19, ENT_VOL_20, ENT_VOL_21, ENT_VOL_22, ENT_VOL_23, ENT_VOL_24, ENT_VOL_25, ENT_VOL_26, ENT_VOL_27, ENT_VOL_28, ENT_VOL_29, ENT_VOL_30,ENT_VOL_31, ENT_VOL_32, ENT_VOL_33}; // ACTIONS
     	enum { ON_PRESS, CONN_MAX }; // CONNECTORS
     	atm_connector connectors[CONN_MAX];
		atm_timer_millis display_timer;
		atm_timer_millis param_timer;
		int evt_counter;
		int vol_control;
		//int last_state;
     	int event( int id ); 
     	void action( int id );	
};
