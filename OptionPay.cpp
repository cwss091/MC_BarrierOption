#include "OptionPay.h"

/* Payoff of the option */

using namespace std;

void Payoff::setvalue(double _strike, bool _call)
{
	strike = _strike;
	call = _call;
}

double VanillaPay::operator() (double spot) // payoff of vanilla option
{
	if(call == true)
	{
		return (spot - strike > 0.0)?(spot - strike):0.0;
	}
	else
	{
		return (strike - spot > 0.0)?(strike - spot):0.0;
	}
}

double BarrierPutPay::operator() (double* price, int size) // arguments are spot price and teh array's size
{
	double temp = price[size-1];
	sort(price, 0, size-1);
	if(price[0] <= barrier) // if down and in the barrier, the option take effect
	{ 
		return (strike - temp > 0.0)?(strike - temp):0.0;
	}
	else
	{ 
		return 0.0;
	}
}

// quick sort to order the price from lowest to highest
void BarrierPutPay::sort(double* price, int l, int r) 
{
	if(l < r)
	{
		int i = r,j = r+1;
		double pivot = price[l];
		double temp;
		for(i = r; i >l; i --)
		{
			if(price[i] >= pivot)
			{
				j--;
				if(i != j)
				{
					temp = price[j];
					price[j] = price[i];
					price[i] = temp;
				}
			}
		}
		temp = price[j-1];
		price[j-1] = pivot;
		price[l] = temp;
		sort(price, l, j-1);
		sort(price, j+1, r);
	}
}

	
