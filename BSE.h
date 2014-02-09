#ifndef _BSE_H__
#define _BSE_H__

/* Price option based on Black Scholes Model */
#include <iostream>
#include "NormDistIntegral.h"
#include <cmath>
#include <utility>


class BS_Option
{
	protected:
		double strike; // strike price
		double rf_rate; // risk-free rate
		double div_yield; // diveidend yield
		double vol; // volatility
		double expiry; // expire date
		double t; // current time
		bool call; // true if all option 
		double d1,d2; 
	public:
		BS_Option();
		std::pair<double,double> d_value(double price); // d1 and d2 calculated
		~BS_Option();
};

// Euro-Option pricing by BS Model 
class BS_EuroOption: public BS_Option
{ 
	public:
	BS_EuroOption(const double& strike_, const double& rf_rate_, const double& div_yield_, const double& vol_, const double& expiry_, const bool& call_, const double& time_);
	// price the option
	double operator() (double price);
	~BS_EuroOption();
};

// Digital-Option pring by BS Model
class BS_DigiOption: public BS_Option
{
	public:
	BS_DigiOption(const double& strike_, const double& rf_rate_, const double& div_yield_, const double& vol_, const double& expiry_, const bool& call_, const double& time_);
	//: strike(strike_), rf_rate(rf_rate_), div_yield(div_yield_), vol(vol_), expiry(expiry_), call(call_), time(time_){}
	double operator() (double price);
	~BS_DigiOption();
};

#endif
