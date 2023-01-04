// Take the number 192 and multiply it by each of 1, 2, and 3:
//     192 × 1 = 192
//     192 × 2 = 384
//     192 × 3 = 576
// By concatenating each product we get the 1 to 9 pandigital, 192384576.
// We will call 192384576 the concatenated product of 192 and (1,2,3)
// The same can be achieved by starting with 9 and multiplying by 1, 2, 3, 4, and 5,
// giving the pandigital, 918273645, which is the concatenated product of 9 and (1,2,3,4,5).
// What is the largest 1 to 9 pandigital 9-digit number that can be formed as the
// concatenated product of an integer with (1,2, ... , n) where n > 1?


#include <algorithm>
#include <iostream>
#include <set>

#include "big_int.h"


std::vector<uint64_t> get_pandigital_multiples() {
    const std::multiset<int8_t> all_digits{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::vector<uint64_t> ret;

    for (uint64_t num = 1; num < 10000; ++num) {
        std::multiset<int8_t> curr_digits;
        uint64_t curr_value{ 0 };
        for (uint64_t mult = 1; mult < 10; ++mult) {
            auto tmp = (BigInt{ num * mult }).get_digits();
            curr_digits.insert(tmp.begin(), tmp.end());
            for (const auto& digit : tmp) {
                curr_value *= 10;
                curr_value += digit;
            }
            if (curr_digits.size() < 9) {
                continue;
            }
            if (curr_digits.size() > 9) {
                break;
            }
            if (all_digits == curr_digits) {
                ret.push_back(curr_value);
            }
        }
    }

    std::sort(ret.begin(), ret.end());

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    auto multiples = get_pandigital_multiples();
    for (const auto &mult : multiples) {
        std::cout << mult << std::endl;
    }
}
