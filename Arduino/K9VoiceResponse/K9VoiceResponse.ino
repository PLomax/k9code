#include <Wire.h>
#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"

/**        
  Connection
  Arduino    VoiceRecognitionModule
   2   ------->     TX
   3   ------->     RX
*/
VR myVR(2,3);    // 2:RX 3:TX, you can choose your favourite pins.

uint8_t records[7]; // save record
uint8_t buf[64];

//Group 1 commands
#define cmd1  (0) //good dog
#define cmd2  (1) //k9
#define cmd3  (2) //remember sarah jane
#define cmd4  (3)  //where is the doctor
#define cmd5  (4)  //Are you smart

#define Wirecmd1  49 //good dog
#define Wirecmd2  50 //k9
#define Wirecmd3  51 //remember sarah jane
#define Wirecmd4  52  //where is the doctor
#define Wirecmd5  53  //Are you smart


//Group 2 commands
//#define cmd18 (17)  //scan the room
//#define cmd21 (20)  //torch on
//#define cmd24 (23)  //torch off
//#define cmd27 (26)  //laser on
//#define cmd30 (29)  //laser off

//Group 3 commands
//#define cmd33 (32)  //calculate


const int chipSelect = 10; //10 for arduino uno
char inputChar;
int digLaserWireSlaveAddress = 5;
int digLaserWireMasterAddress = 9;

void setup()
{
  

  /** initialize */
  myVR.begin(9600);

  Serial.begin(115200);
  if(myVR.clear() == 0)
  {Serial.println("Recognizer cleared.");
  }else{
    Serial.println("Could Not find Voice Recognition Module.");
    Serial.println("Please check connection and restart Arduino.");
    while(1);
  }
  
  loadVoiceCommands();
  Wire.begin(digLaserWireMasterAddress);// join i2c bus with address
}

void loop()
{
  int ret;
  ret = myVR.recognize(buf, 50);
  if(ret>0){
    //Serial.println("Rand:" +  String(randResponse));
    switch(buf[1])
    {
      case cmd1: // good dog
           Serial.print("good dog\nTransmitting:");
           Serial.println(Wirecmd1);
           Wire.beginTransmission(digLaserWireSlaveAddress);
           Wire.write(Wirecmd1);              
           Wire.endTransmission();    // stop transmitting
          Serial.println("done");
        break;
      case cmd2: // k9 (Paul)
           Serial.print("k9\nTransmitting:");
           Serial.println(Wirecmd2);
           Wire.beginTransmission(digLaserWireSlaveAddress); 
           Wire.write(Wirecmd2);              
           Wire.endTransmission();    // stop transmitting
           Serial.println("done");
        break;


      case cmd3: //(Sara Jane)
           Serial.print("k9\nTransmitting:");
           Serial.println(Wirecmd2);
           Wire.beginTransmission(digLaserWireSlaveAddress); 
           Wire.write(Wirecmd2);              
           Wire.endTransmission();    // stop transmitting
           Serial.println("done");
        break;


      case cmd4: //k9 (where is the doctor)            
          Serial.print("where is the doctor\nTransmitting:");
          Serial.println(Wirecmd4);
           Wire.beginTransmission(digLaserWireSlaveAddress); 
           Wire.write(Wirecmd4);              
           Wire.endTransmission();    // stop transmitting
          Serial.println("done");
        break;


      case cmd5: //k9 (Are you smart)
          Serial.print("Are you smart\nTransmitting:");
          Serial.println(Wirecmd5);
           Wire.beginTransmission(digLaserWireSlaveAddress); 
           Wire.write(Wirecmd5);              
           Wire.endTransmission();    // stop transmitting
          Serial.println("done");
        break;
                

           
      default:
        Serial.println("Record function undefined");
        break;
    }
    /** voice recognized */
    printVR(buf);
  }
}

void loadVoiceCommands()
{
  
  if(myVR.load((uint8_t)cmd1) >= 0){Serial.println("cmd1 loaded"); }
  if(myVR.load((uint8_t)cmd2) >= 0){Serial.println("cmd2 loaded"); }
  if(myVR.load((uint8_t)cmd3) >= 0){Serial.println("cmd3 loaded"); }
  if(myVR.load((uint8_t)cmd4) >= 0){Serial.println("cmd4 loaded"); }
  if(myVR.load((uint8_t)cmd5) >= 0){Serial.println("cmd5 loaded"); }

  
}



/**
  @brief   Print signature, if the character is invisible, 
           print hexible value instead.
  @param   buf     --> command length
           len     --> number of parameters
*/
void printSignature(uint8_t *buf, int len)
{
  int i;
  for(i=0; i<len; i++){
    if(buf[i]>0x19 && buf[i]<0x7F){
      Serial.write(buf[i]);
    }
    else{
      Serial.print("[");
      Serial.print(buf[i], HEX);
      Serial.print("]");
    }
  }
}

/**
  @brief   Print signature, if the character is invisible, 
           print hexible value instead.
  @param   buf  -->  VR module return value when voice is recognized.
             buf[0]  -->  Group mode(FF: None Group, 0x8n: User, 0x0n:System
             buf[1]  -->  number of record which is recognized. 
             buf[2]  -->  Recognizer index(position) value of the recognized record.
             buf[3]  -->  Signature length
             buf[4]~buf[n] --> Signature
*/
void printVR(uint8_t *buf)
{
  Serial.println("VR Index\tGroup\tRecordNum\tSignature");

  Serial.print(buf[2], DEC);
  Serial.print("\t\t");

  if(buf[0] == 0xFF){
    Serial.print("NONE");
  }
  else if(buf[0]&0x80){
    Serial.print("UG ");
    Serial.print(buf[0]&(~0x80), DEC);
  }
  else{
    Serial.print("SG ");
    Serial.print(buf[0], DEC);
  }
  Serial.print("\t");

  Serial.print(buf[1], DEC);
  Serial.print("\t\t");
  if(buf[3]>0){
    printSignature(buf+4, buf[3]);
  }
  else{
    Serial.print("NONE");
  }
  Serial.println("\r\n");
}





