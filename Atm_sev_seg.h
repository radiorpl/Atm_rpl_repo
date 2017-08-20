#pragma once

#include <Automaton.h>

class Atm_sev_seg: public Machine {

 public:
  enum { HOME, OFF, MASTER_VOL, VOL_WAV_1, VOL_WAV_2, TRACK_WAV_1, TRACK_WAV_2, PLAY_WAV_1, PLAY_WAV_2 }; // STATES
  enum { EVT_HOME, EVT_OFF, EVT_MASTER_VOL, EVT_VOL_WAV_1, EVT_VOL_WAV_2, EVT_TRACK_WAV_1, EVT_TRACK_WAV_2, EVT_PLAY_WAV_1, EVT_PLAY_WAV_2, ELSE }; // EVENTS
  Atm_sev_seg( void ) : Machine() {};
  Atm_sev_seg& begin( void );
  Atm_sev_seg& trace( Stream & stream );
  Atm_sev_seg& trigger( int event );
  int state( void );
  Atm_sev_seg& write( int digit, int character );
  Atm_sev_seg& writeLevel( int level );
  

 private:
  enum { ENT_OFF, ENT_HOME, ENT_MASTER_VOL, ENT_VOL_WAV_1, ENT_VOL_WAV_2, ENT_TRACK_WAV_1, ENT_TRACK_WAV_2, ENT_PLAY_WAV_1, ENT_PLAY_WAV_2 }; // ACTIONS
  int event( int id ); 
  void action( int id );
  //real pins
  //int digPins[4] = { 9, 10, 11, 12};
  //int segPins[8] = { 2, 14, 7, 8, 6, 20, 21, 5 };
  //serial test pins
  int digPins[1] = { 31 };
  int segPins[1] = { 30 };  
  byte numberLetter[38] = {     
    B11000000, // 0
    B11111001, // 1
    B10100100, // 2
    B10110000, // 3
    B10011001, // 4
    B10010010, // 5
    B10000010, // 6
    B11111000, // 7
    B10000000, // 8
    B10010000,  // 9
    B10001000, // 10 A
    B10000011, // 11 b
    B11000110, // 12 C
    B10100001, // 13 d
    B10000110, // 14 E
    B10001110, // 15 F
    B10010000, // 16 g
    B10001001, // 17 H
    B11001111, // 18 I
    B11000001, // 19 J
    B10001001, // 20 K = H
    B11000111, // 21 L
    B11101010, // 22 m
    B10101011, // 23 n
    B11000000, // 24 O = 0
    B10001100, // 25 P
    B10011000, // 26 q
    B10101111, // 27 r
    B10010010, // 28 S
    B10000111, // 29 t
    B11000001, // 30 U
    B11100011, // 31 v
    B11010101, // 32 w
    B10001001, // 33 X = H
    B10010001, // 34 y
    B10100100,  // 35 Z = 2
	B10111111,  // 36 -
    B11111111  // 37 all off      
  }; 

};
