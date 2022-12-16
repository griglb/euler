// ProjectEuler0043.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "combinatorics.h"


std::vector<uint64_t> get_special_numbers() {
    std::vector<uint64_t> ret;

    auto perms = get_permutations(10, 10);
//    OrderedPermutationList perms{ {1,4,0,6,3,5,7,2,8,9} };

    for (const auto& perm : perms) {
        // d_2 d_3 d_4 divisible by 2 -> d_4 mod 2 == 0
        if (perm[3] % 2)
            continue;
        // d_3 d_4 d_5 divisible by 3 -> d_3 + d_4 + d_5 mod 3 == 0
        if ((perm[2] + perm[3] + perm[4]) % 3)
            continue;
        // d_4 d_5 d_6 divisible by 5 -> d_6 mod 4 in (0, 5)
        if ((perm[5] != 0) && (perm[5] != 5))
            continue;
        // d_5 d_6 d_7 divisible by 7
        if ((100 * perm[4] + 10 * perm[5] + perm[6]) % 7)
            continue;
        // d_6 d_7 d_8 divisible by 11
        if ((100 * perm[5] + 10 * perm[6] + perm[7]) % 11)
            continue;
        // d_7 d_8 d_9 divisible by 13
        if ((100 * perm[6] + 10 * perm[7] + perm[8]) % 13)
            continue;
        // d_8 d_9 d_10 divisible by 17
        if ((100 * perm[7] + 10 * perm[8] + perm[9]) % 17)
            continue;
        ret.push_back(1'000'000'000 * perm[0] +
                        100'000'000 * perm[1] +
                         10'000'000 * perm[2] +
                          1'000'000 * perm[3] +
                            100'000 * perm[4] +
                             10'000 * perm[5] +
                              1'000 * perm[6] +
                                100 * perm[7] +
                                 10 * perm[8] +
                                      perm[9]);
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    auto perms = get_permutations(4, 4);
    for (const auto& perm : perms) {
        for (const auto& el : perm)
            std::cout << el << " ";
        std::cout << std::endl;
    }

    auto numbers = get_special_numbers();
    uint64_t sum{ 0 };
    for (const auto& n : numbers) {
        std::cout << n << std::endl;
        sum += n;
    }
    std::cout << "sum = " << sum << std::endl;

}
