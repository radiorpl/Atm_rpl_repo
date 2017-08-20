/*
8/3/17
Volume control for audio system Atm_wav_1 working in preset steps
8/6/17 
add separate controls for each instance
8/9/17
split vol into 33 steps
added variables for sev seg display level
8/13/17
added display and home parts
8/14/17
added events for all state for trigger()
8/17/17
triggering display still not working. Adding checkMillis function for on_loop
8/18/17
r2 TOTAL REWRITE 
works controlling volumes
haven't worked out displays yet
8/19/17 
attempt to add display
8/20/17
successfully connected sev seg
*/	
#include "Atm_master_vol.h"
#include "audio_system.h"
#include "display_def.h"

int mas_vol_level; //variables for display
int vol_wav_1_level; 
int vol_wav_2_level; 

	
Atm_master_vol& Atm_master_vol::begin( int vol_con ) {
  	// clang-format off
	static const state_t state_table[] PROGMEM = {
		/*               	ON_ENTER    			ON_LOOP   		ON_EXIT		EVT_ENC_UP		EVT_ENC_DOWN		EVT_BTN_1	 	EVT_VOL_CONTROL 	ELSE */	 				
		/*VOL_CONTROL */	ENT_VOL_CONTROL, 		-1,					-1,			VOL_UP,	  	 	VOL_DOWN,			BTN_1,		   VOL_CONTROL,		-1,	
		/*VOL_UP 	  */	ENT_VOL_UP, 			-1,					-1,			-1,	  	 		-1,					-1,				VOL_CONTROL,		-1,
		/*VOL_DOWN 	  */	ENT_VOL_DOWN, 			-1,					-1,			-1,	  	 		-1,					-1,				VOL_CONTROL,		-1,
		/*BTN_1 	  */	ENT_BTN_1, 				-1,					-1,			-1,	  	 		-1,					-1,				VOL_CONTROL,		-1,
	};
    // clang-format on
    Machine::begin( state_table, ELSE );
	vol_control = vol_con;
	volume_position = 17;
	AudioMemory(160);	    
	//sets mixers for 2 wav playback, no effects
	mixer1.gain(0, 0.5); 	//wav1 stereo to mono
    mixer1.gain(1, 0.5);
    mixer2.gain(0, 0.5);	//wav2
    mixer2.gain(1, 0.5);
	mixer6.gain(0, 0.5);	//mix 1 and 2
	mixer6.gain(0, 0.5);
	mixer9.gain(0, 0.5);	//mix with effects
	display_delay = 100;  //wait after entering screen state
	param_delay = 3000;   //wait before return home
    return *this;	
}

int Atm_master_vol::event( int id ) {
  switch ( id ) {
    case EVT_ENC_UP:
		return 0;	  
    case EVT_ENC_DOWN:
		return 0;	
    case EVT_BTN_1:
		return 0;
	case EVT_VOL_CONTROL:
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
void Atm_master_vol::action( int id ) {
	switch ( id ) {
			return;
		case ENT_BTN_1:    ///actually just reset counter frome evt_btn_1
			btn1();
			return;		
		case ENT_VOL_CONTROL:
			setVolume();
			return;		
		case ENT_VOL_UP:
			encoderUp();
			return;	
		case ENT_VOL_DOWN:
			encoderDown();
			return;	
	}
}
/* Optionally override the default trigger() method
 * Control how your machine processes trigger
 */

Atm_master_vol& Atm_master_vol::trigger( int event ) {
  Machine::trigger( event );
  return *this;
}

/* Optionally override the default state() method
 * Control what the machine returns when another process requests its state
 */

int Atm_master_vol::state( void ) {
  return Machine::state();
  }


Atm_master_vol& Atm_master_vol::setVolume( void ) {
	if ( vol_control == 0 ){  		//master
		if( volume_position < 1 ){		//constrain volume_position to bounds of volume array
			volume_position = 0; 	              
		}
		if( volume_position > 27 ){
			volume_position = 28; 	// 0.81, louder will clip master               
		}			
		sgtl5000_1.volume(volume_array[volume_position]);
		Serial.println("master vol");
		Serial.println(volume_position);
		Serial.println("volume");
		Serial.println(volume_array[volume_position]);
	}
	else if ( vol_control == 1 ){  	//wav1
		if( volume_position < 1 ){
			volume_position = 0; 	                
		}
		if( volume_position > 32 ){
			volume_position = 33; 	              
		}		
		mixer6.gain(0, volume_array[volume_position]);
		Serial.println("bira 1 volume");
		Serial.println(volume_position);
		Serial.println(volume_array[volume_position]);
	}
	else if ( vol_control == 2 ){  	//wav2
		if( volume_position < 1 ){
			volume_position = 0; 	              
		}
		if( volume_position > 32 ){
			volume_position = 33; 	               
		}		
		mixer6.gain(1, volume_array[volume_position]);
		Serial.println("bira 2 volume");
		Serial.println(volume_position);
		Serial.println(volume_array[volume_position]);

	}
	return *this;
}

Atm_master_vol& Atm_master_vol::checkMillis( void ) {
	if ( m_display  == param_delay ) {
			//trigger( EVT_TIMER );
		displayMain.trigger( displayMain.EVT_HOME );
			Serial.println("param timer");
			Serial.println("HOME TRIGGERED");
			
		}
	return *this;
}

Atm_master_vol& Atm_master_vol::btn1( void ) {
	Serial.print("m_display = ");
	Serial.println(m_display);
	trigger( EVT_VOL_CONTROL );
	m_display = 0;
	return *this;
}

Atm_master_vol& Atm_master_vol::encoderUp( void ) {	
	if( m_display > param_delay ){
		m_display = 0;
		Serial.println("wait display triggered");			//if display delay expired, display parameter and wait
		if ( vol_control == 0 ){
			displayMain.trigger( displayMain.EVT_MASTER_VOL );
		}
		else if ( vol_control == 1 ){
			displayMain.trigger( displayMain.EVT_VOL_WAV_1 );	
		}
		else if ( vol_control == 2 ){
			displayMain.trigger( displayMain.EVT_VOL_WAV_2 );	
		}		
		delay(display_delay);
		trigger( EVT_VOL_CONTROL );
	}
	else {
		volume_position += 1;								//otherwise, volume increment up
		Serial.println("enc up");
		Serial.println(volume_position);
		trigger( EVT_VOL_CONTROL );
		Serial.println(m_display);
		m_display = 0;
	}
	return *this;
}

Atm_master_vol& Atm_master_vol::encoderDown( void ) {	
	if( m_display > param_delay ){
		m_display = 0;
		Serial.println("wait display triggered");
		if ( vol_control == 0 ){
			displayMain.trigger( displayMain.EVT_MASTER_VOL );
		}
		else if ( vol_control == 1 ){
			displayMain.trigger( displayMain.EVT_VOL_WAV_1 );	
		}
		else if ( vol_control == 2 ){
			displayMain.trigger( displayMain.EVT_VOL_WAV_2 );	
		}		
		delay(100);
		trigger( EVT_VOL_CONTROL );
	}
	else {
		volume_position -= 1;
		Serial.println("enc down");
		Serial.println(volume_position);
		trigger( EVT_VOL_CONTROL );
		Serial.println(volume_position);
		Serial.println(m_display);
		m_display = 0;
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
 * onPress() push connector variants ( slots 1, autostore 0, broadcast 0 )
 */

Atm_master_vol& Atm_master_vol::onPress( Machine& machine, int event ) {
  onPush( connectors, ON_PRESS, 0, 1, 1, machine, event );
  return *this;
}

Atm_master_vol& Atm_master_vol::onPress( atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_PRESS, 0, 1, 1, callback, idx );
  return *this;
}

/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

Atm_master_vol& Atm_master_vol::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "MASTER_VOL\0\EVT_ENC_UP\0EVT_ENC_DOWN\0EVT_BTN_1\0EVT_TIMER\0EVT_VOL_CONTROL\0ELSE\0VOL_CONTROL\0VOL_UP\0VOL_DOWN\0BTN_1" );
  return *this;
}
