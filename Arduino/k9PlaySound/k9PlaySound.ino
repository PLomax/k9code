/*************************************************** 
  This is an example for the Adafruit VS1053 Codec Breakout

  Designed specifically to work with the Adafruit VS1053 Codec Breakout 
  ----> https://www.adafruit.com/products/1381

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>
#include <Wire.h>

// define the pins used
//#define CLK 13       // SPI Clock, shared with SD card
//#define MISO 12      // Input data, from VS1053/SD card
//#define MOSI 11      // Output data, to VS1053/SD card
// Connect CLK, MISO and MOSI to hardware SPI pins. 
// See http://arduino.cc/en/Reference/SPI "Connections"

// These are the pins used for the breakout example
#define BREAKOUT_RESET  9      // VS1053 reset pin (output)
#define BREAKOUT_CS     10     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    8      // VS1053 Data/command select pin (output)
// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer = 
  // create breakout-example object!
 // Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);
  // create shield-example object!
  Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

char inputChar;

//Group 1 commands
#define cmd1  49 //good dog
#define cmd2  50 //k9
#define cmd3  51 //remember sarah jane
#define cmd4  52  //where is the doctor
#define cmd5  53  //Are you smart
#define cmd6  54  //play 'i am k9'

#define newLine 10


int digLaserWireSlaveAddress = 5;
int wireReceivedValue =0;

  
void setup() {
  Serial.begin(9600);
  menu();
  pinMode(LED_BUILTIN, OUTPUT);

  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
  Serial.println(F("VS1053 found"));
  
   if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }

  // list files
  printDirectory(SD.open("/"), 0);
    
    // Set volume for left, right channels. lower numbers == louder volume!
  //  musicPlayer.setVolume(20,20);
    musicPlayer.setVolume(1,1);
  
    // Timer interrupts are not suggested, better to use DREQ interrupt!
    //musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int
  
    // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
    // audio playing
    musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
 
  Wire.begin(digLaserWireSlaveAddress);
  Wire.onReceive(receiveEvent);
}

//wire.h request event method
void receiveEvent() {
  wireReceivedValue = Wire.read();
}

void loop(){  
  if(Serial.available())
  {   
    inputChar = Serial.read();
    Serial.println(int(inputChar));
  }
    
  if(wireReceivedValue != 0)
  {
    playSound( wireReceivedValue);  
    wireReceivedValue = 0;
  } 

   if (inputChar != 0)
   {
      
      playSound( int(inputChar));
      inputChar = 0;
   }



}

void menu()
{
   Serial.println("K9 Sound");
    Serial.println("valid input:");
    Serial.println(cmd1);
    Serial.println(cmd2);
    Serial.println(cmd3);
    Serial.println(cmd4);
    Serial.println(cmd5); 
    Serial.println(cmd6); 
}

void playSound(int inputValue)
{
   int randResponse = random(0, 3) ;
   switch(inputValue)
    {
      case cmd1: // good dog
        Serial.println("good dog");
        delay(300);
        switch(randResponse)
        {
          case 0:
            Serial.println("playing begood");
            musicPlayer.startPlayingFile("BEGOOD.MP3");  
            break;
          case 1:
            Serial.println("playing AFFIRM~1.mp3");
            musicPlayer.startPlayingFile("AFFIRM~1.mp3");  
            break;
          case 2:
            Serial.println("playing AFFIRM2~.mp3");
            musicPlayer.startPlayingFile("AFFIRM~2.mp3");  
            break;
          }
          Serial.println("done");
        break;
      case cmd2: // k9 (Paul)
        Serial.println("k9 (Paul)");

        switch(randResponse)
        {
          case 0:
            musicPlayer.startPlayingFile("AFFIRM~1.mp3");  
            break;
          case 1:
            musicPlayer.startPlayingFile("AFFIRM~2.mp3");  
            break;
          case 2:
            musicPlayer.startPlayingFile("AFFIRM~3.mp3");  
            break;
          }
        Serial.println("done");
        break;


      case cmd3: //(Sara Jane)
        Serial.println("Sara Jane");
        delay(300);
        musicPlayer.startPlayingFile("sjane.mp3"); 
        Serial.println("done");
        break;


      case cmd4: //k9 (where is the doctor)            
        Serial.println("where is the doctor");
        delay(300);
        switch(randResponse)
        {
          case 0:
            musicPlayer.startPlayingFile("nodata.mp3");  
            break;
          case 1:
            musicPlayer.startPlayingFile("1978.mp3");  
            break;
          case 2:
            musicPlayer.startPlayingFile("1978.mp3");  
            break;
          }
        Serial.println("done");
        break;


      case cmd5: //k9 (Are you smart)
        Serial.println("Are you smart");
        delay(300);
        switch(randResponse)
        {
          case 0:
            musicPlayer.startPlayingFile("iamK9.mp3");  
            break;
          case 1:
            musicPlayer.startPlayingFile("effMach.mp3");  
            break;
          case 2:
            musicPlayer.startPlayingFile("naturly.mp3");  
            break;
          }
        Serial.println("done");
        break;
                
      case cmd6:
            musicPlayer.startPlayingFile("IAMK9M3.MP3");  
      break;
      
      case newLine:
        Serial.println("and ... a new line!");
        break;
           
      default:
        Serial.print(inputValue);
        Serial.println(" Record function undefined");
        break;       
    }  
}



/// File listing helper
void printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}

