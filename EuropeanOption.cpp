#include "EuropeanOption.hpp"
#include <boost/math/distributions/normal.hpp>
#include <iostream>
#include <boost/random.hpp>
#include "GlobalFunction.hpp"


double EuropeanOption::CallPrice(double U) const
{
	double temp = sig * std::sqrt(T);
	double d1 = (std::log(U / K) + (b + (sig * sig) * 0.5) * T) / temp;
	double d2 = d1 - temp;
	return (U * std::exp((b - r) * T) * N(d1) - K * std::exp(-r * T) * N(d2));
}

double EuropeanOption::PutPrice(double U) const
{
	double temp = sig * std::sqrt(T);
	double d1 = (std::log(U / K) + (b + (sig * sig) * 0.5) * T) / temp;
	double d2 = d1 - temp;
	return (K * std::exp(-r * T) * N(-d2)) - (U * std::exp((b - r) * T) * N(-d1));
}

double EuropeanOption::CallDelta(double U) const
{
	double temp = sig * std::sqrt(T);
	double d1 = (std::log(U / K) + (b + (sig * sig) * 0.5) * T) / temp;
	return (std::exp((b - r) * T) * N(d1));
}

double EuropeanOption::PutDelta(double U) const
{
	double temp = sig * std::sqrt(T);
	double d1 = (std::log(U / K) + (b + (sig * sig) * 0.5) * T) / temp;
	return (-std::exp((b - r) * T) * N(-d1));
}

double EuropeanOption::CallGamma(double U) const
{
	double temp = sig * std::sqrt(T);
	double d1 = (std::log(U / K) + (b + (sig * sig) * 0.5) * T) / temp;
	return (n(d1) * std::exp((b - r) * T) / (temp * U));
}

double EuropeanOption::PutGamma(double U) const
{
	double temp = sig * std::sqrt(T);
	double d1 = (std::log(U / K) + (b + (sig * sig) * 0.5) * T) / temp;
	return (n(d1) * std::exp((b - r) * T) / (temp * U));
}

double EuropeanOption::n(double x) const
{
	boost::math::normal_distribution<> Normal(0, 1); // Call normal distribution pdf with boost
	return boost::math::pdf(Normal, x);
}

double EuropeanOption::N(double x) const
{
	boost::math::normal_distribution<> Normal(0, 1); // Call normal distribution cdf with boost
	return boost::math::cdf(Normal, x);
}

EuropeanOption::EuropeanOption() :Option()
// Default constructor
{

}

EuropeanOption::EuropeanOption(double NS, double NK, double NT, double Nr, double Nsig, double Nb, bool Ncalltype) : Option(NS, NK, NT, Nr, Nsig, Nb, Ncalltype)
//Parameter constructor
{

}


EuropeanOption::EuropeanOption(std::vector<double> data)
{
	calltype = 1;
	S = data[0];
	K = data[1];
	T = data[2];
	r = data[3];
	sig = data[4];
	b = data[5];
}






EuropeanOption::EuropeanOption(const EuropeanOption& source) : Option(source)
// Copy constructor 
{

}

EuropeanOption::~EuropeanOption()
//Destructor
{

}

EuropeanOption& EuropeanOption::operator =(const EuropeanOption& source)
//Assignment operator
{
	if (this != &source)
	{
		Option::operator=(source);
		b = source.b;
	}
	return *this;
}

double EuropeanOption::Payoff(double U) const
{
	double callpayoff = U - K > 0 ? U - K : 0;
	double putpayoff = K - U > 0 ? K - U : 0;
	return calltype ? callpayoff : putpayoff;
}

double EuropeanOption::MC(double U, unint sample_N) const
{
	boost::mt19937 gen;
	boost::normal_distribution<> dist(0, 1);
	boost::variate_generator<boost::mt19937&, boost::normal_distribution<>> trial(gen, dist);

	double result = 0;
	double lnS0 = std::log(U);
	for (unint i = 0;i < sample_N;i++)
	{
		double M = (double)i + 1;
		double lnST = lnS0 + (b - sig * sig / 2) * T + sig * std::sqrt(T) * trial();
		double payoff = Payoff(std::exp(lnST));
		//std::cout << std::exp(lnST) << std::endl;
		result = result * (M - 1) / M + std::exp(-r * T) * payoff / M;
	}

	return result;
}

double EuropeanOption::FDM(double U, unint S_N, unint T_N) const
{
	std::vector<std::vector<double>> m;
	auto s = linspace(0, 5 * K, S_N);
	for (unint i = 0;i < S_N;i++)
	{
		m.push_back(std::vector<double>(T_N, 0));
	}
	auto boundary = [&](std::vector<std::vector<double>>& m)->void
	{
		double double_S_N = (double)S_N;

		if (calltype)	// call
		{
			for (unint j = 0;j < T_N;j++)
			{
				m[0][j] = 0;
				m[S_N - 1][j] = 5 * K;
			}
		}
		else    // put
		{
			for (unint j = 0;j < T_N;j++)
			{
				m[0][j] = K;
				m[S_N-1][j] = 0;
			}
		}
		for (unint i = 0;i < S_N;i++)
		{
			m[i][T_N - 1] = Payoff(s[i]);
		}
	};


	auto backward = [&](std::vector<std::vector<double>>& m)->void
	{
		double dS = 5 * K / S_N, dT = T / T_N;
		double C_current, C_last, C_next,b1,b2;
		for (int j = T_N - 2;j >= 0;j--)
		{
			for (unint i = 1;i < S_N - 1;i++)
			{
				b1 = sig * sig * s[i] * s[i] / dS / dS * dT / 2, b2 = b * s[i] / dS * dT;
				C_current = 1 - dT * r - 2 * b1 - b2;
				C_next = b1 + b2;
				C_last = b1;
				m[i][j] = C_current * m[i][j + 1] + C_last * m[i - 1][j + 1] + C_next * m[i + 1][j + 1];
			}
			
		}
		
	};
	boundary(m);
	backward(m);
	return m[(unint)(U / 5 / K * S_N)][0];
}

double EuropeanOption::BT(double U, unint height) const
{
	double dT = T / (double)height;
	double u = std::exp(sig * std::sqrt(dT)), d = 1 / u, p = (std::exp(b * dT) - d) / (u - d);
	double discount_factor = std::exp(-r * dT);
	std::vector<std::vector<double>> m;
	m.push_back(std::vector<double>(1, U));
	// forward
	for (unint i = 1;i <height+1;i++)
	{
		m.push_back(std::vector<double>(i + 1, 0));
		m[i][0] = m[i-1][0] * u;
		for (unint j = 1;j < i + 1;j++)
		{
			m[i][j] = m[i - 1][j - 1] * d;
		}
	}
	// backward
	for (unint j = 0;j < height + 1;j++)
	{
		m[height][j] = Payoff(m[height][j]);
	}
	for (int i = height - 1;i >= 0;i--)
	{
		for (unint j=0;j<i+1;j++)
		{
			m[i][j] = (m[i + 1][j] * p + m[i + 1][j + 1] * (1 - p)) * discount_factor;
		}
	}
	return m[0][0];
}


double EuropeanOption::Delta() const
{
	if (calltype)
		return CallDelta(S);
	else
		return PutDelta(S);
}

double EuropeanOption::Delta(double U) const
{
	if (calltype)
		return CallDelta(U);
	else
		return PutDelta(U);
}

std::vector<double> EuropeanOption::Delta(const std::vector<double>& v) const
{
	std::vector<double> temp;
	for (unint i = 0;i < v.size();i++)
		temp.push_back(Delta(v[i]));
	return temp;
}

double EuropeanOption::Gamma() const
{
	if (calltype)
		return CallGamma(S);
	else
		return PutGamma(S);
}

double EuropeanOption::Gamma(double U) const
{
	if (calltype)
		return CallGamma(U);
	else
		return PutGamma(U);
}

std::vector<double> EuropeanOption::Gamma(const std::vector<double>& v) const
{
	std::vector<double> temp;
	for (unint i = 0;i < v.size();i++)
		temp.push_back(Gamma(v[i]));
	return temp;
}

double EuropeanOption::Parity(double U, double option) const
{
	if (calltype)
		return (option - U + K * exp(-r * T)); // Give the price by put-call parity
	else
		return (option + U - K * exp(-r * T)); // Give the price by put-call parity
}

bool EuropeanOption::Parity(double U, double call, double put) const
{
	double temp = call + K * exp(-r * T) - put - U;
	if (abs(temp) < 0.0001) // Allow some round error
		return true;
	else
		return false;
}