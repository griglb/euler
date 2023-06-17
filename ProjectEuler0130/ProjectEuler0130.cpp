// A number consisting entirely of ones is called a repunit.
// We shall define R(k) to be a repunit of length; for example,
//      R(6) = 111111.
//
// Given that n is a positive integer and gcd(n, 10) = 1, it can be shown that
// there always exists a value, k, for which R(k) is divisible by n, and let
// A(n) be the least such value of k; for example, A(7) = 6 and A(41) = 5.
//
// You are given that for all primes, p > 5, that p-1 is divisible by A(p).
// For example, when p=41, A(41) = 5, and 40 is divisible by 5.
//
// However, there are rare composite values for which this is also true;
// the first five examples being 91, 259, 451, 481, 703.
//
// Find the sum of the first twenty-five composite values of n for which
// gcd(n, 10) = 1 and n-1 is divisible by A(n).


#include <iostream>
#include <numeric>
#include <unordered_set>
#include <vector>

#include "big_int.h"
#include "prime_helper.h"


bool num_divides_R(uint64_t num, uint64_t n) {
    if (n < 19) {
        uint64_t R{ 1 };
        for (uint64_t i = 1; i < n; ++i) {
            R *= 10;
            R += 1;
        }
        return 0 == R % num;
    }

    BigInt R{ 1 };
    for (uint64_t i = 0; i < n; ++i) {
        R *= 10;
        R += 1;
    }
    BigInt quot = R / num;
    quot *= num;
    return R == quot;
}


class Finder {
public :
    Finder() {
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

        for (const auto& R : R_small) {
            R_big.push_back(BigInt{ R });
        }
    }

    uint64_t A(uint64_t n) {
        // First check the R(k) values that can fit in 64-bits
        for (uint64_t i = 2; i < 20; ++i) {
            if (0 == R_small[i] % n)
                return i;
        }

        for (uint64_t i = 20; i < 2 * n; ++i) {
            if (i == R_big.size()) {
                BigInt tmp{ R_big.back() };
                tmp *= 10;
                tmp += 1;
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


    std::vector<uint64_t> get_numbers(uint64_t max_value) {
        std::vector<uint64_t> ret;

        PrimeHelper helper;

        auto tmp = helper.get_primes(max_value);
        std::unordered_set<uint64_t> primes{ tmp.begin(), tmp.end() };

        // Process only odd numbers, otherwise GCD(num, 10) != 1
        for (uint64_t num = 7; num <= max_value; num += 2) {
            // Skip multiples of 5, otherwise GCD(num, 10) != 1.
            if (num % 5 == 0)
                continue;
            // Skip prime numbers, we are only looking for composites.
            if (primes.find(num) != primes.end())
                continue;

            if (0 == (num - 1) % A(num))
                ret.push_back(num);
        }

        return ret;
    }


private :
    std::vector<uint64_t> R_small;
    std::vector<BigInt> R_big;

};


int main()
{
    std::cout << "Hello World!\n";

    {
        Finder finder;
        std::vector<uint64_t> numbers{ 7, 27, 41, 91, 143, 259, 451, 481, 703 };
        for (const auto& n : numbers)
            std::cout << "A(" << n << ") = " << finder.A(n) << std::endl;
    }

    {
        Finder finder;
        auto numbers = finder.get_numbers(14800);
        for (const auto& num : numbers)
            std::cout << num << std::endl;
        std::cout << "Found " << numbers.size() << " composite numbers" << std::endl;
        std::cout << "Sum = " << std::accumulate(numbers.begin(), numbers.end(), 0ULL);
    }
}
