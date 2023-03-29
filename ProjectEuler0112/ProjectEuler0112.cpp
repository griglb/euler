// Working from left-to-right if no digit is exceeded by the digit to its left it is
// called an increasing number; for example, 134468.
//
// Similarly if no digit is exceeded by the digit to its right it is called a
// decreasing number; for example, 66420.
//
// We shall call a positive integer that is neither increasing nor decreasing a
// "bouncy" number; for example, 155349.
//
// Clearly there cannot be any bouncy numbers below one-hundred, but just over
// half of the numbers below one-thousand (525) are bouncy.
// In fact, the least number for which the proportion of bouncy numbers first
// reaches 50 % is 538.
//
// Surprisingly, bouncy numbers become more and more common and by the time we
// reach 21780 the proportion of bouncy numbers is equal to 90%.
//
// Find the least number for which the proportion of bouncy numbers is exactly 99%.

#include <iostream>

#include "big_int.h"


bool is_bouncy(int64_t number) {
    if (number < 100)
        return false;

    auto digits = BigInt{number}.get_digits();

    // Is 111 bouncy?
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

uint64_t threshold(double bouncy_percent) {
    uint64_t ret{99};

    uint64_t num_bouncy{0};

    while (num_bouncy <= bouncy_percent * ret) {
        ++ret;
        num_bouncy += is_bouncy(ret);
//        std::cout << num_bouncy << "\t" << ret << std::endl;
        if (num_bouncy*1.0 == bouncy_percent * ret)
            break;
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    {
        std::cout << is_bouncy(134468) << std::endl;
        std::cout << is_bouncy(66420) << std::endl;
        std::cout << is_bouncy(155349) << std::endl;
    }

    {
        uint64_t count{0};
        for (uint64_t num = 1; num < 1000; ++num)
            count += is_bouncy(num);
        std::cout << count << std::endl;
    }

    {
        std::cout << threshold(0.5) << std::endl;
        std::cout << threshold(0.9) << std::endl;
        std::cout << threshold(0.99) << std::endl;
    }

}
