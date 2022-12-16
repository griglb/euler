// ProjectEuler0027.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <set>

#include "prime_helper.h"


bool is_prime(int64_t number) {
    static std::set<uint64_t> KnownPrimes;
    static uint64_t MaxTested;

    if (number <= 0)
        return false;

    if (number > MaxTested) {
        PrimeHelper helper;
        auto primes = helper.get_primes(number);
        for (const auto& p : primes)
            KnownPrimes.insert(p);
        MaxTested = number;
    }

    return KnownPrimes.find(number) != KnownPrimes.end();
}


int32_t num_quadratic_primes(int32_t a, int32_t b) {
    int32_t count{ 0 };
    int32_t maxN = a < 0 ? -a : a;
    for (int32_t n = 0; ; ++n) {
        if (is_prime(n * n + a * n + b))
            count++;
        else
            break;
    }

    return count;
}


int main()
{
    std::cout << "Hello World!\n";

    std::cout << 1 << "\t" << 41 << "\t" << num_quadratic_primes(1, 41) << std::endl;
    std::cout << -79 << "\t" << 1601 << "\t" << num_quadratic_primes(-79, 1601) << std::endl;

    int32_t bestA{ 0 };
    int32_t bestB{ 0 };
    int32_t max_primes{ 0 };
    for (int32_t a = -999; a < 1000; ++a) {
        for (int32_t b = -1000; b <= 1000; ++b) {
            auto tmp = num_quadratic_primes(a, b);
            if (tmp > max_primes) {
                bestA = a;
                bestB = b;
                max_primes = tmp;
            }
//            std::cout << a << "\t" << b << "\t" << num_quadratic_primes(a, b) << std::endl;
        }
        std::cout << a << std::endl;
    }

    std::cout << std::endl;
    std::cout << bestA << "\t" << bestB << "\t" << max_primes << std::endl;
    std::cout << bestA * bestB << std::endl;
}
