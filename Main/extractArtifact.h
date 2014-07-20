#ifndef _EXTRACT_ARTIFACT_H
#define _EXTRACT_ARTIFACT_H

/**
	This is the function called to put up the artifact and place it into our bin.
	It works as follows:
	1. Drop fishing line to height of idol when we detect the idol
	2. Pull the line to height appropriate for arm rotation
	3. Rotate arm
	4. Pull line in further to drop idol
	5. Rotate back to starting position
	6. Drop line to starting height
**/


//input:
//output:
//function calls:
//purpose:
inline void extractArtifact()
{
	//physical parameters
    float spoolCircumference = 25.5  ;
	float gearCircumference = 40.0  ;
	float encoderCircumference = 20.0  ;
	float constant = 24.0*(gearCircumference/(spoolCircumference*encoderCircumference))  ;
	/**the logic is as follows
		I. The spool needs to travel distance/spoolCircumference number of revolutions
		II. This number of revolutions makes the gear travel gearCircumference*revolutions distance
		III. This gear distance is equal to the encoder's gear distance. Which means the encoder travels distance/encoderCircumference
			 number of revolutions
		IV. There are 24 flips of voltages in each encoder revolution.
	**/
	
	//lifting parameters
	int dropSpeed	= 500  ;
	int liftSpeed = -500  ;
	int x0 = 0  ;
	int binPosition = 90  ;
	int timeDelay = 50  ;
	

	//distance parameters
	float dropD = 100  ;
	float peelD = 200 ;
	
	/**
		1.
	**/
	int flips = (int)(dropD*constant)  ;
	int counter = 0  ;
	int oldState = 0  ;
	int currentState = 0  ;
	while (counter<flips)
	{
		motor.speed(idolMotor, dropSpeed)  ;
		currentState = digitalRead(retrievalEncoder)  ;
		if( !(oldState == currentState))
		{
	      counter = counter + 1  ;
		}
		delay(timeDelay)  ;
	}
	/**
		2.
		same drop distance
		same number of flips
	**/
	counter = 0  ;
    while (counter<flips)
	{
		motor.speed(idolMotor, liftSpeed)  ;
		currentState = digitalRead(retrievalEncoder)  ;
		if( !(oldState == currentState))
		{
	      counter = counter + 1  ;
		}
		delay(timeDelay)  ;
	}
	/**
		3.
	**/
	RCServo1.write(binPosition)  ; 
	
    /**
		4.
	**/
	int peelFlips = (int)(peelD * constant)  ;
	counter = 0  ;
    while (counter<peelFlips)
	{
		motor.speed(idolMotor, liftSpeed)  ;
		currentState = digitalRead(retrievalEncoder)  ;
		if( !(oldState == currentState))
		{
	      counter = counter + 1  ;
		}		
		delay(timeDelay)  ;
	}
    /**
		5.
	**/
	RCServo1.write(x0)  ;
	/**
		6.
	**/
	counter = 0  ;
    while (counter<peelFlips)
	{
		motor.speed(idolMotor, dropSpeed)  ;
		currentState = digitalRead(retrievalEncoder)  ;
		if( !(oldState == currentState))
		{
	      counter = counter + 1  ;
		}
		delay(timeDelay)  ;		
	}
}

#endif