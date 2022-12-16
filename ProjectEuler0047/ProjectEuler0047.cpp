// ProjectEuler0047.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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
