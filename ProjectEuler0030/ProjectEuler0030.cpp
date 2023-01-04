// Surprisingly there are only three numbers that can be written as the sum of fourth powers of their digits:
//     1634 = 1^4 + 6^4 + 3^4 + 4^4
//     8208 = 8^4 + 2^4 + 0^4 + 8^4
//     9474 = 9^4 + 4^4 + 7^4 + 4^4
// As 1 = 1^4 is not a sum it is not included.
// The sum of these numbers is 1634 + 8208 + 9474 = 19316.
// Find the sum of all the numbers that can be written as the sum of fifth powers of their digits.


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
