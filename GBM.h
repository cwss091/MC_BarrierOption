#ifndef GBM_H
#define GBM_H

/** A generator of Geometric Brownian Motion samples
    is defined here.  The private members of the class
    contain the parameters that define the "state" of
    the GBM generator.  We assume that the random number
    generator (RNG) is genrand_std_normal(), found in 
    BoxMuller.h  One could make the RNG be an input 
    parameter to the constructor of the GBM class.
**/

#include <cmath>
#include <iostream>
#include "mt19937ar.h"
#include "BoxMuller.h"

class GBM{
   double T;
   double Spot;
   double Vol;
   double Rate;
   double variance;
   double rootVariance;
   double itoCorrection;
   double movedSpot;

  public:
   GBM(double t, double spot, double vol, double rate):T(t), Spot(spot), Vol(vol), Rate(rate)
   {
      variance = Vol*Vol*T;
      rootVariance = sqrt(variance);
      itoCorrection = -0.5*variance;
      movedSpot = Spot*exp(Rate*T + itoCorrection);
// Now, after all parameters are set, we can initialize the random number generator:
// We can let the seed be an array of length 4, like this:
      unsigned long init[4]={0x1234, 0x2345, 0x3456, 0x4567}, length=4;
      init_by_array(init, length); 

 // Or we could just initialize with a long integer by using:
//	  void init_genrand(unsigned long s);
   }

/** One might write a "better" constructor that takes a seed as a parameter.  Here's
 * a possible implementation start:
 *
   GBM(double t, double spot, double vol, double rate, unsigned long seed):T(t), Spot(spot), Vol(vol), Rate(rate) 
  {  ... code needed here ...}

 *
 **/

   inline double operator()(){
      double z = genrand_std_normal(); // gets the next Gaussian sample
      return movedSpot*exp(rootVariance*z); // GBM return the end of a GBM jump
   }

   void print(){
      std::cout << "Spot, Vol, Rate, Time = " << Spot << ", " << Vol << ", " << Rate << ", " << T << std::endl;
      std::cout << "movedSpot =  " << movedSpot << "  " << "rootVariance =  " << rootVariance << std::endl;
   }

/** NOTE: From this we can see that 
          thisSpot is movedSpot multiplied by a (pseudo)random number whose
          natural logarithm is normally distributed with mean 0 and
          variance Vol*sqrt(T);
**/

};
   
      
#endif


