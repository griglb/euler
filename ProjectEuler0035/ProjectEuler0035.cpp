// ProjectEuler0035.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <cmath>
#include <iostream>
#include <set>

#include "prime_helper.h"


std::set<uint64_t> get_circular_primes(int32_t max_val) {
    std::set<uint64_t> ret;

    PrimeHelper helper;

    auto tmp = helper.get_primes(max_val);
    std::set<uint64_t> primes{ tmp.begin(), tmp.end() };

    for (const auto& p : primes) {
        if (p < 10) {
            ret.insert(p);
            continue;
        }

        int32_t num_digits = static_cast<int32_t>(std::ceil(std::log(p) / std::log(10)));
        int32_t div{ 1 };
        for (int32_t n = 1; n < num_digits; ++n)
            div *= 10;

        std::vector<uint64_t> rotations{ p };
        uint64_t tmp{ p };
        for (int32_t n = 0; n < num_digits; ++n) {
            uint64_t first_digit = tmp / div;
            tmp %= div;
            tmp *= 10;
            tmp += first_digit;
            rotations.push_back(tmp);
        }
        bool is_circular{ true };
        for (const auto& rot : rotations) {
            is_circular &= (primes.find(rot) != primes.end());
        }
        if (is_circular)
            ret.insert(p);
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    auto primes = get_circular_primes(1'000'000);
    for (const auto& p : primes) {
        std::cout << p << std::endl;
    }
    std::cout << "count = " << primes.size() << std::endl;
}
