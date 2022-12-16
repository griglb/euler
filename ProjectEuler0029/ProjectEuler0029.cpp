// ProjectEuler0029.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <set>

#include "big_int.h"


std::set<BigInt> get_powers(int32_t maxA, int32_t maxB) {
    std::set<BigInt> ret;

    for (int32_t a = 2; a <= maxA; ++a) {
        for (int32_t b = 2; b <= maxB; ++b) {
            BigInt num{ a };
            for (int32_t i = 1; i < b; ++i) {
                num *= a;
            }
            ret.insert(num);
        }
    }
    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    auto powers = get_powers(5, 5);
    for (const auto& p : powers) {
        std::cout << p << std::endl;
    }

    powers = get_powers(100, 100);
    std::cout << "number of powers = " << powers.size() << std::endl;
}
