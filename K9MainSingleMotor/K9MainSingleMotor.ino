#include <Wire.h> 
#include <Servo.h>
#include <String.h>
/*
 * K9 Main
 * 
 * Version 1.5
 * 
 * Single scooter motor version
 * Created 19-2-2018
 * Updated 12-4-2018
 * 
 * Note:  be sure to remove the tx rx before attempting upload
 */

#define VERSION "1.6"

// connect motor controller pins to Arduino digital pins
// drive motor 
int enA = 5;
int in1 = 7;
int in2 = 6;

int motorSpeed = 255;



// RELAY Settings
int digLaserLED = 22;//white
int digEars     = 24;//brown
int digTail     = 26;//red
int digEyes     = 28;


//wire settings
int wireMasterAddress = 8;
int wireSoundLaserSlaveAddress = 9;
int wireCtrlPanelSlaveAddress = 10;
int wireSteeringSlaveAddress= 11;


const int LASERIN = 97;
const int LASEROUT = 98;

//general settings
int received=0;
int lastCmd=0;

int delayTime =0;
int loopDelay = 000;


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

//  steeringServo.attach(servoPin); 
  
  
  //general controls
  Serial.begin(9600); //set baud rate
  Serial.setTimeout(500);
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
  	delay(10);
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
  	delay(10);
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
;
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
  //Serial.print(".");
  
  
  if (Serial.available() > 0)
  {
  	  received = Serial.read();
      if(received == lastCmd)
      {
         return;
      }
    
      if(received > 99) // only do this for movement commands
        lastCmd = received;

  
    	Serial.print("read: ");
    	Serial.println(received);

    	switch (received) {
    		case 2: //btConnect
    		{
    		  Serial.println("2:Connect");
    		  btConnect();
    		  break;
    		}

        case 3: //tail
        {
          Serial.println("3:tail");
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
        case 40://goodbye
    		{
    			transmitWireSoundLaser(received);
    			break;
    		}
    
    		default:
    		{
    			if(received > 99 && received < 200) //direction and angle values
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


 
void motion(int value)
{
  if(value > 200) //failsafe
    return;
    
  if(value == 100)
  {
    stopDriveMotor();  
  }
  else
  {
        Serial.print("\nmovement cmd:" );
        Serial.println(value);  
    
        int trajectory = (value - 100) / 10;
        int direct  = (value -100) - (trajectory *10);
        delay(100);
    
        Serial.print("trajectory:" );
        Serial.println(trajectory); 
        
        Serial.print("direction:" );
        Serial.println(direct); 
    
        switch(direct)
        {
            case 0:
            {
              Serial.println("stop"); // do nothing
              break;}
            
            case 1:
            {
              Serial.println("fwd");
              forward();
              break;}
    
            case 2:
            {
              reverse();
              Serial.println("rev");          
              break;}      
      
      }
          
      transmitWireSteering (trajectory);
 
  }

  
                         
}



void btConnect()
{
  Serial.println("BT Connected");
  flashEyes();  
  digitalWrite(digEyes, LOW); //turn them on if off
  moveEars();
  transmitWireSoundLaser(41);
  
  
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
    Serial.println("tail");
    digitalWrite(digTail, HIGH); //start wagging
    //transmitWireSoundLaser(57);
    transmitWireCtrlPanel(99); //flash control panel lights
    delay(2000);
    digitalWrite(digTail, LOW); //stop wagging
    Serial.println("tail-done");

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
   {
		digitalWrite(digEyes, LOW); 
    Serial.println("eyes off");
   }
	 else
  {
		digitalWrite(digEyes, HIGH); //open circuit (power the relay)
    Serial.println("eyes on");
  }
}

void flashEyes()
	{
		for(int i=0; i <= 4;i++)
			{
			toggleEyes();
			delay(200);
			}

      digitalWrite(digEyes, LOW); //turn them on if off
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
    Serial.print(" on ctrl pnl slave..");

   Serial.print( wireCtrlPanelSlaveAddress);
   Wire.beginTransmission(wireCtrlPanelSlaveAddress);
   Wire.write(cmdId);    
   Serial.println("sent..");          
   Wire.endTransmission();    // stop transmitting 
   Serial.println("end Transmission..");    
  
}


void transmitWireSteering(int cmdId)
{
   Serial.print("sending..");
   Serial.print(cmdId); 
   Serial.print(" on steering slave..");

   Serial.print( wireSteeringSlaveAddress);
   Wire.beginTransmission(wireSteeringSlaveAddress);
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



