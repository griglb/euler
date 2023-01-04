// The prime factors of 13195 are 5, 7, 13 and 29.
// What is the largest prime factor of the number 600851475143 ?


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
