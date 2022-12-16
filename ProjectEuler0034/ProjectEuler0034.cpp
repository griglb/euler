// ProjectEuler0034.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>


uint64_t factorial(int n) {
    uint64_t prod{ 1 };
    for (int i = 2; i <= n; ++i)
        prod *= i;
    return prod;
}


std::vector<uint64_t> get_digit_factorials() {
    std::vector<uint64_t> ret;

    for (uint64_t num = 10; num < 1'000'000; ++num) {
        uint64_t n{ num };
        uint64_t sum{ 0 };
        for (uint64_t n = num; n > 0; n /= 10) {
            sum += factorial(n % 10);
        }
        if (sum == num)
            ret.push_back(num);
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    for (int i = 1; i < 10; ++i) {
        std::cout << i << "! = " << factorial(i) << std::endl;
    }

    auto factorials = get_digit_factorials();
    uint64_t sum{ 0 };
    for (const auto& num : factorials) {
        std::cout << num << std::endl;
        sum += num;
    }
    std::cout << "sum = " << sum << std::endl;
}
