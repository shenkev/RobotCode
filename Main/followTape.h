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
  con = pro + der  ;
  int leftSpeed = mSpeed + con;
  int rightSpeed = mSpeed - con;
  
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

#endif