#ifndef _FOLLOW_IR_H
#define _FOLLOW_IR_H

#include "signalSmooth.h"
#include "drive.h"


/**
This is the function called to follow IR. We need to tune PID parameters.
**/

//input: original IR signals from circuit
//output: none
//function calls: signalSmooth, drive
//purpose: Analyzes IR signals and determines direction to drive
inline void followIR(int leftIRRaw, int rightIRRaw)
{
  //IR Detection
  int IRError = 0  ;
  int IRLastErr = 0  ;
  int IRPro = 0  ;
  int IRDer = 0  ;
  int IRServoDir = 90  ;
  int IRkP = 0  ;
  int IRkD = 0  ;
  int IRmSpeed = 500  ;
  
  //SignalSmoothing
  float lSmoothSignal = signalSmooth(leftIRRaw)  ;
  float rSmoothSignal = signalSmooth(rightIRRaw)  ;
  
  //Account for circuit differences between left and right IR sensors
  
  //PID
  //positive error means too far to the right
  IRError = (int)(lSmoothSignal - rSmoothSignal)  ;
  IRPro = IRkP * IRError  ;
  //currently not dividing by any dt
  IRDer = (int)((float)IRkD * (float)(IRError-IRLastErr))  ;
  //servo output is between 0 and 180
  IRServoDir = IRPro + IRDer + 90  ;
  
  int lWheelSpeed = IRmSpeed  ;
  int rWheelSpeed = IRmSpeed  ;
  
  IRLastErr = IRError ;
  drive(lWheelSpeed, rWheelSpeed, IRServoDir)  ;  
}

#endif