#ifndef OptionMatrix_HPP
#define OptionMatrix_HPP

#include "EuropeanOption.hpp"
#include "PerpetualAmericanOption.hpp"
#include "OptionMatrix.cpp"
#include <vector>

typedef EuropeanOption EO;
typedef PerpetualAmericanOption PAO;


template <typename T>
class OptionMatrix
{
private:
	std::vector<std::vector<double>> m; // Vectors of vector to save parameter
	unint row; // Number of rows, each row stands for an option
	static unint column; // Static number of columns, 6, by default
public:
	OptionMatrix(); // Default constructor 
	OptionMatrix(unint size); // Size constructor
	OptionMatrix(const OptionMatrix<T>& source); // Copy constructor 
	~OptionMatrix(); // Destructor

	OptionMatrix<T>& operator=(const OptionMatrix<T>& source); // Assignment operator
	double& operator() (unint nrow, unint ncolumn); // Index operator
	std::vector<double> CallPrice() const; // Calculate call prices
	std::vector<double> PutPrice() const; // Calculate put prices

	std::vector<double> CallDelta() const; // Calculate call deltas
	std::vector<double> PutDelta() const; // Calculate put deltas

	std::vector<double> CallGamma() const; // Calculate call gammas
	std::vector<double> PutGamma() const; // Calculate put gammas
};


template <typename T>
unint OptionMatrix<T>::column = 6; // Set it as 6 by default, which remain flexible

template <typename T>
OptionMatrix<T>::OptionMatrix() // Generate one-dimension vector by default
{
	std::vector<double> v(column); // Default column is 6
	m.push_back(v);
	row = 1;
}

template <typename T>
OptionMatrix<T>::OptionMatrix(unint size) // Size constructor
{
	std::vector<double> v(column);  // Default column is 6
	row = size;
	for (unint i = 0;i < size;i++)
		m.push_back(v); // Save empty vectors
}


template <typename T>
OptionMatrix<T>::OptionMatrix(const OptionMatrix<T>& source) :row(source.row) // Copy constructor
{
	for (unint i = 0;i < source.row;i++)
		m[i] = source.m[i];
}

template <typename T>
OptionMatrix<T>::~OptionMatrix() // Destructor
{

}

template <typename T>
OptionMatrix<T>& OptionMatrix<T>::operator=(const OptionMatrix<T>& source) // Assignment operator
{
	if (this != &source)
	{
		row = source.row;
		for (unint i = 0;i < row;i++)
			m[i] = source.m[i];
	}
	return *this;
}

template <typename T>
double& OptionMatrix<T>::operator() (unint nrow, unint ncolumn) // Index operator
{
	if (0 < ncolumn && ncolumn < column + 1 && 0 < nrow && nrow < row + 1)
		return m[nrow - 1][ncolumn - 1]; // By logic
	else
	{
		std::cout << "The index does not match." << std::endl;
		throw - 1;
	}
}

template <typename T>
std::vector<double> OptionMatrix<T>::CallPrice() const
{
	std::vector<double> result(row); // Create the result vector to return 
	for (unint i = 0;i < row;i++)
	{
		T temp = T(m[i]);
		temp.calltype = 1;
		result[i] = temp.Price();
	}
	return result;
}

template <typename T>
std::vector<double> OptionMatrix<T>::PutPrice() const
{
	std::vector<double> result(row);
	for (unint i = 0;i < row;i++)
	{
		T temp = T(m[i]);
		temp.calltype = 0;
		result[i] = temp.Price();
	}
	return result;
}

template <typename T>
std::vector<double> OptionMatrix<T>::CallDelta() const
{
	std::vector<double> result(row);
	for (unint i = 0;i < row;i++)
	{
		T temp = T(m[i]);
		temp.calltype = 1;
		result[i] = temp.Delta();
	}
	return result;
}

template <typename T>
std::vector<double> OptionMatrix<T>::PutDelta() const
{
	std::vector<double> result(row);
	for (unint i = 0;i < row;i++)
	{
		T temp = T(m[i]);
		temp.calltype = 0;
		result[i] = temp.Delta();
	}
	return result;
}

template <typename T>
std::vector<double> OptionMatrix<T>::CallGamma() const
{
	std::vector<double> result(row);
	for (unint i = 0;i < row;i++)
	{
		T temp = T(m[i]);
		temp.calltype = 1;
		result[i] = temp.Gamma();
	}
	return result;
}

template <typename T>
std::vector<double> OptionMatrix<T>::PutGamma() const
{
	std::vector<double> result(row);
	for (unint i = 0;i < row;i++)
	{
		T temp = T(m[i]);
		temp.calltype = 0;
		result[i] = temp.Gamma();
	}
	return result;
}

#endif