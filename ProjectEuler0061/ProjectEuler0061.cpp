// Triangle, square, pentagonal, hexagonal, heptagonal, and octagonal numbers are all
// figurate (polygonal) numbers and are generated by the following formulae:
// Triangle         P3,n=n(n+1)/2       1, 3, 6, 10, 15, ...
// Square           P4,n=n^2            1, 4, 9, 16, 25, ...
// Pentagonal       P5,n=n(3n−1)/2      1, 5, 12, 22, 35, ...
// Hexagonal        P6,n=n(2n−1)        1, 6, 15, 28, 45, ...
// Heptagonal       P7,n=n(5n−3)/2      1, 7, 18, 34, 55, ...
// Octagonal        P8,n=n(3n−2)        1, 8, 21, 40, 65, ...
//
// The ordered set of three 4-digit numbers: 8128, 2882, 8281, has three interesting properties.
//  1. The set is cyclic, in that the last two digits of each number is the first two digits
//     of the next number (including the last number with the first).
//  2. Each polygonal type: triangle (P3,127=8128), square (P4,91=8281), and pentagonal (P5,44=2882),
//     is represented by a different number in the set.
//  3. This is the only set of 4-digit numbers with this property.
// Find the sum of the only ordered set of six cyclic 4-digit numbers for which each polygonal type:
// triangle, square, pentagonal, hexagonal, heptagonal, and octagonal, is represented by a different number in the set.


#include <iostream>
#include <map>

#include "combinatorics.h"


std::vector<uint16_t> get_triangular_numbers() {
    // P(n)   = n(n+1)/2
    // P(n+1) = (n+1)(n+2)/2
    // P(n+1) - P(n) = (n^2 + 3n + 2 - n^2 - n) / 2
    // P(n+1) - P(n) = (2n + 2)/2 = n + 1
    std::vector<uint16_t> ret;
    uint16_t num{0};
    for (int32_t n = 1; n < 150; ++n) {
        num += n;
        if (num < 1000L)
            continue;
        if (num >= 10000L)
            break;
        ret.push_back(num);
    }
    return ret;
}


std::vector<uint16_t> get_square_numbers() {
    // P(n)   = n^2
    // P(n+1) = (n+1)^2
    // P(n+1) - P(n) = n^2 + 2n + 1 - n^2
    // P(n+1) - P(n) = 2n + 1
    std::vector<uint16_t> ret;
    uint16_t num{1};
    for (int32_t n = 1; n < 150; ++n) {
        num += 2*n + 1;
        if (num < 1000L)
            continue;
        if (num >= 10000L)
            break;
        ret.push_back(num);
    }
    return ret;
}


std::vector<uint16_t> get_pentagonal_numbers() {
    // P(n)   = n(3n−1)/2
    // P(n+1) = (n+1)(3n+2)/2
    // P(n+1) - P(n) = (3n^2 + 5n + 2 - 3n^2 + n)/2
    // P(n+1) - P(n) = (6n + 2)/2 = 3n + 1
    std::vector<uint16_t> ret;
    uint16_t num{1};
    for (int32_t n = 1; n < 150; ++n) {
        num += 3 * n + 1;
        if (num < 1000L)
            continue;
        if (num >= 10000L)
            break;
        ret.push_back(num);
    }
    return ret;
}


std::vector<uint16_t> get_hexagonal_numbers() {
    // P(n)   = n(2n−1)
    // P(n+1) = (n+1)(2n+1)
    // P(n+1) - P(n) = 2n^2 + 3n + 1 - 2n^2 + n
    // P(n+1) - P(n) = 4n + 1
    std::vector<uint16_t> ret;
    uint16_t num{1};
    for (int32_t n = 1; n < 150; ++n) {
        num += 4 * n + 1;
        if (num < 1000L)
            continue;
        if (num >= 10000L)
            break;
        ret.push_back(num);
    }
    return ret;
}


std::vector<uint16_t> get_heptagonal_numbers() {
    // P(n)   = n(5n−3)/2
    // P(n+1) = (n+1)(5n+2)/2
    // P(n+1) - P(n) = (5n^2 + 7n + 2 - 5n^2 + 3n)/2
    // P(n+1) - P(n) = (10n + 2)/2 = 5n + 1
    std::vector<uint16_t> ret;
    uint16_t num{1};
    for (int32_t n = 1; n < 150; ++n) {
        num += 5 * n + 1;
        if (num < 1000L)
            continue;
        if (num >= 10000L)
            break;
        ret.push_back(num);
    }
    return ret;
}


std::vector<uint16_t> get_octagonal_numbers() {
    // P(n)   = n(3n−2)
    // P(n+1) = (n+1)(3n+1)
    // P(n+1) - P(n) = (3n^2 + 4n + 1 - 3n^2 + 2n)/2
    // P(n+1) - P(n) = 6n + 1
    std::vector<uint16_t> ret;
    uint16_t num{1};
    for (int32_t n = 1; n < 150; ++n) {
        num += 6 * n + 1;
        if (num < 1000L)
            continue;
        if (num >= 10000L)
            break;
        ret.push_back(num);
    }
    return ret;
}


using DigitMap = std::multimap<uint16_t, uint16_t>;
bool find_cycle(const DigitMap &map1, const DigitMap &map2, const DigitMap &map3,
                const DigitMap &map4, const DigitMap &map5, const DigitMap &map6) {
    for (const auto &[key1, value1] : map1) {
        auto range2 = map2.equal_range(value1);
        for (auto iter2 = range2.first; iter2 != range2.second; ++iter2) {
            auto range3 = map3.equal_range(iter2->second);
            for (auto iter3 = range3.first; iter3 != range3.second; ++iter3) {
                auto range4 = map4.equal_range(iter3->second);
                for (auto iter4 = range4.first; iter4 != range4.second; ++iter4) {
                    auto range5 = map5.equal_range(iter4->second);
                    for (auto iter5 = range5.first; iter5 != range5.second; ++iter5) {
                        auto range6 = map6.equal_range(iter5->second);
                        for (auto iter6 = range6.first; iter6 != range6.second; ++iter6) {
                            if (iter6->second == key1) {
                                std::cout << key1 << " " << iter2->first << " " << iter3->first << " " << iter4->first << " " 
                                        << iter5->first << " " <<  range6.first->first << " " << range6.first->second << std::endl;
                                int32_t val1 = 100 * key1 + iter2->first;
                                int32_t val2 = 100 * iter2->first + iter3->first;
                                int32_t val3 = 100 * iter3->first + iter4->first;
                                int32_t val4 = 100 * iter4->first + iter5->first;
                                int32_t val5 = 100 * iter5->first + iter6->first;
                                int32_t val6 = 100 * iter6->first + key1;
                                std::cout << "sum = " << val1 + val2 + val3 + val4 + val5 + val6 << std::endl;
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }

    return false;
}


void find_figurate_cycle() {
    std::array<DigitMap, 6> numbers;

    auto &P3 = numbers[0];
    {
        auto tris = get_triangular_numbers();
        for (const auto &t : tris) {
            if ((t % 100) > 9)
                P3.insert({t / 100, t % 100});
        }
    }

    auto &P4 = numbers[1];
    {
        auto quads = get_square_numbers();
        for (const auto &q : quads)
            if ((q % 100) > 9)
                P4.insert({q / 100, q % 100});
    }

    auto &P5 = numbers[2];
    {
        auto pents = get_pentagonal_numbers();
        for (const auto &p : pents)
            if ((p % 100) > 9)
                P5.insert({p / 100, p % 100});
    }

    auto &P6 = numbers[3];
    {
        auto hexs = get_hexagonal_numbers();
        for (const auto &h : hexs)
            if ((h % 100) > 9)
                P6.insert({h / 100, h % 100});
    }

    auto &P7 = numbers[4];
    {
        auto hepts = get_heptagonal_numbers();
        for (const auto &h : hepts)
            if ((h % 100) > 9)
                P7.insert({h / 100, h % 100});
    }

    auto &P8 = numbers[5];
    {
        auto octs = get_octagonal_numbers();
        for (const auto &o : octs)
            if ((o % 100) > 9)
                P8.insert({o / 100, o % 100});
    }

    for (const auto &num : numbers) {
        for (const auto &[key,value] : num) {
            std::cout << "(" << key << "," << value << ")";
        }
        std::cout << std::endl;
    }

    auto perms = get_permutations(5, 5);
    for (const auto &perm : perms) {
        for (const auto &p : perm)
            std::cout << p << " ";
        std::cout << std::endl;
        find_cycle(numbers[5], numbers[perm[0]], numbers[perm[1]], numbers[perm[2]], numbers[perm[3]], numbers[perm[4]]);
    }
}


int main()
{
    std::cout << "Hello World!\n";

    // {
    //     auto tris = get_triangular_numbers();
    //     for (const auto &t : tris)
    //         std::cout << t << std::endl;
    // }

    // {
    //     auto quads = get_square_numbers();
    //     for (const auto &q : quads)
    //         std::cout << q << std::endl;
    // }

    // {
    //     auto pents = get_pentagonal_numbers();
    //     for (const auto &p : pents)
    //         std::cout << p << std::endl;
    // }

    // {
    //     auto hexs = get_hexagonal_numbers();
    //     for (const auto &h : hexs)
    //         std::cout << h << std::endl;
    // }

    // {
    //     auto hepts = get_heptagonal_numbers();
    //     for (const auto &h : hepts)
    //         std::cout << h << std::endl;
    // }

    // {
    //     auto octs = get_octagonal_numbers();
    //     for (const auto &o : octs)
    //         std::cout << o << std::endl;
    // }

    find_figurate_cycle();
}
