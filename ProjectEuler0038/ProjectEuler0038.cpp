// ProjectEuler0038.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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
