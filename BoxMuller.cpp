#include <math.h>
#include "mt19937ar.h"  // we use a function from mt19937ar.c below
#include "BoxMuller.h"  // has the definitions of the functions and constants used in BoxMuller.c


double genrand_std_normal(void){

   static int iset = 0;  //retain values between calls
   static double gset;

   double v1, v2, radsqr, weight;

/* ****************************************************************************
 * Box-Muller-Marsaglia algorithm with genrand_real3().  
 * Other uniform generators are possible.
 * The algorithm generates a pair of numbers, returns one of them and saves 
 * the other for the next call to this function.  
 * The designation of the variables gset and iset as static makes their
 * values perist from call to call to this function . . . whereas 
 * the non-static variables v1, v2 do not have to hold their value from call to call.
 ******************************************************************************/
    if(iset ==0 ){
	do{
	    v1 = 2.0*genrand_real3() - 1.0; /* generate a uniform random number */
	    v2 = 2.0*genrand_real3() - 1.0; /* generate another uniform random number */
	    radsqr = v1*v1 + v2*v2;  /* calculate square of the radius of the pair(v1,v2) */
	} while (radsqr >=1 || radsqr == 0); /* until is (v1, v2) inside unit disk, but not the origin */
    
    	weight = sqrt(-2.0*log(radsqr)/radsqr);
    	gset = v1*weight;  /* save this one for next time */
    	iset = 1;   /* set the flag so that next time the "if" condition will be false */
	return v2*weight;
    } 
    else{ /* if iset was set=1 on the last call to this function */
    	iset = 0;  /* clear iset */
	return gset; /* return the value save on the last call to this function */
    }   
}












