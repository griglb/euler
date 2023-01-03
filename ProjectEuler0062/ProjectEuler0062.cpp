// The cube, 41063625 (3453), can be permuted to produce two other cubes: 56623104 (3843) and 66430125 (4053).
// In fact, 41063625 is the smallest cube which has exactly three permutations of its digits which are also cube.
// Find the smallest cube for which exactly five permutations of its digits are cube.

#include <iostream>
#include <set>
#include <vector>

#include "big_int.h"


std::vector<uint64_t> get_cubes(int16_t num_digits) {
    std::vector<uint64_t> ret;

    uint64_t min_val{1};
    for (int16_t n = 1; n < num_digits; ++n)
        min_val *= 10;
    uint64_t max_val = min_val * 10;

    for (uint64_t i = 1; i < min_val; ++i) {
        uint64_t cube = i * i * i;
        if (cube < min_val)
            continue;
        if (cube >= max_val)
            break;
        ret.push_back(cube);
    }

    return ret;
}


void find_cube_perms(int16_t num_perms) {
    using DigitSet = std::multiset<int16_t>;
    for (int32_t num_digits = 2; num_digits < 16; ++num_digits) {
        std::cout << "num_digits = " << num_digits << std::endl;
        // Get all cubes of this size and their digits
        std::multiset<DigitSet> cube_digits;
        auto cubes = get_cubes(num_digits);
        for (const auto & c : cubes) {
            auto tmp = BigInt{c}.get_digits();
            cube_digits.insert({tmp.begin(), tmp.end()});
        }
        // Iterate over all cubes, checking all permutations
        for (const auto & cd : cube_digits) {
            auto range = cube_digits.equal_range(cd);
            if (std::distance(range.first, range.second) == num_perms) {
                std::cout << "found it" << std::endl;
                for (const auto & c : cubes) {
                    auto tmp = BigInt{c}.get_digits();
                    DigitSet digits{tmp.begin(), tmp.end()};
                    if (digits == cd) {
                        std::cout << c << std::endl;
                    }
                }
                for (const auto & d : cd)
                    std::cout << d << " ";
                std::cout << std::endl;
                return;
            }
        }
    }
}


int main()
{
    std::cout << "Hello World!\n";

    {
        auto cubes = get_cubes(4);
        for (const auto & c : cubes)
            std::cout << c << " ";
        std::cout << std::endl;
    }

    find_cube_perms(3);
    find_cube_perms(4);
    find_cube_perms(5);
}

