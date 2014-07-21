#include <phys253.h>        		//***** from 253 template file
#include <LiquidCrystal.h>  		//***** from 253 template file
#include <servo253.h>       		//***** from 253 template file 
//#include "HardwareTimer2.h"

// Tape following Pins
int leftTapeQRD = 3;
int rightTapeQRD = 2;

int leftThreshold = 250;
int rightThreshold = 250;

// Motor Speed Pins
int rightWheelPin = 0;
int leftWheelPin = 1;

//Knobs
int knob_one = 6;
int knob_two = 7;

//PID parameters
int kP = 100;
int kD = 20;

//correction errors
int error = 0;
int lasterror = 1;
int PIDAdjustment = 0;

//wheel controll
int motorSpeed = 500;
int neutral = 90;
int servoPosition = 90;

//debugging
int counter = 1;
//Including Outside Files

void setup() 
{
  //to prevend fuse from blowing
  motor.speed(leftWheelPin, 0)  ;
  motor.speed(rightWheelPin, 0)  ;
  
  //attach driving servo
  RCServo1.attach(RCServo1Output) ;
  RCServo1.write(neutral)  ; 
  
  //set ports to inputs
  portMode(0, INPUT) ;      	 
  portMode(1, INPUT) ;
  //time it takes followTape to run (ms)
  int followTapeTime = 30 ;
  //set the hardware timer
  //Timer2.set(followTapeTime, followTape);
  Serial.begin(9600);
  
    while(1){
    delay(50);
    while( !(stopbutton()) ){
      delay(50);
        LCD.setCursor(0,0);  LCD.clear();  LCD.home() ;
        LCD.print("kP");
        LCD.print(" ");
        LCD.print(kP);
        kP = knob(knob_one); 
        LCD.print(" ");
        LCD.setCursor(0,1);
        LCD.print("kD");
        LCD.print(" ");
        LCD.print(kD);
        kD = knob(knob_two);
        if (startbutton())
        {
          goto loopstart;
        }
    }
    delay(50);
    while( !(stopbutton())) { 
        delay(50);
        LCD.setCursor(0,0);  LCD.clear();  LCD.home() ;
        LCD.print("neutral");
        LCD.print(" ");
        neutral = (int)(knob(knob_one)*180.0/1023.0);
        if (neutral>180)
           neutral = 180;
        if (neutral<0) 
           neutral = 0;
        LCD.print(neutral);
        RCServo1.write(neutral); 
        LCD.setCursor(0,1); 
        LCD.print("motorSpeed");
        LCD.print(" ");
        LCD.print(knob(knob_two));
        motorSpeed = knob(knob_two);
        if (startbutton())
        {
          goto loopstart;
        }
    }
    delay(50);
    while( !(stopbutton())) { 
        delay(50);
        LCD.setCursor(0,0);  LCD.clear();  LCD.home() ;
        LCD.print("leftQRD");
        LCD.print(" ");
        LCD.print(analogRead(leftTapeQRD));
        LCD.print(" ");
        leftThreshold = knob(knob_one);
        LCD.print(leftThreshold);
        LCD.setCursor(0,1); 
        LCD.print("rightQRD");
        LCD.print(" ");
        LCD.print(analogRead(rightTapeQRD));
        LCD.print(" ");
        rightThreshold = knob(knob_two);
        LCD.print(rightThreshold);
        if (startbutton())
        {
          goto loopstart;
        }
    }
  }
        loopstart: ;
}

void loop() 
{
   motor.speed(rightWheelPin, motorSpeed)  ;
   motor.speed(leftWheelPin, motorSpeed)  ;
   followTape();
   LCD.setCursor(0,0);  LCD.clear();  LCD.home() ;
   LCD.print("leftQRD");
   LCD.print(" ");
   LCD.print(analogRead(leftTapeQRD));
   LCD.print("rightQRD");
   LCD.print(" ");
   LCD.print(analogRead(rightTapeQRD));
   LCD.setCursor(0,1); 
   LCD.print("Servo");
   LCD.print(" ");
   LCD.print(neutral+PIDAdjustment);
}

void followTape()
{
  bool leftSensor = (analogRead(leftTapeQRD) > leftThreshold);
  bool rightSensor = (analogRead(rightTapeQRD) > rightThreshold);
  
  if (leftSensor && rightSensor)
    error = 0;
  else if (leftSensor)
    error = 1;
  else if (rightSensor)
    error = -1;
  else
  {
    if (lasterror < 0)
      error = -5;
    else
      error = 5;
  }
  if (lasterror != error)
      lasterror = error;
  PIDAdjustment = (int)((float)kP * (float)error) + (int)((float)kD * (float)(error - lasterror));
  servoPosition = neutral+PIDAdjustment;
  if (servoPosition>180)
    servoPosition = 180;
  if (servoPosition<0)
    servoPosition = 0;
  RCServo1.write(servoPosition); 
}

  

