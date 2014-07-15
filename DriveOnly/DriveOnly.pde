#include <phys253.h>        		//***** from 253 template file
#include <LiquidCrystal.h>  		//***** from 253 template file
#include <servo253.h>       		//***** from 253 template file 

  int leftTapePin = 2  ;
  int rightTapePin = 3  ;
  
    //motor OUTs
  //drive motors
  int leftWheelPin = 0  ;
  int rightWheelPin = 1  ;

void setup() 
{
  portMode(0, INPUT) ;      	 	//***** from 253 template file
  portMode(1, INPUT) ;
  //RCServo0 is the frontWheel
  RCServo0.attach(RCServo0Output) ;	//***** from 253 template file
  //RCServ01 is the retrieval arm
  RCServo1.attach(RCServo1Output) ;	//***** from 253 template file
  //RCServo2 is the zipline arm
  RCServo2.attach(RCServo2Output) ;	//***** from 253 template file

  Serial.begin(9600); //baud speed
}

/**files need to be included after variable declarations so they
can access the variables
**/
#include "drive.h"
#include "followTape.h"

//main control loop
void loop() 
{
         int lTapeRaw = analogRead(leftTapePin)  ;
         int rTapeRaw = analogRead(rightTapePin)  ;
         followTape(lTapeRaw, rTapeRaw)  ;
}

