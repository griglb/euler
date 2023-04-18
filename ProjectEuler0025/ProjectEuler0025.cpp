// The Fibonacci sequence is defined by the recurrence relation:
//     F(n) = F(n−1) + F(n−2), where F(1) = 1 and F(2) = 1.
// Hence the first 12 terms will be:
//     F(1)  = 1
//     F(2)  = 1
//     F(3)  = 2
//     F(4)  = 3
//     F(5)  = 5
//     F(6)  = 8
//     F(7)  = 13
//     F(8)  = 21
//     F(9)  = 34
//     F(10) = 55
//     F(11) = 89
//     F(12) = 144
// The 12th term, F12, is the first term to contain three digits.
// What is the index of the first term in the Fibonacci sequence to contain 1000 digits?


#include <iostream>
#include <utility>

#include "big_int.h"


std::pair<int64_t, BigInt> get_smallest_fibonacci_with_n_digits(uint16_t num_digits) {
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
