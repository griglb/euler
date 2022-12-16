// ProjectEuler0003.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "prime_helper.h"


int main()
{
    std::cout << "Hello World!\n";

    PrimeHelper helper;

    Factorization factorization = helper.get_factorization(600851475143);
    uint64_t largest_factor{ 0 };
    for (const auto [prime, exponent] : factorization) {
        std::cout << prime << "^" << exponent << std::endl;
        if (prime > largest_factor)
            largest_factor = prime;
    }

    std::cout << largest_factor << std::endl;
}
