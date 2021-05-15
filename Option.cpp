#include "Option.hpp"


double Option::ApprCallDelta(double h)
{
	return(CallPrice(S + h) - CallPrice(S - h)) / (2 * h);
}

double Option::ApprPutDelta(double h)
{
	return(PutPrice(S + h) - PutPrice(S - h)) / (2 * h);
}

double Option::ApprCallGamma(double h)
{
	return(CallPrice(S + h) + CallPrice(S - h) - 2 * CallPrice(S)) / (h * h);
}

double Option::ApprPutGamma(double h)
{
	return(PutPrice(S + h) + PutPrice(S - h) - 2 * PutPrice(S)) / (h * h);
}

Option::Option() :S(60), T(0.25), K(65), sig(0.30), r(0.08), b(0.08), calltype(1)
// Default constructor 
{

}


Option::Option(std::vector<double>& data)
{
	calltype = 1;
	S = data[0];
	K = data[1];
	T = data[2];
	r = data[3];
	sig = data[4];
	b = data[5];
}






Option::Option(const Option& source) :S(source.S), T(source.T), K(source.K), sig(source.sig), r(source.r), b(source.b), calltype(source.calltype)
// Copy constructor 
{

}

Option::Option(double NS, double NK, double NT, double Nr, double Nsig, double Nb, bool Ncalltype) :S(NS), T(NT), K(NK), sig(Nsig), r(Nr), calltype(Ncalltype), b(Nb)
//Parameter constructor
{

}

Option::~Option()
//Destructor
{

}

Option& Option::operator =(const Option& source)
//Assignment operator
{
	if (this != &source) // Self-assignment check
	{
		S = source.S;
		T = source.T;
		sig = source.sig;
		r = source.r;
		K = source.K;
		calltype = source.calltype;
	}
	return *this;
}


double Option::Price() const
{
	if (calltype)
		return CallPrice(S);
	else
		return PutPrice(S);
}

double Option::Price(double U) const
{
	if (calltype)
		return CallPrice(U);
	else
		return PutPrice(U);
}

std::vector<double> Option::Price(const std::vector<double>& v) const
{
	std::vector<double> temp; // Create a vector to return
	for (unint i = 0;i < v.size();i++)
		temp.push_back(Price(v[i])); // Save the corresponding value
	return temp;
}

std::string Option::Type() // To give a string for prompt
{
	if (calltype)
		return "call";
	else
		return "put";
}

double Option::ApprDelta(double h)
{
	if (calltype)
		return ApprCallDelta(h);
	else
		return ApprPutDelta(h);
}

double Option::ApprGamma(double h)
{
	if (calltype)
		return ApprCallGamma(h);
	else
		return ApprPutGamma(h);
}