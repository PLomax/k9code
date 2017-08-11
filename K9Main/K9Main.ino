#include <Wire.h> 

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
int digLaserWireMasterAddress = 3;
int digLaserWireSlaveAddress = 4;

const int LASERIN = 97;
const int LASEROUT = 98;

bool laserOnFlag = false;

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

  //laser servo
//  laserServo.attach(digLaserServoPin);  // attaches the servo on pin digServoPin to the servo object 
//  laserServo.write(laserServoPosn);
//  //     laserServo.attach(digLaserServoPin);  // attaches the servo on pin digServoPin to the servo object 
//  delay(1000);
//  laserServo.write(0);
//  laserServo.write(laserServoPosn);
//  delay(1000);

Wire.begin(digLaserWireMasterAddress);// join i2c bus with address
  Wire.onRequest(requestEvent); // register event

  
  //general controls
  Serial.begin(9600); //set baud rate
  Serial.println("commenced");
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


 Serial.println(received);
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
    
    case 8:
      Serial.println("8:tail");
	  digitalWrite(digTail, HIGH);
	  delay(2000);
	  digitalWrite(digTail, LOW);
    break;

    case 9://stop
      Serial.println("9:stop");
      stopped();
      //delay(500);
      break;    
      
    case 10:
      Serial.println("10:ears");
	  digitalWrite(digEars, HIGH);
	  delay(2000);
	  digitalWrite(digEars, LOW);
    break;

      
    case 11:
      Serial.println("11:display");
    break;
          
    case 12:
     Serial.println("12:Laser Out");
     digitalWrite(digLaserLED, HIGH); // Turn on LED
     Wire.beginTransmission(digLaserWireSlaveAddress); // transmit to device #9
     Wire.write(LASEROUT);              // sends x 
     Wire.endTransmission();    // stop transmitting
     Serial.println("gun out");
     delay(500);
    break;

          
    case 13:
     Serial.println("13:Laser In");
     digitalWrite(digLaserLED, LOW); // turn off LED
     Wire.beginTransmission(digLaserWireSlaveAddress); // transmit to device #9
     Wire.write(LASERIN);              // sends y
     Wire.endTransmission();    // stop transmitting
     Serial.println("gun in");	
     delay(500);  
	  
    break;
          
    case 14:  //Note: this is actually switching relay
      Serial.println("14:Laser On Off");
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
    break;
  }

stopped();
//laserServo.detach(); 
//  forward();
//  stopped();
//  
//  leftForward();
//  stopped();
//  
//  rightForward();
//  stopped();
//    
//  reverse();
//  stopped();
//
//  leftReverse();
//  stopped();
//  
//  rightReverse();
//  stopped();
}




//void demoOne()
//{
//  // this function will run the motors in both directions at a fixed speed
//  // turn on motor A
// // digitalWrite(in1, HIGH);
//  //digitalWrite(in2, LOW);
//  // set speed to 200 out of possible range 0~255
//  analogWrite(enA, 200);
//  // turn on motor B
// // digitalWrite(in3, HIGH);
// // digitalWrite(in4, LOW);
//  // set speed to 200 out of possible range 0~255
//  analogWrite(enB, 200);
//  delay(4000);
//  stopped();
//  // now change motor directions
//  digitalWrite(in1, LOW);
//  digitalWrite(in2, HIGH);  
//  digitalWrite(in3, LOW);
//  digitalWrite(in4, HIGH); 
//  delay(2000);
//  // now turn off motors
//  digitalWrite(in1, LOW);
//  digitalWrite(in2, LOW);  
//  digitalWrite(in3, LOW);
//  digitalWrite(in4, LOW);
//}
//void demoTwo()
//{
//  // this function will run the motors across the range of possible speeds
//  // note that maximum speed is determined by the motor itself and the operating voltage
//  // the PWM values sent by analogWrite() are fractions of the maximum speed possible 
//  // by your hardware
//  // turn on motors
//  digitalWrite(in1, LOW);
//  digitalWrite(in2, HIGH);  
//  digitalWrite(in3, LOW);
//  digitalWrite(in4, HIGH); 
//  // accelerate from zero to maximum speed
//  for (int i = 0; i < 256; i++)
//  {
//    analogWrite(enA, i);
//    analogWrite(enB, i);
//    delay(20);
//  } 
//  // decelerate from maximum speed to zero
//  for (int i = 255; i >= 0; --i)
//  {
//    analogWrite(enA, i);
//    analogWrite(enB, i);
//    delay(20);
//  } 
//  // now turn off motors
//  digitalWrite(in1, LOW);
//  digitalWrite(in2, LOW);  
//  digitalWrite(in3, LOW);
//  digitalWrite(in4, LOW);  
//}
