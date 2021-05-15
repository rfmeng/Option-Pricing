#include "PerpetualAmericanOption.hpp"
#include <cmath>



double PerpetualAmericanOption::CallPrice(double U) const
{
	double temp = (0.5 - b / (sig * sig));
	double y1 = temp + sqrt(temp * temp + 2 * r / (sig * sig));
	return K * pow((y1 - 1) * U / (K * y1), y1) / (y1 - 1);
}

double PerpetualAmericanOption::PutPrice(double U) const
{
	double temp = (0.5 - b / (sig * sig));
	double y2 = temp - sqrt(temp * temp + 2 * r / (sig * sig));
	return K * pow((y2 - 1) * U / (K * y2), y2) / (1 - y2);
}

PerpetualAmericanOption::PerpetualAmericanOption() :Option()
{
	T = 0; // T of parent class Option is useless
}

PerpetualAmericanOption::PerpetualAmericanOption(double NS, double NK, double Nr, double Nsig, double Nb, bool Ncalltype) : Option(NS, NK, 0, Nr, Nsig, Nb, Ncalltype)
{

}


PerpetualAmericanOption::PerpetualAmericanOption(std::vector<double> data)
{
	calltype = 1;
	S = data[0];
	K = data[1];
	T = data[2];
	r = data[3];
	sig = data[4];
	b = data[5];
}




PerpetualAmericanOption::PerpetualAmericanOption(const PerpetualAmericanOption& source) : Option(source)
{
	T = 0;
}

PerpetualAmericanOption::~PerpetualAmericanOption()
{

}

PerpetualAmericanOption& PerpetualAmericanOption::operator =(const PerpetualAmericanOption& source)
{
	if (this != &source)
		Option::operator=(source);
	return *this;
}

double PerpetualAmericanOption::Payoff(double U) const
{
	return U - K > 0 ? U - K : 0;
}
