#include "BSE.h"

using namespace std;

MyNormDist StdNormal(0.0, 1.0);

pair<double, double> BS_Option::d_value(double price) // calculate the d
{
	d1 = log(price / strike) + (rf_rate - div_yield + pow(vol, 2.0) / 2) * (expiry - t); 
	d1 = d1 / (vol * sqrt(expiry - t));
	d2 = d1 - vol * sqrt(expiry - t);
	pair<double, double> d (d1, d2);
	return d;
}

BS_Option::BS_Option(){}

BS_Option::~BS_Option(){}

// generate euro option
BS_EuroOption::BS_EuroOption(const double& strike_, const double& rf_rate_, const double& div_yield_, const double& vol_, const double& expiry_, const bool& call_, const double& time_)
{
	strike = strike_; // strike price
	rf_rate = rf_rate_; // risk free rate
	div_yield = div_yield_; // dividend yield
	vol = vol_; // volatility
	expiry = expiry_; // expire date
	call = call_; // whether is call option
	t = time_; // current time
}

BS_EuroOption::~BS_EuroOption(){}

//price the euro option
double BS_EuroOption::operator() (double price)
{
	d1 = log(price / strike) + (rf_rate - div_yield + pow(vol, 2.0) / 2) * (expiry - t); 
	d1 = d1 / (vol * sqrt(expiry - t));
	d2 = d1 - vol * sqrt(expiry - t);
	if(call == true)
	{
		return price * exp(-div_yield * (expiry-t)) * StdNormal.Norm_cdf(d1) - strike * exp(-rf_rate * (expiry-t)) * StdNormal.Norm_cdf(d2);
	}
	else
	{
		return -price * exp(-div_yield * (expiry-t)) * StdNormal.Norm_cdf(-d1) + strike * exp(-rf_rate * (expiry-t)) * StdNormal.Norm_cdf(-d2);
	}
}

// generate digital option
BS_DigiOption::BS_DigiOption(const double& strike_, const double& rf_rate_, const double& div_yield_, const double& vol_, const double& expiry_, const bool& call_, const double& time_)
{
	strike = strike_;
	rf_rate = rf_rate_;
	div_yield = div_yield_;
	vol = vol_;
	expiry = expiry_;
	call = call_;
	t = time_;
}

BS_DigiOption::~BS_DigiOption(){}

//price the digital option
double BS_DigiOption::operator() (double price)
{
	d1 = log(price / strike) + (rf_rate - div_yield + pow(vol, 2.0) / 2) * (expiry - t); 
	d1 = d1 / (vol * sqrt(expiry - t));
	d2 = d1 - vol * sqrt(expiry - t);
	if(call == true)
	{
		return exp(-rf_rate * (expiry-t)) * StdNormal.Norm_cdf(d2);
	}
	else
	{
		return exp(-rf_rate * (expiry-t)) * StdNormal.Norm_cdf(-d2);
	}
}
