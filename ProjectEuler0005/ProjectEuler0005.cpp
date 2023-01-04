// 2520 is the smallest number that can be divided by each of the numbers from 1 to 10 without any remainder.
// What is the smallest positive number that is evenly divisible by all of the numbers from 1 to 20?


#include <iostream>
#include <vector>

#include "prime_helper.h"


int32_t find_lcm(const std::vector<int32_t> factors) {
    PrimeHelper helper;

    Factorization lcm_factors;

    for (const auto& f : factors) {
        Factorization factors = helper.get_factorization(f);
        for (const auto [prime, exponent] : factors) {
            if (lcm_factors.count(prime) == 0) {
                lcm_factors[prime] = exponent;
            }
            else {
                if (exponent > lcm_factors.at(prime)) {
                    lcm_factors[prime] = exponent;
                }
            }
        }
    }

    int32_t ret{ 1 };
    for (const auto [prime, exponent] : lcm_factors) {
        for (uint64_t i = 0; i < exponent; ++i) {
            ret *= prime;
        }
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";
    std::cout << find_lcm({ 1, 2, 3, 4, 5, 6, 7, 8 ,9, 10 }) << std::endl;
    std::cout << find_lcm({ 1, 2, 3, 4, 5, 6, 7, 8 ,9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 }) << std::endl;
}
