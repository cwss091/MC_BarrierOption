/** These are the functions declared in IO.h. **/

#include "IO.h"

using namespace std;

void input_data(double& spot, bool& iscall, double& rate, double& div_yield, double& expiry, double& volatility, double& strike, double& bar, std::vector<int>& div_day, std::ifstream& infile){
	infile >> spot; // spot price
	infile >> iscall; // if call is true
    infile >> rate; // risk free rate
    infile >> div_yield; // dividend yield
    infile >> expiry; // expire date
	infile >> volatility; // volatility
	infile >> strike; // strike price
	infile >> bar; // barrier

	int sizes = div_day.size();
	double temp_str;
	for(int i = 0; i < sizes; i++)
	{  
		infile >> temp_str; // days when divended pays 
		div_day[i] = temp_str;
	}
}

void print_vector(ofstream& outfile, vector<double> v)
{
    vector<double>::iterator it = v.begin();
	while(it != v.end()) outfile << *it++ << ",";
	cout << endl;
	cout << endl;
}

void print_vector(ofstream& outfile, vector<double> u, vector<double> v)
{
	vector<double>::iterator it = v.begin();
    vector<double>::iterator ip = u.begin();
    while(it != v.end()) outfile << *ip++ << "," << *it++ << endl;
}

void print_vector(ofstream& outfile, vector<double> u, vector<double> v, vector<double> w)
{
	for(int i = 0; i < u.size(); i++)
	{
		outfile << u[i] << "," << v[i] << "," << w[i] << endl;
	}
}

void print_vector(ofstream& outfile, vector<double> u, vector<double> v, vector<vector<double> > x)
{ 
	int usize = u.size();
	int vsize = v.size();
	for(int i = 0; i < usize; i++)
	{ 
		for(int j = 0; j < vsize; j++)
		{ 
			outfile << u[i] << "," << v[j] << "," << x[i][j] << endl;
		}
	}
}



