#include <phys253.h>        		//***** from 253 template file
#include <LiquidCrystal.h>  		//***** from 253 template file
#include <servo253.h>       		//***** from 253 template file 

  //global variable declarations
  
  //digital INs
  //Microswitch
  #include "WProgram.h"
#include <HardwareSerial.h>
void setup();
void loop();
int mSwitchPin = 0  ;  
  //wheelEncoders
  int encoder1 = 1  ;
  int encoder2 = 2  ;  
  int cliffQRD = 3  ;
  int idolQRD = 4  ;
  
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
  //idol motor
  int idolMotor = 2  ;
  
  //logistics
  boolean onPebbles = false  ;
  boolean grabbedIdol = false  ;  //set when we grab the last artifact (idol)
  boolean safeRun = false  ;
  int minIRReading = 100  ;
  boolean offCliff = false  ;
  int robotState = 0  ;
  
  const int stateTape = 0  ;
  const int stateIR = 1  ;
  const int stateIdol = 6  ;
  const int stateZipline = 2  ;
  const int stateCliff = 3  ;
  const int stateEscape = 4  ;
  const int stateReturnEarly = 5  ;
  
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
#include "avoidCliff.h"
#include "returnEarly.h"
#include "useZipline.h"
#include "checkEncoders.h"
#include "escape.h"
#include "followTape.h"
/**not sure if signalSmooth.h actually needs to be #included
in the main control file. It is only used by followIR.h
**/
#include "signalSmooth.h"
#include "followIR.h"
#include "extractArtifact.h"

//main control loop
void loop() 
{
  while ( !(startbutton()) ) ;
  
  boolean running = true  ;
  
  while (running)
  {
    int lIR = analogRead(leftIRPin)  ;
    int rIR = analogRead(rightIRPin)  ;
    
    if ((onPebbles) && (grabbedIdol))
    {
      robotState = stateZipline  ;
    }
    else if (digitalRead(cliffQRD))
    {
      robotState = stateCliff  ;
    }  
    /**need to change this so we don't think we're stuck when
       we're purposely standing still
    **/
    else if (checkEncoders())
    {
      robotState = stateEscape  ;
    }
    else if ((lIR>minIRReading) || (rIR>minIRReading))
    {
      if(!safeRun)
      {
        robotState = stateIR  ;
      }
      else
      {
        robotState = stateReturnEarly  ;
      }
    }
    else if (digitalRead(idolQRD))
    {
      robotState = stateIdol  ;
    }
    else
    {
      robotState = stateTape  ;
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
         /**is this function necessary? If we do nothing, will robot
            turn 180 degrees and go back itself?
         **/
         returnEarly()  ;
         break  ;
      case stateIdol:
         extractArtifact()  ;
         break  ;
      case stateTape:
         int lTapeRaw = analogRead(leftTapePin)  ;
         int rTapeRaw = analogRead(rightTapePin)  ;
         followTape(lTapeRaw, rTapeRaw)  ;
         break  ;
    }
  }
}

