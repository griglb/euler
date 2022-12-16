// ProjectEuler0050.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <set>

#include "prime_helper.h"


uint64_t get_sum_prime(int32_t max_val) {
    PrimeHelper helper;

    auto tmp = helper.get_primes(max_val);
    std::set<uint64_t> primes{ tmp.begin(), tmp.end() };

    int32_t most_terms{ 0 };
    uint64_t first_prime{ 0 };
    uint64_t ret{ 0 };

    for (auto iter = primes.begin(); iter != primes.end(); ++iter) {
        uint64_t sum{ *iter };
        auto iter2 = iter;
        int32_t num_primes{ 2 };
        for (++iter2; iter2 != primes.end(); ++iter2, ++num_primes) {
            sum += *iter2;
            if (sum > max_val)
                break;
            if (primes.find(sum) == primes.end())
                continue;
            if (num_primes > most_terms) {
                most_terms = num_primes;
                first_prime = *iter;
                ret = sum;
            }
        }
    }

    std::cout << "longest sum has " << most_terms << " terms starting at " << first_prime << std::endl;
    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    std::cout << get_sum_prime(100) << std::endl;
    std::cout << get_sum_prime(1'000) << std::endl;
    std::cout << get_sum_prime(1'000'000) << std::endl;
}
