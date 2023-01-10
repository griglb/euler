// A perfect number is a number for which the sum of its proper divisors is exactly
// equal to the number. For example, the sum of the proper divisors of 28 would be
// 1 + 2 + 4 + 7 + 14 = 28, which means that 28 is a perfect number.
// A number n is called deficient if the sum of its proper divisors is less than n
// and it is called abundant if this sum exceeds n.
// As 12 is the smallest abundant number, 1 + 2 + 3 + 4 + 6 = 16, the smallest
// number that can be written as the sum of two abundant numbers is 24.
// By mathematical analysis, it can be shown that all integers greater than 28123 can
// be written as the sum of two abundant numbers. However, this upper limit cannot be
// reduced any further by analysis even though it is known that the greatest number
// that cannot be expressed as the sum of two abundant numbers is less than this limit.
// Find the sum of all the positive integers which cannot be written as the sum of two abundant numbers.


#include <iostream>
#include <set>

#include "prime_helper.h"


std::vector<int32_t> get_abundant_numbers(uint64_t max_val) {
    PrimeHelper helper;
    std::vector<int32_t> ret;

    for (uint64_t i = 2; i < max_val; ++i) {
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
