#ifndef PerpetualAmericanOption_HPP
#define PerpetualAmericanOption_HPP

#include "Option.hpp"

class PerpetualAmericanOption :public Option
{
private:
	double CallPrice(double U) const; // To overload PVF
	double PutPrice(double U) const; // To overload PVF
public:
	//Time T of the parent class Option is useless for PerpetualAmericanOption,
	//so just make it 0.
	PerpetualAmericanOption(); // Default constructor 
	PerpetualAmericanOption(double NS, double NK, double Nr, double Nsig, double Nb, bool Ncalltype); // Parameter constructor, without time T

	PerpetualAmericanOption(std::vector<double> data);

	PerpetualAmericanOption(const PerpetualAmericanOption& source); // Copy constructor 
	virtual ~PerpetualAmericanOption(); // Destructor
	PerpetualAmericanOption& operator =(const PerpetualAmericanOption& source); // Assignment operator
	double Payoff(double U) const;
};

#endif