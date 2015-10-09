/***************************************************
  RomeoEdison Auto Mode
 ***************************************************
 The car will run automatically in this mode. Whenever
 the URM37 sensor detects any obstacle, the car will turn left
 or right according to the postion of the obstacle. 
 
 Created 2015-7-27
 By Bill Hong 

 DFrobot inc. 
 ****************************************************/
 

#include <DFRobot.h>
#include <IIC1.h>
#include <Servo1.h>

#define URTRIG 2
#define baseServoPin 3
#define topServoPin 5
#define buzzerPin 6
#define backLEDPin 7
#define URECHO 9
#define frontLEDPin 10




unsigned int DistanceMeasured = 0;                // Distance between the sensor and obstacles

DFrobotEdison MotorLeft;
DFrobotEdison MotorRight;
int currentSpeed = 0;
int currentRightSpeed = 0;
Servo baseServo;
Servo topServo;
int basePos = 90;
int topPos = 90;
int posStep = 5;
Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 
int pos = 90;    // variable to store the servo position 
 
void setup()
{
  //Serial initialization
  Serial.begin(9600);                       // Sets the baud rates to 9600
  Serial.println("Init the sensor");        // Serial is used for communication between the Edison and the computer

  //Initilize Pin Mode
  pinMode(URTRIG, OUTPUT);                   // A low pull on pin COMP/TRIG
  digitalWrite(URTRIG, HIGH);                // Set to HIGH
  pinMode(URECHO, INPUT);                    // Sending Enable PWM mode command
  pinMode(backLEDPin, OUTPUT);               // Set LED pin to show info
  pinMode(frontLEDPin,OUTPUT);       
  digitalWrite(backLEDPin, HIGH);            // backLED is always on Auto Mode

  //Initilize Motor Drivers
  MotorLeft.begin(M2);
  MotorRight.begin(M1);
  MotorLeft.move();
  MotorRight.move();
  motorForward();

  //Initiliza Servo
  baseServo.attach(baseServoPin);
  topServo.attach(topServoPin);
  baseServo.write(basePos);
  topServo.write(topPos);


 myservo.attach(3); 
}


void loop()
{
 
  baseServoSweep();                         //baseServo sweep from 60 to 120 
  MeasureDistance();                        //Get distance bewteen the sensor and the car
  if (DistanceMeasured > 30)
  {
    //if it is safe to run forward
    digitalWrite(frontLEDPin, LOW); 
    motorForward();
  
  }
  else
  {
    //if it is needed to turn around
    digitalWrite(frontLEDPin, HIGH);
    motorBackward();
    motorStop();
    motorMoveAroound();                   //the car will turn left or right according to the current senor postion
    motorStop();
  }
}

void MeasureDistance()
{


  Serial.print("Distance Measured=");
  digitalWrite(URTRIG, LOW);
  delay(10);
  digitalWrite(URTRIG, HIGH);               // reading Pin PWM will output pulses
   unsigned long LowLevelTime = pulseIn(URECHO, LOW) ; 
  
  if (LowLevelTime >= 145000)               // the reading is invalid.
  {
    Serial.print("Invalid");
  }
  else
  {
    DistanceMeasured = LowLevelTime / 50;  // every 50us low level stands for 1cm
    Serial.print(DistanceMeasured);
    Serial.println("cm");
  }
}

inline void baseServoSweep()             //baseServo sweep from 60 to 120 
{
  if(basePos >= 120||basePos <=60) 
    posStep = -posStep;
  basePos+=posStep;                     //postion increases or decreases by step
  baseServo.write(basePos);
  
}

inline void motorBackward()
{
  MotorLeft.setDirection(ANTICLOCKWISE);
  MotorRight.setDirection(ANTICLOCKWISE);
  for(int i=0;i<150;i+=10)
  {
    MotorLeft.setSpeed(i);
    MotorRight.setSpeed(i);
    delay(20);
  }
  currentSpeed = -150;
}


inline void motorForward()
{
  if(currentSpeed!=150)
  {
    MotorLeft.setDirection(CLOCKWISE);
    MotorRight.setDirection(CLOCKWISE);
    for(int i=0;i<150;i+=10)
    {
      MotorLeft.setSpeed(i);
      MotorRight.setSpeed(i);
      delay(20);
    }
    currentSpeed = 150;
  }
  delay(50);
}

inline void motorStop()
{
  MotorLeft.setSpeed(0);
  MotorRight.setSpeed(0);
  delay(50);
}

inline void motorMoveAroound()
{
  bool directionMotorLeft;
  bool directionMotorRight;
  int speedMotorLeft;
  int speedMotorRight;
  int delayTime;
  if(basePos == 90)
  {
    //move backward if sensor detected obstacles at center 
    directionMotorLeft = ANTICLOCKWISE;
    directionMotorRight = ANTICLOCKWISE; 
    speedMotorLeft = 200;
    speedMotorRight = 200;
    delayTime = 200;
  }
  else if (basePos <90)
  {
    //turn left if sensor detected obstacles at right of the car
    directionMotorLeft = ANTICLOCKWISE ;
    directionMotorRight = CLOCKWISE;
    speedMotorLeft = 250 - (90-basePos)/30*50;
    speedMotorRight = 250 - (90-basePos)/30*50;
    delayTime = 300 - (90-basePos)/30*80;
  }
  else
  {
    //turn right if sensor detected obstacles at left of the car
    directionMotorLeft = CLOCKWISE;
    directionMotorRight = ANTICLOCKWISE;
    speedMotorLeft = 250 - (basePos-90)/30*50;
    speedMotorRight = 250 - (basePos-90)/30*50;
    delayTime = 300 - (basePos-90)/30*80;
  }
  
  MotorLeft.setDirection(directionMotorLeft);
  MotorLeft.setSpeed(speedMotorLeft); 
  MotorRight.setDirection(directionMotorRight);
  MotorRight.setSpeed(speedMotorRight); 
  delay(delayTime);
}




