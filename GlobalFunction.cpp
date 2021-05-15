#include "GlobalFunction.hpp"

std::vector<double> linspace(double start, double end, unint num) // Mesh function with size of vector
{
	std::vector<double> result;
	double h = (end - start) / ((double)num - 1);
	for (unint i = 0;i < num;i++)
		result.push_back(start + i * h);
	return result;
}
