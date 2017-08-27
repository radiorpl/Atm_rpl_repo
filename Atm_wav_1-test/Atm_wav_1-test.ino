#include <Atm_wav_1.h>
#include <Atm_master_vol.h>
#include <Atm_enc_button.h> 
#include <button_counters.h> 
#include <Atm_sev_seg.h>
#include <Atm_delay_effect.h>
#include <Automaton.h>
#include "display_def.h"
#include "button_counters.h"
#include "audio_system.h"


#define SDCARD_CS_PIN    10      // Use these with the Teensy Audio Shield
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

Atm_wav_1 wav1, wav2;                               //declare atm machines
Atm_master_vol volMaster, volWav1, volWav2;
Atm_encoder enc1, enc2, enc3;
Atm_button btn1, btn2, btn3;
Atm_enc_button encBtn1, encBtn2, encButton3;
Atm_sev_seg displayMain;
Atm_timer delayTimer, paramTimer;

void setup() {
   Serial.begin( 9600 );
   SPI.setMOSI(SDCARD_MOSI_PIN);
   SPI.setSCK(SDCARD_SCK_PIN);
    if (!(SD.begin(SDCARD_CS_PIN))) {              //To do: change to start in different mode when sd missing
     while (1) {
		    btn1.onPress( encBtn1, encBtn1.EVT_BTN_1 );
       Serial.println("Unable to access the SD card");   
     }   
   }   
   wav1.begin(1);    //start wav instances
   wav2.begin(2);
   volMaster.begin(0);
   volWav1.begin(1);
   volWav2.begin(2);
   enc1.begin(33, 24);
   enc2.begin(25, 26);
   enc3.begin(27, 28);
   enc3.debounce(5);
   btn1.begin(4);
   btn2.begin(5);
   encBtn1.begin(1);
   encBtn2.begin(2);
   displayMain.begin();
   paramTimer.begin(3000);
   paramTimer.onFinish( displayMain, displayMain.EVT_HOME );	 
   sgtl5000_1.enable();
   sgtl5000_1.volume(0.3);	   
   wav1.trace(Serial);
   wav2.trace(Serial); 
   encBtn2.trace(Serial);  
}


void loop() {
	enc1.onChange( ATM_UP, encBtn1, encBtn1.EVT_ENC_UP );
	enc1.onChange( ATM_DOWN, encBtn1, encBtn1.EVT_ENC_DOWN );
	if ( enc_button_counter_1 == 0 ) {
	  btn1.onPress( wav1, wav1.EVT_BTN_1 );
	}
	else if ( enc_button_counter_1	 == 1 ) {
	  btn1.onPress( wav2, wav2.EVT_BTN_1 );
	}
	enc3.onChange( ATM_UP, volMaster, volMaster.EVT_ENC_UP );
	enc3.onChange( ATM_DOWN, volMaster, volMaster.EVT_ENC_DOWN );
	
	
	automaton.run();
}