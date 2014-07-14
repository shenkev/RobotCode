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
	int dropSpeed	= 500  ;
	int liftSpeed = -500  ;
	int x0 = 0  ;
	int binPosition = 90  ;
	
	//can either use an encoder to keep track of how much line we let out or just guess.
	//currently implemented as guessing
	/**
		1.
	**/
	int maxTime = 500  ;
	int timeInterval = 10  ;
	for (int time=0; time<maxTime; time=time+timeInterval)
	{
		motor.speed(idolMotor, dropSpeed)  ;
		delay(timeInterval)  ;
	}
	/**
		2.
	**/
	for (int time=0; time<maxTime; time=time+timeInterval)
	{
		motor.speed(idolMotor, liftSpeed)  ;
		delay(timeInterval)  ;
	}
	/**
		3.
	**/
	RCServo1.write(binPosition)  ; 
    /**
		4.
	**/
	int maxTime2 = 200  ;
	for (int time=0; time<maxTime2; time=time+timeInterval)
	{
		motor.speed(idolMotor, liftSpeed)  ;
		delay(timeInterval)  ;
	}
    /**
		5.
	**/
	RCServo1.write(x0)  ;
	/**
		6.
	**/
	for (int time=0; time<maxTime2; time=time+timeInterval)
	{
		motor.speed(idolMotor, dropSpeed)  ;
		delay(timeInterval)  ;
	}
}

#endif