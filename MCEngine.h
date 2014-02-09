/** An example of an option pricer implemented as a template function.
 * We allow as Payoff any function that takes a double and returns a double.
 * Also, we allow as our random number generator any function that takes no
 * argument and returns a double.  In both cases, we are trusting that the 
 * person who uses this implementation uses meaningful payoffs and random
 * number generators.
 *
 * It is clear that this implementation cannot be used to price options
 * that are path dependent since only simple payoff functions are allowed.
 * **************************/

#ifndef MCENGINE_H
#define MCENGINE_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "OptionPay.h"
#include "Simulator.h"


/* The entities Payoff and Simulator are parameters.  At compile time the compiler will first check the 
* validity of the actual values used and then translate this into machine code.
* Note that to implement a basic Monte Carlo pricer all you need are 
*    a payoff function 
*    a way to simulate the final prices
*    the number of simulations to perform
*    and (perhaps) the discount factor
*
* That's all we have here.
* **********************************/



template <typename Payoff, typename Simulator>
double MonteCarloPricer(int N, Payoff p, Simulator s, double discount)
{
   if(N <= 0){ std::cerr << "\n\t illegal number of simulations "<< std::endl; exit(1);}
   double sum = 0.0;
   for(int i = 0; i< N; ++i){ double z = s(); sum += p(z); } 
   return discount*sum/N;
}

//for EM
template <typename Payoff, typename Simulator>
double MonteCarloPricer_EM(int N, Payoff p, Simulator s, double discount, double spot, int maturity)
{
   if(N <= 0){ std::cerr << "\n\t illegal number of simulations "<< std::endl; exit(1);}
   double sum = 0.0;
   for(int i = 0; i< N; ++i){ double* z = s(30.0); sum += p(z, maturity); delete [] z; } 
   return discount*sum/N;
}


//for GBM
template <typename Payoff, typename Simulator>
double MonteCarloPricer(int N, Payoff p, Simulator s, double discount, double spot)
{
   if(N <= 0){ std::cerr << "\n\t illegal number of simulations "<< std::endl; exit(1);}
   double sum = 0.0;
   for(int i = 0; i< N; ++i){ double z = s(); sum += p(z*spot); } 
   return discount*sum/N;
}

//MC Engine for barrier option without dividends
template <typename Payoff, typename Simulator>
double MonteCarloPricer(int N, Payoff p, Simulator s, double discount, double spot, int maturity)
{
	if(N <= 0){ std::cerr << "\n\t illegal number of simulations "<< std::endl; exit(1);}
	double sum = 0.0;
	for(int i = 0; i< N; ++i)
	{   
		double spot_temp = spot; // initial each rountine's initial spot price
		double* price = new double[maturity]; // set a dynamic array to store routine price
		for(int j = 1; j <= maturity; j++)
		{
			double z = s(); // GBM exponent part
			spot_temp *= z;
			//std::cout << spot_temp << " ";
			price[j-1] = spot_temp;
		}
		//std::cout << std::endl;
		sum += p(price, maturity); 
		delete[] price; // release the memory
	} 
	return discount*sum/N; // the price of option
}

//for barrier option with dividends
template <typename Payoff, typename Simulator>
std::vector<double> MonteCarloPricer(int N, Payoff p, Simulator s, double discount, double spot, int maturity, std::vector<int> divday, double div)
{
	if(N <= 0){ std::cerr << "\n\t illegal number of simulations "<< std::endl; exit(1);}
	double sum = 0.0;
	std::vector<double> Oprice(N);
	for(int i = 0; i< N; ++i)
	{   
		double spot_temp = spot;
		double* price = new double[maturity];
		for(int j = 1; j <= maturity; j++)
		{
			double z = s();
			spot_temp *= z;
			std::vector<int>::iterator it;
			it = find(divday.begin(), divday.end(), j);
			if(it != divday.end())
			{
				spot_temp -= div;
			}
			//std::cout << spot_temp << " ";
			price[j-1] = spot_temp;
		}
		//std::cout << std::endl;
		 Oprice[i] = discount * p(price, maturity);
		delete[] price;
	} 
	//return discount*sum/N;
	return Oprice;
}


// for Halton series
template <typename Payoff, typename Simulator>
double MonteCarloPricer2(int N, Payoff p, Simulator s, double discount, double spot)
{
   if(N <= 0){ std::cerr << "\n\t illegal number of simulations "<< std::endl; exit(1);}
   double sum = 0.0;
   for(int i = 1; i< N; ++i){ double z = s(i); sum += p(z*spot); } 
   return discount*sum/N;
}

#endif
