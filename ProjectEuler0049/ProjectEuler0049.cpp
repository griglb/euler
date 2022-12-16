// ProjectEuler0049.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <array>
#include <iostream>
#include <map>
#include <set>
#include <tuple>
#include <vector>

#include "prime_helper.h"
#include "combinatorics.h"


std::tuple<uint64_t, uint64_t, uint64_t> permutation_sequence() {
    std::tuple<uint64_t, uint64_t, uint64_t> ret;

    PrimeHelper helper;
    auto tmp = helper.get_primes(10000);
    std::set<uint64_t> primes{ tmp.begin(), tmp.end() };
    primes.erase(1487);
    primes.erase(4817);
    primes.erase(8147);

    auto perm_list = get_permutations(4, 4);

    for (const auto& orig : primes) {
        if (orig < 1000)
            continue;

        // treat orig as 4 digit number abcd
        std::array<uint64_t, 4> digits{ orig / 1000, (orig / 100) % 10, (orig / 10) % 10, orig % 10 };

        std::set<uint64_t> other_primes;

        for (const auto& digit_perms : perm_list) {
            uint64_t num = 1000 * digits[digit_perms[0]] +
                            100 * digits[digit_perms[1]] +
                             10 * digits[digit_perms[2]] +
                                  digits[digit_perms[3]];
            if (num < 1000)
                continue;
            if (primes.find(num) != primes.end())
                other_primes.insert(num);
        }
        if (other_primes.size() < 3)
            continue;

        std::map<uint64_t, std::vector<std::pair<uint64_t, uint64_t>>> differences;
        for (const auto p1 : other_primes) {
            for (const auto p2 : other_primes) {
                if (p2 <= p1)
                    continue;
                auto diff = p2 - p1;
                if (!differences.count(diff))
                    differences[diff] = {};
                differences[diff].push_back(std::make_pair(p2, p1));
            }
        }
        for (const auto [diff, pairs] : differences) {
            if (pairs.size() == 2) {
                if (pairs[0].second == pairs[1].first)
                {
                    if (pairs[0].first > pairs[1].first)
                        return std::make_tuple(pairs[1].second, pairs[1].first, pairs[0].first);
                    else
                        return std::make_tuple(pairs[0].second, pairs[0].first, pairs[1].first);
                }
                else if (pairs[1].second == pairs[0].first) {
                    if (pairs[0].first > pairs[1].first)
                        return std::make_tuple(pairs[1].second, pairs[1].first, pairs[0].first);
                    else
                        return std::make_tuple(pairs[0].second, pairs[0].first, pairs[1].first);
                }
            }
        }
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    auto tuple = permutation_sequence();
    auto [a, b, c] = tuple;
    std::cout << a << "\t" << b << "\t" << c << std::endl;
}
