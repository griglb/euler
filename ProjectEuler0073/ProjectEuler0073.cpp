// Consider the fraction, n / d, where nand d are positive integers.
// If n < d and HCF(n, d) = 1, it is called a reduced proper fraction.
//
// If we list the set of reduced proper fractions for d <= 8 in ascending order of size, we get:
//      1/8, 1/7, 1/6, 1/5, 1/4, 2/7, 1/3, 3/8, 2/5, 3/7, 1/2, 4/7, 3/5, 5/8, 2/3, 5/7, 3/4, 4/5, 5/6, 6/7, 7/8
//                                    ---  ---  ---
// It can be seen that there are 3 fractions between 1/3 and 1/2.
//
// How many fractions lie between 1/3 and 1/2 in the sorted set of reduced proper fractions for d <= 12,000?

#include <math.h>

#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

#include "prime_helper.h"


using Fraction = std::pair<uint64_t, uint64_t>;
uint64_t get_num_fractions(uint64_t max_den, const Fraction& lower, const Fraction& upper) {
    uint64_t count{ 0 };

    PrimeHelper helper;

    {
        helper.get_primes(max_den);
    }

    for (uint64_t d = 2; d <= max_den; ++d) {
        // Skip the exact thresholds
        if ((d == lower.second) || (d == upper.second))
            continue;

        // Calculate the range of possible numerators in the (lower, upper) range.
        uint64_t n_low = static_cast<uint64_t>(ceil(1.0 * d * lower.first / lower.second));
        uint64_t n_high = d * upper.first / upper.second;

        // Find all the relatively prime numerators in the range.
        // First create a Sieve of Erastosthenes, only allowing numbers in the acceptable range.
        std::vector<int8_t> num_sieve(n_high+1, 0);
        for (uint64_t n = n_low; n <= n_high; ++n)
            num_sieve[n] = 1;
        // Get the prime factorization of d and eliminate all numbers that aren't relatively prime.
        auto factors = helper.get_factorization_fast(d);
        for (const auto& [prime, exp] : factors) {
            for (uint64_t i = prime; i <= n_high; i += prime)
                num_sieve[i] = 0;
        }
        // Add the number of reduced fractions with d as the denominator.
        count += std::accumulate(num_sieve.begin(), num_sieve.end(), 0LL);
    }

    return count;
}


int main()
{
    std::cout << "Hello World!\n";

    std::cout << "There are " << get_num_fractions(8, { 1,3 }, { 1,2 }) << " fractions between 1/3 and 1/3 with a denominator <= 8" << std::endl;
    std::cout << "There are " << get_num_fractions(12'000, { 1,3 }, { 1,2 }) << " fractions between 1/3 and 1/3 with a denominator <= 12,000" << std::endl;
}
