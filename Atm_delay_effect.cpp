/*
8/20/17
rewritten based on new atm_master_vol
*/
#include "Atm_delay_effect.h"
#include "audio_system.h"
#include "display_def.h"

/* Add optional parameters for the state machine to begin()
 * Add extra initialization code
 */

int send_1_level;
int send_2_level;
int mix_level;
int time_1_level;
int time_2_level;
int time_3_level;
int time_4_level;
int fb_1_level;
int fb_2_level;
int fb_3_level;
int fb_4_level;
int delay_gain_1_level;
int delay_gain_2_level;
int delay_gain_3_level;
int delay_gain_4_level;

Atm_delay_effect& Atm_delay_effect::begin( int param_con ) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*               	ON_ENTER    ON_LOOP  ON_EXIT  EVT_OFF  EVT_CONTROL  EVT_ENC_UP  EVT_ENC_DOWN  EVT_BTN_1    ELSE */
    /*   OFF      */   ENT_OFF,       -1,      -1,   	  -1,     	-1,  		 -1,          	-1,     	-1,       -1,  
   /*    CONTROL  */   ENT_CONTROL,   -1,      -1,   	  -1,     	-1,  		 -1,          	-1,     	-1,       -1, 
   /*    ENC_UP   */   ENT_ENC_UP,    -1,      -1,   	  -1,     	-1,  		 -1,          	-1,     	-1,       -1,   
   /*    ENC_DOWN */   ENT_ENC_DOWN,  -1,      -1,   	  -1,     	-1,  		 -1,          	-1,     	-1,       -1, 
   /*    BTN_1    */   ENT_BTN_1,     -1,      -1,   	  -1,     	-1,  		 -1,          	-1,     	-1,       -1, 	  
	    
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  param_control = param_con;
  display_delay = 100;
  return *this;         
}

/* Add C++ code for each internally handled event (input) 
 * The code must return 1 to trigger the event
 */

int Atm_delay_effect::event( int id ) {
  switch ( id ) {
    case EVT_OFF:
	  return 0;
	case EVT_CONTROL:
	  return 0;
	case EVT_ENC_UP:
	  return 0;
 	case EVT_ENC_DOWN:
	  return 0;
 	case EVT_BTN_1:
      return 0;
  }
  return 0;
}

/* Add C++ code for each action
 * This generates the 'output' for the state machine
 */

void Atm_delay_effect::action( int id ) {
  switch ( id ) {
	case ENT_OFF:
	  off();
	  return;
	case ENT_CONTROL:
	  setLevel();
	  return;
	case ENT_ENC_UP:
	  encUp();
	  return;
	case ENT_ENC_DOWN:
	  encDown();
	  return;
	case ENT_BTN_1:
	  btn1();
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
//my functions
Atm_delay_effect& Atm_delay_effect::setLevel( void ) {
	if ( param_control == 0 ) {		//send wav 1
		if( param_position < 1 ){
			param_position = 0; 	                
		}
		if( param_position > 32 ){
			param_position = 33; 	              
		}
		mixer3.gain(0, param_array[param_position]);
	}
	else if ( param_position == 1) { 	//send wav 2
		if( param_position < 1 ){
			param_position = 0; 	                
		}
		if( param_position > 32 ){
			param_position = 33; 	              
		}
		mixer3.gain(1, param_array[param_position]);
	}
	else if ( param_position == 2) { 	//set delay time 1
		if( param_position < 1 ){
			param_position = 0; 	                
		}
		if( param_position > 11 ){
			param_position = 12; 	              
		}
		delay1.delay(0, speed_array[param_position]);
	}
	else if ( param_position == 3) { 	//set delay time 2
		if( param_position < 1 ){
			param_position = 0; 	                
		}
		if( param_position > 11 ){
			param_position = 12; 	              
		}
		delay1.delay(1, speed_array[param_position]);
	}
	else if ( param_position == 4) { 	//set delay time 3
		if( param_position < 1 ){
			param_position = 0; 	                
		}
		if( param_position > 11 ){
			param_position = 12; 	              
		}
		delay1.delay(2, speed_array[param_position]);
	}
	else if ( param_position == 5) { 	//set delay time 4
		if( param_position < 1 ){
			param_position = 0; 	                
		}
		if( param_position > 11 ){
			param_position = 12; 	              
		}
		delay1.delay(3, speed_array[param_position]);
	}
	else if ( param_position == 6) { 	//fb delay 1
		if( param_position < 1 ){
			param_position = 0; 	                
		}
		if( param_position > 32 ){
			param_position = 33; 	              
		}
		mixer3.gain(2, param_array[param_position]);
	}
	else if ( param_position == 7) { 	//fb delay 2
		if( param_position < 1 ){
			param_position = 0; 	                
		}
		if( param_position > 32 ){
			param_position = 33; 	              
		}
		mixer3.gain(3, param_array[param_position]);
	}
	else if ( param_position == 8) { 	//fb delay 3
		if( param_position < 1 ){
			param_position = 0; 	                
		}
		if( param_position > 32 ){
			param_position = 33; 	              
		}
		mixer4.gain(0, param_array[param_position]);
	}
	else if ( param_position == 9) { 	//fb delay 4
		if( param_position < 1 ){
			param_position = 0; 	                
		}
		if( param_position > 32 ){
			param_position = 33; 	              
		}
		mixer4.gain(1, param_array[param_position]);
	}
	else if ( param_position == 10) { 	//delay 1 level
		if( param_position < 1 ){
			param_position = 0; 	                
		}
		if( param_position > 32 ){
			param_position = 33; 	              
		}
		mixer7.gain(0, param_array[param_position]);
	}
	else if ( param_position == 11) { 	//delay 2 level
		if( param_position < 1 ){
			param_position = 0; 	                
		}
		if( param_position > 32 ){
			param_position = 33; 	              
		}
		mixer7.gain(1, param_array[param_position]);
	}
	else if ( param_position == 13) { 	//delay 3 level
		if( param_position < 1 ){
			param_position = 0; 	                
		}
		if( param_position > 32 ){
			param_position = 33; 	              
		}
		mixer7.gain(2, param_array[param_position]);
	}
	else if ( param_position == 13) { 	//delay 4 level
		if( param_position < 1 ){
			param_position = 0; 	                
		}
		if( param_position > 32 ){
			param_position = 33; 	              
		}
		mixer7.gain(3, param_array[param_position]);
	}
	else if ( param_position == 14) { 	//crossfader master wet/dry mix
		if( param_position < 1 ){
			param_position = 0; 	                
		}
		if( param_position > 32 ){
			param_position = 33; 	              
		}
		mixer9.gain(0, param_array[param_position]);
		mixer9.gain(1, (0.99 - param_array[param_position]));
	}
	return *this;
}

Atm_delay_effect& Atm_delay_effect::encUp( void ) {	
	if( paramTimer.state() == 0 ){
		paramTimer.trigger( paramTimer.EVT_START );
		Serial.println("wait display triggered");			//if display delay expired, display parameter and wait
		if ( param_control == 0 ){
			displayMain.trigger( displayMain.EVT_MASTER_VOL );
		}
		else if ( param_control == 1 ){
			displayMain.trigger( displayMain.EVT_VOL_WAV_1 );	
		}
		else if ( param_control == 2 ){
			displayMain.trigger( displayMain.EVT_VOL_WAV_2 );	
		}		
		delay(display_delay);
		trigger( EVT_CONTROL );
	}
	else {
		param_position += 1;								//otherwise, param increment up
		Serial.println("enc up");
		Serial.println(param_position);
		trigger( EVT_CONTROL );
		paramTimer.trigger( paramTimer.EVT_START );
	}
	return *this;
}

Atm_delay_effect& Atm_delay_effect::encDown( void ) {	
	if( paramTimer.state() == 0 ){
		paramTimer.trigger( paramTimer.EVT_START );
		Serial.println("wait display triggered");			//if display delay expired, display parameter and wait
		if ( param_control == 0 ){
			displayMain.trigger( displayMain.EVT_MASTER_VOL );
		}
		else if ( param_control == 1 ){
			displayMain.trigger( displayMain.EVT_VOL_WAV_1 );	
		}
		else if ( param_control == 2 ){
			displayMain.trigger( displayMain.EVT_VOL_WAV_2 );	
		}		
		delay(display_delay);
		trigger( EVT_CONTROL );
	}
	else {
		param_position -= 1;								//otherwise, param increment down
		Serial.println("enc down");
		Serial.println(param_position);
		trigger( EVT_CONTROL );
		paramTimer.trigger( paramTimer.EVT_START );
	}
	return *this;
}

Atm_delay_effect& Atm_delay_effect::btn1( void ) {
	trigger( EVT_CONTROL );
	paramTimer.trigger( paramTimer.EVT_START );
	Serial.println("trigger paramTimer");
	return *this;
}

Atm_delay_effect& Atm_delay_effect::off( void ) {
	paramTimer.trigger( paramTimer.EVT_START );
	Serial.println("trigger paramTimer");
    mixer3.gain(0, 0.0);		//signal inputs
    mixer3.gain(1, 0.0);
  	mixer3.gain(2, 0.0);		//feedback inputs
  	mixer3.gain(3, 0.0);
  	mixer4.gain(0, 0.0);
  	mixer4.gain(1, 0.0);
    mixer9.gain(1, 0.0);		//master effect mix
	return *this;
}
/* Nothing customizable below this line                          
 ************************************************************************************************
*/

/* Public event methods
 *
 */


/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

Atm_delay_effect& Atm_delay_effect::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "DELAY_EFFECT\0EVT_BTN_1\0EVT_BTN_2\0EVT_ENC_UP_1\0EVT_ENC_DOWN_1\0EVT_ENC_UP_2\0EVT_ENC_DOWN_2\0ELSE\0OFF\0ON\0SEND_1\0SEND_2\0MIX\0TIME_1\0FEEDBACK_1\0TIME_2\0FEEDBACK_2\0TIME_3\0FEEDBACK_3\0TIME_4\0FEEDBACK_4" );
  return *this;
}



