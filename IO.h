#ifndef _IO_H__
#define _IO_H__

/* IO file input and out data from program */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

// input the parameters
void input_data(double& spot, bool& iscall, double& rate, double& div_yield, double& expiry, double& volatility, double& strike, double& bar, std::vector<int>& div_day, std::ifstream& infile);

// print one vector
void print_vector(std::ofstream& outfile, std::vector<double> v);

// print two vectors simultaneously
void print_vector(std::ofstream& outfile, std::vector<double> u, std::vector<double> v);

// print three vectors simultaneously
void print_vector(std::ofstream& outfile, std::vector<double> u, std::vector<double> v, std::vector<double> w);

// print two vector and one vector<vector> simultaneously
void print_vector(std::ofstream& outfile, std::vector<double> u, std::vector<double> v, std::vector<std::vector<double> > x);

// show vector on console
template<typename T>
void showvector(std::vector<T>& v)
{ 
	typename std::vector<T>::iterator it = v.begin();
	while(it != v.end()) 
	{ 
		std::cout << *it << " ";
		it++;
	}
	std::cout << std::endl;
}

#endif
