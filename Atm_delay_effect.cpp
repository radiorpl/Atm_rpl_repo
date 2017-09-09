/*
8/20/17
rewritten based on new atm_master_vol
*/
#include "Atm_delay_effect.h"
#include "audio_system.h"
#include "display_def.h"
#include "button_counters.h"

/* Add optional parameters for the state machine to begin()
 * Add extra initialization code
 */

int on_off;
int send_1_level;
int send_2_level;
int delay_mix_level;
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
    /*               	ON_ENTER    ON_LOOP  ON_EXIT  EVT_OFF  EVT_CONTROL  EVT_ENC_UP   EVT_ENC_DOWN  EVT_BTN_1   ELSE */
    /*   OFF      */   ENT_OFF,       -1,      -1,   	  -1,     CONTROL,       -1,          -1,        -1,        -1,  
   /*    CONTROL  */   ENT_CONTROL,   -1,      -1,   	 OFF,      -1,	      ENC_UP,      ENC_DOWN,    BTN_1,      -1,
   /*    ENC_UP   */   ENT_ENC_UP,    -1,      -1,   	  -1,     CONTROL,		 -1,          -1,     	 -1,        -1,   
   /*    ENC_DOWN */   ENT_ENC_DOWN,  -1,      -1,   	  -1,     CONTROL,		 -1,          -1,     	 -1,        -1, 
   /*    BTN_1    */   ENT_BTN_1,     -1,      -1,   	  -1,     CONTROL,		 -1,          -1,     	 -1,        -1, 	  
	    
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  param_control = param_con;
  display_delay = 100;
  mixer3.gain(0, 0.5);		//signal inputs
  mixer3.gain(1, 0.5);
  mixer3.gain(2, 0.5);		//feedback inputs
  mixer3.gain(3, 0.5);	
  mixer4.gain(0, 0.5);
  mixer4.gain(1, 0.5);
  mixer5.gain(0, 0.5);
  mixer5.gain(1, 0.5);
  mixer7.gain(0, 0.5);
  mixer7.gain(1, 0.5);
  mixer7.gain(2, 0.5);
  mixer7.gain(3, 0.5);
  mixer8.gain(0, 0.5);
  mixer9.gain(1, 0.5);		//master effect mix
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
	  encoderUp();
	  return;
	case ENT_ENC_DOWN:
	  encoderDown();
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
		//Serial.println( "send 1 level:" );
		//Serial.println( param_array[param_position] );
		if ( param_position > -1 && param_position < 4){
			send_1_level = 0;
		}
		else if ( param_position > 3 && param_position < 7){
			send_1_level = 1;
		}
		else if ( param_position > 6 && param_position < 10){
			send_1_level = 2;
		}
		else if ( param_position > 9 && param_position < 13){
			send_1_level = 3;
		}
		else if ( param_position > 12 && param_position < 16){
			send_1_level = 4;
		}
		else if ( param_position > 15 && param_position < 19){
			send_1_level = 5;
		}
		else if ( param_position > 18 && param_position < 23){
			send_1_level = 6;
		}
		else if ( param_position > 22 && param_position < 27){
			send_1_level = 7;
		}
		else if ( param_position > 26 && param_position < 31){
			send_1_level = 8;
		}
		else if ( param_position > 30 && param_position < 33){
			send_1_level = 9;
		}
	}
	else if ( param_control == 1 ) { 	//send wav 2
		if( param_position < 1 ){
			param_position = 0; 	                
		}
		if( param_position > 32 ){
			param_position = 33; 	              
		}
		mixer3.gain(1, param_array[param_position]);
		//Serial.println( param_array[param_position] );
		if ( param_position > -1 && param_position < 4){
			send_2_level = 0;
		}
		else if ( param_position > 3 && param_position < 7){
			send_2_level = 1;
		}
		else if ( param_position > 6 && param_position < 10){
			send_2_level = 2;
		}
		else if ( param_position > 9 && param_position < 13){
			send_2_level = 3;
		}
		else if ( param_position > 12 && param_position < 16){
			send_2_level = 4;
		}
		else if ( param_position > 15 && param_position < 19){
			send_2_level = 5;
		}
		else if ( param_position > 18 && param_position < 23){
			send_2_level = 6;
		}
		else if ( param_position > 22 && param_position < 27){
			send_2_level = 7;
		}
		else if ( param_position > 26 && param_position < 31){
			send_2_level = 8;
		}
		else if ( param_position > 30 && param_position < 33){
			send_2_level = 9;
		}
	}
	else if ( param_control == 2 ) { 	//set delay time 1
		if( param_position < 1 ){
			param_position = 0; 	                
		}
		if( param_position > 11 ){
			param_position = 12; 	              
		}
		delay1.delay(0, speed_array[param_position]);
		//Serial.println( param_array[param_position] );
	}
	else if ( param_control == 3 ) { 	//set delay time 2
		if( param_position < 1 ){
			param_position = 0; 	                
		}
		if( param_position > 11 ){
			param_position = 12; 	              
		}
		delay1.delay(1, speed_array[param_position]);
	}
	else if ( param_control == 4 ) { 	//set delay time 3
		if( param_position < 1 ){
			param_position = 0; 	                
		}
		if( param_position > 11 ){
			param_position = 12; 	              
		}
		delay1.delay(2, speed_array[param_position]);
	}
	else if ( param_control == 5 ) { 	//set delay time 4
		if( param_position < 1 ){
			param_position = 0; 	                
		}
		if( param_position > 11 ){
			param_position = 12; 	              
		}
		delay1.delay(3, speed_array[param_position]);
	}
	else if ( param_control == 6 ) { 	//fb delay 1
		if( param_position < 1 ){
			param_position = 0; 	                
		}
		if( param_position > 32 ){
			param_position = 33; 	              
		}
		mixer3.gain(2, param_array[param_position]);
		//Serial.println( param_array[param_position] );
	}
	else if ( param_control == 7 ) { 	//fb delay 2
		if( param_position < 1 ){
			param_position = 0; 	                
		}
		if( param_position > 32 ){
			param_position = 33; 	              
		}
		mixer3.gain(3, param_array[param_position]);
	}
	else if ( param_control == 8 ) { 	//fb delay 3
		if( param_position < 1 ){
			param_position = 0; 	                
		}
		if( param_position > 32 ){
			param_position = 33; 	              
		}
		mixer4.gain(0, param_array[param_position]);
	}
	else if ( param_control == 9 ) { 	//fb delay 4
		if( param_position < 1 ){
			param_position = 0; 	                
		}
		if( param_position > 32 ){
			param_position = 33; 	              
		}
		mixer4.gain(1, param_array[param_position]);
	}
	else if ( param_control == 10 ) { 	//delay 1 level
		if( param_position < 1 ){
			param_position = 0; 	                
		}
		if( param_position > 32 ){
			param_position = 33; 	              
		}
		mixer7.gain(0, param_array[param_position]);
	}
	else if ( param_control == 11 ) { 	//delay 2 level
		if( param_position < 1 ){
			param_position = 0; 	                
		}
		if( param_position > 32 ){
			param_position = 33; 	              
		}
		mixer7.gain(1, param_array[param_position]);
	}
	else if ( param_control == 12 ) { 	//delay 3 level
		if( param_position < 1 ){
			param_position = 0; 	                
		}
		if( param_position > 32 ){
			param_position = 33; 	              
		}
		mixer7.gain(2, param_array[param_position]);
	}
	else if ( param_control == 13 ) { 	//delay 4 level
		if( param_position < 1 ){
			param_position = 0; 	                
		}
		if( param_position > 32 ){
			param_position = 33; 	              
		}
		mixer7.gain(3, param_array[param_position]);
	}
	else if ( param_control == 14 ) { 	//crossfader master wet/dry mix
		if( param_position < 1 ){
			param_position = 0; 	                
		}
		if( param_position > 32 ){
			param_position = 33; 	              
		}
		mixer9.gain(1, param_array[param_position]);
		mixer9.gain(0, (0.99 - param_array[param_position]));
	}
	return *this;
}

Atm_delay_effect& Atm_delay_effect::off( void ) {
	paramTimer.trigger( paramTimer.EVT_START );
	Serial.println("trigger paramTimer");
    //mixer3.gain(0, 0.0);		//signal inputs
    //mixer3.gain(1, 0.0);
  	//mixer3.gain(2, 0.0);		//feedback inputs
  	//mixer3.gain(3, 0.0);
  	
	/*
	mixer4.gain(0, 0.0);
  	mixer4.gain(1, 0.0);
    mixer9.gain(1, 0.0);		//master effect mix
	*/
	return *this;
}
/*
Delay display states
DELAY_SEND_1, DELAY_SEND_2, DELAY_TIME_1, DELAY_TIME_2, DELAY_TIME_3, DELAY_TIME_4, DELAY_FB_1, DELAY_FB_2, DELAY_FB_3, DELAY_FB_4, DELAY_GAIN_1, DELAY_GAIN_2, DELAY_GAIN_3, DELAY_GAIN_4, DELAY_MIX
*/
Atm_delay_effect& Atm_delay_effect::btn1( void ) {
    if ( (displayMain.state() == displayMain.DELAY_SEND_1) || (displayMain.state() == displayMain.DELAY_SEND_2) || (displayMain.state() == displayMain.DELAY_TIME_1) || (displayMain.state() == displayMain.DELAY_FB_1) || (displayMain.state() == displayMain.DELAY_MIX) ) {
		if ( enc_button_counter_5 == 0 ) {
			displayMain.trigger( displayMain.EVT_DELAY_SEND_1 );
		}
		else if ( enc_button_counter_5 == 1 ) {
			displayMain.trigger( displayMain.EVT_DELAY_SEND_2 );
		}
		else if ( enc_button_counter_5 == 2 ) {
			displayMain.trigger( displayMain.EVT_DELAY_TIME_1 );
		}
		else if ( enc_button_counter_5 == 3 ) {
			displayMain.trigger( displayMain.EVT_DELAY_FB_1 );
		}
		else if ( enc_button_counter_5 == 4 ) {
			displayMain.trigger( displayMain.EVT_DELAY_MIX );
		}
		paramTimer.trigger( paramTimer.EVT_START );
		trigger( EVT_CONTROL );						//back to control state
    }
	else {
   		paramTimer.trigger( paramTimer.EVT_START );   //trigger timer
   		Serial.println("wait display triggered");
		if ( param_control == 0 ) {
			displayMain.trigger( displayMain.EVT_DELAY_SEND_1 );
		}
		else if ( param_control == 1 ) {
			displayMain.trigger( displayMain.EVT_DELAY_SEND_2 );
		}
		else if ( param_control == 2 ) {
			displayMain.trigger( displayMain.EVT_DELAY_TIME_1 );
		}
		else if ( param_control == 6 ) {
			displayMain.trigger( displayMain.EVT_DELAY_FB_1 );
		}
		else if ( param_control == 13 ) {
			displayMain.trigger( displayMain.EVT_DELAY_MIX );
		}					
		delay(display_delay);       	//display wait
		trigger( EVT_CONTROL );			
	}
	return *this;
}

Atm_delay_effect& Atm_delay_effect::encoderUp( void ) {	
	if ( (displayMain.state() == displayMain.DELAY_SEND_1) || (displayMain.state() == displayMain.DELAY_SEND_2) || (displayMain.state() == displayMain.DELAY_TIME_1) || (displayMain.state() == displayMain.DELAY_FB_1) || (displayMain.state() == displayMain.DELAY_MIX) ) {
		param_position += 1;
		if ( param_control == 0) {
			displayMain.trigger( displayMain.EVT_DELAY_SEND_1 );
		}
		else if ( param_control == 1) {
			displayMain.trigger( displayMain.EVT_DELAY_SEND_2 );
		}
		else if ( param_control == 2) {
			displayMain.trigger( displayMain.EVT_DELAY_TIME_1 );
		}
		else if ( param_control == 6) {
			displayMain.trigger( displayMain.EVT_DELAY_FB_1 );
		}
		else if ( param_control == 14) {
			displayMain.trigger( displayMain.EVT_DELAY_MIX );
		}
		trigger( EVT_CONTROL );
		paramTimer.trigger( paramTimer.EVT_START );
	}										
	else {
   		paramTimer.trigger( paramTimer.EVT_START );   //trigger timer
   		Serial.println("wait display triggered");
		if ( param_control == 0 ) {
			displayMain.trigger( displayMain.EVT_DELAY_SEND_1 );
		}
		else if ( param_control == 1 ) {
			displayMain.trigger( displayMain.EVT_DELAY_SEND_2 );
		}
		else if ( param_control == 2 ) {
			displayMain.trigger( displayMain.EVT_DELAY_TIME_1 );
		}
		else if ( param_control == 6 ) {
			displayMain.trigger( displayMain.EVT_DELAY_FB_1 );
		}
		else if ( param_control == 14 ) {
			displayMain.trigger( displayMain.EVT_DELAY_MIX );
		}					
		delay(display_delay);       	//display wait
		trigger( EVT_CONTROL );			
	}
	return *this;
}

Atm_delay_effect& Atm_delay_effect::encoderDown( void ) {	
	if ( (displayMain.state() == displayMain.DELAY_SEND_1) || (displayMain.state() == displayMain.DELAY_SEND_2) || (displayMain.state() == displayMain.DELAY_TIME_1) || (displayMain.state() == displayMain.DELAY_FB_1) || (displayMain.state() == displayMain.DELAY_MIX) ) {
		param_position -= 1;
		if ( param_control == 0 ) {
			displayMain.trigger( displayMain.EVT_DELAY_SEND_1 );
		}
		else if ( param_control == 1 ) {
			displayMain.trigger( displayMain.EVT_DELAY_SEND_2 );
		}
		else if ( param_control == 2 ) {
			displayMain.trigger( displayMain.EVT_DELAY_TIME_1 );
		}
		else if ( param_control == 6 ) {
			displayMain.trigger( displayMain.EVT_DELAY_FB_1 );
		}
		else if ( param_control == 14 ) {
			displayMain.trigger( displayMain.EVT_DELAY_MIX );
		}
		trigger( EVT_CONTROL );
		paramTimer.trigger( paramTimer.EVT_START );
	}										
	else {
   		paramTimer.trigger( paramTimer.EVT_START );   //trigger timer
   		Serial.println("wait display triggered");
		if ( param_control == 0 ) {
			displayMain.trigger( displayMain.EVT_DELAY_SEND_1 );
		}
		else if ( param_control == 1 ) {
			displayMain.trigger( displayMain.EVT_DELAY_SEND_2 );
		}
		else if ( param_control == 2 ) {
			displayMain.trigger( displayMain.EVT_DELAY_TIME_1 );
		}
		else if ( param_control == 6 ) {
			displayMain.trigger( displayMain.EVT_DELAY_FB_1 );
		}
		else if ( param_control == 14 ) {
			displayMain.trigger( displayMain.EVT_DELAY_MIX );
		}					
		delay(display_delay);       	//display wait
		trigger( EVT_CONTROL );			
	}
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
    "DELAY_EFFECT\0EVT_OFF\0EVT_CONTROL\0EVT_ENC_UP\0EVT_ENC_DOWN\0EVT_BTN_1\0ELSE\0OFF\0CONTROL\0ENC_UP\0ENC_DOWN\0BTN_1" );
  return *this;
}



