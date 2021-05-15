#ifndef EuropeanOption_HPP
#define EuropeanOption_HPP

#include "Option.hpp"

class EuropeanOption :public Option
{
private:
	double CallPrice(double U) const; // To overload PVF
	double PutPrice(double U) const; // To overload PVF
	double CallDelta(double U) const; // Calculate delta for price U
	double PutDelta(double U) const; // Calculate delta for price U
	double CallGamma(double U) const; // Calculate gamma for price U
	double PutGamma(double U) const; // Calculate gamma for price U
	double n(double x) const; // Normal distribution pdf for convenience
	double N(double x) const; // Normal distribution cdf for convenience
public:


	EuropeanOption(); // Default constructor
	EuropeanOption(double NS, double NK, double NT, double Nr, double Nsig, double Nb, bool Ncalltype); // Parameter constructor

	EuropeanOption(std::vector<double> data);

	EuropeanOption(const EuropeanOption& source); // Copy constructor 
	virtual ~EuropeanOption(); // Destructor
	EuropeanOption& operator =(const EuropeanOption& source); // Assignment operator

	double Payoff(double U) const;
	double MC(double U, unint N) const;
	double FDM(double U, unint sample_N,unint T_N) const;
	double BT(double U, unint height) const;

	double Delta() const;
	double Delta(double U) const;
	std::vector<double> Delta(const std::vector<double>& v) const; // Calculate delta for a vector of price
	double Gamma() const;
	double Gamma(double U) const;
	std::vector<double> Gamma(const std::vector<double>& v) const; // Calculate gamma for a vector of price

	double Parity(double U, double option) const; // Put-call parity mechanism, version 1
	bool Parity(double U, double call, double put) const; // Put-call parity mechanism, version 2

};

#endif