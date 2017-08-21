
/*calls custom automation library to turn wav player on and off
7/16/17
modified to use encoders to move tracks up and down
works but way too sensitive, needs debouncing or something
7/20/17
added good working debounce to atm_encoder library
7/28/17
Works playing 2 instances of Atm_wav_1
8/2/17
Button toggles which instance a single encoder and button control
8/3/17
works with Atm_wav_1_multi_r0 and Atm_master_vol_r0
8/4/17
worker
8/13/17
for Atm_wav_1_multi_effects_r0
8/16/17
fixed master volume by moving sgtl declarations into ino file
*/
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


#define SDCARD_CS_PIN    10 // Use these with the Teensy Audio Shield
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

Atm_wav_1 wav1;    //declare atm machines
Atm_wav_1 wav2;
Atm_master_vol volMaster;   
Atm_master_vol volWav1;
Atm_master_vol volWav2;
Atm_encoder enc1;
Atm_encoder enc2;
Atm_button btn1;
Atm_button btn2;
Atm_button btn3;
Atm_enc_button encBtn1;
Atm_enc_button encBtn2;
Atm_delay_effect delayEffect;
Atm_sev_seg displayMain;
Atm_timer timer;

//const int  buttonPin1 = 3;      //non atm button
//const int  buttonPin2 = 7;      //non atm button

void setup() {
   Serial.begin( 9600 );
   SPI.setMOSI(SDCARD_MOSI_PIN);
   SPI.setSCK(SDCARD_SCK_PIN);
    if (!(SD.begin(SDCARD_CS_PIN))) {
     while (1) {
       Serial.println("Unable to access the SD card");   
     }   
   }   
   wav1.begin(1);    //start wav instances
   wav2.begin(2);
   volMaster.begin(0);
   volWav1.begin(1);
   volWav2.begin(2);
   delayEffect.begin(1);
   enc1.begin(1, 0);
   enc1.onChange( ATM_UP, wav1, wav1.EVT_ENC_UP );
   enc1.onChange( ATM_DOWN, wav1, wav1.EVT_ENC_DOWN );
   enc2.begin(4, 6);
   enc2.onChange( ATM_UP, volMaster, volMaster.EVT_ENC_UP );
   enc2.onChange( ATM_DOWN, volMaster, volMaster.EVT_ENC_DOWN );
   enc2.debounce( 5 );  
   btn1.begin(2);
   btn2.begin(18);
   btn2.onPress( encBtn1, encBtn1.EVT_BTN_1 );
   btn3.begin(16);
   btn3.onPress( encBtn2, encBtn2.EVT_BTN_1 );
   //btn3.onPress2( volMaster, volMaster.EVT_BTN_1);
   encBtn1.begin(1);
   encBtn2.begin(2);
   displayMain.begin();
   timer.begin(3000);
   timer.onFinish( displayMain, displayMain.EVT_HOME );	
   
   sgtl5000_1.enable();
   sgtl5000_1.volume(0.3);	
   /*
   wav1.trace(Serial);
   wav2.trace(Serial);
   
   btn3.trace(Serial);
   
   */
   displayMain.trace(Serial);
   volMaster.trace(Serial);
   //volWav1.trace(Serial);
   //volWav2.trace(Serial);
   //enc2.trace( Serial );
  
   
}

void loop() {
  //control of encoder and button function for track and stop/start
  if (enc_button_counter_1 == 0){ 						//counter 1 is for number of bira instances, attached to btn2
    enc1.onChange( ATM_DOWN, wav1, wav1.EVT_ENC_UP ); //control bira 1
    enc1.onChange( ATM_UP, wav1, wav1.EVT_ENC_DOWN );
    }
   else if (enc_button_counter_1 == 1){
    enc1.onChange( ATM_DOWN, wav2, wav2.EVT_ENC_UP ); //control bira 2
    enc1.onChange( ATM_UP, wav2, wav2.EVT_ENC_DOWN );
    }

  if (enc_button_counter_1 == 0){
    btn1.onPress( wav1, wav1.EVT_BTN_1 ); //control bira 1
    }
  else if (enc_button_counter_1 == 1){
    btn1.onPress( wav2, wav2.EVT_BTN_1 );
    }
//switch which volume the encoder controls
  
  if (enc_button_counter_2 == 0){						//counter 2 is for number of volume instance, attached to btn3
	enc2.onChange( ATM_UP, volMaster, volMaster.EVT_ENC_UP );
    enc2.onChange( ATM_DOWN, volMaster, volMaster.EVT_ENC_DOWN );
	
	if( enc_button_counter_2 != last_enc_button_counter_2 ){
		displayMain.trigger( displayMain.EVT_MASTER_VOL );
		volMaster.trigger( volMaster.EVT_BTN_1 );
	}
	last_enc_button_counter_2 = enc_button_counter_2;
    }	
  else if (enc_button_counter_2 == 1){
    enc2.onChange( ATM_UP, volWav1, volWav1.EVT_ENC_UP );
    enc2.onChange( ATM_DOWN, volWav1, volWav1.EVT_ENC_DOWN );
	
	if( enc_button_counter_2 != last_enc_button_counter_2 ){
		displayMain.trigger( displayMain.EVT_VOL_WAV_1 );
		volWav1.trigger( volWav1.EVT_BTN_1 );
	}
	last_enc_button_counter_2 = enc_button_counter_2;
    }
  else if (enc_button_counter_2 == 2){
    enc2.onChange( ATM_UP, volWav2, volWav2.EVT_ENC_UP );
    enc2.onChange( ATM_DOWN, volWav2, volWav2.EVT_ENC_DOWN );

	if( enc_button_counter_2 != last_enc_button_counter_2 ){
		displayMain.trigger( displayMain.EVT_VOL_WAV_2 );
		volWav2.trigger( volWav2.EVT_BTN_1 );
	}
	last_enc_button_counter_2 = enc_button_counter_2;
    }
	
  automaton.run();
}
