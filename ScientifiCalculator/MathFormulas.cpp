#include "framework.h"
#include <vector>
#include <numeric>

#include "MathFormulas.h"
#include "CalculateButtons.h"

using namespace std;

double calc_root(double radicand, double index) {
    if (index < 2.0) {
        MessageBox(NULL, L"Index should be 2 or greater!", L"FAIL", MB_OK);
        return 0.0;
    }
    if (index != static_cast<int>(index)) {
        MessageBox(NULL, L"Index should be only integer!", L"FAIL", MB_OK);
        return 0.0;
    }

    bool is_radicand_negative = false;
    if (radicand < 0.0) {
        is_radicand_negative = true;
        radicand *= -1.0;
    }

    double result = pow(radicand, 1.0 / index);

    if (is_radicand_negative)
        return result * -1.0;

    return result;
}

double calc_logarithmic(double argument, double base) {
    if (argument != static_cast<int>(argument)) {
        MessageBox(NULL, L"Argument should be only integer!", L"FAIL", MB_OK);
        return 0.0;
    }
    if (base != static_cast<int>(base)) {
        MessageBox(NULL, L"Base should be only integer!", L"FAIL", MB_OK);
        return 0.0;
    }
    if (base < 2.0 || argument <= 0.0) {
        MessageBox(NULL, L"Undefinied!", L"FAIL", MB_OK);
        return 0.0;
    }

    if (argument == 1.0)
        return 0.0;

    if (argument == base)
        return 1.0;

    double result = log(argument) / log(base);
    return result;
}

double calc_exponential(double base, double exponential) {
    if (base == exponential && exponential == 0) {
        MessageBox(NULL, L"Undefinied!", L"FAIL", MB_OK);
        return 0.0;
    }

    if (base == 0.0)
        return 0.0;

    if (exponential == 0.0)
        return 1.0;

    bool is_exponential_negative = false, is_base_negative = false;

    if (base < 0.0) {
        if (exponential != static_cast<int>(exponential)) {
            MessageBox(NULL, L"Undefinied", L"FAIL", MB_OK);
            return 0.0;
        }

        is_base_negative = true;
        base *= -1.0;
    }

    if (exponential < 0.0) {
        is_exponential_negative = true;
        exponential *= -1.0;
    }

    double result = pow(base, exponential);

    if (is_base_negative && (static_cast<int>(exponential) % 2 != 0))
        result *= -1.0;

    if (is_exponential_negative)
        return (1.0 / result);

    return result;
}
double calc_factoriel(double number) {
    if (number < 0.0 || number != static_cast<int>(number)) {
        MessageBox(NULL, L"Undefinied", L"FAIL", MB_OK);
        return 0.0;
    }

    if (number == 0.0)
        return 1.0;

    int results = number;
    for (int i = number - 1; i > 0; i--)
        results *= i;

    return results;
}

double calc_variation_permutation(double value_n, double value_k) {
    if (value_n < value_k) {
        MessageBox(NULL, L"Undefinied", L"FAIL", MB_OK);
        return 0.0;
    }
    if (value_n != static_cast<int>(value_n) || value_k != static_cast<int>(value_k)) {
        MessageBox(NULL, L"Variables should be only integer!", L"FAIL", MB_OK);
        return 0.0;
    }
    if (value_n < 0.0 || value_k < 0.0) {
        MessageBox(NULL, L"Variables should be positive numbers!", L"FAIL", MB_OK);
        return 0.0;
    }

    if (value_k == 0.0)
        return 1.0;

    if (value_k == 1.0)
        return value_n;

    double result = calc_factoriel(value_n - value_k);
    result = calc_factoriel(value_n) / result;
    return result;
}
double find_common_factor(int a, int b) {
    if (a == 0)
        return b;

    return find_common_factor(b % a, a);
}
double calc_greatest_common_factor(std::vector<double> number_storage) {
    int result = number_storage[0];

    if (result < 0)
        result *= -1;

    int buffer = 0;
    for (int i = 1; i < number_storage.size(); i++) {
        buffer = static_cast<int>(number_storage[i]);

        if (buffer < 0)
            buffer *= -1;

        result = find_common_factor(buffer, result);

        if (result == 1)
            return 1;
    }
    return result;
}
double calc_sigma(double index, double limitation, double increase) {
    if (index != static_cast<int>(index) || limitation != static_cast<int>(limitation)) {
        MessageBox(NULL, L"Variables should be only integer!", L"FAIL", MB_OK);
        return 0.0;
    }

    double results = 0.0;
    for (int i = index; i < limitation; i++)
        results += increase;

    return results;
}
double calc_pi(double index, double limitation, double increase) {
    if (index != static_cast<int>(index) || limitation != static_cast<int>(limitation)) {
        MessageBox(NULL, L"Variables should be only integer!", L"FAIL", MB_OK);
        return 0.0;
    }

    double results = 1.0;
    for (int i = index; i < limitation; i++)
        results *= increase;

    return results;
}
double calc_binomial_power(double value_a, double value_b, double power) {
    if (power != static_cast<int>(power)) {
        MessageBox(NULL, L"Power should be integer!", L"FAIL", MB_OK);
        return 0.0;
    }

    return pow((value_a + value_b), power);
}
double calc_sum_power(double value_a, double value_b, double power) {
    if (power != static_cast<int>(power)) {
        MessageBox(NULL, L"Power should be integer!", L"FAIL", MB_OK);
        return 0.0;
    }

    return pow(value_a, power) + pow(value_b, power);
}
double calc_natural_logarithmic(double value) {
    if (value <= 0.0) {
        MessageBox(NULL, L"Undefinied", L"FAIL", MB_OK);
        return 0.0;
    }

    if (value == 1.0)
        return 0.0;

    return log(value);
}
double calc_combination_binominal_coefficient(double value_n, double value_k) {
    if (value_n < value_k) {
        MessageBox(NULL, L"Undefinied", L"FAIL", MB_OK);
        return 0.0;
    }
    if (value_n != static_cast<int>(value_n) || value_k != static_cast<int>(value_k)) {
        MessageBox(NULL, L"Variables should be only integer!", L"FAIL", MB_OK);
        return 0.0;
    }
    if (value_n < 0.0 || value_k < 0.0) {
        MessageBox(NULL, L"Variables should be positive numbers!", L"FAIL", MB_OK);
        return 0.0;
    }

    if (value_k == 0.0)
        return 1.0;

    if (value_k == 1.0)
        return value_n;

    if (value_n - 1 == value_k)
        return value_n;

    double result = calc_factoriel(value_n - value_k);
    result = calc_factoriel(value_k) * result;
    result = calc_factoriel(value_n) / result;
    return result;
}
double calc_sgn(double value) {
    if (value > 0.0)
        return 1.0;

    if (value < 0.0)
        return -1.0;

    return 0.0;
}
double calc_matrix(std::vector<double> array_results, int rows) {
    Eigen::MatrixXd matrix(rows, rows);

    for (int count = 0, i = 0; i < rows; ++i) {
        for (int j = 0; j < rows; ++j) {
            matrix(i, j) = array_results[count];
            ++count;
        }
    }

    return matrix.determinant();
}
int optimise_gcf(int value_a, int value_b) {
    while (value_b != 0) {
        int temp = value_b;
        value_b = value_a % value_b;
        value_a = temp;
    }
    return value_a;
}

double calc_greatest_common_factor(const std::vector<int>& numbers) {
    int result = numbers[0];

    for (size_t i = 1; i < numbers.size(); ++i)
        result = optimise_gcf(result, numbers[i]);

    return (double)result;
}