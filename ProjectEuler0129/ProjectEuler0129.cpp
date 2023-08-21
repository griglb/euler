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
#include <numeric>
#include <unordered_set>
#include <vector>

#include "big_int.h"
#include "chunked_prime_helper.h"
#include "gcd.h"
#include "prime_helper.h"


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

// If A(n) = x, then A(n^p) = n^(p-1) * A(n)
// For composite n = a x b, A(n) = LCM(A(a), A(b)) if GCD(a, b) = 1

// 1287 = 33 x 39 = (3 x 11) x (3 x 13) = 3^2 x 11 x 13
//   A(3) = 3
//   A(11) = 2
//   A(13) = 6
//   A(33) = 6
//   A(39) = 6
//   A(1287) = 18
//   A(1287) = LCM( LCM(3 * A(3), A(11)), A(13) )
//           = LCM( LCM(9, 2), 6) = LCM(18, 6) = 18

// 4719 = 33 x 143 = (3 x 11) x (11 x 13)
//   A(3) = 3
//   A(11) = 2
//   A(13) = 6
//   A(33) = 6
//   A(143) = 6
//   A(4719) = 66
//   A(4719) = LCM( LCM(A(3), 11 * A(11)), A(13) )
//           = LCM( LCM(3, 22), 6) = lCM(66, 6) = 66

class Finder {
public:
    Finder(uint64_t max_value) {
        R_small = std::vector<uint64_t>{ 0,
                                         1,
                                         11,
                                         111,
                                         1'111,
                                         11'111,
                                         111'111,
                                         1'111'111,
                                         11'111'111,
                                         111'111'111,
                                         1'111'111'111,
                                         11'111'111'111,
                                         111'111'111'111,
                                         1'111'111'111'111,
                                         11'111'111'111'111,
                                         111'111'111'111'111,
                                         1'111'111'111'111'111,
                                         11'111'111'111'111'111,
                                         111'111'111'111'111'111,
                                         1'111'111'111'111'111'111
        };

        R_big.reserve(max_value);
        for (const auto& R : R_small) {
            R_big.push_back(BigInt{ R });
        }
        helper_.get_primes(max_value);
    }

    uint64_t A(uint64_t n) {
        // First check the R(k) values that can fit in 64-bits
        for (uint64_t i = 2; i < 20; ++i) {
            if (0 == R_small[i] % n)
                return i;
        }

        for (uint64_t i = 20; i < 2 * n; ++i) {
            if (i == R_big.size()) {
                BigInt tmp;
                tmp.set_digits(BigInt::Digits(i, 1));
                R_big.push_back(tmp);
            }

            BigInt R = R_big[i];
            BigInt quot = R / n;
            quot *= n;
            if (R == quot)
                return i;
        }

        throw "Need more digits!";
    }

    uint64_t A_fast(uint64_t n) {
        if (a_for_n.find(n) != a_for_n.end())
            return a_for_n.at(n);

        auto factors = helper_.get_factorization_fast(n);

        std::vector<uint64_t> terms;
        terms.reserve(factors.size());

        for (const auto& [prime, exp] : factors) {
            if (a_for_n.find(prime) == a_for_n.end())
                a_for_n[prime] = A(prime);
            terms.push_back(a_for_n.at(prime));
            for (uint64_t e = 1; e < exp; ++e)
                terms.back() *= prime;
        }

        uint64_t A{ 1 };
        for (const auto& t : terms)
            A = lcm(A, t);
        a_for_n[n] = A;
        return A;
    }

private:
    PrimeHelper helper_;
    std::vector<uint64_t> R_small;
    std::vector<BigInt> R_big;
    std::map < uint64_t, uint64_t> a_for_n;

};


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    Finder finder(1000);
    //    std::vector<uint64_t> numbers{ 3,   7,   11,   13,   17,  19,  21,  23,  29,   33,   39, 51, 57, 77,
    //                                   9,  49,  121,  169,  289, 361, 441, 529, 841, 1089, 1521,  // squares
    //                                  27, 343, 1331, 2197, 4913,  // cubes
    //                                  143, 1287, 4719  };  // products with gcd > 1
    //    for (const auto& n : numbers) {
    //        std::cout << "A(" << n << ") = " << finder.A(n) << "\t" << finder.A_fast(n) << std::endl;
    //    }
    //}

    {
        constexpr uint64_t min_value{ 999'500 };
        constexpr uint64_t max_value{ 1'100'000 };

        PrimeHelper helper;
        auto primes = helper.get_primes(max_value);
        for (uint64_t n = min_value; n <= max_value; ++n) {
            if ((0 == n % 2) || (0 == n % 5))
                continue;

            auto factors = helper.get_factorization_fast(n);

            if (factors.size() < 5)
                continue;

            std::cout << n << " = ";
            for (const auto& [prime, exp] : factors) {
                if (exp > 1)
                    std::cout << prime << "^" << exp << " x ";
                else
                    std::cout << prime << " x ";
            }
            std::cout << std::endl;
        }
    }
    return 0;
    {
        constexpr uint64_t max_value{ 10'000 };

        Finder finder{ max_value };
        PrimeHelper helper;
        auto primes = helper.get_primes(max_value);

        for (uint64_t n = 3; n < max_value; ++n) {
            // Skip all multiples of 2 or 5, so gcd(n, 10) = 1
            if ((0 == n % 2) || (0 == n % 5))
                continue;

            auto a = finder.A_fast(n);
            std::cout << n << "\t" << a << std::endl;
            if (a > 1'000'000)
                break;
        }
    }

}
