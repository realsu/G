/*
todo:
Rückgabewerte komisch?
Spielt er wirklich die richtigen Folder/tracks ab?

finished playing:
1st track is finished playing in SD card 7E FF 06 3D 00 00 01 xx xx EF
2nd track is finished playing in SD card 7E FF 06 3D 00 00 02 xx xx EF

Returned data of feedback from module
Module returns ACK 7E FF 06 41 00 00 00 xx xx EF
In order to enhance stability between data communication, the function of a feedback from module is added.
Once there is a feedback to MCU from the module, it means the module has successfully received the command
that MCU sent out. 0x41 is the returned command by module.
*/

#include <SerialCommand.h>

#define arduinoLED 33   // Arduino LED on board

SerialCommand sCmd;     // The demo SerialCommand object
#include <DFPlayer_Mini_Mp3.h>
int time = millis();
byte track = 1;
byte folder = 1;
byte vol = 15;
//
void setup () {
  pinMode(arduinoLED, OUTPUT);      // Configure the onboard LED for output
  digitalWrite(arduinoLED, LOW);    // default to LED off
  Serial1.begin (9600);
  mp3_set_serial (Serial1);	//set Serial for DFPlayer-mini mp3 module
  mp3_reset();
  mp3_set_volume (vol);

  // Setup callbacks for SerialCommand commands
  sCmd.addCommand("p", m_play);
  sCmd.addCommand("pp", m_pause);
  sCmd.addCommand("s", m_stop);
  sCmd.addCommand("v", m_vol);
  sCmd.addCommand("c",    m_choose);  // choose folder / track
  sCmd.addCommand("gs",    m_getstate);  // choose folder / track
  sCmd.addCommand("?",    consoleprint);  // choose folder / track
  sCmd.setDefaultHandler(unrecognized);      // Handler for command that isn't matched  (says "What?")
  Serial.begin(9600);
  delay(2000);
  consoleprint();
}


//
void loop () {
  sCmd.readSerial();     // We don't do much, just process serial commands
  getserial();

}

void getserial() {
  if (Serial1.available() > 0) {

    byte incomingByte = Serial1.read();
    Serial.print(incomingByte, HEX);
    if (incomingByte != 0x0EF)
      Serial.print(" ");
    else
      Serial.println(" end of Message");
  }
}

void consoleprint() {
  Serial.println();
  Serial.println("**** MP3 console ******");
  Serial.println("*  p = play           *");
  Serial.println("*  s = stop           *");
  Serial.println("* pp = pause          *");
  Serial.println("*  v = vol (0 to 30   *");
  Serial.println("*  c = folder / track *");
  Serial.println("* gs = get state      *");
  Serial.println("*  ? = print commands *");
  Serial.println("***********************");
}
void m_getstate() {
  Serial.println("Status: ");
  mp3_get_state (); 
}
void m_play() {
  mp3_play (track, folder);
  Serial.println();
  Serial.print("Playing: Track: ");
  Serial.print(track);
  Serial.print(" Folder: ");
  Serial.println(track);
}

void m_pause() {
  mp3_pause ();
  Serial.println();
  Serial.println("Pausing...");
}

void m_stop() {
  mp3_stop();
  Serial.println();
  Serial.println("Playback stopped");
}

void m_choose() {
  char *arg;
  arg = sCmd.next();
  if (arg != NULL) {
    folder = atoi(arg);    // Converts a char string to an integer
    Serial.println();
    Serial.print("Choosed and now playing: ");
    Serial.print("Folder: ");
    Serial.print(folder);
  }
  arg = sCmd.next();
  if (arg != NULL) {
    track = atoi(arg);    // Converts a char string to an integer
    Serial.print(" Track: ");
    Serial.println(track);
  }
  mp3_play (track, folder);
}

void m_vol() {
  char *arg;
  arg = sCmd.next();
  if (arg != NULL) {
    vol = atoi(arg);    // Converts a char string to an integer
    Serial.println();
    Serial.print("Volume set to: ");
    Serial.println(vol);
  }
  mp3_set_volume(vol);
}

void unrecognized(const char *command) {
  Serial.println("What?");
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