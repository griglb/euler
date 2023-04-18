// 2^15 = 32768 and the sum of its digits is 3 + 2 + 7 + 6 + 8 = 26.
// What is the sum of the digits of the number 2^1000?


#include <iostream>
#include <numeric>
#include <vector>

#include "big_int.h"


std::vector<int16_t> get_power_of_2(int16_t power) {
    std::vector<int16_t> current{ 1 };

    for (int16_t i = 0; i < power; ++i) {
        std::vector<int16_t> tmp;
        for (const auto& digit : current) {
            tmp.push_back(2 * digit);
        }
        int16_t carry{ 0 };
        for (auto iter = tmp.rbegin(); iter != tmp.rend(); ++iter) {
            if (*iter > 9) {
                carry = *iter / 10;
                *iter %= 10;
                if (iter+1 != tmp.rend()) {
                    *(iter + 1) += carry;
                    carry = 0;
                }
            }
        }
        if (carry > 0) {
            tmp.insert(tmp.begin(), carry);
        }

        current = tmp;
    }

    return current;
}


BigInt::Digits get_power_2(int16_t power) {
    BigInt ret{ 1 };
    for (int16_t i = 0; i < power; ++i) {
        ret *= 2;
    }
    return ret.get_digits();
}


void print_digit_sums(int16_t max_power) {
    int64_t num{ 1 };
    for (int16_t i = 0; i < max_power; ++i) {
        int64_t tmp = num;
        int64_t sum{ 0 };
        while (tmp > 0) {
            sum += tmp % 10;
            tmp /= 10;
        }
        auto digits = get_power_of_2(i);
        auto sum2 = std::accumulate(digits.begin(), digits.end(), 0);

        std::cout << i << "\t" << num << "\t" << sum << "\t" << sum2 << std::endl;
        num *= 2;
    }
}


int main()
{
    std::cout << "Hello World!\n";
    print_digit_sums(63);

    for (int16_t i = 0; i < 10; ++i) {
        auto power = get_power_of_2(i);
        std::cout << i << "\t";
        for (const auto& digit : power) {
            std::cout << digit;
        }
        std::cout << std::endl;
    }

    auto digits = get_power_of_2(1000);
    for (const auto& digit : digits) {
        std::cout << digit << " ";
    }
    std::cout << std::endl;

    auto sum = std::accumulate(digits.begin(), digits.end(), 0);
    std::cout << sum << std::endl;

    BigInt::Digits digits2 = get_power_2(1000);
    for (const auto& digit : digits2) {
        std::cout << static_cast<int16_t>(digit) << " ";
    }
    std::cout << std::endl;

    sum = std::accumulate(digits2.begin(), digits2.end(), 0);
    std::cout << sum << std::endl;
}
