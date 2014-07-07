#ifndef _SIGNAL_SMOOTH_H
#define _SIGNAL_SMOOTH_H

/**
This is the algorithm used to reduce noise in the IR signals as we move on the rocks.
**/

//input: a single original IR signal from circuit
//output: weighted average of input values
//purpose: smooths the IR data using Gaussian smooth
inline float signalSmooth(int originalSignal)
{
  int signalArray[7];
  // 1 3 6 7 6 3 1 smooth
  //increment old datapoints down array
  for(int i = 0; i < 6; i++)
  {
    signalArray[i+1] = signalArray[i];
  }
  
  // new data point
  signalArray[0] = originalSignal  ;
  
  //there is a delay of 3 function calls since the newest
  //measured point is given a weight of 1 then after 3 calculations
  //the max weight of 7
  
  return (float)(1*signalArray[0] + 3*signalArray[1] 
  + 6*signalArray[2] + 7*signalArray[3] + 6*signalArray[4] 
  + 3*signalArray[5] + 1*signalArray[6])/(27.0)  ;
}

#endif