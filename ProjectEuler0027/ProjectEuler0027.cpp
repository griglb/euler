// Euler discovered the remarkable quadratic formula:
//                          n^2 + n + 41
// It turns out that the formula will produce 40 primes for the consecutive integer values 0 <= n <= 39.
// However, when n = 40, 40^2 + 40 + 41 = 40(40 + 1) + 41 is divisible by 41, and certainly when
// n = 41, 41^2 + 41 + 41 is clearly divisible by 41.
// The incredible formula
//                          n^2 - 79n + 1601
// was discovered, which produces 80 primes for the consecutive values 0 <= n <= 79.
// The product of the coefficients, −79 and 1601, is −126479.
// Considering quadratics of the form:
//     n^2 + an + b
// where |a| < 1000 and |b| <= 1000
// where |n| is the modulus/absolute value of n
// e.g.  |11| = 11 and |-4| = 4
// Find the product of the coefficients, a and b, for the quadratic expression that produces
// the maximum number of primes for consecutive values of n, starting with n=0.


#include <iostream>
#include <set>

#include "prime_helper.h"


bool is_prime(uint64_t number) {
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
