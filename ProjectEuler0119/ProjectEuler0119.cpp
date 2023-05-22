// The number 512 is interesting because it is equal to the sum of its digits
// raised to some power : 5 + 1 + 2 = 8, and 8^3 = 512.
// Another example of a number with this property is 614656 = 28^4.
//
// We shall define an to be the nth term of this sequence and insist that a
// number must contain at least two digits to have a sum.
//
// You are given that a_2 = 512 and a_10 = 614656.
//
// Find a_30.

#include <iostream>
#include <numeric>
#include <set>
#include <vector>

#include "big_int.h"


// std::vector<uint64_t> get_interesting_numbers(uint8_t how_many) {
//     std::vector<uint64_t> ret;

//     uint64_t curr_num{ 9 };

//     while (ret.size() < how_many) {
//         curr_num += 1;
//         if (0 == (curr_num % 1'000'000))
//             std::cout << curr_num << "\t" << ret.size() << std::endl;
//         auto digits = BigInt{curr_num}.get_digits();
//         uint64_t sum = std::accumulate(digits.begin(), digits.end(), 0ULL);
//         if (1 == sum)
//             continue;
//         if ((curr_num % sum) != 0)
//             continue;
//         if ((((curr_num % 2) == 0) != ((sum % 2) == 0)) ||
//             (((curr_num % 3) == 0) != ((sum % 3) == 0)) ||
//             (((curr_num % 5) == 0) != ((sum % 5) == 0)))
//             continue;
//         uint64_t power{ sum };
//         while (curr_num > power)
//             power *= sum;
//         if (curr_num == power)
//             ret.push_back(power);
//     }

//     return ret;
// }


std::set<uint64_t> get_interesting_numbers(uint8_t how_many) {
    std::set<uint64_t> ret;

    std::vector<uint64_t> powers(100);
    for (uint64_t i = 0; i < 100; ++i)
        powers[i] = i;

    while (ret.size() < how_many) {
        for (size_t i = 2; i < powers.size(); ++i) {
            powers[i] *= i;

            auto digits = BigInt{powers[i]}.get_digits();
            const uint64_t sum = std::accumulate(digits.begin(), digits.end(), 0ULL);
            if (sum == i)
                ret.insert(powers[i]);
        }
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    {
        auto nums = get_interesting_numbers(30);
        size_t i{ 1 };
        for (const auto &num : nums)
            std::cout << i++ << " : " << num << std::endl;
    }
}

// 1 : 81
// 2 : 512
// 3 : 2401
// 4 : 4913
// 5 : 5832
// 6 : 17576
// 7 : 19683
// 8 : 234256
// 9 : 390625
// 10 : 614656
