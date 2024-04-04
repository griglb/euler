// Let D(n) be the n-th positive integer that has the sum of its digits a prime.
// For example, D(61) = 157 and D(10^8) = 403539364
// Find D(10^16).


#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <unordered_set>
#include <vector>

#include "prime_helper.h"

uint64_t digit_sum(uint64_t n) {
    uint64_t sum{ 0 };
    while (n > 0) {
        sum += n % 10;
        n /= 10;
    }

    return sum;
}


std::vector<uint64_t> find_d(uint64_t max_n) {
    std::vector<uint64_t> ret{ 0 };

    const std::unordered_set<uint64_t> primes { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47,
                                                53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107,
                                                109, 113, 127, 131, 137, 139, 149, 151, 157, 163 };

    for (uint64_t n = 2; n <= max_n; n++) {
        uint64_t sum{ 0 };
        auto tmp = n;
        while (tmp > 0) {
            sum += tmp % 10;
            tmp /= 10;
        }

        if (primes.find(sum) != primes.cend()) {
            ret.push_back(n);
        }
    }

    return ret;
}

uint64_t d_n(uint64_t n) {
    const std::unordered_set<uint64_t> primes { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47,
                                                53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107,
                                                109, 113, 127, 131, 137, 139, 149, 151, 157, 163 };

    uint64_t ret{ 0 };
    for (uint64_t i = 2; ; ++i) {
        // if (i % 1'000'000 == 0) {
        //     std::cout << i << std::endl;
        // }

        uint64_t sum{ 0 };
        auto tmp = i;
        while (tmp > 0) {
            sum += tmp % 10;
            tmp /= 10;
        }

        if (primes.find(sum) != primes.cend()) {
            if (++ret == n) {
                return i;
            }
        }
    }
}


int main()
{
    std::cout << "Hello World!\n";

    {
        auto d_n = find_d(2000);
        uint64_t n = 0;
        for (auto d : d_n) {
            std::cout << n++ << "\t" << d << "\t" << digit_sum(d) << std::endl;
        }
    }

    {
        std::cout << "D(61) = " << d_n(61) << std::endl;
        std::cout << "D(10^8) = " << d_n(100'000'000) << std::endl;
    }

    {
        PrimeHelper helper;
        helper.get_primes(100'000'000);
    }

    return 0;
}
