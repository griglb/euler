// If we calculate a^2 mod 6 for 0 <= a <= 5 we get : 0, 1, 4, 3, 4, 1.
//
// The largest value of a such that is a^2 == a mod 6 is 4.
// Let's call M(n) the largest value of a < n such that a^2 == a (mod n).
// So M(6) = 4.
//
// Find sum M(n) for 1 <= n <= 10^7.


#define _CRT_SECURE_NO_WARNINGS 1
#include <chrono>
#include <ctime>
#include <iostream>
#include <unordered_set>

#include "prime_helper.h"


// We know that all modular systems have cyclical behavior when you look
// at the series n, n^2, n^3, ... n^k.  The period of this cycle will vary
// for different starting n values for each modulus M. The period of that
// cycle will reach its maximum when the modulus is prime, with a length
// of M-1 for prime M for all starting n>1.
// We therefore know that M(n) = 1 for prime n, because the only way the
// cycle can have a period of M-1 is if no numbers >1 are idempotent.
// 
//


int64_t M_brute(int64_t n) {
    int64_t ret{ 1 };

    for (int64_t i = 2; i < n; ++i) {
        if ((i * i % n) == i)
            ret = i;
    }

    return ret;
}


int64_t sum_M_brute(int64_t max_n) {
    int64_t ret{ 1 };

    for (int64_t n = 2; n <= max_n; ++n)
        ret += M_brute(n);

    return ret;
}


int64_t M_factors(int64_t n, const Factorization &factors) {
    int64_t ret{ 1 };

    for (const auto& [prime, exp] : factors) {
        int64_t base = prime;
        for (int64_t i = 2; i <= exp; ++i)
            base *= prime;

        for (int64_t i = base; i < n; i += base) {
            if ((i * i % n) == i)
                ret = std::max(ret, i);
        }
    }

    return ret;
}


int64_t sum_M_primes(int64_t max_n) {
    int64_t ret{ 1 };

    {
        auto now = std::chrono::system_clock::now();
        auto now_t = std::chrono::system_clock::to_time_t(now);
        std::cout << "0  " << std::ctime(&now_t) << std::endl;
    }

    PrimeHelper helper;
    auto tmp = helper.get_primes(max_n);
    std::unordered_set<int64_t> primes{ tmp.cbegin(), tmp.cend() };

    for (int64_t n = 2; n <= max_n; ++n) {
        if (n % 100'000 == 0) {
            auto now = std::chrono::system_clock::now();
            auto now_t = std::chrono::system_clock::to_time_t(now);
            std::cout << n << "  " << std::ctime(&now_t) << std::endl;
        }
        // Skip the prime numbers
        if (primes.find(n) != primes.cend()) {
            ret += 1;
            continue;
        }

        Factorization factors = helper.get_factorization_fast(n);
        // Skip the powers of a single prime
        if (factors.size() == 1) {
            ret += 1;
            continue;
        }

        ret += M_factors(n, factors);
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    {
        for (int64_t n = 2; n <= 100; ++n)
            std::cout << "M(" << n << ") = " << M_brute(n) << std::endl;
    }

    {
        PrimeHelper helper;
        helper.get_primes(100);

        for (int64_t n = 2; n <= 100; ++n) {
            auto factors = helper.get_factorization_fast(n);
            std::cout << "M(" << n << ") = " << M_brute(n) << "\t" << M_factors(n, factors) << std::endl;
        }
    }
    
    {
        std::cout << "sum M to 100 = " << sum_M_brute(100) << "\t" << sum_M_primes(100) << std::endl;
        std::cout << "sum M to 1000 = " << sum_M_brute(1000) << "\t" << sum_M_primes(1000) << std::endl;
        std::cout << "sum M to 10000 = " << sum_M_brute(10000) << "\t" << sum_M_primes(10000) << std::endl;
//        std::cout << "sum M to 100000 = " << sum_M_brute(100000) << "\t" << sum_M_primes(100000) << std::endl;
    }

    {
        auto ans = sum_M_primes(10'000'000);
        std::cout << "The answer is " << ans << std::endl;
    }
}
