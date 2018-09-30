
/*
 *	Copyright:	DFRobot
 *	name:		DFPlayer_Mini_Mp3 sample code
 *	Author:		lisper <lisper.li@dfrobot.com>
 *	Date:		2014-05-30
 *	Description:	sample code for DFPlayer Mini, this code is test on Uno
 *			note: mp3 file must put into mp3 folder in your tf card
 */

 
#include <DFPlayer_Mini_Mp3.h>

//
void setup () {
	Serial1.begin (9600);
	mp3_set_serial (Serial1);	//set Serial for DFPlayer-mini mp3 module 
	mp3_set_volume (15);
}


//
void loop () {   
 //mp3_play ();	  
	mp3_play (1);
	delay (6000);
	mp3_play (2);
	delay (6000);
mp3_play (3);
	delay (6000);
mp3_play (4);
	delay (6000);
mp3_play (5);
	delay (6000);
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
