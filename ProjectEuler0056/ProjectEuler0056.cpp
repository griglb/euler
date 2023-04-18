// A googol (10^100) is a massive number: one followed by one-hundred zeros;
// 100^100 is almost unimaginably large: one followed by two-hundred zeros.
// Despite their size, the sum of the digits in each number is only 1.

// Considering natural numbers of the form, a^b, where a, b < 100, what is the maximum digital sum?

#include <algorithm>
#include <iostream>

#include "big_int.h"

uint64_t biggest_sum() {
    uint64_t max_sum{0};
    for (int16_t a = 1; a < 100; ++a) {
        for (int16_t b = 1; b < 100; ++b) {
            BigInt product{a};
            for (int16_t exp = 1; exp < b; ++exp)
                product *= a;
            const auto digits = product.get_digits();
            uint64_t sum{0};
            for (const auto &dig : digits) {
                sum += dig;
            }
            if (sum > max_sum) {
                max_sum = sum;
                std::cout << "new max sum of " << max_sum << " for " << a << "^" << b << std::endl;
            }
        }
    }
    return max_sum;
}


int main()
{
    std::cout << "Hello World!\n";

    std::cout << biggest_sum() << std::endl;
}
