#include <Wire.h> 
#include <Servo.h>

/*
  Steering servo code
  Created 15-4-2018
  Modified 15-4-2018
*/


//Servo
Servo steeringServo;
int servoPin = 9;
int digSteeringWireSlaveAddress = 11;
int wireReceivedValue =-1;
int received=-1;
#define VERSION "1.0"






void setup() {
  Wire.begin(digSteeringWireSlaveAddress);
  Wire.onReceive(receiveEvent);
  steeringServo.attach(servoPin); 

  Serial.begin(9600); //set baud rate
  
  Serial.println("**********");
    Serial.println("*K9 Steering ");
  Serial.println(VERSION);
  Serial.println("**********");
}

void receiveEvent() {
  wireReceivedValue = Wire.read();
  Serial.print("wire received:");
  Serial.print(wireReceivedValue);
}

void loop() {
    
    delay(10); //take this out
    received = wireReceivedValue;
    wireReceivedValue=0;
    
    Serial.print(".");
    
    if (Serial.available() > 0)
    {
      received = Serial.read();
  
      Serial.println(received);
    }

    motion(received);
    received = -1;
    wireReceivedValue=-1;
}


void motion(int trajectory)
{
  
      switch(trajectory)
    {
        case 0:
        {
          Serial.println("centre");
          steeringServo.write(90); //centre
          break;}
        
        case 1:
        {
          Serial.println("left");
          steeringServo.write(30);
          break;}

        case 2:
        {
          Serial.println("right");  
          steeringServo.write(140);        
          break;}      
      
      }   
}
