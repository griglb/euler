// ProjectEuler1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>


std::vector<int32_t> get_multiples(const std::vector<int32_t> factors, int32_t max_val) {
    std::vector<int32_t> multiples;
    for (int32_t i = 1; i < max_val; ++i) {
        for (const auto fac : factors) {
            if (i % fac == 0) {
                multiples.push_back(i);
                break;
            }
        }
    }

    return multiples;
}

int32_t main()
{
    std::cout << "Hello World!\n";
    std::vector<int32_t> multiples = get_multiples({ 3, 5 }, 1000);
    int32_t sum = 0;
    for (const auto val : multiples) {
        std::cout << val << std::endl;
        sum += val;
    }
    std::cout << sum << std::endl;
}
