#pragma once
/*
Works controlling master vol along with Atm_wav_1_loop_r2
*/

#include <Automaton.h>
#include <Audio.h>

class Atm_master_vol : public Machine {
	public:
		enum { VOL_CONTROL, VOL_UP, VOL_DOWN }; //STATES
		enum { EVT_ENC_UP, EVT_ENC_DOWN, EVT_BTN_1, EVT_TIMER, EVT_VOL_CONTROL, ELSE }; //EVENTS
		//enum {WAV_START, WAV_STOP}; //ACTIONS
		
		Atm_master_vol( void ) : Machine(){};
		Atm_master_vol& begin( int vol_con );
		Atm_master_vol& setVolume( void );
		Atm_master_vol& checkMillis( int m_instance );
		Atm_master_vol& enterHome( void );
		Atm_master_vol& enterDisplay( void );
		Atm_master_vol& encoderUp( void );
		Atm_master_vol& encoderDown( void ); 
		Atm_master_vol& trace( Stream& stream );
		Atm_master_vol& onPress( Machine& machine, int event = 0 );
		Atm_master_vol& onPress( atm_cb_push_t callback, int idx = 0 ); 
		Atm_master_vol& trigger( int event );
		int state( void );
		//atm_counter controlCounter; 

		
		
	private:
     	enum { ENT_CHECK_MILLIS, ENT_HOME, ENT_DISPLAY, ENT_VOL_CONTROL, ENT_VOL_UP, ENT_VOL_DOWN }; // ACTIONS
     	enum { ON_PRESS, CONN_MAX }; // CONNECTORS
     	atm_connector connectors[CONN_MAX];
		//atm_timer_millis display_timer;
		//atm_timer_millis param_timer;
		elapsedMillis m_display;
		int display_delay;
		int param_delay;	
		int last_volume_position;
		int volume_position;
		int vol_control;
		float volume;
		//int last_state;
     	int event( int id ); 
     	void action( int id );
		float volume_array [34]{
			0.0, 0.03, 0.06, 0.09, 0.12, 0.15, 0.18, 0.21, 0.24, 0.27, 0.30, 0.33, 0.36, 0.39, 0.42, 0.45, 0.48, 0.51, 0.54, 0.57, 0.60, 0.63, 0.66, 0.69, 0.72, 0.75, 0.78, 0.81, 0.84, 0.87, 0.90, 0.93, 0.96, 0.99
		};	
};
