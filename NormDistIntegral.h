#ifndef _NORMDISTINTEGRAL_H__
#define _NORMDISTINTEGRAL_H__

/* Generate Normal Distribution */

#include <iostream>
#include <cmath>

const double PI = 3.1415926;

class MyNormDist
{
	double mean; // the mean of Normal Distribution
	double std_v; // standard deviation
	public:
	MyNormDist(double mean_, double std_v_);
	~MyNormDist();
	double Norm_pdf(double x); // probability density function
	double Norm_cdf(double x); // cumulative distribution function
	double Norm_igral(const double& low_b, const double& up_b); // integral
};

#endif
