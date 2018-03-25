#include <Wire.h> 
#include <Servo.h>
#include <String.h>
/*
 * K9 Main
 * 
 * Version 1.3
 * 
 * Single scooter motor version
 * Created 19-2-2018
 */

#define VERSION "1.3"

// connect motor controller pins to Arduino digital pins
// drive motor 
int enA = 10;
int in1 = 9;
int in2 = 8;

int motorSpeed = 255;
//int motorSpeedHalf = motorSpeed/2;

//Servo
Servo steeringServo;
int servoPin = 7;

// RELAY Settings
int digLaserLED = 22;
int digEars     = 24;
int digTail     = 26;
int digEyes     = 28;


//wire settings
int wireMasterAddress = 8;
int wireSoundLaserSlaveAddress = 9;
int wireCtrlPanelSlaveAddress = 10;


const int LASERIN = 97;
const int LASEROUT = 98;

//general settings
long received=0;
String lastReceived = "";
String receivedStr ="";

int delayTime =200;
int loopDelay = 700;


void setup()
{
  // set drive motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

// relay inputs
  pinMode(digLaserLED, 	OUTPUT);
  pinMode(digEars, 		OUTPUT);    
  pinMode(digTail, 		OUTPUT);    
  pinMode(digEyes , 	OUTPUT);   
  
  digitalWrite(digLaserLED, LOW); //set laser LED off
  digitalWrite(digEyes, LOW); //turn on eyes

  //IC2 Wire settings
  Wire.begin(wireMasterAddress);// join i2c bus with master address
  Wire.onRequest(requestEvent); // register event
 // Wire.onReceive(receiveEvent);

  steeringServo.attach(servoPin); 
  
  
  //general controls
  Serial.begin(9600); //set baud rate
  Serial.println("commenced");

  menu(); //show menu
  
}

void forward()
{
    Serial.println("forward");
    Serial.print("Speed:");
    Serial.println(motorSpeed);
    
    if(digitalRead(in1) == LOW && digitalRead(in2) == HIGH)
    {
  	stopDriveMotor(); //brake if running first before changing direction
  	delay(100);
    }
  
    // turn on drive motor
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    // set speed to 200 out of possible range 0~255
    analogWrite(enA, motorSpeed);
    delay(delayTime);
  }




void reverse()
{
    Serial.println("reverse");
    Serial.print("Speed:");
    Serial.println(motorSpeed);
  
  if(digitalRead(in1) == HIGH && digitalRead(in2) == LOW)
    {
  	stopDriveMotor();//brake if running first before changing direction
  	delay(100);
    }
  
   // turn on drive motor - change motor directions
   digitalWrite(in1, LOW);
   digitalWrite(in2, HIGH);  
    // set speed to 200 out of possible range 0~255
   analogWrite(enA, motorSpeed);
   delay(delayTime);
  }



void stopDriveMotor()
{
    Serial.println("stop");
  // now turn off drive motor
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);  
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

  int ordinal =0;
  String commandStr ="";
  delay(loopDelay);
  Serial.println(".");
  if (Serial.available() > 0)
  {
	receivedStr = Serial.readString();
  if(lastReceived == receivedStr)
  {
    return; // do nothing & exit loop

  }
  else
  {
    lastReceived = receivedStr;
  }
  stringSplitter(receivedStr);
  
	Serial.print("read: ");
	Serial.println(receivedStr);

  ordinal = receivedStr.indexOf('~');
  while(ordinal > 0) //while there is a valid command to process
  {
      String newCommand =  receivedStr.substring(0,ordinal);
      receivedStr = receivedStr.substring(ordinal+1);
 
      ordinal = receivedStr.indexOf('~');
           
      if(newCommand != commandStr) //only do it once per group
      {
        
        commandStr = newCommand;
        received = newCommand.toInt();

    	 switch (received) {
    		case 2: //tail
    		{
    		  Serial.println("8:tail");
    		  moveTail();
    		  break;
    		}
    
    		  
    		case 4: //ears
    		Serial.println("10:ears");
    		 {
    		 moveEars();
    		 break;
    		 }
    		  
    		case 6: //display
    		  {
    		  Serial.println("11:display");
    		  break;
    		  }
    		  
    		case 8: //laser Out
    		{
    		 Serial.println("12:Laser Out");
    		 laserOut();
    		 break;
    		}
    			  
    		case 10: //laser In
    		 {
    		  Serial.println("13:Laser In");
    		  laserIn();
    		  break;
    		 }
    			  
    		case 12:  //Laser On Off: Note: this is actually switching relay
    		  {
    		  Serial.println("14:Laser On Off");
    		  laserOnOff();
    		  break;
    		  }
    		  
    		case 15: //toggle eyes on off
    		{
    			toggleEyes();	
    			break;
    		}		
    		
    		case 16:
    		{
    			flashEyes();
    			digitalWrite(digEyes, LOW); //turn them on if off
    			break;
    		}
    
    		case 30://Affirmative
    		case 31://Negative
    		case 32://Master	
    		case 33://Mistress
    		case 34:// no available data
    		case 35: //ill be good
    		case 36://Doctor last spoke
    		case 37://Efficient Machine
    		case 38:// iamk9M3
    		case 39: // naturally
    		{
    			transmitWireSoundLaser(received);
    			break;
    		}
    
    		default:
    		{
    			if(received >=99 && received < 400) //direction and angle values
    			  {
              motion(received);
    				}
           else
           {
                Serial.print("invalid value:");
                Serial.print(received);
            }
    			}
    		}
    		
    	  }
  }
  } // end of while loop
	}

 
void motion(int value)
{
  //direction is hundreds
  //direction speed is units
  //turn value is 10s
  //LEFT should only be values up to 40 multiples of 10 (10,20,30,40)
  //RIGHT should only be values over  multiples of 10   (50,60,70,80)
  
  int steeringPosn = 0;
  if(value >=100 && value < 200)
  {
    //stop
    stopDriveMotor();
    steeringPosn = (value -100)/10;
    steeringServo.write(steeringPosn); 
  }
  if(value >=200 && value < 300)
  {
    //reverse
    steeringPosn = (value - 200)/10;
    int speedOrd = (value - 200) - steeringPosn;
    motorSpeed = 25.5 * speedOrd; //255 = max speed
    reverse();
    steeringServo.write(steeringPosn); 
  }

  if(value >=300 && value < 400)
  {
    //forward
    steeringPosn = (value -300)/10;
    int speedOrd = (value - 200) - steeringPosn;
    motorSpeed = 25.5 * speedOrd; //255 = max speed
    forward();
    steeringServo.write(steeringPosn); 
  }

  steeringServo.write(steeringPosn); 
}


void moveEars()
{
   digitalWrite(digEars, HIGH);
   //transmitWireSoundLaser(56);
   transmitWireCtrlPanel(98);
   delay(2000);
   digitalWrite(digEars, LOW); 
}

void moveTail()
{
    Serial.println("8:tail");
    digitalWrite(digTail, HIGH); //start wagging
    //transmitWireSoundLaser(57);
    transmitWireCtrlPanel(99); //flash control panel lights
    delay(2000);
    digitalWrite(digTail, LOW); //stop wagging
     Serial.println("8:tail-done");

}

void laserIn()
{
     digitalWrite(digLaserLED, LOW); // turn off LED
     transmitWireSoundLaser(LASERIN);  //retract the laser
     Serial.println("laser in");  
     delay(500);  
}

void laserOut()
{
     digitalWrite(digLaserLED, HIGH); // Turn on LED
     transmitWireCtrlPanel(LASEROUT);
     transmitWireSoundLaser(LASEROUT);     
	 //TODO: check this as it was incorrectly set to 98.. make sure 98 in wiresound is updated with correct value of 97 for LASEROUT       
     Serial.println("laser out");
     delay(2000);
     digitalWrite(digLaserLED, LOW); // turn off LED
}


void laserOnOff()
{
      if(digitalRead(digLaserLED) == HIGH)
      {
         //turn off laser led
         digitalWrite(digLaserLED, LOW);
      }
      else
      {
        //turn on laser led
        digitalWrite(digLaserLED, HIGH);  
      }
}



void toggleEyes()
{	//this is a 'normally closed' circuit
      if(digitalRead(digEyes) == HIGH)
		digitalWrite(digEyes, LOW); 
	 else
		digitalWrite(digEyes, HIGH); //open circuit (power the relay)
}

void flashEyes()
	{
		for(int i=0; i < 4;i++)
			{
			toggleEyes();
			delay(300);
			}
	}



void transmitWireSoundLaser(int cmdId)
{
   Serial.print("begin trans..");
   Wire.beginTransmission(wireSoundLaserSlaveAddress);
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

   Serial.print( wireCtrlPanelSlaveAddress);
   Wire.beginTransmission(wireCtrlPanelSlaveAddress);
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


/*
void rightForward()
{
  // turn on motor A
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  // set speed to 200 out of possible range 0~255
  analogWrite(enA, motorSpeed);
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
  analogWrite(enB, motorSpeed);
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
  analogWrite(enB, motorSpeed);
   delay(delayTime);
  }

void rightReverse()
{
  // now change motor directions
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);  
    // set speed to 200 out of possible range 0~255
  analogWrite(enA, motorSpeed);
//  digitalWrite(in3, LOW);
//  digitalWrite(in4, HIGH); 
//    // set speed to 200 out of possible range 0~255
//  analogWrite(enB, motorSpeed);
  delay(delayTime);
  }
  
  
*/
