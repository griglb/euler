// It is well known that if the square root of a natural number is not an integer, then it is irrational.
// The decimal expansion of such square roots is infinite without any repeating pattern at all.
//
// The square root of two is 1.41421356237309504880..., and the digital sum of the first one hundred decimal digits is 475.
//
// For the first one hundred natural numbers, find the total of the digital sums of the first one hundred decimal digits
// for all the irrational square roots.


#include <iostream>
#include <set>

#include "big_int.h"


BigInt get_square_root(uint64_t number, int16_t num_digits) {
    // The BigInt class only does integer math, not floating point.  But we can
    // scale the square root by 10^N to make it an integer.  This means multiplying
    // number by 10^2N, which is the same as adding 2N 0's to its digits.
    BigInt num{ number };
    auto digits = num.get_digits();
    for (int16_t i = 0; i < 2 * num_digits; ++i)
        digits.push_back(0);
    num.set_digits(digits);

    // Now use Newton's method for the square root.
    //    f(x) = x^2 - a = 0
    //    x_n+1 = x_n - f(x_n)/f'(x_n)
    //    x_n+1 = x_n - (x_n^2 - a) / (2 * x_n)
    //    x_n+1 = x_n - x_n / 2 + a / (2 * x_n)
    //    x_n+1 = (x_n + a / x_n) / 2

    BigInt x_n{ num };
    for (int i = 0; i < 1000; ++i) {
        // We want x_n+1 = (x_n + (num / x_n) / 2
        // Can do this with a sequence of in-place operations:
        //    x_n+1 = num
        //    x_n+1 /= x_n
        //    x_n+1 += x_n
        //    x_n+1 /= 2
        BigInt x_np1{ num };
        x_np1 /= x_n;
        x_np1 += x_n;
        x_np1 /= 2;

//        std::cout << i << "\t" << x_np1 << std::endl;

        if (x_np1 == x_n)
            break;

        x_n = x_np1;
    }

    return x_n;
}


int main()
{
    std::cout << "Hello World!\n";

    {
        std::cout << get_square_root(2, 20) << std::endl;
    }

    {
        auto root = get_square_root(2, 99);
        uint64_t sum{ 0 };
        for (const auto& dig : root.get_digits())
            sum += dig;
        std::cout << sum << std::endl;
    }

    {
        uint64_t sum{ 0 };
        std::set squares{ 4, 9, 16, 25, 36, 49, 64, 81 };
        for (int i = 2; i < 100; ++i) {
            // Skip the perfect squares.
            if (squares.find(i) != squares.end())
                continue;
            auto root = get_square_root(i, 99);
            for (const auto& dig : root.get_digits())
                sum += dig;
            std::cout << sum << std::endl;
        }
    }
}
