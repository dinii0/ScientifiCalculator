#ifndef MATHFROMULAS_H
#define MATHFROMULAS_H

#include "eigen/Eigen/Dense"

double calc_root(double radicand, double index);
double calc_logarithmic(double argument, double base);
double calc_exponential(double base, double exponential);
double calc_factoriel(double number);
double calc_variation_permutation(double value_n, double value_k);
double calc_greatest_common_factor(std::vector<double> number_storage);
double calc_sigma(double index, double limitation, double increase);
double calc_pi(double index, double limitation, double increase);
double calc_binomial_power(double value_a, double value_b, double power);
double calc_sum_power(double value_a, double value_b, double power);
double calc_natural_logarithmic(double value);
double calc_combination_binominal_coefficient(double value_n, double value_k);
double calc_sgn(double value);
double calc_matrix(std::vector<double> array_results, int rows);
double calc_greatest_common_factor(const std::vector<int>& numbers);

#endif