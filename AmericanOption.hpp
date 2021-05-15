#ifndef AmericanOption_HPP
#define AmericanOption_HPP

#include "Option.hpp"

class AmericanOption: public Option
{
private:
	double CallPrice(double U) const; // To overload PVF
	double PutPrice(double U) const; // To overload PVF
	double CallDelta(double U) const; // Calculate delta for price U
	double PutDelta(double U) const; // Calculate delta for price U
	double CallGamma(double U) const; // Calculate gamma for price U
	double PutGamma(double U) const; // Calculate gamma for price U
	std::vector<double> Regression(std::vector<std::vector<double>>& A, std::vector<double>& y) const;
public:

	AmericanOption(); // Default constructor
	AmericanOption(double NS, double NK, double NT, double Nr, double Nsig, double Nb, bool Ncalltype); // Parameter constructor
	AmericanOption(std::vector<double> data);
	AmericanOption(const AmericanOption& source); // Copy constructor 
	virtual ~AmericanOption(); // Destructor
	AmericanOption& operator =(const AmericanOption& source); // Assignment operator

	double Payoff(double U) const;
	double MC(double U, unint S_N, unint T_N) const;
	double BT(double U, unint height) const;
};


#endif