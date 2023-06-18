// A number consisting entirely of ones is called a repunit.
// We shall define R(k) to be a repunit of length k; for example,
//      R(6) = 111111.
//
// Let us consider repunits of the form R(10^n).
//
// Although R(10), R(100), or R(1000) are not divisible by 17,
// R(10000) is divisible by 17.
// Yet there is no value of n for which R(10^n) will divide by 19.
// In fact, it is remarkable that 11, 17, 41, and 73 are the only four primes
// below one-hundred that can be a factor of R(10^n).
//
// Find the sum of all the primes below one-hundred thousand that will never
// be a factor of R(10^n).


#include <iostream>
#include <map>
#include <numeric>
#include <optional>
#include <utility>

#include "big_int.h"
#include "prime_helper.h"


// This problem is almost the inverse of Problem 132.
// There we wanted to find the primes that divide R(10^9).
// Here we want all the primes that don't divide R(10^n).
// The brute force approach works, because of the lessons learned in that problem:
//      - if k divides n, then R(k) divides R(n)
//      - we need to consider all factors of 10^n < 100,000
//          - this is all numbers of the form 2^a x 5^b < 100,000
//          - therefore a in [0, 18] and b in [0, 9] since 2^19 > 100,000 and 5^9 > 100,000
// Again from Problem 130, we know that p-1 % A(p) == 0, so skip any divisor of 10^n > p
// and not holding this relationship.


void solve() {
    constexpr uint64_t max_value{ 100'000 };

    std::vector<uint64_t> answer{ 2, 5 };

    std::map<uint64_t, BigInt> R_factors;

    uint64_t pow5{ 1 };
    for (uint8_t exp5 = 0; exp5 < 10; ++exp5, pow5 *= 5) {
        std::cout << pow5 << std::endl;
        uint64_t pow2{ 1 };
        for (uint8_t exp2 = 0; exp2 < 19; ++exp2, pow2 *= 2) {
            const uint64_t prod{ pow2 * pow5 };
            if (1 == prod)
                continue;
            // Skip any product > the max prime
            if (prod > max_value)
                continue;

            BigInt R_prod;
            R_prod.set_digits(BigInt::Digits(prod, 1));
            R_factors[prod] = R_prod;
        }
    }

    //for (const auto& [key, value] : R_factors)
    //    std::cout << key << "\t" << value << std::endl;
    std::cout << std::endl << "There are " << R_factors.size() << " factors" << std::endl;

    PrimeHelper helper;
    auto primes = helper.get_primes(max_value);
    for (const auto& p : primes) {
        if ((2 == p) || (5 == p))
            continue;

        BigInt p_big{ p };

        std::cout << p_big << "\t" << answer.size() << std::endl;

        std::optional<std::pair<uint64_t, uint64_t>> found;
        for (const auto& [div, R] : R_factors) {
            // From Problem 130, we know that p-1 % A(p) == 0, so skip any div > p and not holding this relationship.
            if (div > p)
                continue;
            if (((p - 1) % div) != 0)
                continue;

            BigInt quot = R / p;
            quot *= p;
            if (R == quot) {
                found = { p, div };
                std::cout << "\t" << p << " divides R(" << div << ")" << std::endl;
                break;
            }
        }

        if (!found.has_value()) {
            answer.push_back(p);
        }
        else {
            // Divide all mulitples of div by the new prime factor
            for (auto& [div, R] : R_factors) {
                if (0 == div % found->second)
                    R /= found->first;
            }
        }
    }

    //for (const auto& [key, value] : R_factors)
    //    std::cout << key << "\t" << value << std::endl;

    std::cout << "The list of primes is" << std::endl;
    for (const auto& ans : answer)
        std::cout << ans << std::endl;
    std::cout << answer.size() << " primes that don't divide R(10^n)" << std::endl;
    std::cout << "Sum = " << std::accumulate(answer.begin(), answer.end(), 0ULL) << std::endl;
}


int main()
{
    std::cout << "Hello World!\n";

    {
        solve();
    }
}
