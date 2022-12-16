// ProjectEuler0025.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <utility>

#include "big_int.h"


std::pair<int64_t, BigInt> get_smallest_fibonacci_with_n_digits(int num_digits) {
    std::vector<BigInt> fibonaccis { BigInt(0), BigInt(1) };

    while (fibonaccis.back().get_digits().size() < num_digits) {
        auto iter = fibonaccis.crbegin();
        BigInt tmp = *iter;
        tmp += *(++iter);
        fibonaccis.push_back(tmp);
    }

    return std::make_pair<>(fibonaccis.size()-1, fibonaccis.back());
}


int main()
{
    std::cout << "Hello World!\n";

    auto answer = get_smallest_fibonacci_with_n_digits(1000);
    std::cout << "F(" << answer.first << ") = " << answer.second << std::endl;
}
