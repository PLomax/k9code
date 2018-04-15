  /*
 * K9 Laser Sound
 * 
 * Version 1.1
 * 
 *  * Note: use pin 5 for laser servo in out
 */

// Controlling a servo position using a potentiometer (variable resistor) 
#include <Wire.h> 
#include <Servo.h>
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h> 

//laser settings
Servo laserServo;  // create servo object to control a servo 
 
int servoPosn=85;    
int received=0;
int wireReceivedValue = 0;

const int LASERIN = 98;
const int LASEROUT = 97;
const int TEST = 99;

int digLaserServoPin = 5;
int digLaserWireMasterAddress = 8;
int digLaserWireSlaveAddress = 9;
bool laserOnFlag = false;

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

Adafruit_VS1053_FilePlayer musicPlayer =  Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

char inputChar;

//Group 1 commands
#define cmd1  49 //good dog
#define cmd2  50 //k9
#define cmd3  51 //remember sarah jane
#define cmd4  52  //where is the doctor
#define cmd5  53  //Are you smart
#define cmd6  54  //play 'i am k9'
#define cmd7  55  //laser
#define cmd8  56  //ears
#define cmd9  57  //tail

#define cmd30 30 //Affirmative
#define cmd31 31 //Negative
#define cmd32 32 //Master
#define cmd33 33 //Mistress
#define cmd34 34 // no available data
#define cmd35 35 //ill be good
#define cmd36 36 //Doctor last spoke
#define cmd37 37 //Efficient Machine
#define cmd38 38 // Naturally
#define cmd39 39 // a treat
#define cmd40 40 //bye
#define cmd41 41 //ready Master

#define newLine 10






void setup() 
{ 
  laserServo.attach(digLaserServoPin);  // attaches the servo on pin 9 to the servo object 
  laserServo.write(servoPosn);  
  Serial.begin(9600); //set baud rate
  Wire.begin(digLaserWireSlaveAddress);
  Wire.onReceive(receiveEvent);


  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
  Serial.println(F("VS1053 found"));
  
   if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }

 // printDirectory(SD.open("/"), 0);
  
  //  musicPlayer.setVolume(20,20);
  musicPlayer.setVolume(1,1);
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
  menu();
  
} //end of setup

//wire.h request event method
void receiveEvent() {
  wireReceivedValue = Wire.read();
}

 
void loop() 
{

  received=0;
  Serial.println(".");
  if (Serial.available() > 0)
  {
    received = Serial.read();

    Serial.println(received);
  }

  if(wireReceivedValue != 0)
  {
    received = wireReceivedValue;
    playSound( wireReceivedValue);
    Serial.print("WireReceivedValue:"); 
    Serial.println(wireReceivedValue); 
    wireReceivedValue = 0;
  } 
  
  if (received == TEST)
  {
     Serial.println("test");
     playSound(cmd30);
     Serial.println("laser out");
     servoPosn=0;
     laserServo.attach(digLaserServoPin); 
     laserServo.write(servoPosn); 
     delay(1000);
     Serial.println("laser in");
     servoPosn=85;
     laserServo.attach(digLaserServoPin); 
     laserServo.write(servoPosn); 
     Serial.println("test complete");
  }


  if (received == LASERIN || wireReceivedValue== LASERIN)
  {
     servoPosn=0;
     laserServo.attach(digLaserServoPin); 
     laserServo.write(servoPosn); 
  //   laserServo.detach();
      playSound(cmd7);
    Serial.println("gun out");
    delay(1000); 
  }

  if (received == LASEROUT || wireReceivedValue == LASEROUT)
  {
     servoPosn=85;
     laserServo.attach(digLaserServoPin); 
     laserServo.write(servoPosn); 
    // laserServo.detach();
    Serial.println("gun in");
    delay(1000); 
  }
  
                          // waits for the servo to get there 
  delay(200);
  laserServo.detach();
 
}

void menu()
{
   Serial.println("K9 Laser Sound");

   Serial.println("Version 1.2");

   Serial.println("Note: use pin 5 for laser servo in out control");

   Serial.println("TEST: enter 'c' to play sound and cycle laser");
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
            musicPlayer.startPlayingFile("BEGOOD.MP3");  
            break;
          case 1:
            musicPlayer.startPlayingFile("AFFIRM~1.mp3");  
            break;
          case 2:
            musicPlayer.startPlayingFile("AFFIRM~2.mp3");  
            break;
          }
          Serial.println("done");
        break;
      case cmd2: // k9 (Paul)
      case cmd30:
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
      case cmd7:
            musicPlayer.startPlayingFile("laser.MP3");  
      break;
       case cmd8://ears
        switch(randResponse)
        {
          case 0:
            musicPlayer.startPlayingFile("master3.MP3");  
            break;
          case 1:
            musicPlayer.startPlayingFile("iamK9.mp3");  
            break;
          case 2:
            musicPlayer.startPlayingFile("nodata.mp3");  
            break;
          }  
      break;     

       case cmd9://tail
        switch(randResponse)
        {
          case 0:
            musicPlayer.startPlayingFile("BEGOOD.MP3");  
            break;
          case 1:
            musicPlayer.startPlayingFile("IAMK9M3.mp3");  
            break;
          case 2:
            musicPlayer.startPlayingFile("effMach.mp3");  
            break;
          } 

      case cmd31: //negative
        musicPlayer.startPlayingFile("negative.mp3");
        break;  

      case cmd32: //Master
        musicPlayer.startPlayingFile("master3.mp3");
        break;  

      case cmd33: //Mistress
        musicPlayer.startPlayingFile("mistress.mp3");
        break;  

      case cmd34: //no available data
        musicPlayer.startPlayingFile("nodata.mp3");
        break;  

      case cmd35: //ill be good
        musicPlayer.startPlayingFile("BEGOOD.mp3");
        break;  

      case cmd36: //Doctor last spoke
        musicPlayer.startPlayingFile("1978.mp3");
        break;  

       case cmd37: //Efficient Machine
        musicPlayer.startPlayingFile("effMach.mp3");
        break;  

      case cmd38: //iamk9M3
        musicPlayer.startPlayingFile("iamk9M3.mp3");
        break;  

      case cmd39: //naturly
        musicPlayer.startPlayingFile("naturly.mp3");
        break;  


      case cmd40: //bye
        musicPlayer.startPlayingFile("bye.mp3");
        break;  

        
      case cmd41: //read master
        musicPlayer.startPlayingFile("readyM.mp3");
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






