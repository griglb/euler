// A natural number, N, that can be written as the sum and product of a given set
// of at least two natural numbers, { a1, a2, ... , ak } is called a product-sum number:
//      N = a1 + a2 + ... + ak = a1 * a2 * ... * ak.
// For example, 6 = 1 + 2 + 3 = 1 * 2 * 3.
//
// For a given set of size, k, we shall call the smallest N with this property a minimal
// product-sum number.  The minimal product-sum numbers for sets of size, k = 2, 3, 4, 5,
// and 6 are as follows.
//
//      k = 2 : 4 = 2 * 2 = 2 + 2
//      k = 3 : 6 = 1 * 2 * 3 = 1 + 2 + 3
//      k = 4 : 8 = 1 * 1 * 2 * 4 = 1 + 1 + 2 + 4
//      k = 5 : 8 = 1 * 1 * 2 * 2 * 2 = 1 + 1 + 2 + 2 + 2
//      k = 6 : 12 = 1 * 1 * 1 * 1 * 2 * 6 = 1 + 1 + 1 + 1 + 2 + 6
//
// Hence for 2 <= k <= 6, the sum of all the minimal product-sum numbers is
//      4 + 6 + 8 + 12 = 30; note that 8 is only counted once in the sum.
// In fact, as the complete set of minimal product-sum numbers for 2 <= k <= 12 is
// { 4, 6, 8, 12, 15, 16 }, the sum is 61.
//
// What is the sum of all the minimal product-sum numbers for 2 <= k <= 12000?


#include <algorithm>
#include <cmath>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <vector>

#include "combinatorics.h"
#include "prime_helper.h"


// To complete the set up to k = 12:
//      k =  7 : 12 = 1 * 1 * 1 * 1 * 1 * 3 * 4 = 1 + 1 + 1 + 1 + 1 + 3 + 4
//      k =  8 : 12 = 1 * 1 * 1 * 1 * 1 * 2 * 2 * 3 = 1 + 1 + 1 + 1 + 1 + 2 + 2 + 3
//      k =  9 : 15 = 1 * 1 * 1 * 1 * 1 * 1 * 1 * 3 * 5 = 1 + 1 + 1 + 1 + 1 + 1 + 1 + 3 + 5
//      k = 10 : 16 = 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 4 * 4 = 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 4 + 4
//      k = 11 : 16 = 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 2 * 2 * 4 = 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 2 + 2 + 4
//      k = 12 : 16 = 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 2 * 2 * 2 * 2 = 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 2 + 2 + 2 + 2

// Call function for minimal product-sum number with k terms MPS(k)
// Posit 1 : MPS(k) is a not strictly monotonically increasing function
// Posit 2 : MPS(k) is never a prime number - the only way to have a prime product
//           is (k-1) 1's and the prime, which is never going to sum to the same number
// Posit 3 : For any even k, the number 2*k is a product-sum number, but rarely the minimal
//           product-sum number.
//           It is {(k-2) 1's, 2, k}, which yields yields 2*k as both a sum and product.
//           This provides an upper bound for MPS(k).
// Posit 4 : For any composite N, every possible combination of factors can be a product-sum
//           with enough 1's, but not necessarily a minimum product-sum number.

// Let us examine a couple numbers to see how many product-sums they can form:
//      N = 24 = 2^3*3
// Factorizations of 24: 2*12, 2*2*6, 2*2*2*3, 2*3*4, 3*8, 4*6
// This yields these product-sums:
//      {10 x 1, 2, 12}         -> k = 12
//      {14 x 1, 2, 2, 6}       -> k = 17
//      {15 x 1, 2, 2, 2, 3}    -> k = 19
//      {15 x 1, 2, 3, 4}       -> k = 18
//      {13 x 1, 3, 8}          -> k = 15
//      {14 x 1, 4, 6}          -> k = 16
//
//      N = 32 = 2^5
// Factorizations of 32: 2*16, 2*2*8, 2*2*2*4, 2*2*2*2*2, 2*4*4, 4*8
// This yields these product-sums:
//      {14 x 1, 2, 16}         -> k = 16
//      {20 x 1, 2, 2, 8}       -> k = 23
//      {22 x 1, 2, 2, 2, 4}    -> k = 26
//      {22 x 1, 2, 2, 2, 2, 2} -> k = 27 
//      {22 x 1, 2, 4, 4}       -> k = 25
//      {20 x 1, 4, 8}          -> k = 22
//

using Factors = std::multiset<uint64_t>;
using AllFactorizations = std::set<Factors>;
using AllKnownFactorizations = std::vector<AllFactorizations>;
using NsforK = std::set<uint64_t>;
using KValues = std::vector<NsforK>;

AllKnownFactorizations g_allKnownFactorizations;
KValues g_K;
PrimeHelper g_helper;


AllFactorizations get_all_factorizations(uint64_t number) {
// std::cout << "In get_all_factorizations(" << number << ")" << std::endl;
    // Resize the global collection if it isn't big enough.
    if (number > g_allKnownFactorizations.size())
        g_allKnownFactorizations.resize(number+1);

    // If we've already calculated the factorizations, return them.
    if (!g_allKnownFactorizations[number].empty())
        return g_allKnownFactorizations[number];

    auto primes = g_helper.get_primes(number);
    // Skip prime numbers
    if (*primes.crbegin() == number)
        return {{number}};

    AllFactorizations all_my_factorizations;
    all_my_factorizations.insert({number});

    Factorization my_factors = g_helper.get_factorization(number);

    for (const auto &[prime, exp] : my_factors) {
        AllFactorizations all_subfactorizations = get_all_factorizations(number / prime);
        for (const auto &subfactorizations : all_subfactorizations) {
            size_t num_factors = subfactorizations.size();
            for (size_t i = 0; i < num_factors; ++i) {
                Factors my_factorization = subfactorizations;
                Factors::iterator iter = my_factorization.begin();
                std::advance(iter, i);
                uint64_t new_factor = *iter * prime;
                my_factorization.erase(iter);
                my_factorization.insert(new_factor);
//                *iter *= prime;
                all_my_factorizations.insert(my_factorization);
            }
            Factors my_factorization = subfactorizations;
            my_factorization.insert(prime);
            all_my_factorizations.insert(my_factorization);
        }
    }
    g_allKnownFactorizations[number] = all_my_factorizations;
    return all_my_factorizations;
}


uint64_t get_product_sum_for_factorization(const Factors &factors) {
    if (factors.size() == 1)
        return 0;

    uint64_t number = std::accumulate(factors.cbegin(), factors.cend(), 1ULL, std::multiplies<uint64_t>());
    uint64_t sum = std::accumulate(factors.cbegin(), factors.cend(), 0ULL);
    uint64_t num_ones = number - sum;
    return num_ones + factors.size();
}


uint64_t solve_it(uint64_t max_k) {
    g_allKnownFactorizations.resize(2 * max_k + 1);
    g_K.resize(max_k+1);
    auto tmp = g_helper.get_primes(2 * max_k);
    std::set<uint64_t> primes{tmp.cbegin(), tmp.cend()};

    for (uint64_t N = 2; N <= 2 * max_k; ++N) {
        if (primes.find(N) != primes.cend())
            continue;
        auto all_factorizations = get_all_factorizations(N);
        for (const auto &factorizations : all_factorizations) {
            uint64_t k = get_product_sum_for_factorization(factorizations);
// std::cout << N << " = ";
// for (const auto &f : factorizations)
//     std::cout << f << "  ";
// std::cout << " -> " << k << std::endl;
            if (k <= max_k)
                g_K[k].insert(N);
        }
    }

    for (uint64_t k = 2; k <= max_k; ++k) {
        std::cout << k << " : ";
        for (const auto & N: g_K[k])
            std::cout << N << ", ";
        std::cout << std::endl;
    }

    std::set<uint64_t> unique_n;
    for (uint64_t k = 2; k <= max_k; ++k)
        unique_n.insert(*g_K[k].cbegin());

    return std::accumulate(unique_n.cbegin(), unique_n.cend(), 0ULL);
}


int main() {
    std::cout << "Hello World!\n";

//     {
//         auto all_my_factors = get_all_factorizations(15);
//         std::cout << "printing..." << std::endl;
//         for (const auto &my_factors : all_my_factors) {
//             for (const auto &fac : my_factors)
//                 std::cout << fac << "  ";
//             std::cout << "  ->  k = " << get_product_sum_for_factorization(my_factors);
//             std::cout << std::endl;
//         }
//     }
// return 0;
    {
        std::cout << solve_it(12000) << std::endl;
    }
}
