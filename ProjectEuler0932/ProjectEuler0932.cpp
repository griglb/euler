// For the year 2025
//                  2025 = (20 + 25)^2
//
// Given positive integers a and b, the concatenation ab we call a 2025-number if ab = (a + b)^2.
// Other examples are 3025 and 81.
// Note 9801 is not a 2025-number because the concatenation of 98 and 1 is 981.
//
// Let T(n) be the sum of all 2025-numbers with n digits or less.
// You are given T(4) = 5131.
//
// Find T(16).


#include <cmath>
#include <iostream>


// My initial thought was that you only had to worry about values where the units digit of
// the square was the same as the sum, ie a+b ends in 1, 5, or 6.  The examples given support
// this.  But some brute force code proves this false.

// We just need to iterate over all the sums (a+b) and look at their squares.  For each square,
// partition it into the two substrings a & b and see if their sum is correct.
// We could stringify, split, and convert back to integers, or just use integer division and mod
// with powers of 10.

int64_t T(int32_t n) {
    int64_t ret = 0;

    double max_sum = std::pow(10, n / 2);
    std::cout << "max_sum = " << max_sum << std::endl;
    for (int64_t sum = 2; sum <= max_sum; ++sum) {
        const int64_t square = sum * sum;

        for (const int64_t pow10 : {10, 100, 1'000, 10'000, 100'000, 1'000'000, 10'000'000, 100'000'000}) {
            if (square >= pow10) {
                const int64_t a = square / pow10;
                const int64_t b = square % pow10;
                if (b < pow10 / 10) continue;
                if (a + b == sum) {
                    ret += square;
                    std::cout << sum << "\t" << square << "\t" << a << "\t" << b << std::endl;
                }
            }
        }
    }

    return ret;
}

int main()
{
    std::cout << "Hello World!\n";

    {
        auto ret = T(4);
        std::cout << "T(4) = " << ret << std::endl;
    }
    {
        auto ret = T(16);
        std::cout << "T(16) = " << ret << std::endl;
    }
}
