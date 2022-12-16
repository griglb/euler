// ProjectEuler0020.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "big_int.h"


BigInt factorial(int n) {
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
