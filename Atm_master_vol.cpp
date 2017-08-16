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
*/	
#include "Atm_master_vol.h"
#include "audio_system.h"
#include "display_def.h"

int mas_vol_level; //variables for display
int vol_wav_1_level; 
int vol_wav_2_level; 

	
Atm_master_vol& Atm_master_vol::begin( int volCon ) {
  	// clang-format off
	static const state_t state_table[] PROGMEM = {
		/*               	ON_ENTER    		ON_LOOP   ON_EXIT	EVT_ENC_UP		EVT_ENC_DOWN		EVT_BTN_1	 	EVT_TIMER_1   EVT_TIMER_2 	EVT_VOL_0  EVT_VOL_1  EVT_VOL_2  EVT_VOL_3  EVT_VOL_4  EVT_VOL_5  EVT_VOL_6  EVT_VOL_7  EVT_VOL_8  EVT_VOL_9  EVT_VOL_10  EVT_VOL_11  EVT_VOL_12  EVT_VOL_13  EVT_VOL_14  EVT_VOL_15  EVT_VOL_16  EVT_VOL_17  EVT_VOL_18  EVT_VOL_19  EVT_VOL_20  EVT_VOL_21  EVT_VOL_22  EVT_VOL_23  EVT_VOL_24  EVT_VOL_25  EVT_VOL_26  EVT_VOL_27  EVT_VOL_28  EVT_VOL_29  EVT_VOL_30  EVT_VOL_31  EVT_VOL_32  EVT_VOL_33	ELSE */
		/*	START_UP */		ENT_VOL_17, 			-1,		-1,			VOL_18,	  	 	VOL_16,				-1,					-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,
		/*	HOME 	*/		ENT_HOME, 				-1,		-1,			DISPLAY_1, 		DISPLAY_1,	  	 DISPLAY_1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,			
		/*DISPLAY_1 */		ENT_DISPLAY, 			-1,		-1,			-1,	  	 		-1,					-1,				EXIT_DISPLAY,	-1, 		VOL_0, 	VOL_1, 	  VOL_2, 		VOL_3, 	   VOL_4,   VOL_5, 	  VOL_6, 	  VOL_7, 	VOL_8, 		VOL_9, 		VOL_10, 	VOL_11, 	VOL_12, 	VOL_13, 	VOL_14, 	VOL_15, 	VOL_16, 	VOL_17, 	VOL_18, 	VOL_19, 	VOL_20, 	VOL_21, 	VOL_22, 	VOL_23, 	VOL_24, 	VOL_25, 	VOL_26, 	VOL_27, 	VOL_28, 	VOL_29, 	VOL_30, 	VOL_31, 	VOL_32, 	VOL_33,	 	 -1,			  
		/*	VOL_0 */		ENT_VOL_0, 				-1,		-1,			VOL_1,	  	 	-1,					-1,					-1,		  HOME,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,	  
		/*	VOL_1 */		ENT_VOL_1, 				-1,		-1,			VOL_2,	  	 	VOL_0,				-1,					-1,		  HOME,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,
		/*	VOL_2 */		ENT_VOL_2, 				-1,		-1,			VOL_3,	  	 	VOL_1,				-1,					-1,		  HOME,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,
		/*	VOL_3*/			ENT_VOL_3, 				-1,		-1,			VOL_4,	  	 	VOL_2,				-1,					-1,		  HOME,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,
		/*	VOL_4 */		ENT_VOL_4, 				-1,		-1,			VOL_5,	  	 	VOL_3,				-1,					-1,		  HOME,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,
		/*	VOL_5 */		ENT_VOL_5, 				-1,		-1,			VOL_6,	  	 	VOL_4,				-1,					-1,		  HOME,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,	  
		/*	VOL_6 */		ENT_VOL_6, 				-1,		-1,			VOL_7,	  	 	VOL_5,				-1,					-1,		  HOME,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,
		/*	VOL_7 */		ENT_VOL_7, 				-1,		-1,			VOL_8,	  	 	VOL_6,				-1,					-1,		  HOME,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,
		/*	VOL_8*/			ENT_VOL_8, 				-1,		-1,			VOL_9,	  	 	VOL_7,				-1,					-1,		  HOME,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,
		/*	VOL_9 */		ENT_VOL_9, 				-1,		-1,			VOL_10,	  	 	VOL_8,				-1,					-1,		  HOME,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,	
		/*	VOL_10 */		ENT_VOL_10, 			-1,		-1,			VOL_11,	  	 	VOL_9,				-1,					-1,		  HOME,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,	  
		/*	VOL_11 */		ENT_VOL_11, 			-1,		-1,			VOL_12,	  	 	VOL_10,				-1,					-1,		  HOME,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,
		/*	VOL_12 */		ENT_VOL_12, 			-1,		-1,			VOL_13,	  	 	VOL_11,				-1,					-1,		  HOME,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,
		/*	VOL_13*/		ENT_VOL_13, 			-1,		-1,			VOL_14,	  	 	VOL_12,				-1,					-1,		  HOME,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,
		/*	VOL_14 */		ENT_VOL_14, 			-1,		-1,			VOL_15,	  	 	VOL_13,				-1,					-1,		  HOME,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,
		/*	VOL_15 */		ENT_VOL_15, 			-1,		-1,			VOL_16,	  	 	VOL_14,				-1,					-1,		  HOME,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,	  
		/*	VOL_16 */		ENT_VOL_16, 			-1,		-1,			VOL_17,	  	 	VOL_15,				-1,					-1,		  HOME,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,
		/*	VOL_17 */		ENT_VOL_17, 			-1,		-1,			VOL_18,	  	 	VOL_16,				-1,					-1,		  HOME,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,
		/*	VOL_18*/		ENT_VOL_18,				-1,		-1,			VOL_19,	  	 	VOL_17,				-1,					-1,		  HOME,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,
		/*	VOL_19 */		ENT_VOL_19, 			-1,		-1,			VOL_20,	  	 	VOL_18,				-1,					-1,		  HOME,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		
		/*	VOL_20 */		ENT_VOL_20, 			-1,		-1,			VOL_21,	  	 	VOL_19,				-1,					-1,		  HOME,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,	  
		/*	VOL_21 */		ENT_VOL_21, 			-1,		-1,			VOL_22,	  	 	VOL_20,				-1,					-1,		  HOME,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,
		/*	VOL_22 */		ENT_VOL_22, 			-1,		-1,			VOL_23,	  	 	VOL_21,				-1,					-1,		  HOME,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,
		/*	VOL_23*/		ENT_VOL_23, 			-1,		-1,			VOL_24,	  	 	VOL_22,				-1,					-1,		  HOME,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,
		/*	VOL_24 */		ENT_VOL_24, 			-1,		-1,			VOL_25,	  	 	VOL_23,				-1,					-1,		  HOME,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,
		/*	VOL_25 */		ENT_VOL_25, 			-1,		-1,			VOL_26,	  	 	VOL_24,				-1,					-1,		  HOME,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,	  
		/*	VOL_26 */		ENT_VOL_26, 			-1,		-1,			VOL_27,	  	 	VOL_25,				-1,					-1,		  HOME,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,
		/*	VOL_27 */		ENT_VOL_27, 			-1,		-1,			VOL_28,	  	 	VOL_26,				-1,					-1,		  HOME,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,
		/*	VOL_28*/		ENT_VOL_28, 			-1,		-1,			VOL_29,	  	 	VOL_27,				-1,					-1,		  HOME,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,
		/*	VOL_29 */		ENT_VOL_29, 			-1,		-1,			VOL_30,	  	 	VOL_28,				-1,					-1,		  HOME,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,	
		/*	VOL_30 */		ENT_VOL_30, 			-1,		-1,			VOL_31,	  	 	VOL_29,				-1,					-1,		  HOME,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,	
		/*	VOL_31 */		ENT_VOL_31, 			-1,		-1,			VOL_32,	  	 	VOL_30,				-1,					-1,		  HOME,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,	
		/*	VOL_32 */		ENT_VOL_32, 			-1,		-1,			VOL_33,	  	 	VOL_31,				-1,					-1,		  HOME,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,	
		/*	VOL_33 */		ENT_VOL_33, 			-1,		-1,			 -1,	  	 	VOL_32,				-1,					-1,		  HOME,			-1,		 -1,		-1,	  	 	-1,			-1,		-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,		 -1,		-1,	  	 	-1,			-1,				-1,			-1,			-1,			-1,	  	 -1,		-1,			-1,			-1,				-1,		 -1,	
		
		
	};
    // clang-format on
    Machine::begin( state_table, ELSE );
	vol_control = volCon;
	AudioMemory(160);
	sgtl5000_1.enable();
	sgtl5000_1.volume(0.5);	    
	mixer1.gain(0, 0.5); //sets mixers for 2 wav playback, no effects
    mixer1.gain(1, 0.5);
    mixer2.gain(0, 0.5);
    mixer2.gain(1, 0.5);
	mixer6.gain(0, 0.5);
	mixer6.gain(0, 0.5);
	mixer9.gain(0, 0.5);
    return *this;	
}

int Atm_master_vol::event( int id ) {
  switch ( id ) {
    case EVT_ENC_UP:
		param_timer.set(3000);	
		return 0;  
    case EVT_ENC_DOWN:
		param_timer.set(3000);	
		return 0;  
    case EVT_BTN_1:
		param_timer.set(3000);	
		return 0;
	case EVT_TIMER_1:
		display_timer.expired(this);	
		return 0; 
	case EVT_TIMER_2:
		param_timer.expired(this);	
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
		case ENT_HOME:
			trigger( displayMain.EVT_HOME );
		 	return;
			
  		case ENT_DISPLAY:
			if ( vol_control == 0){
				trigger( displayMain.EVT_MASTER_VOL );
			}
			else if ( vol_control == 1){
				trigger( displayMain.EVT_VOL_WAV_1 );
			}
			else if ( vol_control == 2){
				trigger( displayMain.EVT_VOL_WAV_2 );
			}
			display_timer.set(100);
  		  	return;
			
		case EXIT_DISPLAY:
			if (evt_counter == 0){
				trigger( EVT_VOL_0 );    
			}
			else if (evt_counter == 1){
				trigger( EVT_VOL_1 );    
			}
			else if (evt_counter == 2){
				trigger( EVT_VOL_2 );    
			}	
			else if (evt_counter == 3){
				trigger( EVT_VOL_3 );    
			}	
			else if (evt_counter == 4){
				trigger( EVT_VOL_4 );    
			}	
			else if (evt_counter == 5){
				trigger( EVT_VOL_5 );    
			}	
			else if (evt_counter == 6){
				trigger( EVT_VOL_6 );    
			}	
			else if (evt_counter == 7){
				trigger( EVT_VOL_7 );    
			}	
			else if (evt_counter == 8){
				trigger( EVT_VOL_8 );    
			}	
			else if (evt_counter == 9){
				trigger( EVT_VOL_9 );    
			}	
			else if (evt_counter == 10){
				trigger( EVT_VOL_10 );    
			}	
			else if (evt_counter == 11){
				trigger( EVT_VOL_11 );    
			}	
			else if (evt_counter == 12){
				trigger( EVT_VOL_12 );    
			}	
			else if (evt_counter == 13){
				trigger( EVT_VOL_13 );    
			}
			else if (evt_counter == 14){
				trigger( EVT_VOL_14 );    
			}	
			else if (evt_counter == 15){
				trigger( EVT_VOL_15 );    
			}	
			else if (evt_counter == 16){
				trigger( EVT_VOL_16 );    
			}	
			else if (evt_counter == 17){
				trigger( EVT_VOL_17 );    
			}	
			else if (evt_counter == 18){
				trigger( EVT_VOL_8 );    
			}	
			else if (evt_counter == 19){
				trigger( EVT_VOL_9 );    
			}	
			else if (evt_counter == 20){
				trigger( EVT_VOL_10 );    
			}	
			else if (evt_counter == 21){
				trigger( EVT_VOL_11 );    
			}	
			else if (evt_counter == 22){
				trigger( EVT_VOL_12 );    
			}	
			else if (evt_counter == 23){
				trigger( EVT_VOL_13 );    
			}
			else if (evt_counter == 24){
				trigger( EVT_VOL_24 );    
			}	
			else if (evt_counter == 25){
				trigger( EVT_VOL_25 );    
			}	
			else if (evt_counter == 26){
				trigger( EVT_VOL_26 );    
			}	
			else if (evt_counter == 27){
				trigger( EVT_VOL_27 );    
			}	
			else if (evt_counter == 28){
				trigger( EVT_VOL_28 );    
			}	
			else if (evt_counter == 29){
				trigger( EVT_VOL_29 );    
			}	
			else if (evt_counter == 30){
				trigger( EVT_VOL_30 );    
			}	
			else if (evt_counter == 31){
				trigger( EVT_VOL_31 );    
			}	
			else if (evt_counter == 32){
				trigger( EVT_VOL_32 );    
			}	
			else if (evt_counter == 33){
				trigger( EVT_VOL_33 );    
			}							
			return;
			
		case ENT_VOL_0:
			setVolume(0.0);			//set volume
			if ( vol_control == 0){  //set level variables for display of different controls
				mas_vol_level = 0;
			}
			else if ( vol_control == 1){
				vol_wav_1_level = 0;
			}
			else if ( vol_control == 2){
				vol_wav_2_level = 0;
			}
			evt_counter = 0;
			return;
		case ENT_VOL_1:
			setVolume(0.03);
			return;
			evt_counter = 1;	
		case ENT_VOL_2:
			setVolume(0.06);
			evt_counter = 2;
			return;
		case ENT_VOL_3:
			setVolume(0.09);
			evt_counter = 3;
			return;	
		case ENT_VOL_4:
			setVolume(0.12);
			if ( vol_control == 0){
				mas_vol_level = 1;
			}
			else if ( vol_control == 1){
				vol_wav_1_level = 1;
			}
			else if ( vol_control == 2){
				vol_wav_2_level = 1;
			}
			evt_counter = 4;
			return;			
		case ENT_VOL_5:
			setVolume(0.15);
			evt_counter = 5;
			return;
		case ENT_VOL_6:
			setVolume(0.18);
			evt_counter = 6;
			return;	
		case ENT_VOL_7:
			setVolume(0.21);
			if ( vol_control == 0){
				mas_vol_level = 2;
			}
			else if ( vol_control == 1){
				vol_wav_1_level = 2;
			}
			else if ( vol_control == 2){
				vol_wav_2_level = 2;
			}
			evt_counter = 7;
			return;
		case ENT_VOL_8:
			setVolume(0.24);
			evt_counter = 8;
			return;	
		case ENT_VOL_9:
			setVolume(0.27);
			evt_counter = 9;
			return;	
		case ENT_VOL_10:
			setVolume(0.30);
			if ( vol_control == 0){
				mas_vol_level = 3;
			}
			else if ( vol_control == 1){
				vol_wav_1_level = 3;
			}
			else if ( vol_control == 2){
				vol_wav_2_level = 3;
			}
			evt_counter = 10;
			return;
		case ENT_VOL_11:
			setVolume(0.33);
			evt_counter = 11;
			return;	
		case ENT_VOL_12:
			setVolume(0.36);
			evt_counter = 12;
			return;
		case ENT_VOL_13:
			setVolume(0.39);
			if ( vol_control == 0){
				mas_vol_level = 4;
			}
			else if ( vol_control == 1){
				vol_wav_1_level = 4;
			}
			else if ( vol_control == 2){
				vol_wav_2_level = 4;
			}
			evt_counter = 13;
			return;	
		case ENT_VOL_14:
			setVolume(0.42);
			evt_counter = 14;
			return;			
		case ENT_VOL_15:
			setVolume(0.45);
			evt_counter = 15;
			return;
		case ENT_VOL_16:
			setVolume(0.48);
			evt_counter = 16;
			return;	
		case ENT_VOL_17:
			setVolume(0.51);
			if ( vol_control == 0){
				mas_vol_level = 5;
			}
			else if ( vol_control == 1){
				vol_wav_1_level = 5;
			}
			else if ( vol_control == 2){
				vol_wav_2_level = 5;
			}
			evt_counter = 17;
			return;
		case ENT_VOL_18:
			setVolume(0.54);
			evt_counter = 18;
			return;	
		case ENT_VOL_19:
			setVolume(0.57);
			evt_counter = 19;
			return;
		case ENT_VOL_20:
			setVolume(0.60);
			if ( vol_control == 0){
				mas_vol_level = 6;
			}
			else if ( vol_control == 1){
				vol_wav_1_level = 6;
			}
			else if ( vol_control == 2){
				vol_wav_2_level = 6;
			}
			return;
			evt_counter = 20;
		case ENT_VOL_21:
			setVolume(0.63);
			evt_counter = 21;
			return;	
		case ENT_VOL_22:
			setVolume(0.66);
			evt_counter = 22;
			return;			
		case ENT_VOL_23:
			setVolume(0.69);
			evt_counter = 23;
			return;	
		case ENT_VOL_24:
			setVolume(0.72);
			if ( vol_control == 0){
				mas_vol_level = 7;
			}
			else if ( vol_control == 1){
				vol_wav_1_level = 7;
			}
			else if ( vol_control == 2){
				vol_wav_2_level = 7;
			}
			evt_counter = 24;
			return;			
		case ENT_VOL_25:
			setVolume(0.75);
			evt_counter = 25;
			return;
		case ENT_VOL_26:
			setVolume(0.78);
			evt_counter = 26;
			return;	
		case ENT_VOL_27:
			setVolume(0.81);
			if ( vol_control == 0){
				mas_vol_level = 8;
			}
			else if ( vol_control == 1){
				vol_wav_1_level = 8;
			}
			else if ( vol_control == 2){
				vol_wav_2_level = 8;
			}
			evt_counter = 27;
			return;
		case ENT_VOL_28:
			setVolume(0.84);
			evt_counter = 28;
			return;	
		case ENT_VOL_29:
			setVolume(0.87);
			evt_counter = 29;
			return;	
		case ENT_VOL_30:
			setVolume(0.90);//
			if ( vol_control == 2){
				vol_wav_2_level = 9;
			}
			evt_counter = 30;
			return;
		case ENT_VOL_31:
			setVolume(0.93);
			if ( vol_control == 0){
				mas_vol_level = 9;
			}
			else if ( vol_control == 1){
				vol_wav_1_level = 9;
			}
			evt_counter = 31;
			return;
		case ENT_VOL_32:
			setVolume(0.96);
			evt_counter = 32;
			return;
		case ENT_VOL_33:
			setVolume(0.99);
			evt_counter = 33;
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


Atm_master_vol& Atm_master_vol::setVolume( float volume ) {
	if ( vol_control == 0 ){  //master
		if(volume > 0.8){
			volume = 0.8; //louder will clip
		}
	sgtl5000_1.volume(volume);
	Serial.println("master vol");
	Serial.println(volume);
	}
	else if ( vol_control == 1 ){  //wav1
	mixer6.gain(0, volume);
	Serial.println("bira 1 volume");
	Serial.println(volume);
	}
	else if ( vol_control == 2 ){  //wav2
	mixer6.gain(1, volume);
	Serial.println("bira 2 volume");
	Serial.println(volume);
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
    "MASTER_VOL\0EVT_ENC_DOWN\0EVT_ENC_UP\0EVT_BTN_1\0ELSE\0VOL_0\0VOL_1\0VOL_5\0VOL_7\0VOL_10" );
  return *this;
}
