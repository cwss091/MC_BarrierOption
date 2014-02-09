#ifndef _OPTIONPAY_H__
#define _OPTIONPAY_H__

/* Payoff of the option */

#include <iostream>
#include <vector>
//#include "BinomialTree.h"

class Payoff
{ 
	protected:
		double strike; //strike price
		bool call; // if call, be true
	public:
		Payoff(){}
		void setvalue(double _strike, bool _call); // setvalue
		virtual double operator() (double spot) = 0;
		~Payoff(){}
};

// Payoff of Vanilla Option
class VanillaPay: public Payoff
{ 
	public:
		double operator() (double spot);
};

class BarrierPutPay: public Payoff
{
	double barrier; // barrier price
	public:
	BarrierPutPay(double barrier_):barrier(barrier_){}
	double operator() (double spot){}
	double operator() (double* price, int size); // size is the price's size
	void sort(double* price, int l, int r);
};

#endif

