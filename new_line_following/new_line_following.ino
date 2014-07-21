#include <phys253.h>        		//***** from 253 template file
#include <LiquidCrystal.h>  		//***** from 253 template file
#include <servo253.h>       		//***** from 253 template file 

// Tape following Pins
int leftTapeQRD = 3;
int rightTapeQRD = 2;

int leftThreshold = 400;
int rightThreshold = 130;

// Motor Speed Pins
int rightWheelPin = 0;
int leftWheelPin = 1;

//Knobs
int knob_one = 6;
int knob_two = 7;

//PID parameters
int kP = 100;
int kD = 20;

//correction errors (negative is too far left, positive is too far right)
int error = 0; 
int lasterror = 0;
int preverrorDuration = 0;
int timeSinceDiffErr = 1;
int PIDAdjustment = 0;

//wheel controll
int motorSpeed = 150;
int sharpTurnSpeed = 100;
int neutral = 106;
int servoPosition = 90;

//debugging
int counter = 1;
//Including Outside Files

void setup() 
{
  //to prevent fuse from blowing
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
        LCD.print("motorSpeed");
        LCD.print(" ");
        LCD.print(knob(knob_two));
        motorSpeed = knob(knob_two);
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
  
  if (leftSensor && rightSensor) // on tape
  {
    error = 0;
    //set servo and motors to regular values
    RCServo1.write(neutral);
    motor.speed(rightWheelPin, motorSpeed) ;
    motor.speed(leftWheelPin, motorSpeed) ;
  }
  else if (leftSensor) //too far right
  {
    error = 1;
    //servo calculation
    PIDAdjustment = (int)((float)kP * (float)error) + (int)((float)kD * (float)(error - lasterror) / (float)(preverrorDuration + timeSinceDiffErr) );
    servoPosition = neutral+PIDAdjustment;
    if (servoPosition>180)
      servoPosition = 180;
    else if (servoPosition<0)
      servoPosition = 0;
    RCServo1.write(servoPosition);
    //regular motor speed for small errors
    motor.speed(rightWheelPin, motorSpeed) ;
    motor.speed(leftWheelPin, motorSpeed) ;
  }
  else if (rightSensor) //too far left
  {
    error = -1;
    //servo calculation
    PIDAdjustment = (int)((float)kP * (float)error) + (int)((float)kD * (float)(error - lasterror) / (float)(preverrorDuration + timeSinceDiffErr) );
    servoPosition = neutral+PIDAdjustment;
    if (servoPosition>180)
      servoPosition = 180;
    else if (servoPosition<0)
      servoPosition = 0;
    RCServo1.write(servoPosition);
    //regular motor speed for small errors
    motor.speed(rightWheelPin, motorSpeed) ;
    motor.speed(leftWheelPin, motorSpeed) ;
  }
  else
  {
    if (lasterror < 0) //way too far left
    {
      error = -5;
      //servo calculation
      PIDAdjustment = (int)((float)kP * (float)error) + (int)((float)kD * (float)(error - lasterror) / (float)(preverrorDuration + timeSinceDiffErr) );
      servoPosition = neutral+PIDAdjustment;
      RCServo1.write(servoPosition);
      if (servoPosition>180)
        servoPosition = 180;
      else if (servoPosition<0)
        servoPosition = 0;
      //motor calculation
      PIDMotorAdjustment = (int)((float)kP * (float)error) + (int)((float)kD * (float)(error - lasterror) / (float)(preverrorDuration + timeSinceDiffErr) );
      motor.speed(rightWheelPin, motorSpeed-PIDMotorAdjustment) ;
      motor.speed(leftWheelPin, motorSpeed+PIDMotorAdjustment) ;
    }
    else // way too far right
    {
      error = 5;
      //servo calculation
      PIDAdjustment = (int)((float)kP * (float)error) + (int)((float)kD * (float)(error - lasterror) / (float)(preverrorDuration + timeSinceDiffErr) );
      servoPosition = neutral+PIDAdjustment;
      if (servoPosition>180)
        servoPosition = 180;
      else if (servoPosition<0)
        servoPosition = 0;
      RCServo1.write(servoPosition);
      //motor calculation
      PIDMotorAdjustment = (int)((float)kP * (float)error) + (int)((float)kD * (float)(error - lasterror) / (float)(preverrorDuration + timeSinceDiffErr) );
      motor.speed(rightWheelPin, motorSpeed-PIDMotorAdjustment) ;
      motor.speed(leftWheelPin, motorSpeed+PIDMotorAdjustment) ;
    }
  }
  if (lasterror != error)
  {
    lasterror = error;   
    preverrorDuration = timeSinceDiffErr;
    timeSinceDiffErr = 1;
  }
  timeSinceDiffErr = timeSinceDiffErr + 1;

  

