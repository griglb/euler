// By listing the first six prime numbers: 2, 3, 5, 7, 11, and 13, we can see that the 6th prime is 13.
// What is the 10 001st prime number?


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
