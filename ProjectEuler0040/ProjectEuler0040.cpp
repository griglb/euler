// An irrational decimal fraction is created by concatenating the positive integers:
//              0.123456789101112131415161718192021...
//                           -
// It can be seen that the 12th digit of the fractional part is 1.
// If d_n represents the nth digit of the fractional part, find the value of the following expression.
// d_1 × d_10 × d_100 × d_1000 × d_10000 × d_100000 × d_1000000


#include <iostream>

#include "big_int.h"


BigInt::Digits get_champernownes_digits(size_t num_digits) {
    BigInt::Digits ret{ 0 };

    for (size_t i = 1; i < num_digits; ++i) {
        BigInt num{ i };
        auto tmp = num.get_digits();

        std::copy(tmp.begin(), tmp.end(), std::back_inserter(ret));
        if (ret.size() >= num_digits)
            break;
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    auto digits = get_champernownes_digits(20);
    for (const auto dig : digits) {
        std::cout << static_cast<int32_t>(dig) << std::endl;
    }
    std::cout << "d_12 = " << static_cast<int32_t>(digits[12]) << std::endl;

    digits = get_champernownes_digits(1'000'001);
    std::cout << "d_1 = " << static_cast<int32_t>(digits[1]) << std::endl;
    std::cout << "d_10 = " << static_cast<int32_t>(digits[10]) << std::endl;
    std::cout << "d_100 = " << static_cast<int32_t>(digits[100]) << std::endl;
    std::cout << "d_1000 = " << static_cast<int32_t>(digits[1'000]) << std::endl;
    std::cout << "d_10000 = " << static_cast<int32_t>(digits[10'000]) << std::endl;
    std::cout << "d_100000 = " << static_cast<int32_t>(digits[100'000]) << std::endl;
    std::cout << "d_1000000 = " << static_cast<int32_t>(digits[1'000'000]) << std::endl;
    std::cout << "product = " << static_cast<int32_t>(digits[1]) * static_cast<int32_t>(digits[10]) *
                                 static_cast<int32_t>(digits[100]) * static_cast<int32_t>(digits[1'000]) *
                                 static_cast<int32_t>(digits[10'000]) * static_cast<int32_t>(digits[100'000]) *
                                 static_cast<int32_t>(digits[1'000'000]) << std::endl;
}
