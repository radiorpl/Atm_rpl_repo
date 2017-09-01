

/*
src file for function to play wav in loop
button stops or starts
7/15/17
loop function works by putting playCheck in ON_LOOP event 
added button skips tracks
added play function has input filename
7/19/17
changed to encoders to toggle tracks up and down
works but needs debouncing or something b/c encoder is too sensitive
8/1/17
added instances, this version with 2 instances
looping works
8/14/17
add display stuff
*/
#include "Atm_wav_1.h"
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "display_def.h"

// GUItool: begin automatically generated code
AudioPlaySdWav           playSdWav2;     //xy=88,160
AudioPlaySdWav           playSdWav1;     //xy=89,66
AudioMixer4              mixer2;         //xy=232,163
AudioMixer4              mixer1;         //xy=238,84
AudioEffectDelay         delay1;         //xy=309,331
AudioMixer4              mixer3;         //xy=495,35
AudioMixer4              mixer4;         //xy=505,131
AudioMixer4              mixer7;         //xy=525,381
AudioMixer4              mixer5;         //xy=678,70
AudioMixer4              mixer6;         //xy=680,261
AudioMixer4              mixer8;         //xy=682,375
AudioMixer4              mixer9;         //xy=855,328
AudioOutputI2S           i2s1;           //xy=1023,334
AudioConnection          patchCord1(playSdWav2, 0, mixer2, 0);
AudioConnection          patchCord2(playSdWav2, 1, mixer2, 1);
AudioConnection          patchCord3(playSdWav1, 0, mixer1, 0);
AudioConnection          patchCord4(playSdWav1, 1, mixer1, 1);
AudioConnection          patchCord5(mixer2, 0, mixer3, 1);
AudioConnection          patchCord6(mixer2, 0, mixer6, 1);
AudioConnection          patchCord7(mixer1, 0, mixer3, 0);
AudioConnection          patchCord8(mixer1, 0, mixer6, 0);
AudioConnection          patchCord9(delay1, 0, mixer3, 2);
AudioConnection          patchCord10(delay1, 0, mixer7, 0);
AudioConnection          patchCord11(delay1, 1, mixer3, 3);
AudioConnection          patchCord12(delay1, 1, mixer7, 1);
AudioConnection          patchCord13(delay1, 2, mixer7, 2);
AudioConnection          patchCord14(delay1, 2, mixer4, 0);
AudioConnection          patchCord15(delay1, 3, mixer7, 3);
AudioConnection          patchCord16(delay1, 3, mixer4, 1);
AudioConnection          patchCord17(mixer3, 0, mixer5, 0);
AudioConnection          patchCord18(mixer4, 0, mixer5, 1);
AudioConnection          patchCord19(mixer7, 0, mixer8, 0);
AudioConnection          patchCord20(mixer5, delay1);
AudioConnection          patchCord21(mixer6, 0, mixer9, 0);
AudioConnection          patchCord22(mixer8, 0, mixer9, 1);
AudioConnection          patchCord23(mixer9, 0, i2s1, 0);
AudioConnection          patchCord24(mixer9, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=478,626
// GUItool: end automatically generated code

//display levels
int track_1_level;
int track_2_level;
int last_track_1_level;
int last_track_2_level;


Atm_wav_1& Atm_wav_1::begin( int instance ) {
  	// clang-format off
	static const state_t state_table[] PROGMEM = {
		/*               	ON_ENTER    	ON_LOOP    	ON_EXIT		EVT_WAV_OFF		EVT_WAV_PLAY	 EVT_ENC_UP		EVT_ENC_DOWN	EVT_BTN_1	EVT_PLAY_CHECK     	ELSE */
		/* WAV_PLAY */		ENT_WAV_PLAY, ENT_PLAY_CHECK,     -1,	      WAV_OFF,		  WAV_PLAY,	      TRACK_UP,	  	 TRACK_DOWN,	  BTN_1,     	-1,				-1,	  
		/*	WAV_OFF */		ENT_WAV_OFF,	 -1,		  -1,	      WAV_OFF,	  	  WAV_PLAY,	        -1,	  	        -1,	          BTN_1,		-1,			   	-1,
		/* TRACK_UP */		ENT_TRACK_UP,	 -1,		  -1,	      WAV_OFF,	      WAV_PLAY,			-1,		        -1,	 			-1,			-1,				-1,
	  /* TRACK_DOWN */		ENT_TRACK_DOWN,	 -1,		  -1,	      WAV_OFF,	      WAV_PLAY,			-1,		        -1,	 			-1,			-1,				-1,
		/*	BTN_1   */		ENT_BTN_1,		 -1,		  -1,	      WAV_OFF,	      WAV_PLAY,			-1,		        -1,	 			-1,			-1,				-1,
	// /*PLAY_CHECK  */		ENT_PLAY_CHECK,	 -1,		  -1,	      WAV_OFF,	      WAV_PLAY,			-1,		        -1,	 			-1,			-1,				-1,	
	};
	
    // clang-format on
    Machine::begin( state_table, ELSE );	//r2 moved audio setup to master vol
	player_instance = instance;
	track_1_level = 0;
	track_2_level = 0;
	display_delay = 100; 
    return *this;	
}

int Atm_wav_1::event( int id ) {
  switch ( id ) {
  	case EVT_WAV_PLAY:
  	  return 0;
	case EVT_WAV_OFF:
	  return 0;
    case EVT_ENC_UP:
	  return 0;  
    case EVT_ENC_DOWN:
      return 0;  
    case EVT_BTN_1:
	  return 0;  
    case EVT_PLAY_CHECK: 
	  return 0;
  	}  
  	return 0;
}
/* Add C++ code for each action
 * This generates the 'output' for the state machine
 *
 * Available connectors:
 *   push( connectors, ON_PRESS, 0, <v>, <up> );
 */
void Atm_wav_1::action( int id ) {
	switch ( id ) {
		case ENT_WAV_PLAY:
			if ( player_instance == 1 && track_1_level == 1 ) {
			  play("DRONE1.WAV");	
			}
			else if( player_instance == 2 && track_2_level == 1 ) {
			  play("DRONE1.WAV");		
			}
			else if( player_instance == 1 && track_2_level == 2 ) {
			  play("DRONE2.WAV");		
			}
			else if( player_instance == 2 && track_2_level == 2 ) {
			  play("DRONE2.WAV");		
			}
			else if( player_instance == 1 && track_2_level == 3 ) {
			  play("DRONE3.WAV");		
			}
			else if( player_instance == 2 && track_2_level == 3 ) {
			  play("DRONE3.WAV");		
			}
			else if( player_instance == 1 && track_2_level == 4 ) {
			  play("DRONE4.WAV");		
			}
			else if( player_instance == 2 && track_2_level == 4 ) {
			  play("DRONE4.WAV");		
			}
			else if( player_instance == 1 && track_2_level == 5 ) {
			  play("DRONE5.WAV");		
			}
			else if( player_instance == 2 && track_2_level == 5 ) {
			  play("DRONE5.WAV");		
			}
			return;
		
		case ENT_WAV_OFF:
			if ( player_instance == 1 ) {
				stop();
				
			}
			else if( player_instance == 2 ) {
				stop();
			}
			return;
			
		
		case ENT_TRACK_UP:
		  encoderUp();
		  return;
		case ENT_TRACK_DOWN:
		  encoderDown();
		  return;
		case ENT_BTN_1:
		  btn1();
		  return;
		  /*
		case ENT_PLAY_CHECK:
		  playCheck();
		  return;
		  */
	}	
}

/* Optionally override the default trigger() method
 * Control how your machine processes trigger
 */

Atm_wav_1& Atm_wav_1::trigger( int event ) {
  Machine::trigger( event );
  return *this;
}

/* Optionally override the default state() method
 * Control what the machine returns when another process requests its state
 */

int Atm_wav_1::state( void ) {
  return Machine::state();
}

Atm_wav_1& Atm_wav_1::play( const char *filename ) {
	if ( track_1_level < 2) {				//set track range low	
		track_1_level = 1;
	}
	if ( track_2_level < 2) {
		track_2_level = 1;
	}
	if ( track_1_level > 4 ) {				//set track range high
		track_1_level = 5;
	}
	if ( track_2_level > 4) {
		track_2_level = 5;
	}	
	if ( player_instance == 1 ) {
		if ( track_1_level != last_track_1_level || playSdWav1.isPlaying() == false ) {
		playSdWav1.play( filename ); //play wav file
		last_track_1_level = track_1_level;
	    }
	}
	else if ( player_instance == 2 ) {
		if ( track_2_level != last_track_2_level || playSdWav2.isPlaying() == false ) {
		playSdWav2.play( filename ); //play wav file
		last_track_2_level = track_2_level;
		}
	}
	delay(10);
	Serial.println( filename );
	
	
	
return *this;
}

Atm_wav_1& Atm_wav_1::stop( void ) {
	if ( player_instance == 1 ) {
	playSdWav1.stop(); //stop wav file
	track_1_level = 0;
	Serial.println("stop 1");
	//Serial.println(state());
	}
	else if ( player_instance == 2 ) {
	playSdWav2.stop(); //stop wav file
	track_2_level = 0;
	Serial.println("stop 2");
	}
	return *this;
}

Atm_wav_1& Atm_wav_1::encoderUp( void ) {	
	if( paramTimer.state() == 0 ){
		paramTimer.trigger( paramTimer.EVT_START );
		Serial.println("wait display triggered");			//if display delay expired, display parameter and wait
		if ( player_instance == 1 ){
			displayMain.trigger( displayMain.EVT_TRACK_WAV_1 );
		}
		else if ( player_instance == 2 ){
			displayMain.trigger( displayMain.EVT_TRACK_WAV_2 );	
		}
		delay(display_delay);
		trigger( EVT_WAV_PLAY );
	}
	else {
		if ( player_instance == 1 ){			//otherwise, track increment up
			track_1_level += 1;
		}
		else if ( player_instance == 2 ){
			track_2_level += 1;
		}							
		Serial.println("track enc up");
		trigger( EVT_WAV_PLAY );
		paramTimer.trigger( paramTimer.EVT_START );
	}
	return *this;
}

Atm_wav_1& Atm_wav_1::encoderDown( void ) {	
	if( paramTimer.state() == 0 ){
		paramTimer.trigger( paramTimer.EVT_START );
		Serial.println("wait display triggered");			//if display delay expired, display parameter and wait
		if ( player_instance == 1 ){
			displayMain.trigger( displayMain.EVT_TRACK_WAV_1 );
		}
		else if ( player_instance == 2 ){
			displayMain.trigger( displayMain.EVT_TRACK_WAV_2 );	
		}
		delay(display_delay);
		trigger( EVT_WAV_PLAY );
	}
	else {
		if ( player_instance == 1 ){			//otherwise, track increment down
			track_1_level -= 1;
		}
		else if ( player_instance == 2 ){
			track_2_level -= 1;
		}							
		Serial.println("track enc down");
		trigger( EVT_WAV_PLAY );
		paramTimer.trigger( paramTimer.EVT_START );
	}
	return *this;
}

Atm_wav_1& Atm_wav_1::btn1( void ) {
    if ( paramTimer.state() == 0 ) {
		//delayTimer.trigger( delayTimer.EVT_START );
   		paramTimer.trigger( paramTimer.EVT_START );
   		Serial.println("wait display triggered");
		if ( player_instance == 1 ) {
		displayMain.trigger( displayMain.EVT_TRACK_WAV_1 );
		}
		else if ( player_instance == 2 ) {
		displayMain.trigger( displayMain.EVT_TRACK_WAV_2 );
		}							
		delay(display_delay);
		if ( player_instance == 1 ) {
			if ( track_1_level == 0 ) {
				trigger( EVT_WAV_OFF );
			}
			else {
				trigger( EVT_WAV_PLAY);
			}
		}
		else if ( player_instance == 2 ) {
			if ( track_2_level == 0 ) {
				trigger( EVT_WAV_OFF );
			}
			else {
				trigger( EVT_WAV_PLAY);
			}
		}		 
	}
	else {
		if ( player_instance == 1 ) {
			if ( track_1_level == 0 ) {
				trigger( EVT_WAV_OFF );
			}
			else {
				trigger( EVT_WAV_PLAY);
			}
		}
		else if ( player_instance == 2 ) {
			if ( track_2_level == 0 ) {
				trigger( EVT_WAV_OFF );
			}
			else {
				trigger( EVT_WAV_PLAY);
			}
		}
	}
	return *this;
}
Atm_wav_1& Atm_wav_1::playCheck( void ) {    //================================NEW saturday night: track_1_level in if
	
	if ( player_instance == 1 && track_1_level != 0 && playSdWav1.isPlaying() == false ) { //doesn't stop trigger with true/false or 0/1
		trigger( EVT_WAV_PLAY );
		Serial.println("playCheck");
	}
	else if ( player_instance == 2 && track_1_level != 0 && playSdWav2.isPlaying() == false ) { //doesn't stop trigger with true/false or 0/1
		trigger( EVT_WAV_PLAY );
		Serial.println("playCheck");
	}
			
	return *this;
}
/* Nothing customizable below this line                          
 ************************************************************************************************
*/

/* Public event methods
 *
 */
/*
Atm_wav_1& Atm_wav_1::btn_1() {
  trigger( EVT_BTN_1 );
  return *this;
}
*/
/*
 * onPress() push connector variants ( slots 1, autostore 0, broadcast 0 )
 */
/*
Atm_wav_1& Atm_wav_1::onPress( Machine& machine, int event ) {
  onPush( connectors, ON_PRESS, 0, 1, 1, machine, event );
  return *this;
}

Atm_wav_1& Atm_wav_1::onPress( atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_PRESS, 0, 1, 1, callback, idx );
  return *this;
}
*/
/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

Atm_wav_1& Atm_wav_1::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "WAV_1\0EVT_WAV_OFF\0EVT_WAV_PLAY\0EVT_ENC_DOWN\0EVT_ENC_UP\0EVT_BTN_1\0EVT_PLAY_CHECK\0ELSE\0WAV_PLAY\0WAV_OFF\0TRACK_UP\0TRACK_DOWN\0BTN_1" );
  return *this;
}
/*
Atm_led& Atm_led::trigger( int event ) {
  if ( event > ELSE ) {
    play();
  } else {
    Machine::trigger( event );
  }
  return *this;
}
*/
