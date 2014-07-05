#ifndef _DRIVE_H
#define _DRIVE_H


//input: final motor output values
//output: none
//Driving function
inline void drive(int lWheelSpeed, int rWheelSpeed, int activeWheelServoDir)
{
  motor.speed(leftWheelPin, lWheelSpeed)  ;
  motor.speed(rightWheelPin, rWheelSpeed)  ;   
  RCServo0.write(activeWheelServoDir)  ; 
}