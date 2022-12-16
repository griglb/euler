// ProjectEuler0030.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <numeric>

#include "big_int.h"


std::vector<uint64_t> digit_powers(uint64_t max_val, int32_t power) {
    std::vector<uint64_t> ret;

    for (uint64_t num = 10; num <= max_val; ++num) {
        BigInt tmp{ num };
        auto digits = tmp.get_digits();
        uint64_t sum{ 0 };
        for (const auto dig : digits) {
            uint64_t val{ 1 };
            for (int32_t i = 0; i < power; ++i)
                val *= dig;
            sum += val;
        }
        if (sum == num)
            ret.push_back(num);
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    auto powers = digit_powers(10000, 4);
    for (const auto p : powers)
        std::cout << p << std::endl;

    std::cout << "fifth powers" << std::endl;
    powers = digit_powers(4000000, 5);
    for (const auto p : powers)
        std::cout << p << std::endl;
    std::cout << "sum = " << std::accumulate(powers.begin(), powers.end(), 0) << std::endl;
}
