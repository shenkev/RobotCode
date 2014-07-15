#ifndef _DRIVE_H
#define _DRIVE_H

/**
This is the function called to drive the wheels
**/

//input: final motor output values
//output: none
//Driving function
inline void drive(int lWheelSpeed, int rWheelSpeed, int servoDir)
{
  if(servoDir>180)
  {
	servoDir = 180	;
  }
  else if(servoDir<0)
  {
	servoDir = 0	;
  }
  motor.speed(leftWheelPin, lWheelSpeed)  ;
  motor.speed(rightWheelPin, rWheelSpeed)  ;   
  RCServo0.write(servoDir)  ; 
}

#endif