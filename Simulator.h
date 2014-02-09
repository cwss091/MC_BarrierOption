/* This Simulator cited from Prof. Demko's Simulator.h
 * I have changed the class Halton, HaltonGBM, EulerMarayama
 * This file generates random number and simulates stochastic process
 */


#include <cmath>
#include <ctime>
#include <string>
#include "mt19937ar.h"
#include "BoxMuller.h"

class Simulator{
public:
  virtual double operator()(void)= 0; // argument gives step size
  virtual void Seed(unsigned long int) = 0;  // initializes RNG
  virtual void Seed() = 0;
  virtual void Skip(int) = 0; //  goes through several simulations;
};

class Mersenne:public Simulator{
  public:
    Mersenne(){Seed();}
    Mersenne(unsigned long s){Seed(s);}
    void Seed(unsigned long s){ init_genrand(s);}
    void Seed(){ init_genrand(0x13579bdf);}
    void Skip(int n){ for(int i =0; i< n; ++i) genrand_real3();}
    double operator()(){return genrand_real3();}
};

class Box_Muller:public Mersenne{
public:
    Box_Muller():Mersenne(){}
    Box_Muller(unsigned long s):Mersenne(s){}
    double operator()(){return genrand_std_normal();}
};

class GBM:Box_Muller{
    double drift;
    double sigma;
    double dT;
    double Ito;
    double variance;
public:
    GBM(double r, double s, double dt):Box_Muller(), drift(r), sigma(s), dT(dt)
    {
        Ito = exp( (drift - 0.5*sigma*sigma)*dT);
        variance = sigma*sqrt(dT);
    }
    
    GBM(unsigned long s, double r, double sig, double dt):Box_Muller(s), drift(r), sigma(sig), dT(dt)
    { 
        Ito = exp( (drift - 0.5*sigma*sigma)*dT);
        variance = sigma*sqrt(dT);
    }
    
    double operator()(double T){ 
        double It = exp( (drift - 0.5*sigma*sigma)*T);
        double var = sigma*sqrt(T);
        double z = genrand_std_normal();
        return It*exp(z*var);
    }
    
    double operator()(){ 
        double z = genrand_std_normal();
        return Ito*exp(z*variance);
    }
};


class Halton{
    unsigned int base;
    // maybe more
public:
    Halton()
	{
		Seed();
	} // will generate Halton numbers from algorithm
    Halton(std::string filename, unsigned int toRead ){}// get numbers from a file
	double operator()(int N)
	{
		//this function's copyright belongs to Prof. Demko
		if(N == 1) return 1.0 / N;
		if(N < base) return (N % base) / (double)(base);
		int q = N;
		double power = 1.0;
		double return_value = 0.0;
		while(q > base - 1)
		{
			double r = q % base;
			return_value += r / pow(base, power);
			q = q / base;
			power += 1.0;
		}
		return_value += q / (double)pow(base, power);
		return return_value;
	} // argument gives step size
    void Seed(long int n){ base = n;}  //
    void Seed(){base = 2;}
    void Skip(int n){} // n is the index;
	//friend class HaltonNormal;
};

class HaltonGBM{
    double drift;
    double sigma;
    double dT;
    double Ito;
    double variance;
	unsigned int base;

public:
	HaltonGBM(double r, double s, double dt):drift(r), sigma(s), dT(dt)
	{
		Ito = exp( (drift - 0.5*sigma*sigma)*dT);
        variance = sigma*sqrt(dT);
		Seed();
	}
	double operator()(int N)
	{
		//this function's copyright belongs to Prof. Demko
		if(N == 1) return 1.0 / N;
		if(N < base) return (N % base) / (double)(base);
		int q = N;
		double power = 1.0;
		double return_value = 0.0;
		while(q > base - 1)
		{
			double r = q % base;
			return_value += r / pow(base, power);
			q = q / base;
			power += 1.0;
		}
		return_value += q / (double)pow(base, power);
		return_value = InvStdNormal(return_value);
		return Ito*exp(return_value * variance);
	}// argument gives step size
	double InvStdNormal(double u)
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
	}

	void Seed(long int base){}  // initializes RNG
	void Seed(){base = 2;}
	void Skip(int ) ; //  goes through several simulations;
};

class EulerMaruyama:public Box_Muller{
    double spot;
    double drift;
    double sigma;
    double dT;
    int NumSteps;
    double det;
    double var;
public:
    EulerMaruyama(double sp, double d, double s, double dt, int n):Box_Muller(), spot(sp),drift(d), sigma(s), dT(dt), NumSteps(n){ det = dT*drift; var = sigma*sqrt(dT);}
    
    EulerMaruyama(double sp,double d, double s, double dt, int n, unsigned long seed):Box_Muller(seed), spot(sp), drift(d), sigma(s), dT(dt), NumSteps(n){ det = dT*drift; var = sigma*sqrt(dT);}
    
	//double operator()(){}
    double* operator()(double intecp){ 
        double x = spot;
		double *price = new double [NumSteps];
        for(int i = 0;  i< NumSteps; ++i){
            double z = genrand_std_normal();
            x *= 1+ det + var*z;
			price[i] = x;
			sigma = (intecp - 0.6 * (x - 70.0) * exp((-i-1)/365.0)) / 100.0;
			var = sigma * sqrt(dT);
        }
		//for(int i = 0; i < NumSteps;i++)
		//	std::cout << price[i] << " ";
		//std::cout << std::endl;
        return price;
     }
};
