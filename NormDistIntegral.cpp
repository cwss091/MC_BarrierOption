#include "NormDistIntegral.h"

using namespace std;

MyNormDist::MyNormDist(double mean_, double std_v_)
{
	mean = mean_;
	std_v = std_v_;
}

MyNormDist::~MyNormDist(){}

// probability density function
double MyNormDist::Norm_pdf(double x)
{
	double ND_f = 1.0 / (std_v * sqrt(2.0*PI)) * exp(-pow((x-mean), 2.0) / (2.0*pow(std_v, 2.0))); 
	return ND_f;
}
// cumulative distribution function
double MyNormDist::Norm_cdf(double x)
{
	double inf = 8.0; // the up bound of integral
	double h = 1.0 / 200; // the length of each step
	double x_0 = abs(x);
	double Nd_cdf = 0; // Simpon's rule
	for(double i=x_0; i<=inf; i=i+h)
	{
		double temp = Norm_pdf(i) + 4*Norm_pdf(i+0.5*h) + Norm_pdf(i+h);
		Nd_cdf += temp * h / 6;
	}
	if(x > 0)
	{
		Nd_cdf = 1.0 - Nd_cdf;
	}
	return Nd_cdf;
}

double MyNormDist::Norm_igral(const double& low_b, const double& up_b)
{
	return Norm_cdf(up_b) - Norm_cdf(low_b);
}

