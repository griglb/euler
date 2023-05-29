// Let p_n be the nth prime : 2, 3, 5, 7, 11, ..., and let r be the remainder when
// (p_n - 1)^n + (p_n + 1)^n is divided by (p_n)^2
//
// For example, when n = 3, p_3 = 5 and 4^3 + 5^3 = 280 == 5 mod 25.
//
// The least value of n for which the remainder first exceeds 10^9 is 7037.
//
// Find the least value of n for which the remainder first exceeds 10^10.


#include <cstdint>
#include <iostream>

#include "prime_helper.h"


uint64_t get_remainder(uint64_t n, uint64_t p_n) {
    const uint64_t f1{ p_n - 1 };
    const uint64_t f2{ p_n + 1 };
    const uint64_t sq{ p_n * p_n };

    uint64_t t1{ 1 };
    uint64_t t2{ 1 };
    for (uint64_t ex = 0; ex < n; ++ex) {
        t1 *= f1;
        t1 %= sq;
        t2 *= f2;
        t2 %= sq;
    }

    return (t1 + t2) % sq;
}


void solve_for() {
    static PrimeHelper helper;
    auto primes = helper.get_primes(300'000);
    std::cout << primes.size() << " primes" << std::endl;
    uint64_t min_rem{ 1 };

    for (uint64_t n = 1; n < 100'000; ++n) {
        auto rem = get_remainder(n, primes[n - 1]);
        for (int32_t i = 0; i < 11; ++i) {
            if (rem > min_rem) {
                std::cout << min_rem << "\t" << n << "\t" << rem << std::endl;
                min_rem *= 10;
            }
        }
        if (rem > 10'000'000'000)
            return;
    }
}


int main()
{    
    std::cout << "Hello World!\n";

    //{
    //    std::cout << get_remainder(3) << std::endl;
    //    std::cout << get_remainder(7037) << std::endl;
    //}

    {
        solve_for();
    }
}
