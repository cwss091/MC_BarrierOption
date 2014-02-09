#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

#include "BSE.h"
#include "OptionPay.h"
#include "MCEngine.h"
#include "IO.h"

const double DofY = 365.0; // days of one year is 365

using namespace std;

double Average(vector<double> u)
{
	int size_u = u.size();
	vector<double>::iterator it = u.begin();
	double ave = 0;
	for(it = u.begin(); it < u.end(); it++)
	{
		ave += *it;
	}
	ave = ave / size_u;
	return ave;
}

vector<double> Square(vector<double> u)
{
	vector<double> v(u.size());
	vector<double>::iterator it = u.begin();
	vector<double>::iterator ip = v.begin();
	while(it != u.end())
	{
		*ip = (*it) * (*it);
		it++;
		ip++;
	}
	return v;
}

double Sample_Sigma(vector<double> u)
{
	vector<double> square_u = Square(u);
	double ave_u = Average(u);
	double ave_usq = Average(square_u);
	return sqrt(ave_usq - ave_u * ave_u);
}

/*double InvStdNormal(double u)
{
	const double A[] = {2.50662823884, -18.61500062529, 41.39119773534, -25.44106049637};

	const double B[] = {-8.47351093090, 23.08336743743, -21.06224101826, 3.13082909833};

	const double C[] = {0.3374754822726147, 0.9761690190917186, 
							   0.1607979714918209, 0.0276438810333863,
							   0.0038405729373609, 0.0003951896511919, 
							   0.0000321767881768, 0.0000002888167364, 
							   0.0000003960315187};
	double x = 0.0;
	double y = u-0.5;
	if(fabs(y) < 0.42){
		double r = y*y;
		x = y*(((A[3]*r + A[2])*r + A[1])*r + A[0])/( (((B[3]*r+B[2])*r+B[1])*r+B[0])*r +1);
	}
	else{
		double r = u;
		if(y>0) r = 1-u; // really y >= 0.42
		r = log(-log(r));
		x = C[0] + r*(C[1]+r*(C[2]+r*(C[3]+r*(C[4]+r*(C[5]+r*(C[6]+r*(C[7]+r*C[8])))))));
		if(y < 0) x = -x;
	}
	return x;
}*/

int main()
{ 
	ifstream InputIdx("./InputIndex.txt");
	if(!InputIdx)
	{
		cout << "No input index file, please set your input index file" << endl;
	}
	
	double spot; // spot price
	double d_expirate; // maturity days
	double strike; // strike price
	double barrier; // barrier
	bool iscall; // if call is true
	double rf_rate; // risk free interest rate
	double div_y; // dividended yield
	vector<int> divday(2); // dividend pays day
	double vol; // constant volatility

	input_data(spot, iscall, rf_rate, div_y, d_expirate, vol, strike, barrier, divday, InputIdx);
	int maturity_day = int(d_expirate); // maturity day (day)
	//cout << sizeof(maturity_day) << endl;
	d_expirate /= DofY; // maturity day (year)
	//divday[0] /= DofY;
	//divday[1] /= DofY;
	
	cout << "Initial underlying price is " << spot << endl;
	cout << "Risk free rate is " << rf_rate << endl;
	cout << "Strike price is " << strike << endl;
	cout << "Barrier price is " << barrier << endl;
	cout << "Dividend yield is " << div_y << endl;
	cout << "The constant volatility is " << vol << endl;
	cout << "The maturity is " << d_expirate << " years" << endl;
	cout << "The days when dividends pay are " ;
	showvector(divday);

	InputIdx.close();
	
	//BS formular to price European vanilla put option ignoring divdend
	BS_EuroOption BS_Euro(strike, rf_rate, 0.0, vol, d_expirate, iscall, 0);
	double Evp_N_BSP = BS_Euro(spot);
	cout << " The price of European Vanilla Option (no dividends) priced by Black-Scholes Formula is ";
	cout << Evp_N_BSP << endl;

	//Monte Carlo method (GBM) to price European vanilla put option ignoring divdend
	VanillaPay Evp_Pay;
	Evp_Pay.setvalue(strike, iscall);
	int N = 32000; //steps for Monte Carlo
	double discount  = exp(-rf_rate * d_expirate);
	GBM Si_gbm(rf_rate, vol, d_expirate); // generate the GBM 
	double Evp_N_MCP = MonteCarloPricer(N, Evp_Pay, Si_gbm, discount, spot); // price the European option
	cout << " The price of European Vanilla Option (no dividend) priced by Monte Carlo method is ";
	cout << Evp_N_MCP << endl;

	//Monte Carlo method (Halton numbers) to price European vanilla put option ignoring divdend
	HaltonGBM Si_hgbm(rf_rate, vol, d_expirate);
	double Evp_N_MHCP = MonteCarloPricer2(N, Evp_Pay, Si_hgbm, discount, spot);
	cout << " The price of European Vanilla Option (no dividends priced by Monte Carlo method with Halton numbers is ";
	cout << Evp_N_MHCP << endl;

	//Monte Carlo method (GBM) to price the barrier option ignoring dividend
	double dT = 1 / DofY; 
	GBM Smt_gbm(rf_rate, vol, dT);
	BarrierPutPay BP_Pay(barrier);
	BP_Pay.setvalue(strike, iscall);
	double Bp_N_MCP = MonteCarloPricer(N, BP_Pay, Smt_gbm, discount, spot, maturity_day);
	cout << " The price of barrier Option (no dividend) priced by Monte Carlo method is ";
	cout << Bp_N_MCP << endl;

	//Monte Carlo method (GBM) to price the barrier option with dividends
	vector<double> Bp_Y_MCP = MonteCarloPricer(N, BP_Pay, Smt_gbm, discount, spot, maturity_day, divday, div_y);
	double ave_BP_Y_MCP = Average(Bp_Y_MCP); // average of sample option prices
	double sigma_BP_Y_MCP = Sample_Sigma(Bp_Y_MCP); // std deviation of sample option prices
	double miu = 1.96 * sigma_BP_Y_MCP / sqrt(Bp_Y_MCP.size());
	double lowB = ave_BP_Y_MCP - miu;//95% confidence interval lower bound
	double upB = ave_BP_Y_MCP + miu; //95% confidence interval uper bound

	cout << "The price of barrier option under Market Scenario I with dividends is ";
	cout << ave_BP_Y_MCP << endl;
	cout << "The confidence interval of the result is " << "[" << lowB << " ," << upB << "]" << endl;


	//Monte Carlo method (Euler-Maruyama) to price the barrier option with fluctuant volatility ignoring dividends
	double vol0 = (30 - 0.6*(spot - 70.0)*exp(-d_expirate)) / 100.0;
	EulerMaruyama Si_EM(spot, rf_rate, vol0, dT, maturity_day);
	double Bp_N_MEMP = MonteCarloPricer_EM(N, BP_Pay, Si_EM, discount, spot, maturity_day);
	cout << " The price of barrier Option (no dividend) under Market Scenario II without dividends priced by monte carlo method is ";
	cout << Bp_N_MEMP << endl;

	//test the convergency of GBM and Halton Seris
	N = 10000;
	vector<double> gbm_price(N);
	vector<double> Halton_price(N);
	vector<double> Num_Simu(N);
	for(int i = 1; i <= N; i++)
	{
		gbm_price[i-1] = MonteCarloPricer(i, Evp_Pay, Si_gbm, discount, spot);
		Halton_price[i-1] = MonteCarloPricer2(i, Evp_Pay, Si_hgbm, discount, spot);
		Num_Simu[i-1] = i;
	}
	ofstream outfile("./Convergency.csv");
	outfile << "trials" << "," << "GBM" << "," << "Halton" << endl;
	print_vector(outfile, Num_Simu, gbm_price, Halton_price);
	outfile.close();

	return 0;
}

