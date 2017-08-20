/*
8/8/16
started, compiled, basic operation and triggers tested, haven't tested with audio yet
-member of Atm_wav_1_multi_r5
=== Common Anode digits ===
Arduino  Display  Digit
 9         12       0 Leftmost
 10         9       1
 11         8       2
 12         6       3 Rightmost
 
=== Cathode Segments ===
Teensy    GPIOD    Display   Segment  
  2        0        11         A
  14       1        7          B
  7        2        4          C
  8        3        2          D
  6        4        1          E
  20       5        10         F
  21       6        5          G
  5        7        3         DEC  

===Segment Layout===
    A
  F   B
    G
  E   C
    D   
          byte .GFEDCBA
segment code, Bxxxxxxxx
*/ 
#include "Atm_sev_seg.h"
#include "display_levels.h"

int level = 0;

Atm_sev_seg& Atm_sev_seg::begin() {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*                ON_ENTER  			ON_LOOP  		ON_EXIT   EVT_HOME		EVT_OFF  EVT_MASTER_VOL  EVT_VOL_WAV_1  EVT_VOL_WAV_2  EVT_TRACK_WAV_1  EVT_TRACK_WAV_2  EVT_PLAY_WAV_1  EVT_PLAY_WAV_2  ELSE */
    /*        HOME */    ENT_HOME,      		-1,      	 	-1,     HOME,      		-1,     MASTER_VOL,    VOL_WAV_1,     VOL_WAV_2,           -1,             -1,           -1,             -1,           -1,
	/*         OFF */    ENT_OFF,      			-1,      		-1,     HOME,    		-1,     MASTER_VOL,    VOL_WAV_1,     VOL_WAV_2,           -1,             -1,           -1,             -1,           -1,       
    /*  MASTER_VOL */    ENT_MASTER_VOL,  		-1,  			-1,     HOME,      		-1,     MASTER_VOL,    VOL_WAV_1,     VOL_WAV_2,           -1,             -1,           -1,             -1,           -1,
    /*   VOL_WAV_1 */    ENT_VOL_WAV_1, 		-1,      		-1,     HOME,     		-1,     MASTER_VOL,    VOL_WAV_1,     VOL_WAV_2,           -1,             -1,           -1,             -1,           -1,
    /*   VOL_WAV_2 */    ENT_VOL_WAV_2,   		-1,      		-1,     HOME,     		-1,     MASTER_VOL,    VOL_WAV_1,     VOL_WAV_2,           -1,             -1,           -1,             -1,           -1,
    /* TRACK_WAV_1 */    ENT_TRACK_WAV_1,  	ENT_TRACK_WAV_1,    -1,     HOME,      		-1,     MASTER_VOL,    VOL_WAV_1,     VOL_WAV_2,           -1,             -1,           -1,             -1,           -1,
    /* TRACK_WAV_2 */    ENT_TRACK_WAV_2, 	ENT_TRACK_WAV_2,    -1,     HOME,      		-1,     MASTER_VOL,    VOL_WAV_1,     VOL_WAV_2,           -1,             -1,           -1,             -1,           -1,
    /*  PLAY_WAV_1 */    ENT_PLAY_WAV_1,  	ENT_PLAY_WAV_1,     -1,     HOME,      		-1,     MASTER_VOL,    VOL_WAV_1,     VOL_WAV_2,           -1,             -1,           -1,             -1,           -1,
    /*  PLAY_WAV_2 */    ENT_PLAY_WAV_2,  	ENT_PLAY_WAV_2,     -1,     HOME,      		-1,     MASTER_VOL,    VOL_WAV_1,     VOL_WAV_2,           -1,             -1,           -1,             -1,           -1,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  //GPIOD_PDDR=0xff;    //PORT_D
  //GPIOD_PDOR=0x00;
  pinMode( digPins[0], OUTPUT ); //set digPins
  /*
  pinMode( digPins[1], OUTPUT );
  pinMode( digPins[2], OUTPUT );
  pinMode( digPins[3], OUTPUT );
  */
  pinMode( segPins[0], OUTPUT ); //set segPins
  /*
  pinMode( segPins[1], OUTPUT );
  pinMode( segPins[2], OUTPUT );
  pinMode( segPins[3], OUTPUT );
  pinMode( segPins[4], OUTPUT );
  pinMode( segPins[5], OUTPUT );
  pinMode( segPins[6], OUTPUT );
  pinMode( segPins[7], OUTPUT );
  */
  return *this;          
}

/* Add C++ code for each internally handled event (input) 
 * The code must return 1 to trigger the event
 */

int Atm_sev_seg::event( int id ) {
  switch ( id ) {
     case EVT_HOME:
        return 0;
	case EVT_OFF:
      return 0;
    case EVT_MASTER_VOL:
      return 0;
    case EVT_VOL_WAV_1:
      return 0;
    case EVT_VOL_WAV_2:
      return 0;
    case EVT_TRACK_WAV_1:
      return 0;
    case EVT_TRACK_WAV_2:
      return 0;
    case EVT_PLAY_WAV_1:
      return 0;
    case EVT_PLAY_WAV_2:
      return 0;
  }
  return 0;
}

/* Add C++ code for each action
 * This generates the 'output' for the state machine
 */

void Atm_sev_seg::action( int id ) {
  switch ( id ) {
  	case ENT_HOME:
  	  //write(0, 11); write(1, 11); write(2, 36); write(3, 2); //BB-2
	  Serial.println("HOME-sev-seg");
  	  return;
	case ENT_OFF:
	  //write(0, 37); write(1, 37); write(2, 37); write(3, 37); //all off
	  Serial.println("----");
	  return;
	case ENT_MASTER_VOL:
	  level = mas_vol_level;
	  //write(0, 31); write(1, 24); write(2, 21); writeLevel(level); //VOL(level)
	  Serial.println("MASTER VOL sev seg");
	  Serial.println("MASTER VOL sev seg");
	  Serial.print(level);
	  return;
  	case ENT_VOL_WAV_1:
	  level = vol_wav_1_level;
  	  //write(0, 31); write(1, 21); write(2, 1); writeLevel(level); //VL1(level)
	  Serial.print("VOL 1 sev seg");
	  Serial.print(level);
	  return;
    case ENT_VOL_WAV_2:
	  level = vol_wav_2_level;
      //write(0, 31); write(1, 21); write(2, 2); writeLevel(level); //VL2(level)
	  Serial.print("VOL 2 sev seg");
	  Serial.print(level);
  	  return;
   	case ENT_TRACK_WAV_1:
      write(0, 29); write(1, 27); write(2, 1); writeLevel(level); //tr1(level)
	  Serial.print("TRACK 1 ");
	  Serial.print(level);
  	  return;
    case ENT_TRACK_WAV_2:
      write(0, 29); write(1, 27); write(2, 2); writeLevel(level); //tr2(level)
	  Serial.print("TRACK 2 ");
	  Serial.print(level);
      return;
	case ENT_PLAY_WAV_1:
	  write(0, 32); write(1, 10); write(2, 31); write(3, 1); ///WAV1
	  Serial.println("WAV 1");
	  return;
  	case ENT_PLAY_WAV_2:
  	  write(0, 32); write(1, 10); write(2, 31); write(3, 2); ///WAV2
	  Serial.println("WAV 2");
  	  return;
	  
  }
}

/* Optionally override the default trigger() method
 * Control how your machine processes triggers
 */

Atm_sev_seg& Atm_sev_seg::trigger( int event ) {
  Machine::trigger( event );
  return *this;
}

/* Optionally override the default state() method
 * Control what the machine returns when another process requests its state
 */

int Atm_sev_seg::state( void ) {
  return Machine::state();
}

Atm_sev_seg& Atm_sev_seg::write( int digit, int character ){
	if (digit == 0){
	    //digitalWriteFast(digPins[0],HIGH); //first digit on
	    //digitalWriteFast(digPins[1],LOW); //other pins off
	    //digitalWriteFast(digPins[2],LOW);
	    //digitalWriteFast(digPins[3],LOW);
	    //GPIOD_PDOR=numberLetter[character]; //set character
	    delay(2);
	 }
	 /*
	  else if (digit == 1) {
	    digitalWriteFast(digPins[0],LOW); //other pins off
	    digitalWriteFast(digPins[1],HIGH); //second digit on
	    digitalWriteFast(digPins[2],LOW);
	    digitalWriteFast(digPins[3],LOW);
	    GPIOD_PDOR=numberLetter[character]; //set character
		Serial.println("dig 2");
		Serial.println(character);
	    delay(2);
	  }
	  else if (digit == 2) {
	    digitalWriteFast(digPins[0],LOW); //other pins off
	    digitalWriteFast(digPins[1],LOW); 
	    digitalWriteFast(digPins[2],HIGH); //third digit on 
	    digitalWriteFast(digPins[3],LOW);
	    GPIOD_PDOR=numberLetter[character]; //set character
		Serial.println("dig 3");
		Serial.println(character);
	    delay(2);
	  }  
	  else if (digit == 3) {
	    digitalWriteFast(digPins[0],LOW); //other pins off
	    digitalWriteFast(digPins[1],LOW); 
	    digitalWriteFast(digPins[2],LOW);  
	    digitalWriteFast(digPins[3],HIGH); //Fourth digit on
	    GPIOD_PDOR=numberLetter[character]; //set character
		Serial.println("dig 4");
		Serial.println(character);
	    delay(2);
	  }
	 */
  return *this;
}

Atm_sev_seg& Atm_sev_seg::writeLevel( int level ) {
    	digitalWriteFast(digPins[0],LOW); //other pins off
    	//digitalWriteFast(digPins[1],LOW); 
    	//digitalWriteFast(digPins[2],LOW);  
    	//digitalWriteFast(digPins[3],HIGH); //Fourth digit on
		//GPIOD_PDOR=numberLetter[level]; //set character
		Serial.println(level);
		delay(2);
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

Atm_sev_seg& Atm_sev_seg::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "SEV_SEG\0EVT_HOME\0EVT_OFF\0EVT_MASTER_VOL\0EVT_VOL_WAV_1\0EVT_VOL_WAV_2\0EVT_TRACK_WAV_1\0EVT_TRACK_WAV_2\0EVT_PLAY_WAV_1\0EVT_PLAY_WAV_2\0ELSE\0HOME\0OFF\0MASTER_VOL\0VOL_WAV_1\0VOL_WAV_2\0TRACK_WAV_1\0TRACK_WAV_2\0PLAY_WAV_1\0PLAY_WAV_2" );
  return *this;
}