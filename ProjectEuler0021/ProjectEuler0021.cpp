// Let d(n) be defined as the sum of proper divisors of n (numbers less than n which divide evenly into n).
// If d(a) = b and d(b) = a, where a ≠ b, then a and b are an amicable pair and each of a and b are called amicable numbers.
// For example, the proper divisors of 220 are 1, 2, 4, 5, 10, 11, 20, 22, 44, 55 and 110; therefore d(220) = 284.
// The proper divisors of 284 are 1, 2, 4, 71 and 142; so d(284) = 220.
// Evaluate the sum of all the amicable numbers under 10000.


#include <iostream>
#include <set>
#include <vector>

#include "prime_helper.h"


std::vector<int32_t> get_amicable_numbers(int max_val) {
    PrimeHelper helper;
    int internal_max = max_val * 2.6;

    std::vector<uint64_t> sums;
    sums.push_back(-1);
    sums.push_back(0);

    for (int i = 2; i < internal_max; ++i) {
        uint64_t sum{ 0 };
        for (const auto& fact : helper.get_proper_divisors(i)) {
            sum += fact;
        }
        sums.push_back(sum);
    }

    std::vector<int32_t> ret;
    for (int i = 2; i < max_val; ++i) {
        uint64_t index1 = sums[i];
        if (index1 == i) {
            std::cout << "Found perfect number: " << i << std::endl;
            continue;
        }
        if (index1 > internal_max) {
            std::cout << "Sum of divisors of " << i << " is greater than " << max_val << ": " << index1 << std::endl;
            continue;
        }
        if (sums[index1] == i) {
            ret.push_back(i);
        }
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    PrimeHelper helper;
    ULongLongVec divisors = helper.get_proper_divisors(220);
    for (const auto& elem : divisors) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    divisors = helper.get_proper_divisors(284);
    for (const auto& elem : divisors) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    std::vector<int32_t> amicables = get_amicable_numbers(10000);
    int32_t sum{ 0 };
    for (const auto& elem : amicables) {
        std::cout << elem << std::endl;
        sum += elem;
    }
    std::cout << "sum = " << sum << std::endl;

}
