#include "AmericanOption.hpp"
#include <boost/math/distributions/normal.hpp>
#include <iostream>
#include <boost/random.hpp>
#include "GlobalFunction.hpp"

std::vector<double> AmericanOption::Regression(std::vector<std::vector<double>>& A, std::vector<double>& y) const
{
	// A is n*3, y is n
	std::vector<std::vector<double>> ATA;	// 3*3
	for (int i = 0;i < 3;i++)
	{
		ATA.push_back(std::vector<double>(3, 0));
	}
	for (int i = 0;i < 3;i++)
	{
		for (int j = 0;j < 3;j++)
		{
			for (int k = 0;k < A.size();k++)
			{
				ATA[i][j] += A[k][j] * A[k][i];
			}
			//std::cout<<"2: " << ATA[i][j] << std::endl;
		}
	}

	std::vector<double> ATy(3,0);	// 3*1
	for (int i = 0;i < 3;i++)
	{
		for (int j = 0;j < A.size();j++)
		{
			ATy[i] += A[j][i] * y[j];
		}
	}
	double detATA = ATA[0][0] * (ATA[1][1] * ATA[2][2] - ATA[1][2] * ATA[2][1])\
				  - ATA[1][0] * (ATA[0][1] * ATA[2][2] - ATA[0][2] * ATA[2][1])\
				  + ATA[2][0] * (ATA[0][1] * ATA[1][2] - ATA[0][2] * ATA[1][1]);
	//std::cout << "det:" << detATA << std::endl;
	double invATA[3][3] =	// without dividing detATA
	{ 
		{
			(ATA[1][1] * ATA[2][2] - ATA[1][2] * ATA[2][1]),
			(ATA[0][2] * ATA[2][1] - ATA[0][1] * ATA[2][2]),
			(ATA[0][1] * ATA[1][2] - ATA[0][2] * ATA[1][1]),
		},
		{
			(ATA[2][0] * ATA[1][2] - ATA[1][0] * ATA[2][2]),
			(ATA[0][0] * ATA[2][2] - ATA[2][0] * ATA[0][2]),
			(ATA[1][0] * ATA[0][2] - ATA[0][0] * ATA[1][2]),
		},
		{
			(ATA[1][0] * ATA[2][1] - ATA[1][1] * ATA[2][0]),
			(ATA[0][1] * ATA[2][0] - ATA[0][0] * ATA[2][1]),
			(ATA[0][0] * ATA[1][1] - ATA[1][0] * ATA[0][1]),
		}
	};
	std::vector<double> result(3, 0);
	for (int i = 0;i < 3;i++)
	{
		for (int j = 0;j < 3;j++)
		{
			result[i] += invATA[i][j] * ATy[j];
		}
		result[i] /= detATA;
	}
	//std::cout << "1: " << detATA << std::endl;
	return result;
}


double AmericanOption::CallPrice(double U) const
{
	return 0;
}

double AmericanOption::PutPrice(double U) const
{
	return 0;
}

double AmericanOption::CallDelta(double U) const
{
	return 0;
}

double AmericanOption::PutDelta(double U) const
{
	return 0;
}

double AmericanOption::CallGamma(double U) const
{
	return 0;
}

double AmericanOption::PutGamma(double U) const
{
	return 0;
}

AmericanOption::AmericanOption() :Option()
// Default constructor
{

}

AmericanOption::AmericanOption(double NS, double NK, double NT, double Nr, double Nsig, double Nb, bool Ncalltype) : Option(NS, NK, NT, Nr, Nsig, Nb, Ncalltype)
//Parameter constructor
{

}


AmericanOption::AmericanOption(std::vector<double> data)
{
	calltype = 1;
	S = data[0];
	K = data[1];
	T = data[2];
	r = data[3];
	sig = data[4];
	b = data[5];
}


AmericanOption::AmericanOption(const AmericanOption& source) : Option(source)
// Copy constructor 
{

}

AmericanOption::~AmericanOption()
//Destructor
{

}

AmericanOption& AmericanOption::operator =(const AmericanOption& source)
//Assignment operator
{
	if (this != &source)
	{
		Option::operator=(source);
		b = source.b;
	}
	return *this;
}

double AmericanOption::Payoff(double U) const
{
	double callpayoff = U - K > 0 ? U - K : 0;
	double putpayoff = K - U > 0 ? K - U : 0;
	return calltype ? callpayoff : putpayoff;
}

double AmericanOption::MC(double U, unint S_N, unint T_N) const
{
	boost::mt19937 gen;
	boost::normal_distribution<> dist(0, 1);
	boost::variate_generator<boost::mt19937&, boost::normal_distribution<>> trial(gen, dist);

	double dT = T / T_N;
	std::vector<std::vector<double>> m;
	for (unint i = 0;i < S_N;i++)
	{
		m.push_back(std::vector<double>(T_N, U));
	}
	// forward
	auto forward = [&](std::vector<std::vector<double>>& m)->void
	{
		double mean = (b - sig * sig / 2) * dT;
		for (unint i = 0;i < S_N;i++)
		{
			for (unint j = 1;j < T_N;j++)
			{
				m[i][j] = m[i][j - 1] * std::exp(mean+sig*std::sqrt(dT)*trial());
			}
		}
	};

	auto backward = [&](std::vector<std::vector<double>>& m)->void
	{
		double discount_factor = std::exp(-r * dT);
		for (unint i = 0;i < S_N;i++)
		{
			m[i][T_N - 1] = Payoff(m[i][T_N - 1]);
		}
		for (int j = T_N - 2;j >= 0;j--)
		{
			std::vector<std::vector<double>> A;
			std::vector<double> y;
			for (unint i = 0;i < S_N;i++)
			{
				if (Payoff(m[i][j])>0)
				{
					A.push_back({ 1,m[i][j],m[i][j] * m[i][j] });
					y.push_back(m[i][j + 1] * discount_factor);
				}
			}
			if (!A.size())	// never early exercise
			{
				for (unint i = 0;i < S_N;i++)
				{
					m[i][j] = m[i][j + 1] * discount_factor;
				}
			}
			else    // exist early exercising
			{
				std::vector<double> coef = Regression(A, y);
				for (unint i = 0;i < S_N;i++)
				{
					double EV = Payoff(m[i][j]);

					/*if (j == 47 && i == 833)
					{
						std::cout<<"mark: " << m[i][j] << std::endl;
					}*/

					if (EV>0)	// potential early exercising
					{
						double EHV = coef[0] + coef[1] * m[i][j] + coef[2] * m[i][j] * m[i][j];
						m[i][j] = EV > EHV ? EV : EHV;
					}
					else
					{
						m[i][j] = m[i][j + 1] * discount_factor;
					}

				}
				
				//if (j <=300) { std::cout <<"A: "<<j<<" "<< coef[0] << "," << coef[1] << "," << coef[2] << ","<<std::endl; }

			}
		}
	};

	forward(m);
	//for (auto i : m[833]) { std::cout << "P: " << i << std::endl; }
	backward(m);

	double result = 0;
	for (int i = 0;i < S_N;i++)
	{
		//result = result * i / ((double)i + 1) + m[i][0] / ((double)i + 1);
		result += m[i][0];
		//std::cout <<"2:"<<i<<" "<< result << std::endl;
	}
	//for (unint i = 0;i < S_N;i++) { std::cout <<"1:" << i << " " << m[i][0] << std::endl; }
	//for (unint i = 0;i < T_N;i++) { std::cout << "1:" << i << " " << m[833][i] << std::endl; }
	return result/(double)S_N;
}

double AmericanOption::BT(double U, unint height) const
{
	double dT = T / (double)height;
	double u = std::exp(sig * std::sqrt(dT)), d = 1 / u, p = (std::exp(b * dT) - d) / (u - d);
	double discount_factor = std::exp(-r * dT);
	std::vector<std::vector<double>> m;
	auto max = [](double& x, double& y)
	{
		return x > y ? x : y;
	};
	m.push_back(std::vector<double>(1, U));
	// forward
	for (unint i = 1;i < height + 1;i++)
	{
		m.push_back(std::vector<double>(i + 1, 0));
		m[i][0] = m[i - 1][0] * u;
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
		for (int j = 0;j < i + 1;j++)
		{
			double HV = (m[i + 1][j] * p + m[i + 1][j + 1] * (1 - p)) * discount_factor;
			double EV = Payoff(m[i][j]);
			m[i][j] = max(HV, EV);
		}
	}
	return m[0][0];
}