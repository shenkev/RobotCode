#ifndef _AVOID_CLIFF_H
#define _AVOID_CLIFF_H

/**
This is the function called to stop us from going over the cliff.
It should involve going in reverse for a period of time or simply stopping.
**/

//input:
//output:
//function calls:
//purpose:
inline void avoidCliff()
{
	int reverseSpeed = -500  ;
	int serDir = 90  ;
	drive(reverseSpeed, reverseSpeed, serDir)  ;
}

#endif
