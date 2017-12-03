#include <Wire.h> 
/*
 * K9 Main
 * 
 * Version 1.1
 * 
 * 
 */

#define VERSION "1.1"

// connect motor controller pins to Arduino digital pins
// motor one
int enA = 10;
int in1 = 9;
int in2 = 8;

// motor two
int enB = 5;
int in3 = 7;
int in4 = 6;
int motorSpeedFull = 255;
int motorSpeedHalf = 160;


// RELAY Settings
int digLaserLED = 22;
int digEars     = 24;
int digTail     = 26;
int digEyes     = 28;


//laser settings
int digLaserServoPin = 30;
int digLaserWireMasterAddress = 8;
int digLaserWireSlaveAddress = 9;
int digCtrlPanelWireSlaveAddress = 10;


const int LASERIN = 97;
const int LASEROUT = 98;

bool laserOnFlag = false;

//Sound by Wire settings

#define Wirecmd1  49 //good dog
#define Wirecmd2  50 //k9
#define Wirecmd3  51 //remember sarah jane
#define Wirecmd4  52  //where is the doctor
#define Wirecmd5  53  //Are you smart
#define Wirecmd6  54  //play I am k9


//general settings
int received=0;
int delayTime =200;
int loopDelay = 0;


void setup()
{
  // set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

// relay inputs
  pinMode(digLaserLED, 	OUTPUT);
  pinMode(digEars, 		OUTPUT);    
  pinMode(digTail, 		OUTPUT);    
  pinMode(digEyes , 	OUTPUT);   
  
  digitalWrite(digLaserLED, LOW); //set laser LED off
  digitalWrite(digEyes, LOW); //turn on eyes

  //IC2 Wire settings
  Wire.begin(digLaserWireMasterAddress);// join i2c bus with address
  Wire.onRequest(requestEvent); // register event
 // Wire.onReceive(receiveEvent);

  
  //general controls
  Serial.begin(9600); //set baud rate
  Serial.println("commenced");

  menu();
  //transmitWireSound(Wirecmd6); // play I am k9 via wire
}

void forward()
{
  // turn on motor A
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  // set speed to 200 out of possible range 0~255
  analogWrite(enA, motorSpeedFull);
  // turn on motor B
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  // set speed to 200 out of possible range 0~255
  analogWrite(enB, motorSpeedFull);
  delay(delayTime);
  }


void rightForward()
{
  // turn on motor A
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  // set speed to 200 out of possible range 0~255
  analogWrite(enA, motorSpeedFull);
//  // turn on motor B
//  digitalWrite(in3, HIGH);
//  digitalWrite(in4, LOW);
//  // set speed to 200 out of possible range 0~255
//  analogWrite(enB, motorSpeed);
  delay(delayTime);
  }

void leftForward()
{
//  // turn on motor A
//  digitalWrite(in1, HIGH);
//  digitalWrite(in2, LOW);
//  // set speed to 200 out of possible range 0~255
//  analogWrite(enA, motorSpeed);
  // turn on motor B
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  // set speed to 200 out of possible range 0~255
  analogWrite(enB, motorSpeedFull);
  delay(delayTime);
  }


void reverse()
{
  // now change motor directions
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);  
    // set speed to 200 out of possible range 0~255
  analogWrite(enA, motorSpeedFull);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH); 
    // set speed to 200 out of possible range 0~255
  analogWrite(enB, motorSpeedFull);
   delay(delayTime);
  }


void leftReverse()
{
//  // now change motor directions
//  digitalWrite(in1, LOW);
//  digitalWrite(in2, HIGH);  
//    // set speed to 200 out of possible range 0~255
//  analogWrite(enA, motorSpeedHalf);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH); 
    // set speed to 200 out of possible range 0~255
  analogWrite(enB, motorSpeedFull);
   delay(delayTime);
  }

void rightReverse()
{
  // now change motor directions
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);  
    // set speed to 200 out of possible range 0~255
  analogWrite(enA, motorSpeedFull);
//  digitalWrite(in3, LOW);
//  digitalWrite(in4, HIGH); 
//    // set speed to 200 out of possible range 0~255
//  analogWrite(enB, motorSpeed);
  delay(delayTime);
  }
  

void stopped()
{

  // now turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);  
 // delay(500);
}


//wire.h request event method
void requestEvent() {
  Wire.write("received"); // respond with message of 8 bytes
// as expected by master
}

void receiveEvent()
{
  
}


void loop()
{

  delay(loopDelay);
  received=0;
  Serial.println("reading");
  if (Serial.available() > 0)
  {
    received = Serial.read();
    Serial.println("read");
    Serial.println(received);
  }

  switch (received) {
    case 1://forward
      Serial.println("1:forward");
      stopped();
      forward();
     // delay(2000);
     // stopped();
      break;

    case 2://forward left
      Serial.println("2:left forward");
      stopped();
      leftForward();
      //delay(2000);
      //stopped();
      break;

    case 3://forward right
      Serial.println("3:right forward");
      stopped();
      rightForward();
      //delay(2000);
      //stopped();
      break;

    case 4://reverse
      Serial.println("4:reverse");
      stopped();
      reverse();
      //delay(2000);
      //stopped();
      break;

    case 5://reverse left
      Serial.println("5:reverse left");
      stopped();
      leftReverse();
      //delay(2000);
      //stopped();
      break;

    case 6://reverse right
      Serial.println("6:reverse right");
      stopped();
      rightReverse();
      //delay(2000);
      //stopped();
      break;
    
    case 8: //tail
      Serial.println("8:tail");
      moveTail();
    break;

    case 9://stop
      Serial.println("9:stop");
      stopped();
      //delay(500);
      break;    
      
    case 10: //ears
    Serial.println("10:ears");
    moveEars();
    break;

      
    case 11: //display
      Serial.println("11:display");
    break;
          
    case 12: //laser Out
     Serial.println("12:Laser Out");
     laserOut();
     break;

          
    case 13: //laser In
     Serial.println("13:Laser In");
     laserIn();
     break;
          
    case 14:  //Laser On Off: Note: this is actually switching relay
      Serial.println("14:Laser On Off");
      laserOnOff();
      break;

    case 30://Affirmative
      transmitWireSound(30);
      break;

    case 31://Negative
      transmitWireSound(31);
      break;

   case 32://Master
      transmitWireSound(32);
      break;
  
  case 33://Mistress
      transmitWireSound(33);
      break;

    case 34:// no available data
      transmitWireSound(34);
      break;

   case 35: //ill be good
      transmitWireSound(35);
      break;  


    case 36://Doctor last spoke
      transmitWireSound(36);
      break;

   case 37://Efficient Machine
      transmitWireSound(37);
      break;  


    case 38:// iamk9M3
      transmitWireSound(38);
      break;

    case 39: // naturally
      transmitWireSound(39);
      break;


              
  }

stopped();
}

void moveEars()
{

   digitalWrite(digEars, HIGH);
   //transmitWireSound(56);
   transmitWireCtrlPanel(98);
   delay(2000);
   digitalWrite(digEars, LOW); 

}

void moveTail()
{
    Serial.println("8:tail");
    digitalWrite(digTail, HIGH);
    //transmitWireSound(57);
    transmitWireCtrlPanel(99);
    delay(2000);
    digitalWrite(digTail, LOW);
     Serial.println("8:tail-done");

}

void laserIn()
{
     digitalWrite(digLaserLED, LOW); // turn off LED
     Wire.beginTransmission(digLaserWireSlaveAddress); // transmit to device #9
     Wire.write(LASERIN);              // sends y
     Wire.endTransmission();    // stop transmitting
     Serial.println("gun in");  
     delay(500);  
}

void laserOut()
{
     digitalWrite(digLaserLED, HIGH); // Turn on LED
     transmitWireCtrlPanel(97);
     Wire.beginTransmission(digLaserWireSlaveAddress); // transmit to device #9
     Wire.write(LASEROUT);              // sends x 
     Wire.endTransmission();    // stop transmitting
     Serial.println("gun out");
     delay(2000);
     digitalWrite(digLaserLED, LOW); // turn off LED
}


void laserOnOff()
{
      if(laserOnFlag == true)
      {
         //turn off laser led
         laserOnFlag= false;
         digitalWrite(digLaserLED, LOW);
      }
      else
      {
        //turn on laser led
        laserOnFlag = true;
        digitalWrite(digLaserLED, HIGH);  
      }
}



void eyesOn()
{
  
}

void eyesOff()
{
  
}



void transmitWireSound(int cmdId)
{
   Serial.print("begin trans..");
   Wire.beginTransmission(digLaserWireSlaveAddress);
   Serial.print("wire writing..");
   Wire.write(cmdId); 
   Serial.print("ending transmission..");             
   Wire.endTransmission();    // stop transmitting 
   Serial.println("sent..");
  
}

void transmitWireCtrlPanel(int cmdId)
{
   Serial.print("sending..");
   Serial.print(cmdId); 
    Serial.print(" on slave..");

   Serial.print( digCtrlPanelWireSlaveAddress);
   Wire.beginTransmission(digCtrlPanelWireSlaveAddress);
   Wire.write(cmdId);    
   Serial.println("sent..");          
   Wire.endTransmission();    // stop transmitting 
   Serial.println("end Transmission..");    
  
}

void menu()
{
  Serial.println("************"); 
  Serial.println("K9 Main"); 
  Serial.print("Version:");
  Serial.println(VERSION);
  Serial.println("************");   
}


