#ifndef GlobalFunction_HPP
#define GlobalFunction_HPP

#include <vector>
#include "EuropeanOption.hpp"

typedef unsigned int unint;

std::vector<double> linspace(double start, double end, unint num); // Mesh function for convenience
//double MC(Option *const target, double U, unint N, unint T);

#endif