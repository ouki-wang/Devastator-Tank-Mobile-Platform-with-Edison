#include <Servo1.h> 
  
Servo baseServo;  // create servo object to control a servo 
Servo topServo;                // a maximum of eight servo objects can be created 
  
int pos = 90;    // variable to store the servo position 
  
void setup() 
{ 
  baseServo.attach(3);  // attaches the servo on pin 3 to the servo object 
  topServo.attach(5);
} 
  
  
void loop() 
{ 
 
    baseServo.write(pos);              // tell servo to go to position in variable 'pos' 
    topServo.write(pos);
} 
