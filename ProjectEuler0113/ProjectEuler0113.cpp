// Working from left-to-right if no digit is exceeded by the digit to its left it
// is called an increasing number; for example, 134468.
//
// Similarly if no digit is exceeded by the digit to its right it is called a
// decreasing number; for example, 66420.
//
// We shall call a positive integer that is neither increasing nor decreasing a
// "bouncy" number; for example, 155349.
//
// As n increases, the proportion of bouncy numbers below n increases such that
// there are only 12951 numbers below one-million that are not bouncy and only
// 277032 non-bouncy numbers below 10^10.
//
// How many numbers below a googol(10^100) are not bouncy?


#include <array>
#include <iostream>
#include <numeric>

#include "big_int.h"


// Unlike Problem 112, we can't interate over a googol, we need to calculate
// the number of non-bouncy numbers of a given size.
// Can we base the count on the last digit?

// 1 digit numbers - all 9 are non-bouncy
// 2 digit numbers - all 90 are non-bouncy
//                   9 are of form XX
//                   36 are increasing of form XY where Y > X (X > 0)
//                     1 of form X2
//                     2 of form X3
//                     3 of form X4
//                     4 of form X5
//                     5 of form X6
//                     6 of form X7
//                     7 of form X8
//                     8 of form X9
//                   45 are decreasing of form YX where Y > X (Y > 0, so X=0 allowed)
//                     9 of form Y0
//                     8 of form Y1
//                     7 of form Y2
//                     6 of form Y3
//                     5 of form Y4
//                     4 of form Y5
//                     3 of form Y6
//                     2 of form Y7
//                     1 of form Y8
// 3 digit numbers - 375 are non-bouncy (Problem 112 says there are 525 bouncy numbers under 1000)
//                   9 of form XXX
//                 156 = 36 + 120 are increasing
//                    2 = 1 + 1 of form XY2
//                    5 = 2 + 3 of form XY3
//                    9 = 3 + 6 of form XY4
//                   14 = 4 + 10 of form XY5
//                   20 = 5 + 15 of form XY6
//                   27 = 6 + 21 of form XY7
//                   35 = 7 + 28 of form XY8
//                   44 = 8 + 36 of form XY9
//                 210 = 45 + 165 are decreasing
//                   54 = 9 + 45 of form YX0
//                   44 = 8 + 36 of form YX1
//                   36 = 7 + 28 of form YX2
//                   27 = 6 + 21 of form YX3
//                   20 = 5 + 15 of form YX4
//                   14 = 4 + 10 of form YX5
//                    9 = 3 + 6 of form YX6
//                    5 = 2 + 3 of form YX7
//                    2 = 1 + 1 of form YX8
// 4 digit numbers - 1200 are non-bouncy (8325 bouncy numbers under 10000 => 8325 - 525 = 7800 bouncy 4 digit numbers)
//                   9 of the form XXXX
//                 486 = 36 + 450 are increasing
//                    3 = 1 + 2 of form XYZ2
//                    9 = 2 + 7 of form XYZ3
//                    19 = 3 + 16 of form XYZ4
//                    34 = 4 + 30 of form XYZ5
//                    55 = 5 + 50 of form XYZ6
//                    83 = 6 + 77 of form XYZ7
//                    119 = 7 + 112 of form XYZ8
//                    164 = 8 + 156 of form XYZ9
//                 705 = 45 + 660 are decreasing
//                    219 = 9 + 210 of form ZYX0
//                    164 = 8 + 156 of form ZYX1
//                    119 = 7 + 112 of form ZYX2
//                    83 = 6 + 77 of form ZYX3
//                    55 = 5 + 50 of form ZYX4
//                    34 = 4 + 30 of form ZYX5
//                    19 = 3 + 16 of form ZYX6
//                    9 = 2 + 7 of form ZYX7
//                    3 = 1 + 2 of form ZYX8


bool is_bouncy(int64_t number) {
    if (number < 100)
        return false;

    auto digits = BigInt{number}.get_digits();

    if (digits[1] > digits[0]) {
        for (size_t d = 2; d < digits.size(); ++d) {
            if (digits[d-1] > digits[d]) {
                return true;
            }
        }
    }
    else if (digits[1] < digits[0]) {
        for (size_t d = 2; d < digits.size(); ++d) {
            if (digits[d-1] < digits[d]) {
                return true;
            }
        }
    }
    else {
        digits.erase(digits.cbegin());
        BigInt num;
        num.set_digits(digits);
        return is_bouncy(num.to_int());
    }

    return false;
}


struct Counts {
    uint64_t num_bouncy;
    uint64_t num_increasing;
    uint64_t num_decreasing;
    uint64_t num_repeated;
};


Counts get_counts(int8_t num_digits) {
    uint64_t min_val{1};
    for (int8_t n = 1; n < num_digits; ++n)
        min_val *= 10;
    uint64_t max_val{min_val * 10};

    Counts ret {0,0,0,0};
    for (uint64_t num = min_val; num < max_val; ++num) {
        if (is_bouncy(num))
            ++ret.num_bouncy;
        else {
            auto digits = BigInt{num}.get_digits();
            if (*digits.cbegin() == *digits.crbegin())
                ++ret.num_repeated;
            else if (*digits.cbegin() > *digits.crbegin())
                ++ret.num_increasing;
            else
                ++ret.num_decreasing;
        }
    }

    return ret;
}


uint64_t get_num_non_bouncy(int8_t num_digits) {
    using CountsByLastDigit = std::array<uint64_t, 10>;

    CountsByLastDigit num_increasing{0};
    CountsByLastDigit num_decreasing{0};
    CountsByLastDigit num_repeating{0, 1, 1, 1, 1, 1, 1, 1, 1, 1};

    for (int16_t n = 1; n < num_digits; ++n) {
        std::cout << n << std::endl;

        const CountsByLastDigit prev_num_increasing{num_increasing};
        for (int16_t i = 2; i < 10; ++i) {
            num_increasing[i] = std::accumulate(num_repeating.cbegin()+2, num_repeating.cbegin()+i+1, uint64_t{0}) +
                                std::accumulate(prev_num_increasing.cbegin()+2, prev_num_increasing.cbegin()+i+1, uint64_t{0});
            std::cout << num_increasing[i] << "\t";
        }
        std::cout << std::endl;

        const CountsByLastDigit prev_num_decreasing{num_decreasing};
        for (int16_t i = 8; i >= 0; --i) {
            num_decreasing[i] = std::accumulate(num_repeating.cbegin()+i+1, num_repeating.cbegin()+10, uint64_t{0}) +
                                std::accumulate(prev_num_decreasing.cbegin()+i, prev_num_decreasing.cbegin()+9, uint64_t{0});
            std::cout << num_decreasing[i] << "\t";
        }
        std::cout << std::endl;
    }

    return std::accumulate(num_increasing.cbegin(), num_increasing.cend(), uint64_t{0}) +
           std::accumulate(num_decreasing.cbegin(), num_decreasing.cend(), uint64_t{0}) + 
           std::accumulate(num_repeating.cbegin(), num_repeating.cend(), uint64_t{0});

}


int main()
{
    std::cout << "Hello World!\n";

    {
        auto counts = get_counts(6);
        std::cout << "num_bouncy = " << counts.num_bouncy << std::endl;
        std::cout << "num_increasing = " << counts.num_increasing << std::endl;
        std::cout << "num_decreasing = " << counts.num_decreasing << std::endl;
        std::cout << "num_repeated = " << counts.num_repeated << std::endl;
        std::cout << "total = " << counts.num_bouncy + counts.num_increasing + counts.num_decreasing + counts.num_repeated << std::endl;
    }

    {
        uint64_t sum{0};
        for (int8_t n = 1; n < 101; ++n)
            sum += get_num_non_bouncy(n);
        std::cout << sum << std::endl;
//        std::cout << get_num_non_bouncy(6) << std::endl;
    }
}
