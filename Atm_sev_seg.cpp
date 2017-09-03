/*
8/8/16
started, compiled, basic operation and triggers tested, haven't tested with audio yet
-member of Atm_wav_1_multi_r5
8/24/17
rewrite for PCA9551 i2c led controller 
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
#include "Wire.h"

int level = 0;

Atm_sev_seg& Atm_sev_seg::begin() {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*                ON_ENTER  			ON_LOOP  		ON_EXIT   EVT_HOME		EVT_OFF  EVT_MASTER_VOL  EVT_VOL_WAV_1  EVT_VOL_WAV_2  EVT_TRACK_WAV_1  EVT_TRACK_WAV_2  EVT_PLAY_WAV_1  EVT_PLAY_WAV_2  EVT_DELAY   EVT_DELAY_SEND_1   EVT_DELAY_SEND_2   EVT_DELAY_TIME_1   EVT_DELAY_TIME_2   EVT_DELAY_TIME_3   EVT_DELAY_TIME_4   EVT_DELAY_FB_1   EVT_DELAY_FB_2   EVT_DELAY_FB_3   EVT_DELAY_FB_4   EVT_DELAY_GAIN_1   EVT_DELAY_GAIN_2   EVT_DELAY_GAIN_3   EVT_DELAY_GAIN_4   EVT_DELAY_MIX   ELSE */
    /*        HOME */    ENT_HOME,      		-1,      	 	-1,     HOME,      	   OFF,     MASTER_VOL,    VOL_WAV_1,     VOL_WAV_2,     TRACK_WAV_1,     TRACK_WAV_2,     PLAY_WAV_1,    PLAY_WAV_2,     DELAY,       DELAY_SEND_1,      DELAY_SEND_2,      DELAY_TIME_1,      DELAY_TIME_2,      DELAY_TIME_3,      DELAY_TIME_4,      DELAY_FB_1,      DELAY_FB_2,      DELAY_FB_3,      DELAY_FB_4,      DELAY_GAIN_1,      DELAY_GAIN_2,      DELAY_GAIN_3,      DELAY_GAIN_4,      DELAY_MIX,     HOME,                                                                                  
	/*         OFF */    ENT_OFF,      			-1,      		-1,     HOME,    	   OFF,     MASTER_VOL,    VOL_WAV_1,     VOL_WAV_2,     TRACK_WAV_1,     TRACK_WAV_2,     PLAY_WAV_1,    PLAY_WAV_2,     DELAY,       DELAY_SEND_1,      DELAY_SEND_2,      DELAY_TIME_1,      DELAY_TIME_2,      DELAY_TIME_3,      DELAY_TIME_4,      DELAY_FB_1,      DELAY_FB_2,      DELAY_FB_3,      DELAY_FB_4,      DELAY_GAIN_1,      DELAY_GAIN_2,      DELAY_GAIN_3,      DELAY_GAIN_4,      DELAY_MIX, 	OFF,
    /*  MASTER_VOL */    ENT_MASTER_VOL,  		-1,  			-1,     HOME,      	   OFF,     MASTER_VOL,    VOL_WAV_1,     VOL_WAV_2,     TRACK_WAV_1,     TRACK_WAV_2,     PLAY_WAV_1,    PLAY_WAV_2,     DELAY,       DELAY_SEND_1,      DELAY_SEND_2,      DELAY_TIME_1,      DELAY_TIME_2,      DELAY_TIME_3,      DELAY_TIME_4,      DELAY_FB_1,      DELAY_FB_2,      DELAY_FB_3,      DELAY_FB_4,      DELAY_GAIN_1,      DELAY_GAIN_2,      DELAY_GAIN_3,      DELAY_GAIN_4,      DELAY_MIX,     MASTER_VOL,
    /*   VOL_WAV_1 */    ENT_VOL_WAV_1, 		-1,      		-1,     HOME,     	   OFF,     MASTER_VOL,    VOL_WAV_1,     VOL_WAV_2,     TRACK_WAV_1,     TRACK_WAV_2,     PLAY_WAV_1,    PLAY_WAV_2,     DELAY,       DELAY_SEND_1,      DELAY_SEND_2,      DELAY_TIME_1,      DELAY_TIME_2,      DELAY_TIME_3,      DELAY_TIME_4,      DELAY_FB_1,      DELAY_FB_2,      DELAY_FB_3,      DELAY_FB_4,      DELAY_GAIN_1,      DELAY_GAIN_2,      DELAY_GAIN_3,      DELAY_GAIN_4,      DELAY_MIX,     VOL_WAV_1,
    /*   VOL_WAV_2 */    ENT_VOL_WAV_2,   		-1,      		-1,     HOME,     	   OFF,     MASTER_VOL,    VOL_WAV_1,     VOL_WAV_2,     TRACK_WAV_1,     TRACK_WAV_2,     PLAY_WAV_1,    PLAY_WAV_2,     DELAY,       DELAY_SEND_1,      DELAY_SEND_2,      DELAY_TIME_1,      DELAY_TIME_2,      DELAY_TIME_3,      DELAY_TIME_4,      DELAY_FB_1,      DELAY_FB_2,      DELAY_FB_3,      DELAY_FB_4,      DELAY_GAIN_1,      DELAY_GAIN_2,      DELAY_GAIN_3,      DELAY_GAIN_4,      DELAY_MIX,     VOL_WAV_2,
    /* TRACK_WAV_1 */    ENT_TRACK_WAV_1,  	    -1,    			-1,     HOME,      	   OFF,     MASTER_VOL,    VOL_WAV_1,     VOL_WAV_2,     TRACK_WAV_1,     TRACK_WAV_2,     PLAY_WAV_1,    PLAY_WAV_2,     DELAY,       DELAY_SEND_1,      DELAY_SEND_2,      DELAY_TIME_1,      DELAY_TIME_2,      DELAY_TIME_3,      DELAY_TIME_4,      DELAY_FB_1,      DELAY_FB_2,      DELAY_FB_3,      DELAY_FB_4,      DELAY_GAIN_1,      DELAY_GAIN_2,      DELAY_GAIN_3,      DELAY_GAIN_4,      DELAY_MIX,     TRACK_WAV_1,
    /* TRACK_WAV_2 */    ENT_TRACK_WAV_2, 	    -1,    			-1,     HOME,      	   OFF,     MASTER_VOL,    VOL_WAV_1,     VOL_WAV_2,     TRACK_WAV_1,     TRACK_WAV_2,     PLAY_WAV_1,    PLAY_WAV_2,     DELAY,       DELAY_SEND_1,      DELAY_SEND_2,      DELAY_TIME_1,      DELAY_TIME_2,      DELAY_TIME_3,      DELAY_TIME_4,      DELAY_FB_1,      DELAY_FB_2,      DELAY_FB_3,      DELAY_FB_4,      DELAY_GAIN_1,      DELAY_GAIN_2,      DELAY_GAIN_3,      DELAY_GAIN_4,      DELAY_MIX,     TRACK_WAV_2,
    /*  PLAY_WAV_1 */    ENT_PLAY_WAV_1,  	    -1,     		-1,     HOME,      	   OFF,     MASTER_VOL,    VOL_WAV_1,     VOL_WAV_2,     TRACK_WAV_1,     TRACK_WAV_2,     PLAY_WAV_1,    PLAY_WAV_2,     DELAY,       DELAY_SEND_1,      DELAY_SEND_2,      DELAY_TIME_1,      DELAY_TIME_2,      DELAY_TIME_3,      DELAY_TIME_4,      DELAY_FB_1,      DELAY_FB_2,      DELAY_FB_3,      DELAY_FB_4,      DELAY_GAIN_1,      DELAY_GAIN_2,      DELAY_GAIN_3,      DELAY_GAIN_4,      DELAY_MIX,	    PLAY_WAV_1,
    /*  PLAY_WAV_2 */    ENT_PLAY_WAV_2,  	    -1,     		-1,     HOME,      	   OFF,     MASTER_VOL,    VOL_WAV_1,     VOL_WAV_2,     TRACK_WAV_1,     TRACK_WAV_2,     PLAY_WAV_1,    PLAY_WAV_2,     DELAY,       DELAY_SEND_1,      DELAY_SEND_2,      DELAY_TIME_1,      DELAY_TIME_2,      DELAY_TIME_3,      DELAY_TIME_4,      DELAY_FB_1,      DELAY_FB_2,      DELAY_FB_3,      DELAY_FB_4,      DELAY_GAIN_1,      DELAY_GAIN_2,      DELAY_GAIN_3,      DELAY_GAIN_4,      DELAY_MIX,     PLAY_WAV_2, 
	/*  DELAY      */    ENT_DELAY,  	        -1,     		-1,     HOME,      	   OFF,     MASTER_VOL,    VOL_WAV_1,     VOL_WAV_2,     TRACK_WAV_1,     TRACK_WAV_2,     PLAY_WAV_1,    PLAY_WAV_2,     DELAY,       DELAY_SEND_1,      DELAY_SEND_2,      DELAY_TIME_1,      DELAY_TIME_2,      DELAY_TIME_3,      DELAY_TIME_4,      DELAY_FB_1,      DELAY_FB_2,      DELAY_FB_3,      DELAY_FB_4,      DELAY_GAIN_1,      DELAY_GAIN_2,      DELAY_GAIN_3,      DELAY_GAIN_4,      DELAY_MIX,     DELAY,
  /*  DELAY_SEND_1 */    ENT_DELAY_SEND_1,  	-1,     		-1,     HOME,      	   OFF,     MASTER_VOL,    VOL_WAV_1,     VOL_WAV_2,     TRACK_WAV_1,     TRACK_WAV_2,     PLAY_WAV_1,    PLAY_WAV_2,     DELAY,       DELAY_SEND_1,      DELAY_SEND_2,      DELAY_TIME_1,      DELAY_TIME_2,      DELAY_TIME_3,      DELAY_TIME_4,      DELAY_FB_1,      DELAY_FB_2,      DELAY_FB_3,      DELAY_FB_4,      DELAY_GAIN_1,      DELAY_GAIN_2,      DELAY_GAIN_3,      DELAY_GAIN_4,      DELAY_MIX,     DELAY_SEND_1, 
  /*  DELAY_SEND_2 */    ENT_DELAY_SEND_2,      -1,     		-1,     HOME,      	   OFF,     MASTER_VOL,    VOL_WAV_1,     VOL_WAV_2,     TRACK_WAV_1,     TRACK_WAV_2,     PLAY_WAV_1,    PLAY_WAV_2,     DELAY,       DELAY_SEND_1,      DELAY_SEND_2,      DELAY_TIME_1,      DELAY_TIME_2,      DELAY_TIME_3,      DELAY_TIME_4,      DELAY_FB_1,      DELAY_FB_2,      DELAY_FB_3,      DELAY_FB_4,      DELAY_GAIN_1,      DELAY_GAIN_2,      DELAY_GAIN_3,      DELAY_GAIN_4,      DELAY_MIX,     DELAY_SEND_2,
  /*  DELAY_TIME_1 */    ENT_DELAY_TIME_1,      -1,     		-1,     HOME,      	   OFF,     MASTER_VOL,    VOL_WAV_1,     VOL_WAV_2,     TRACK_WAV_1,     TRACK_WAV_2,     PLAY_WAV_1,    PLAY_WAV_2,     DELAY,       DELAY_SEND_1,      DELAY_SEND_2,      DELAY_TIME_1,      DELAY_TIME_2,      DELAY_TIME_3,      DELAY_TIME_4,      DELAY_FB_1,      DELAY_FB_2,      DELAY_FB_3,      DELAY_FB_4,      DELAY_GAIN_1,      DELAY_GAIN_2,      DELAY_GAIN_3,      DELAY_GAIN_4,      DELAY_MIX,     DELAY_TIME_1,
  /*  DELAY_TIME_2 */    ENT_DELAY_TIME_2,  	-1,     		-1,     HOME,      	   OFF,     MASTER_VOL,    VOL_WAV_1,     VOL_WAV_2,     TRACK_WAV_1,     TRACK_WAV_2,     PLAY_WAV_1,    PLAY_WAV_2,     DELAY,       DELAY_SEND_1,      DELAY_SEND_2,      DELAY_TIME_1,      DELAY_TIME_2,      DELAY_TIME_3,      DELAY_TIME_4,      DELAY_FB_1,      DELAY_FB_2,      DELAY_FB_3,      DELAY_FB_4,      DELAY_GAIN_1,      DELAY_GAIN_2,      DELAY_GAIN_3,      DELAY_GAIN_4,      DELAY_MIX,     DELAY_TIME_2,
  /*  DELAY_TIME_3 */    ENT_DELAY_TIME_3,      -1,     		-1,     HOME,      	   OFF,     MASTER_VOL,    VOL_WAV_1,     VOL_WAV_2,     TRACK_WAV_1,     TRACK_WAV_2,     PLAY_WAV_1,    PLAY_WAV_2,     DELAY,       DELAY_SEND_1,      DELAY_SEND_2,      DELAY_TIME_1,      DELAY_TIME_2,      DELAY_TIME_3,      DELAY_TIME_4,      DELAY_FB_1,      DELAY_FB_2,      DELAY_FB_3,      DELAY_FB_4,      DELAY_GAIN_1,      DELAY_GAIN_2,      DELAY_GAIN_3,      DELAY_GAIN_4,      DELAY_MIX,     DELAY_TIME_3,
	/*DELAY_TIME_4 */    ENT_DELAY_TIME_4,      -1,     		-1,     HOME,      	   OFF,     MASTER_VOL,    VOL_WAV_1,     VOL_WAV_2,     TRACK_WAV_1,     TRACK_WAV_2,     PLAY_WAV_1,    PLAY_WAV_2,     DELAY,       DELAY_SEND_1,      DELAY_SEND_2,      DELAY_TIME_1,      DELAY_TIME_2,      DELAY_TIME_3,      DELAY_TIME_4,      DELAY_FB_1,      DELAY_FB_2,      DELAY_FB_3,      DELAY_FB_4,      DELAY_GAIN_1,      DELAY_GAIN_2,      DELAY_GAIN_3,      DELAY_GAIN_4,      DELAY_MIX,     DELAY_TIME_4,
	/*DELAY_FB_1   */    ENT_DELAY_FB_1,  	    -1,     		-1,     HOME,          OFF,     MASTER_VOL,    VOL_WAV_1,     VOL_WAV_2,     TRACK_WAV_1,     TRACK_WAV_2,     PLAY_WAV_1,    PLAY_WAV_2,     DELAY,       DELAY_SEND_1,      DELAY_SEND_2,      DELAY_TIME_1,      DELAY_TIME_2,      DELAY_TIME_3,      DELAY_TIME_4,      DELAY_FB_1,      DELAY_FB_2,      DELAY_FB_3,      DELAY_FB_4,      DELAY_GAIN_1,      DELAY_GAIN_2,      DELAY_GAIN_3,      DELAY_GAIN_4,      DELAY_MIX,     DELAY_FB_1,
    /*DELAY_FB_2   */    ENT_DELAY_FB_2,  	    -1,     		-1,     HOME,      	   OFF,     MASTER_VOL,    VOL_WAV_1,     VOL_WAV_2,     TRACK_WAV_1,     TRACK_WAV_2,     PLAY_WAV_1,    PLAY_WAV_2,     DELAY,       DELAY_SEND_1,      DELAY_SEND_2,      DELAY_TIME_1,      DELAY_TIME_2,      DELAY_TIME_3,      DELAY_TIME_4,      DELAY_FB_1,      DELAY_FB_2,      DELAY_FB_3,      DELAY_FB_4,      DELAY_GAIN_1,      DELAY_GAIN_2,      DELAY_GAIN_3,      DELAY_GAIN_4,      DELAY_MIX,     DELAY_FB_2,
	/*DELAY_FB_3   */    ENT_DELAY_FB_3  	    -1,     		-1,     HOME,      	   OFF,     MASTER_VOL,    VOL_WAV_1,     VOL_WAV_2,     TRACK_WAV_1,     TRACK_WAV_2,     PLAY_WAV_1,    PLAY_WAV_2,     DELAY,       DELAY_SEND_1,      DELAY_SEND_2,      DELAY_TIME_1,      DELAY_TIME_2,      DELAY_TIME_3,      DELAY_TIME_4,      DELAY_FB_1,      DELAY_FB_2,      DELAY_FB_3,      DELAY_FB_4,      DELAY_GAIN_1,      DELAY_GAIN_2,      DELAY_GAIN_3,      DELAY_GAIN_4,      DELAY_MIX,     DELAY_FB_3,
	/*DELAY_FB_4   */    ENT_DELAY_FB_4,  	    -1,     		-1,     HOME,      	   OFF,     MASTER_VOL,    VOL_WAV_1,     VOL_WAV_2,     TRACK_WAV_1,     TRACK_WAV_2,     PLAY_WAV_1,    PLAY_WAV_2,     DELAY,       DELAY_SEND_1,      DELAY_SEND_2,      DELAY_TIME_1,      DELAY_TIME_2,      DELAY_TIME_3,      DELAY_TIME_4,      DELAY_FB_1,      DELAY_FB_2,      DELAY_FB_3,      DELAY_FB_4,      DELAY_GAIN_1,      DELAY_GAIN_2,      DELAY_GAIN_3,      DELAY_GAIN_4,      DELAY_MIX,     DELAY_FB_4,
   /*DELAY_GAIN_1  */    ENT_DELAY_GAIN_1,      -1,     	    -1,     HOME,      	   OFF,     MASTER_VOL,    VOL_WAV_1,     VOL_WAV_2,     TRACK_WAV_1,     TRACK_WAV_2,     PLAY_WAV_1,    PLAY_WAV_2,     DELAY,       DELAY_SEND_1,      DELAY_SEND_2,      DELAY_TIME_1,      DELAY_TIME_2,      DELAY_TIME_3,      DELAY_TIME_4,      DELAY_FB_1,      DELAY_FB_2,      DELAY_FB_3,      DELAY_FB_4,      DELAY_GAIN_1,      DELAY_GAIN_2,      DELAY_GAIN_3,      DELAY_GAIN_4,      DELAY_MIX,     DELAY_GAIN_1,
   /*DELAY_GAIN_2  */    ENT_DELAY_GAIN_2,      -1,     	    -1,     HOME,      	   OFF,     MASTER_VOL,    VOL_WAV_1,     VOL_WAV_2,     TRACK_WAV_1,     TRACK_WAV_2,     PLAY_WAV_1,    PLAY_WAV_2,     DELAY,       DELAY_SEND_1,      DELAY_SEND_2,      DELAY_TIME_1,      DELAY_TIME_2,      DELAY_TIME_3,      DELAY_TIME_4,      DELAY_FB_1,      DELAY_FB_2,      DELAY_FB_3,      DELAY_FB_4,      DELAY_GAIN_1,      DELAY_GAIN_2,      DELAY_GAIN_3,      DELAY_GAIN_4,      DELAY_MIX,     DELAY_GAIN_2,
   /*DELAY_GAIN_3  */    ENT_DELAY_GAIN_3,      -1,    		    -1,     HOME,      	   OFF,     MASTER_VOL,    VOL_WAV_1,     VOL_WAV_2,     TRACK_WAV_1,     TRACK_WAV_2,     PLAY_WAV_1,    PLAY_WAV_2,     DELAY,       DELAY_SEND_1,      DELAY_SEND_2,      DELAY_TIME_1,      DELAY_TIME_2,      DELAY_TIME_3,      DELAY_TIME_4,      DELAY_FB_1,      DELAY_FB_2,      DELAY_FB_3,      DELAY_FB_4,      DELAY_GAIN_1,      DELAY_GAIN_2,      DELAY_GAIN_3,      DELAY_GAIN_4,      DELAY_MIX,     DELAY_GAIN_3,
   /*DELAY_GAIN_4  */    ENT_DELAY_GAIN_4,      -1,    		    -1,     HOME,      	   OFF,     MASTER_VOL,    VOL_WAV_1,     VOL_WAV_2,     TRACK_WAV_1,     TRACK_WAV_2,     PLAY_WAV_1,    PLAY_WAV_2,     DELAY,       DELAY_SEND_1,      DELAY_SEND_2,      DELAY_TIME_1,      DELAY_TIME_2,      DELAY_TIME_3,      DELAY_TIME_4,      DELAY_FB_1,      DELAY_FB_2,      DELAY_FB_3,      DELAY_FB_4,      DELAY_GAIN_1,      DELAY_GAIN_2,      DELAY_GAIN_3,      DELAY_GAIN_4,      DELAY_MIX,     DELAY_GAIN_4,
	/*  DELAY_MIX  */    ENT_DELAY_MIX,  	    -1,     		-1,     HOME,      	   OFF,     MASTER_VOL,    VOL_WAV_1,     VOL_WAV_2,     TRACK_WAV_1,     TRACK_WAV_2,     PLAY_WAV_1,    PLAY_WAV_2,     DELAY,       DELAY_SEND_1,      DELAY_SEND_2,      DELAY_TIME_1,      DELAY_TIME_2,      DELAY_TIME_3,      DELAY_TIME_4,      DELAY_FB_1,      DELAY_FB_2,      DELAY_FB_3,      DELAY_FB_4,      DELAY_GAIN_1,      DELAY_GAIN_2,      DELAY_GAIN_3,      DELAY_GAIN_4,      DELAY_MIX,     DELAY_MIX,
	              
  
  
  
  
  
  
  
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  for ( int pinDig = 0; pinDig < pinCountDig; pinDig++ ) {    // set digit pins as outputs
    pinMode(digPins[pinDig], OUTPUT);
  }
  Wire.begin();					//begin i2c
  Wire.beginTransmission(96);  //set blink time
  Wire.write(command[1]);
  Wire.write(18); 			  //500ms
  Wire.write(128);				// 50% duty
  Wire.endTransmission();
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
    case EVT_DELAY:
       return 0;
 	case EVT_DELAY_SEND_1:
       return 0;
    case EVT_DELAY_SEND_2:
       return 0;
    case EVT_DELAY_TIME_1:
       return 0;
    case EVT_DELAY_TIME_2:
       return 0;
    case EVT_DELAY_TIME_3:
       return 0;
    case EVT_DELAY_TIME_4:
       return 0;
    case EVT_DELAY_FB_1:
       return 0;
    case EVT_DELAY_FB_2:
       return 0;
    case EVT_DELAY_FB_3:
      return 0;
	case EVT_DELAY_FB_4:
      return 0;
    case EVT_DELAY_GAIN_1:
      return 0;
    case EVT_DELAY_GAIN_2:
      return 0;
    case EVT_DELAY_GAIN_3:
      return 0;
    case EVT_DELAY_GAIN_4:
      return 0;
    case EVT_DELAY_TIME_4:
      return 0;
    case EVT_DELAY_MIX:
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
	  write(0, 11); write(1, 11); write(2, 32); write(3, 2); //bb-2
  	  return;
	case ENT_OFF:
	  write(0, 36); write(1, 36); write(2, 36); write(3, 36); //all off
	  return;
	case ENT_MASTER_VOL:
	  level = mas_vol_level;
	  write(0, 21); write(1, 10); write(2, 27); writeLevel(level); //mAS(level)
	  return;
  	case ENT_VOL_WAV_1:
	  level = vol_wav_1_level;
  	  write(0, 10); write(1, 36); write(2, 36); writeLevel(level); //A  (level)
	  return;
    case ENT_VOL_WAV_2:
	  level = vol_wav_2_level;
  	  write(0, 11); write(1, 36); write(2, 36); writeLevel(level); //b (level)
  	  return;
   	case ENT_TRACK_WAV_1:
	  level = track_1_level;
      write(0, 28); write(1, 26); write(2, 10); writeLevel(level); //trA(level)
  	  return;
    case ENT_TRACK_WAV_2:
	  level = track_2_level;
      write(0, 28); write(1, 26); write(2, 11); writeLevel(level); //trb(level)
      return;
	case ENT_PLAY_WAV_1:
	  level = last_state_1;
	  if ( level == 0 ) {
	  	write(0, 10); write(1, 23); write(2, 15); write(3, 15); //Aoff
	  }
	  else if ( level != 0 ) {
	  	write(0, 10); write(1, 36); write(2, 23); write(3, 22); //A on
	  }
	  return;
  	case ENT_PLAY_WAV_2:
	  level = last_state_2;
	  if ( level == 0 ) {
	  	write(0, 11); write(1, 23); write(2, 15); write(3, 15); //boff
	  }
	  else if ( level != 0 ) {
	  	write(0, 11); write(1, 36); write(2, 23); write(3, 22); //b on
	  }
	  return;
	case ENT_DELAY:
	  return;
  	case ENT_DELAY_SEND_1:
  	  return;  
  	case ENT_DELAY_SEND_2:
  	  return;
	case ENT_DELAY_TIME_1:
	  return;  
  	case ENT_DELAY_TIME_2:
  	  return;
	case ENT_DELAY_TIME_3:
	  return;  
	case ENT_DELAY_TIME_4:
	  return;
  	case ENT_DELAY_FB_1:
  	  return;    
  	case ENT_DELAY_FB_2:
  	  return;
	case ENT_DELAY_FB_3:
	  return;  
	case ENT_DELAY_FB_4:
	  return;
	case ENT_DELAY_GAIN_1:
	  return;  
	case ENT_DELAY_GAIN_2:
	  return;
	case ENT_DELAY_GAIN_3:
	  return;  
	case ENT_DELAY_GAIN_4:
	  return;
	case ENT_DELAY_MIX:
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
		//write digits
	  if (digit == 0){
		digitalWriteFast(digPins[0],HIGH); //first digit on
	    digitalWriteFast(digPins[1],LOW); //other pins off
	    digitalWriteFast(digPins[2],LOW);
	    digitalWriteFast(digPins[3],LOW);
	  	//i2c
	  	Wire.beginTransmission(96);
	  	Wire.write(command[0]);
		Wire.write(seg0[character]);
		Wire.write(seg1[character]);
	  	Wire.endTransmission();
	    delay(2);
	  	Wire.beginTransmission(96);
	  	Wire.write(command[0]);
		Wire.write(seg0[36]);
		Wire.write(seg1[36]);
	  	Wire.endTransmission();
	 }
	  else if (digit == 1) {
	    digitalWriteFast(digPins[0],LOW); //other pins off
	    digitalWriteFast(digPins[1],HIGH); //second digit on
	    digitalWriteFast(digPins[2],LOW);
	    digitalWriteFast(digPins[3],LOW);
	  	Wire.beginTransmission(96);
	  	Wire.write(command[0]);
		Wire.write(seg0[character]);
		Wire.write(seg1[character]);
	  	Wire.endTransmission();
	    //Serial.println(digit);
	    delay(2);
	  	Wire.beginTransmission(96);
	  	Wire.write(command[0]);
		Wire.write(seg0[36]);
		Wire.write(seg1[36]);
	  	Wire.endTransmission();
	  }
	  else if (digit == 2) {
	    digitalWriteFast(digPins[0],LOW); //other pins off
	    digitalWriteFast(digPins[1],LOW); 
	    digitalWriteFast(digPins[2],HIGH); //third digit on 
	    digitalWriteFast(digPins[3],LOW);
	  	Wire.beginTransmission(96);
	  	Wire.write(command[0]);
		Wire.write(seg0[character]);
		Wire.write(seg1[character]);
	  	Wire.endTransmission();
	    delay(2);
	  	Wire.beginTransmission(96);
	  	Wire.write(command[0]);
		Wire.write(seg0[36]);
		Wire.write(seg1[36]);
	  	Wire.endTransmission();
	  }  
	  else if (digit == 3) {
	    digitalWriteFast(digPins[0],LOW); //other pins off
	    digitalWriteFast(digPins[1],LOW); 
	    digitalWriteFast(digPins[2],LOW);  
	    digitalWriteFast(digPins[3],HIGH); //Fourth digit on
	  	Wire.beginTransmission(96);
	  	Wire.write(command[0]);
		Wire.write(seg0[character]);
		Wire.write(seg1[character]);
	  	Wire.endTransmission();
	    delay(2);
	  	Wire.beginTransmission(96);
	  	Wire.write(command[0]);
		Wire.write(seg0[36]);
		Wire.write(seg1[36]);
	  	Wire.endTransmission();
	  }
  return *this;
}

Atm_sev_seg& Atm_sev_seg::writeBlink( int digit, int character ){
		//write digits
	  if (digit == 0){
		digitalWriteFast(digPins[0],HIGH); //first digit on
	    digitalWriteFast(digPins[1],LOW); //other pins off
	    digitalWriteFast(digPins[2],LOW);
	    digitalWriteFast(digPins[3],LOW);
	  	//i2c
	  	Wire.beginTransmission(96);
	  	Wire.write(command[0]);
		Wire.write(segBlink0[character]);
		Wire.write(segBlink1[character]);
	  	Wire.endTransmission();
	    delay(2);
	  	Wire.beginTransmission(96);
	  	Wire.write(command[0]);
		Wire.write(segBlink0[36]);
		Wire.write(segBlink1[36]);
	  	Wire.endTransmission();
	 }
	  else if (digit == 1) {
	    digitalWriteFast(digPins[0],LOW); //other pins off
	    digitalWriteFast(digPins[1],HIGH); //second digit on
	    digitalWriteFast(digPins[2],LOW);
	    digitalWriteFast(digPins[3],LOW);
	  	Wire.beginTransmission(96);
	  	Wire.write(command[0]);
		Wire.write(segBlink0[character]);
		Wire.write(segBlink1[character]);
	  	Wire.endTransmission();
	    //Serial.println(digit);
	    delay(2);
	  	Wire.beginTransmission(96);
	  	Wire.write(command[0]);
		Wire.write(segBlink0[36]);
		Wire.write(segBlink1[36]);
	  	Wire.endTransmission();
	  }
	  else if (digit == 2) {
	    digitalWriteFast(digPins[0],LOW); //other pins off
	    digitalWriteFast(digPins[1],LOW); 
	    digitalWriteFast(digPins[2],HIGH); //third digit on 
	    digitalWriteFast(digPins[3],LOW);
	  	Wire.beginTransmission(96);
	  	Wire.write(command[0]);
		Wire.write(segBlink0[character]);
		Wire.write(segBlink1[character]);
	  	Wire.endTransmission();
	    delay(2);
	  	Wire.beginTransmission(96);
	  	Wire.write(command[0]);
		Wire.write(segBlink0[36]);
		Wire.write(segBlink1[36]);
	  	Wire.endTransmission();
	  }  
	  else if (digit == 3) {
	    digitalWriteFast(digPins[0],LOW); //other pins off
	    digitalWriteFast(digPins[1],LOW); 
	    digitalWriteFast(digPins[2],LOW);  
	    digitalWriteFast(digPins[3],HIGH); //Fourth digit on
	  	Wire.beginTransmission(96);
	  	Wire.write(command[0]);
		Wire.write(segBlink0[character]);
		Wire.write(segBlink1[character]);
	  	Wire.endTransmission();
	    delay(2);
	  	Wire.beginTransmission(96);
	  	Wire.write(command[0]);
		Wire.write(segBlink0[36]);
		Wire.write(segBlink1[36]);
	  	Wire.endTransmission();
	  }
  return *this;
}


Atm_sev_seg& Atm_sev_seg::writeLevel( int level ) {
    	digitalWriteFast(digPins[0],LOW); //other pins off
    	digitalWriteFast(digPins[1],LOW); 
    	digitalWriteFast(digPins[2],LOW);  
    	digitalWriteFast(digPins[3],HIGH); //Fourth digit on
	  	Wire.beginTransmission(96);
	  	Wire.write(command[0]);
		Wire.write(seg0[level]);
		Wire.write(seg1[level]);
	  	Wire.endTransmission();
		//Serial.println(level);
		delay(2);
	  	Wire.beginTransmission(96);
	  	Wire.write(command[0]);
		Wire.write(segBlink0[36]);
		Wire.write(segBlink1[36]);
	  	Wire.endTransmission();
		return *this;
}

Atm_sev_seg& Atm_sev_seg::writeLevelBlink( int level ) {
    	digitalWriteFast(digPins[0],LOW); //other pins off
    	digitalWriteFast(digPins[1],LOW); 
    	digitalWriteFast(digPins[2],LOW);  
    	digitalWriteFast(digPins[3],HIGH); //Fourth digit on
	  	Wire.beginTransmission(96);
	  	Wire.write(command[0]);
		Wire.write(segBlink0[level]);
		Wire.write(segBlink1[level]);
	  	Wire.endTransmission();
		//Serial.println(level);
		delay(2);
	  	Wire.beginTransmission(96);
	  	Wire.write(command[0]);
		Wire.write(segBlink0[36]);
		Wire.write(segBlink1[36]);
	  	Wire.endTransmission();
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