// n! means n × (n − 1) × ... × 3 × 2 × 1
// For example, 10! = 10 × 9 × ... × 3 × 2 × 1 = 3628800,
// and the sum of the digits in the number 10! is 3 + 6 + 2 + 8 + 8 + 0 + 0 = 27.
// Find the sum of the digits in the number 100!


#include <iostream>

#include "big_int.h"


BigInt factorial(int64_t n) {
    BigInt ret{ 1 };
    for (int64_t i = 2; i < n; ++i) {
        ret *= i;
    }
    return ret;
}


int main()
{
    std::cout << "Hello World!\n";
    BigInt fac = factorial(10);
    std::cout << fac << std::endl;

    fac = factorial(100);
    std::cout << fac << std::endl;
    int64_t sum{ 0 };
    for (const auto& digit : fac.get_digits())
        sum += digit;
    std::cout << sum << std::endl;
}
