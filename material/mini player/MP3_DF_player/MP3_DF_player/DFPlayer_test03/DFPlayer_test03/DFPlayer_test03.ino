/*
todo:
Rückgabewerte komisch?
Spielt er wirklich die richtigen Folder/tracks ab?
werden die Tracks/Ordner/Volume Werte in HEX konvertiert?
finished playing:
1st track is finished playing in SD card 7E FF 06 3D 00 00 01 xx xx EF
2nd track is finished playing in SD card 7E FF 06 3D 00 00 02 xx xx EF

Returned data of feedback from module
Module returns ACK 7E FF 06 41 00 00 00 xx xx EF
In order to enhance stability between data communication, the function of a feedback from module is added.
Once there is a feedback to MCU from the module, it means the module has successfully received the command
that MCU sent out. 0x41 is the returned command by module.
*/

static const char* errortext[] = {"Module busy", "Currently sleep mode", "Serial receiving error",
                                  "Checksum incorrect", "Specified track is out of current track scope", "Specified track is not found", "Inter-cut error", "SD card reading failed", "Entered into sleep mode"
                                 };

#include <SerialCommand.h>
#define arduinoLED 33   // Arduino LED on board

SerialCommand sCmd;     // The demo SerialCommand object
#include <DFPlayer_Mini_Mp3.h>
long time = millis();
byte track = 1;
byte folder = 1;
byte vol = 15;
byte inc_message[16]; // incoming message buffer
byte inc_counter = 0;
boolean inc_flag = 0; // flag for incoming messages
boolean inc_busy = 0;
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
  sCmd.addCommand("v", m_vol); // 1 arg (vol)
  sCmd.addCommand("c",    m_choose);  // 2 arg (folder,track) choose folder / track
  sCmd.addCommand("gs",    m_getstate);  // get state
  sCmd.addCommand("fo",    m_getfolders);  // 1 arg (folder) get totaly files in folder
  sCmd.addCommand("?",    consoleprint);  // print commands
  sCmd.setDefaultHandler(unrecognized);      // Handler for command that isn't matched  (says "What?")
  Serial.begin(9600);
  delay(2000);
  consoleprint();
}


//
void loop () {
  sCmd.readSerial();     //  process serial commands
  getserial();
  delay(10);

}

void getserial() {


  if (Serial1.available() ) { // getting incoming message
    byte inChar = Serial1.read();
    if (inChar == 0x7e)
    {
      inc_flag = 1;
      inc_message[0] = inChar;
      inc_counter = 1;
      delay(100);
      while(Serial1.available())
      
      {
      inc_message[inc_counter]=Serial1.read();
   //   Serial.println(inc_message[inc_counter]);
      inc_counter++;
      //delay(10);
      }
    }
 
    
  }


if (inc_flag == 1)
{

  Serial.println("*****incoming Message: ******");
  for (byte i = 0; i < inc_counter; i++) {
    Serial.print(inc_message[i], HEX);
    Serial.print (" ");
  }
  Serial.println();
  Serial.println("******END incoming***********");
  proc_message();
  inc_flag = 0;

}
}

void proc_message() { // processing incoming message

  if (inc_message[3] == 0x40) { // error occurs
    Serial.print("Error: ");
    Serial.println(errortext[inc_message[6] - 1]);
  }
  if (inc_message[3] == 0x3A && inc_message[6] == 2) { // sd card plugged in
    Serial.print("SD card plugged in!");
  }
  if (inc_message[3] == 0x3B && inc_message[6] == 2) { // sd card plugged out
    Serial.print("SD card plugged out!");
  }

  if (inc_message[3] == 0x3d) { // track finished
    Serial.print("Track finished playing: ");
  }
  if (inc_message[3] == 0x3f && inc_message[6] == 2) { // sd card online (startup)
    Serial.print("SD card online");
  }

  if (inc_message[3] == 0x4e) { // query nr/tracks in folder
    if ( inc_message[6] != 0) { // check if folder not empty (error msg = 7E FF 06 40 00 00 06 FE B5 EF)
 Serial.print(" tracks: ");
      Serial.println(inc_message[6]);
     
    }
    else
      Serial.println("Folder is empty! ");
  }
} // end void proc_message

void consoleprint() {
  Serial.println();
  Serial.println("******* MP3 console ************");
  Serial.println("*  p = play                    *");
  Serial.println("*  s = stop                    *");
  Serial.println("* pp = pause                   *");
  Serial.println("*  v = vol (0 to 30            *");
  Serial.println("*  c = folder / track          *");
  Serial.println("* gs = get state               *");
  Serial.println("* fo = get nr/tracks in folder *");
  Serial.println("*  ? = print commands          *");
  Serial.println("********************************");
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

void m_getfolders() {
  char *arg;
  byte qfolder;
  arg = sCmd.next();
  if (arg != NULL) {
    qfolder = atoi(arg);    // Converts a char string to an integer
    Serial.println();
    Serial.print("Query number/tracks in folder: ");
    Serial.println(qfolder);
  }
  mp3_get_track_in_folders (qfolder);
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
