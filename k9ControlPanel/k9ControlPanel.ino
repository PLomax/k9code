
/*
 * Version 1.0
 * 2-12-2017
 */

#include <Wire.h> 
#include <Math.h>
#include <String.h>




const int ROW1 = 11;
const int  ROW2 = 12;
const int  ROW3 = 13;
const int COL1 = 4;
const int COL2 = 5;
const int COL3 = 6;
const int COL4 = 7;


const int LASEROUT = 97; //a
const int EARS = 98; //b
const int TAIL = 99; //c
const int MISC1 = 100; //d
const int MISC2 = 101; //e
const int MISC3 = 102; //f
const int MISC4 = 103; //g
const int MISC5 = 104; //h
const int MISC6 = 105; //i


int delayInt = 400;

const int digCtrlPanelWireSlaveAddress = 10;
int wireReceivedValue =0;
int received=0;
int randNumber = random(1,5); 
String versionNumber = "Version 1.0";

// the setup function runs once when you press reset or power the board
void setup() {
  Wire.begin(digCtrlPanelWireSlaveAddress);
  Wire.onReceive(receiveEvent);
  
  pinMode(ROW1, OUTPUT);
  pinMode(ROW2, OUTPUT);
  pinMode(ROW3, OUTPUT);
  pinMode(COL1, OUTPUT);
  pinMode(COL2, OUTPUT);
  pinMode(COL3, OUTPUT);
  pinMode(COL4, OUTPUT); 
   setAll(HIGH);
   Serial.begin(9600); //set baud rate
   Serial.println("K9 Control Panel");
   Serial.println(versionNumber);
}


void receiveEvent() {
  wireReceivedValue = Wire.read();
  Serial.print("wire received:");
  Serial.print(wireReceivedValue);
}


void setAll(int val)
{
  digitalWrite(COL1, val);   
  digitalWrite(COL2, val);   
  digitalWrite(COL3, val);  
  digitalWrite(COL4, val);  
   
  digitalWrite(ROW1, val);   
  digitalWrite(ROW2, val);   
  digitalWrite(ROW3, val);  
    
 }

 void randomSim()
 {
   randNumber = random(1,6); 
   setAll(HIGH);
   
   switch(randNumber)
   {
     case 1:
    {
         for(int col=COL1;col <= COL4;col++)
         {
           digitalWrite(col, LOW);
              delay(delayInt/randNumber);
         }
        break;
    }
     case 2:
    {
         for (int rw =ROW1;rw<=ROW3;rw++)
          {         
            digitalWrite(rw, LOW); 
               delay(delayInt/randNumber);
          }
        break;
    }

    case 3:
    {
         for(int col=COL4;col > COL1;col--)
         {
               digitalWrite(col, LOW);
                  delay(delayInt/randNumber);
         }
        break;
    }

    case 4:
    {
        for(int col=COL4;col > COL1;col--)
         {
               digitalWrite(col, LOW);
               delay(delayInt/randNumber);
         }
        break;
    }

    case 5:
    {
         for(int rw=ROW3;rw > ROW1;rw--)
         {
               digitalWrite(rw, LOW);
               delay(delayInt/randNumber);
         }
        break;
    }
         
   }


   setAll(HIGH);
  
  }

  void ears()
  {
    
       Serial.println("ears");
       setAll(HIGH);
       for(int i=0;i<6;i++)
       {
        digitalWrite(COL1, HIGH);
        delay(200);       
        digitalWrite(COL1, LOW);        
        delay(200);  
        }

  }
  
  void laserOut()
  {
             
       Serial.println("laser Out");
       setAll(HIGH);
       for(int i=0;i<15;i++)
       {
        digitalWrite(COL2, HIGH);
        delay(30);       
        digitalWrite(COL2, LOW);        
        delay(100);  
        }
  }

  
  void tail()
  {
       
       Serial.println("tail");
       
       setAll(HIGH);
       for(int i=0;i<8;i++)
       {
        digitalWrite(COL2, LOW);
        digitalWrite(COL4, HIGH); 
        delay(200);       
        digitalWrite(COL4, LOW); 
        digitalWrite(COL2, HIGH);  
      //  digitalWrite(COL3, LOW);     
      //  digitalWrite(COL2, LOW);   
      //  digitalWrite(COL1, LOW); 
        delay(200);  
        }
  }

  void misc(int cnt)
  {
      Serial.print("misc:");
      Serial.println(cnt);
      setAll(HIGH);

      if(cnt  == MISC1 || cnt  == MISC2 || cnt  == MISC3 )
      {
      for(int i=0;i<3;i++)
       {
        digitalWrite(ROW3, HIGH);
        delay(500);       
        digitalWrite(ROW3, LOW);        
        delay(1000);  
        }
      }
      else
      {
       for(int i=0;i<3;i++)
       {
        digitalWrite(ROW1, HIGH);
        delay(1000);       
        digitalWrite(ROW1, LOW);        
        delay(400);  
        }
      }
  }

// the loop function runs over and over again forever
void loop() {
    received = wireReceivedValue;
    wireReceivedValue=0;
    
    Serial.print(".");
    
    if (Serial.available() > 0)
    {
      received = Serial.read();
  
      Serial.println(received);
    }

  switch(received)
  {
     case LASEROUT:
      {
        Serial.println("bhho");
        laserOut();
        break;
      }



    case  EARS:
      {
        ears();
        break;
      }
    
    case  TAIL:
      {
        tail();
        break;
      }
    
    case  MISC1:
      {
        misc(1);
        break;
      }


    
    case  MISC2:
      {
        misc(2);
        break;
      }


          
     case MISC3:
      {
        misc(3);
        break;
      }


          
    case  MISC4:
      {
        misc(4);
        break;
      }


          
     case MISC5:
      {
        misc(5);
        break;
      }

          
     case MISC6:
      {
        misc(6);
        break;
      }

                
   }
   
  randomSim();

}
