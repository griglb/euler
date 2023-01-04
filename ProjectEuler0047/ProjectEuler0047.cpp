// The first two consecutive numbers to have two distinct prime factors are:
//      14 = 2 × 7
//      15 = 3 × 5
// The first three consecutive numbers to have three distinct prime factors are:
//      644 = 2^2 × 7 × 23
//      645 = 3 × 5 × 43
//      646 = 2 × 17 × 19
// Find the first four consecutive integers to have four distinct prime factors each.
// What is the first of these numbers?


#include <iostream>

#include "prime_helper.h"

PrimeHelper helper;


uint64_t get_first_in_quartet() {

    for (uint64_t num = 1; num < 1000000; ++num) {
        if (helper.get_factorization(num).size() != 4)
            continue;
        if (helper.get_factorization(num + 1).size() != 4)
            continue;
        if (helper.get_factorization(num + 2).size() != 4)
            continue;
        if (helper.get_factorization(num + 3).size() != 4)
            continue;
        return num;
    }

    return 0;
}


void print_factorization(uint64_t num) {
    std::cout << num << " =";
    for (const auto [prime, exponent] : helper.get_factorization(num)) {
        std::cout << " " << prime;
        if (exponent > 1)
            std::cout << "^" << exponent;
    }
    std::cout << std::endl;
}


int main()
{
    std::cout << "Hello World!\n";

    auto answer = get_first_in_quartet();
    print_factorization(answer);
    print_factorization(answer + 1);
    print_factorization(answer + 2);
    print_factorization(answer + 3);
}
