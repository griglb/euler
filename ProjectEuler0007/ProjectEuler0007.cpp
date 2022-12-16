// ProjectEuler0007.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "prime_helper.h"

int64_t get_nth_prime(int index) {
    PrimeHelper helper;

    int64_t max_val = index;
    while (true) {
        ULongLongVec primes = helper.get_primes(max_val);
        if (primes.size() > index) {
            return primes[index-1];
        }
        max_val += index;
        std::cout << "max_val now = " << max_val << std::endl;
    }
}


int main()
{
    std::cout << "Hello World!\n";
    std::cout << get_nth_prime(6) << std::endl;
    std::cout << get_nth_prime(10001) << std::endl;
}
