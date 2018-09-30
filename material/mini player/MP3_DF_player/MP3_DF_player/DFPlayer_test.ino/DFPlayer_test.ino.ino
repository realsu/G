
/*
 *	Copyright:	DFRobot
 *	name:		DFPlayer_Mini_Mp3 sample code
 *	Author:		lisper <lisper.li@dfrobot.com>
 *	Date:		2014-05-30
 *	Description:	sample code for DFPlayer Mini, this code is test on Uno
 *			note: mp3 file must put into mp3 folder in your tf card
 */

 
#include <DFPlayer_Mini_Mp3.h>
int time=millis();
byte track=1;
byte folder =1;
//
void setup () {
	Serial1.begin (9600);
	mp3_set_serial (Serial1);	//set Serial for DFPlayer-mini mp3 module 
	mp3_set_volume (10);
}


//
void loop () {   
 if (millis()-time>10000) {
   time=millis();
   mp3_play (track,folder);
   Serial.print("Current track: ");
   Serial.print (track);
   Serial.print (" folder:");
   Serial.print (track);
   track++;
   folder++;
   if (track >2)
   track=1;
   if (folder >3)
   folder=1;
 }

  if (Serial1.available() > 0) {
             
               byte incomingByte = Serial1.read();
                Serial.print(incomingByte, HEX);
                if (incomingByte != 0x0EF)
                Serial.print(" ");
                else
                Serial.println(" ");
        }

}

/*
   mp3_play ();		//start play
   mp3_play (5);	//play "mp3/0005.mp3"
   mp3_next ();		//play next 
   mp3_prev ();		//play previous
   mp3_set_volume (uint16_t volume);	//0~30
   mp3_set_EQ ();	//0~5
   mp3_pause ();
   mp3_stop ();
   void mp3_get_state (); 	//send get state command
   void mp3_get_volume (); 
   void mp3_get_u_sum (); 
   void mp3_get_tf_sum (); 
   void mp3_get_flash_sum (); 
   void mp3_get_tf_current (); 
   void mp3_get_u_current (); 
   void mp3_get_flash_current (); 
   void mp3_single_loop (boolean state);	//set single loop 
   void mp3_DAC (boolean state); 
   void mp3_random_play (); 
 */
