#ifndef _FOLLOW_TAPE_H
#define _FOLLOW_TAPE_H

#include "drive.h"

/**
This is the function called to follow tape. We need to tune PID parameters.
**/

//input: original tape readings from circuit
//output: none
//function calls: drive
inline void followTape(int leftTapeRaw, int rightTapeRaw)
{
  //var declarations
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
  int servoDir = 90;  //sum of control terms
 
  //PID parameters
  int kP = 200;
  int kI = 0;
  int kD = 0;

  if((leftTapeRaw>threshold) && (rightTapeRaw>threshold)) error = 0;
  if((leftTapeRaw>threshold) && (rightTapeRaw<threshold)) error = -1;
  //to the right
  if((leftTapeRaw<threshold) && (rightTapeRaw>threshold)) error = 1;
  //to the left
  if((leftTapeRaw<threshold)&&(rightTapeRaw<threshold)) 
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
  servoDir = pro + der + 90;
  //Servo Output is integer from 0 to 180 so an error of 0 should correspond to 90
  int lSpeed = mSpeed  ;
  int rSpeed = mSpeed  ;
  
  //debugging
  if (debugc==30) 
   { 
   LCD.setCursor(0,0);  
   LCD.clear();  LCD.home() ;
   LCD.print(leftTapeRaw);
   LCD.print(" ");
   LCD.print(rightTapeRaw);//,left_sensor,kP,kD,p,d); 
   LCD.print(" ");
   LCD.print(kP);
   LCD.print(" ");
   LCD.print(kD);
   LCD.print(" ");
   LCD.print(servoDir);
   LCD.setCursor(0,1);
   LCD.print(" ");
   LCD.print(error);
   
   kD = knob(knob_one);
   kP = knob(knob_two);
   debugc=0; 
   }
  timeSinceDiffErr = timeSinceDiffErr + 1  ;
  debugc = debugc + 1  ;
  //currently not using front wheel servo
  drive(lSpeed, rSpeed, servoDir)  ;
}

#endif
