#include <phys253.h>        		//***** from 253 template file
#include <LiquidCrystal.h>  		//***** from 253 template file
#include <servo253.h>       		//***** from 253 template file 

#include "drive.h"

  //global parameters
  
  //digital INs
  //Microswitch
  int mSwitchPin = 0  ;  
  //wheelEncoders
  int encoder1 = 1  ;
  int encoder2 = 2  ;  
  int cliffQRD = 3  ;
  
  //analog INs
  int leftIRPin = 0  ;
  int rightIRPin = 1  ;
  int leftTapePin = 2  ;
  int rightTapePin = 3  ;
  //zipline Arm Pots
  int arm1Pot = 4  ;
  int arm2Pot = 5  ;
  
  //motor OUTs
  //drive motors
  int leftWheelPin = 0  ;
  int rightWheelPin = 1  ;
  
  //logistics
  boolean onPebbles = false  ;
  boolean grabbedIdol = false  ;  //set when we grab the last artifact (idol)
  boolean safeRun = false  ;
  int minIRReading = 100  ;
  boolean offCliff = false  ;
  int robotState = 0  ;
  
  const int stateTape = 0  ;
  const int stateIR = 1  ;
  const int stateZipline = 2  ;
  const int stateCliff = 3  ;
  const int stateEscape = 4  ;
  const int stateReturnEarly = 5  ;
  
  //IR Detection
  int IRError = 0  ;
  int IRLastErr = 0  ;
  int IRPro = 0  ;
  int IRDer = 0  ;
  int IRCon = 0  ;
  int IRkP = 0  ;
  int IRkD = 0  ;
  int IRmSpeed = 500  ;
  
  //Tape Following
  // knobs used to change parameters
  int knob_one = 6;
  int knob_two = 7;

  int threshold = 250  ;
  int mSpeed = 500  ;
  //off course value (positive is too far left, negative is too far right)
  int error = 0  ;
  int lastError = 0  ;
  int recErr = 0  ;
  int harshCorrectionCoe = 4  ;
  
  //counters
  int prevErrDuration = 0;    //loops in last state
  int timeSinceDiffErr = 1;    //loops in this state
  int debugc = 0;          //counter for debugging loop
  int pro = 0;    //control term from proportional component
  int der = 0;    //control term from derivative componenet
  int con = 0;  //sum of control terms
 
  //PID parameters
  int kP = 200;
  int kI = 0;
  int kD = 0;
  
void setup() 
{
  
  portMode(0, INPUT) ;      	 	//***** from 253 template file
  portMode(1, INPUT) ;
  //frontWheel
  RCServo0.attach(RCServo0Output) ;	//***** from 253 template file
  RCServo1.attach(RCServo1Output) ;	//***** from 253 template file
  RCServo2.attach(RCServo2Output) ;	//***** from 253 template file

  Serial.begin(9600); //baud speed
  
  

}

//main control loop
void loop() 
{
  while ( !(startbutton()) ) ;
  
  boolean running = true  ;
  
  while (running)
  {
    int lIR = analogRead(leftIRPin)  ;
    int rIR = analogRead(rightIRPin)  ;
    
    boolean checkState = true  ;
    
    while (checkState)
    {
      if ((onPebbles) && (grabbedIdol))
      {
        robotState = stateZipline  ;
        break  ;
      }
      else if (digitalRead(cliffQRD))
      {
        robotState = stateCliff  ;
        break  ;
      }  
      //need to change this so we don't think we're stuck when
      //we're purposely standing still
      else if (checkEncoders())
      {
        robotState = stateEscape  ;
        break  ;
      }
      else if ((lIR>minIRReading) || (rIR>minIRReading))
      {
        if(!safeRun)
        {
          robotState = stateIR  ;
          break  ;
        }
        else
        {
          robotState = stateReturnEarly  ;
          break  ;
        }
      }
      else
      {
        robotState = stateTape  ;
      }
    }
    
    switch(robotState)
    {
      case stateZipline:
        //assuming we don't have another while loop after using zipline
        useZipline()  ;
      //add case for picking up idol
      case stateCliff:
        avoidCliff()  ;
        break  ;
      case stateEscape:
        escape()  ;
        break  ;
      case stateIR:
         followIR(lIR, rIR)  ;
         break  ;
      case stateReturnEarly:
         //is this function necessary? If we do nothing, will robot
         //turn 180 degrees and go back itself?
         returnEarly()  ;
         break  ;
      case stateTape:
         int lTape = analogRead(leftTapePin)  ;
         int rTape = analogRead(rightTapePin)  ;
         followTape(lTape, rTape)  ;
         break  ;
    }
  }
}

//input:
//output:
//function calls:
//purpose:
void avoidCliff()
{
}

//input:
//output:
//function calls:
//purpose:
void returnEarly()
{
}

//input:
//output:
//function calls:
//purpose:
void useZipline()
{
}

//input:
//output:
//function calls:
//purpose:
boolean checkEncoders()
{
  return 1  ;
}

//input:
//output:
//function calls:
//purpose:
void escape()
{
}

//input: original tape readings from circuit
//output: none
//function calls: drive
void followTape(int leftTapeReading, int rightTapeReading)
{
  if((leftTapeReading>threshold) && (rightTapeReading>threshold)) error = 0;
  if((leftTapeReading>threshold) && (rightTapeReading<threshold)) error = -1;
  //to the right
  if((leftTapeReading<threshold) && (rightTapeReading>threshold)) error = 1;
  //to the left
  if((leftTapeReading<threshold)&&(rightTapeReading<threshold)) 
  {
    if(lastError>0) error = harshCorrectionCoe  ;
    if(lastError<0) error = -1*harshCorrectionCoe  ;
  }
  if(!(error==lastError))
  {
   recErr=lastError  ;
   lastError = error  ;
   prevErrDuration = timeSinceDiffErr  ;
   timeSinceDiffErr=1  ; 
  }
  pro = kP*error  ;
  der = (int)((float)kD*(float)(error-recErr)/(float)(prevErrDuration+timeSinceDiffErr))  ;
  con = pro + der  ;
  int leftSpeed = mSpeed + con;
  int rightSpeed = mSpeed - con;
  
  //debugging
  if (debugc==30) 
   { 
   LCD.setCursor(0,0);  
   LCD.clear();  LCD.home() ;
   LCD.print(leftTapeReading);
   LCD.print(" ");
   LCD.print(rightTapeReading);//,left_sensor,kP,kD,p,d); 
   LCD.print(" ");
   LCD.print(kP);
   LCD.print(" ");
   LCD.print(kD);
   LCD.print(" ");
   LCD.print(leftSpeed);
   LCD.setCursor(0,1);
   LCD.print(rightSpeed);
   LCD.print(" ");
   LCD.print(error);
   
   kD = knob(knob_one);
   kP = knob(knob_two);
   debugc=0; 
   }
  timeSinceDiffErr = timeSinceDiffErr + 1  ;
  debugc = debugc + 1  ;
  //currently not using front wheel servo
  drive(leftSpeed, rightSpeed, 0)  ;
}

//input: original IR signals from circuit
//output: none
//function calls: signalSmooth, drive
//purpose: Analyzes IR signals and determines direction to drive
void followIR(int leftIRInput, int rightIRInput)
{
  //SignalSmoothing
  float lSmoothSignal = signalSmooth(leftIRInput)  ;
  float rSmoothSignal = signalSmooth(rightIRInput)  ;
  
  //Account for circuit differences between left and right IR sensors
  
  //PID
  //positive error means too far to the right
  IRError = (int)(lSmoothSignal - rSmoothSignal)  ;
  IRPro = IRkP * IRError  ;
  //currently not dividing by any dt
  IRDer = (int)((float)IRkD * (float)(IRError-IRLastErr))  ;
  IRCon = IRPro + IRDer  ;
  
  int lWheelSpeed = IRmSpeed + IRCon  ;
  int rWheelSpeed = IRmSpeed - IRCon  ;
  int activeWheelDir  ;
  
  IRLastErr = IRError ;
  drive(lWheelSpeed, rWheelSpeed, activeWheelDir)  ;  
}

//input: a single original IR signal from circuit
//output: weighted average of input values
//purpose: smooths the IR data using Gaussian smooth
float signalSmooth(int originalSignal)
{
  int signalArray[7];
  // 1 3 6 7 6 3 1 smooth
  //increment old datapoints down array
  for(int i = 0; i < 6; i++)
  {
    signalArray[i+1] = signalArray[i];
  }
  
  // new data point
  signalArray[0] = originalSignal  ;
  
  //there is a delay of 3 function calls since the newest
  //measured point is given a weight of 1 then after 3 calculations
  //the max weight of 7
  
  return (float)(1*signalArray[0] + 3*signalArray[1] 
  + 6*signalArray[2] + 7*signalArray[3] + 6*signalArray[4] 
  + 3*signalArray[5] + 1*signalArray[6])/(27.0)  ;
} 
