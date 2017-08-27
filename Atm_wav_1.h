#pragma once
/*
header file for function to play wav in loop
button stops or starts
7/15/17
loop function works by putting playCheck in ON_LOOP event 
added button skips tracks
added play function has input filename
7/19/17
added instances, this version with 2 instances
looping works
8/1/17
*/

#include <Automaton.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
//#include "audio_system.h"

class Atm_wav_1 : public Machine {
	public:
		enum { WAV_OFF, WAV_1_ON, WAV_2_ON, WAV_3_ON, WAV_4_ON, WAV_5_ON }; //STATES
		enum { EVT_ENC_DOWN, EVT_ENC_UP, EVT_BTN_1, EVT_PLAY_CHECK, ELSE }; //EVENTS
		//enum {WAV_START, WAV_STOP}; //ACTIONS
		
		Atm_wav_1( void ) : Machine(){};
		Atm_wav_1& begin( int instance );
		Atm_wav_1& play( const char *filename );   
		Atm_wav_1& stop( void );
		Atm_wav_1& playCheck( void );
		Atm_wav_1& trace( Stream& stream );
		Atm_wav_1& onPress( Machine& machine, int event = 0 );
		Atm_wav_1& onPress( atm_cb_push_t callback, int idx = 0 ); 
		Atm_wav_1& trigger( int event );
		int state( void );
		
		
	private:
     	enum { ENT_WAV_1_ON, ENT_WAV_2_ON, ENT_WAV_3_ON, ENT_WAV_4_ON, ENT_WAV_5_ON, ENT_WAV_OFF, ENT_PLAY_CHECK }; // ACTIONS
     	enum { ON_PRESS, CONN_MAX }; // CONNECTORS
     	atm_connector connectors[CONN_MAX];
		int playerNo;
		//atm_timer_millis check_timer;
		//atm_counter counter;

		
     	int event( int id ); 
     	void action( int id );	
};