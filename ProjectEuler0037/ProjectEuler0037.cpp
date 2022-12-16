// ProjectEuler0037.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "prime_helper.h"


std::set<uint64_t> get_truncatable_primes() {
    constexpr int32_t max_val{ 1'000'000 };
    std::set<uint64_t> ret;

    PrimeHelper helper;

    const auto tmp = helper.get_primes(max_val);
    std::set<uint64_t> primes{ tmp.begin(), tmp.end() };

    for (uint64_t num = 10; num < max_val && ret.size() < 11; ++num) {
        // Skip composite numbers
        if (primes.find(num) == primes.end())
            continue;

        // Now truncate right to left
        bool is_good{ true };
        uint64_t copy{ num / 10 };
        while (copy > 0) {
            is_good &= primes.find(copy) != primes.end();
            copy /= 10;
        }
        if (!is_good)
            continue;
        // Now truncate left to right
        is_good = true;
        std::stringstream str;
        str << num;
        auto digits = str.str();
        digits.erase(0, 1);
        while (digits.size() > 0) {
            std::stringstream str;
            str << digits;
            str >> copy;
            is_good &= primes.find(copy) != primes.end();
            digits.erase(0, 1);
        }
        if (!is_good)
            continue;

        ret.insert(num);
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    auto primes = get_truncatable_primes();
    uint64_t sum{ 0 };
    for (const auto& p : primes) {
        std::cout << p << std::endl;
        sum += p;
    }
    std::cout << "sum = " << sum << std::endl;
}
