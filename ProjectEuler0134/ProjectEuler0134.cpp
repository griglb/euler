// Consider the consecutive primes p1=19 and p2=23.
// It can be verified that 1219 is the smallest number such that the last digits are formed by p1
// whilst also being divisible by p2.
//
// In fact, with the exception of p1=3 and p2=5, for every pair of consecutive primes, p2 > p1,
// there exist values of n for which the last digits are formed by p1 and n is divisible by p2.
// Let S be the smallest of these values of n.
//
// Find sum(S) for every pair of consecutive primes with 5 <= p1 <= 1,000,000.


#include <array>
#include <iostream>
#include <numeric>

#include "prime_helper.h"


// We know for primes > 5, they can only end in 1, 3, 7, or 9.  Look at the multiplication table of
// these digits and their resulting product's unit digit:
//          1   3   7   9
//      1   1   3   7   9
//      3   3   9   1   7
//      7   7   1   9   3
//      9   9   7   3   1
// Each row or column of this table is a permutation of the 4 digits.  So if we have p1 ending in X
// and p2 ending in Y, then we can very quickly identify the last digit of the number multiplied by p2
// to get S.
// For example, with the example p1=19, p2=23, we need a units digit to multiple by 3 to get 9.
// This turns out to be 3.  So we only need to consider numbers of the form 10*k + 3:
//      k   10k+3   p2 * 10x+3
//      0     3         69
//      1    13        299
//      2    23        529
//      3    33        759
//      4    43        989
//      5    53       1219

// The one exception to this is when p1=5, p2=7, which is the easily to find S=35.


using Row = std::array<uint64_t, 10>;

const std::array<Row, 10> Unit_factors{ { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },      // factors for 0
                                          { 0, 1, 0, 3, 0, 0, 0, 7, 0, 9 },      // factors for 1
                                          { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },      // factors for 2
                                          { 0, 7, 0, 1, 0, 0, 0, 9, 0, 3 },      // factors for 3
                                          { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },      // factors for 4
                                          { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },      // factors for 5
                                          { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },      // factors for 6
                                          { 0, 3, 0, 9, 0, 0, 0, 1, 0, 7 },      // factors for 7
                                          { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },      // factors for 8
                                          { 0, 9, 0, 7, 0, 0, 0, 3, 0, 1 } } };  // factors for 9


std::vector<uint64_t> get_S(uint64_t max_value) {
    std::vector<uint64_t> ret;

    PrimeHelper helper;
    auto primes = helper.get_primes(max_value * 1.1);

    for (size_t i = 0; i < primes.size(); ++i) {
        const auto& p1 = primes[i];

        if (p1 < 5)
            continue;
        if (p1 > max_value)
            break;

        if (5 == p1) {
            ret.push_back(35);
            continue;
        }

        uint64_t mod{ 1 };
        uint64_t tmp{ p1 };
        while (tmp > 0) {
            mod *= 10;
            tmp /= 10;
        }

        const auto& p2 = primes[i + 1];

        for (uint64_t S = p2 * Unit_factors[p2 % 10][p1 % 10]; ; S += 10 * p2) {
            if (p1 == (S % mod)) {
                ret.push_back(S);
//                std::cout << p1 << "\t" << p2 << "\t" << S << "\t" << S / p2 << std::endl;
                break;
            }
        }
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    {
        auto S = get_S(1'000'000);
        //for (const auto& el : S)
        //    std::cout << el << std::endl;
        std::cout << "Sum = " << std::accumulate(S.begin(), S.end(), 0ULL) << std::endl;
    }
}
