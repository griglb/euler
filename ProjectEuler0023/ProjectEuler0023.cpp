// ProjectEuler0023.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <set>

#include "prime_helper.h"


std::vector<int32_t> get_abundant_numbers(int max_val) {
    PrimeHelper helper;
    std::vector<int32_t> ret;

    for (int i = 2; i < max_val; ++i) {
        uint64_t sum{ 0 };
        for (const auto& fact : helper.get_proper_divisors(i)) {
            sum += fact;
        }
        if (sum > i) {
            ret.push_back(i);
        }
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    std::vector<int32_t> abundants = get_abundant_numbers(28123);
    std::cout << "abundants " << abundants.size() << std::endl;
    //for (const auto& num : abundants) {
    //    std::cout << num << std::endl;
    //}

    std::set<int32_t> sums;
    for (const auto& num1 : abundants) {
        for (const auto& num2 : abundants) {
            sums.insert(num1 + num2);
        }
    }

    std::cout << "sums " << sums.size() << std::endl;
    //for (const auto& num : sums) {
    //    std::cout << num << std::endl;
    //}

    std::set<int32_t> all_numbers;
    for (int i = 1; i < 28124; ++i)
        all_numbers.insert(i);

    for (const auto& num : sums) {
        all_numbers.erase(num);
    }

    int64_t sum{ 0 };
    for (const auto& num : all_numbers) {
        std::cout << num << std::endl;
        sum += num;
    }
    std::cout << "all_numbers " << all_numbers.size() << " " << sum << std::endl;
}
