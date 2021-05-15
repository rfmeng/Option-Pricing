#ifndef Option_HPP
#define Option_HPP

#include <iostream>
#include <vector>

typedef unsigned int unint; //For convenience

class Option
{
private:
	virtual double CallPrice(double U) const = 0; // Pure virtual function, subclass must overload it
	virtual double PutPrice(double U) const = 0; // Pure virtual function, subclass must overload it
	//All the instance of the subclass can have approximate greeks functions,
	//because all of them have overloaded pricing functions.
	double ApprCallDelta(double h); // Approximate delta for call
	double ApprPutDelta(double h); // Approximate delta for call
	double ApprCallGamma(double h); // Approximate delta for call
	double ApprPutGamma(double h); // Approximate delta for call
public:
	double S; // Underlying asset price
	double K; // Exercise price
	double T; // Expiration time
	double r; // Risk-free rate
	double sig; // Sigma
	double b; // Cost of carry

	bool calltype; // Bool type, 1 for call and 0 for put

	Option(); // Default constructor

	Option(std::vector<double>& data);

	Option(const Option& source); // Copy constructor 
	Option(double NS, double NK, double NT, double Nr, double Nsig, double Nb, bool Ncalltype); // Parameter constructor
	virtual ~Option(); // Destructor

	Option& operator =(const Option& source); // Assignment operator

	virtual double Payoff(double U) const = 0;

	double Price() const; // Price function for underlying asset's price saved in the current object
	double Price(double U) const; // Price function for another price
	std::vector<double> Price(const std::vector<double>& v) const; // Pricer for vector
	std::string Type(); // Return "call" or "put", which depends on calltype.
	double ApprDelta(double h); // Approximate delta for current type
	double ApprGamma(double h); // Approximate Gamma for current type

};

#endif