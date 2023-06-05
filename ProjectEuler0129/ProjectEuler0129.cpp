// A number consisting entirely of ones is called a repunit.
// We shall define R(k) to be a repunit of length; for example,
//      R(6) = 111111.
//
// Given that n is a positive integer and gcd(n, 10) = 1, it can be shown that
// there always exists a value, k, for which R(k) is divisible by n, and let
// A(n) be the least such value of k; for example, A(7) = 6 and A(41) = 5.
//
// The least value of n for which first exceeds ten is 17.
//
// Find the least value of n for which first A(n) exceeds one-million.


#include <iostream>

#include "chunked_prime_helper.h"


// Prime factorizations of the first few R(k) values:
//      k   factors
//      1   1
//      2   11
//      3   3, 37
//      4   11, 101
//      5   41, 241
//      6   3, 7, 11, 13, 37
//      7   239, 4649
//      8   11, 73, 101, 137
//      9   3, 3, 37, 333667
//     10   11, 41, 271, 9091
//     11   21649, 513239

// In order to solve this problem, you can't use basic prime factorization of R(k).
// To get a solution where A(n) > 1,000,000 would require factoring a number with
// over 1,000,000 digits.  More sophisticated number theory techniques are needed.


int main()
{
    std::cout << "Hello World!\n";

    {
        ChunkedPrimeHelper<10'000'000> helper;
//        helper.get_primes(1e8);
        uint64_t r{ 1 };
        for (uint64_t k = 1; k < 12; ++k) {
            auto factors = helper.get_factorization_fast(r);
            std::cout << k << "\t" << r << "\t";
            for (const auto& [prime, exp] : factors) {
                if (1 == exp)
                    std::cout << prime << " x ";
                else
                    std::cout << prime << "^" << exp << " x ";
            }
            std::cout << std::endl;

            r *= 10;
            r += 1;
        }
    }
}
