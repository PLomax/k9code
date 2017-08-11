// Controlling a servo position using a potentiometer (variable resistor) 
#include <Wire.h> 
#include <Servo.h> 

//laser settings
Servo laserServo;  // create servo object to control a servo 
 
int servoPosn=85;    
int received=0;
int wireReceivedValue = 0;

const int LASERIN = 98;
const int LASEROUT = 97;

int digLaserServoPin = 8;
int digLaserWireMasterAddress = 3;
int digLaserWireSlaveAddress = 4;
bool laserOnFlag = false;
 
void setup() 
{ 
  laserServo.attach(digLaserServoPin);  // attaches the servo on pin 9 to the servo object 
  laserServo.write(servoPosn);  
  Serial.begin(9600); //set baud rate
  Serial.println("commenced");
  Wire.begin(digLaserWireSlaveAddress);
  Wire.onReceive(receiveEvent);
} 

//wire.h request event method
void receiveEvent() {
  wireReceivedValue = Wire.read();
}

 
void loop() 
{

  received=0;
  Serial.println("reading");
  if (Serial.available() > 0)
  {
    received = Serial.read();
    Serial.println("read");
    Serial.println(received);
  }


 Serial.println(received);

  if (received == LASERIN || wireReceivedValue== LASERIN)
  {
     servoPosn=0;
     laserServo.attach(digLaserServoPin); 
     laserServo.write(servoPosn); 
  //   laserServo.detach();

    Serial.println("gun out");
  }

  if (received == LASEROUT || wireReceivedValue == LASEROUT)
  {
     servoPosn=85;
     laserServo.attach(digLaserServoPin); 
     laserServo.write(servoPosn); 
    // laserServo.detach();
    Serial.println("gun in");
  }
  
  delay(1000);                           // waits for the servo to get there 

  laserServo.detach();
 
}
