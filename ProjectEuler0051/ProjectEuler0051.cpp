// By replacing the 1st digit of the 2-digit number *3,
// it turns out that six of the nine possible values:
//       13, 23, 43, 53, 73, and 83, are all prime.
// By replacing the 3rd and 4th digits of 56**3 with the same digit,
// this 5-digit number is the first example having seven primes among
// the ten generated numbers, yielding the family:
//       56003, 56113, 56333, 56443, 56663, 56773, and 56993.
// Consequently 56003, being the first member of this family,
// is the smallest prime with this property.
// Find the smallest prime which, by replacing part of the number
// (not necessarily adjacent digits) with the same digit,
// is part of an eight prime value family.


#include <iostream>
#include <numeric>
#include <set>
#include <unordered_set>
#include <vector>

#include "big_int.h"
#include "combinatorics.h"
#include "prime_helper.h"


std::set<std::set<uint64_t>> get_prime_sets(uint64_t num_digits, uint64_t num_replace) {
    std::set<std::set<uint64_t>> ret;

    PrimeHelper helper;

    uint64_t min_val{1};
    for (uint64_t n = 1; n < num_digits; ++n)
        min_val *= 10;
    uint64_t max_val = min_val * 10;

    auto tmp = helper.get_primes(max_val);
    const std::unordered_set<uint64_t> primes{ tmp.begin(), tmp.end() };

    const auto combos = get_combinations(num_digits, num_replace);
    uint64_t max_perm{1};
    for (uint64_t n = num_replace; n < num_digits; ++n)
        max_perm *= 10;
    std::vector<BigInt> perms(max_perm);
    std::iota(perms.begin(), perms.end(), BigInt{ });
    // for (const auto & p : perms)
    //     std::cout << p << " ";
    // std::cout << std::endl;

    std::set<uint8_t> all_digits;
    for (uint8_t i = 0; i < num_digits; ++i)
        all_digits.insert(i);

    for (const auto &combo : combos) {
        // Get positions of digits that don't change for this combination.
        auto fixed_digits = all_digits;
        for (const auto &c : combo)
            fixed_digits.erase(c);
        // Iterate over all possible sets of fixed digits.
        for (const auto &perm : perms) {
            std::set<uint64_t> prime_set;
            auto perm_digits = perm.get_digits();
            while (perm_digits.size() < num_digits - num_replace)
                perm_digits.insert(perm_digits.begin(), 0);
            auto perm_digit_iter = perm_digits.begin();
            std::vector<uint64_t> digits(num_digits);
            for (const auto & fd : fixed_digits)
                digits[fd] = *perm_digit_iter++;
            for (uint64_t new_digit = 0; new_digit < 10; ++new_digit) {
                for (const auto &c : combo)
                    digits[c] = new_digit;
                uint64_t new_value{0};
                for (const auto &d : digits) {
                    new_value *= 10;
                    new_value += d;
                }
                if (new_value < min_val)
                    continue;
                if (primes.find(new_value) != primes.end())
                    prime_set.insert(new_value);
            }
            if (!prime_set.empty())
                ret.insert(prime_set);
        }
    }

    return ret;
}


std::set<uint64_t> get_prime_sets(uint64_t num_primes) {
    uint64_t num_digits{2};
    while (true) {
        for (uint64_t num_replace = 1; num_replace < num_digits; ++num_replace) {
            std::cout << num_digits << "\t" << num_replace << std::endl;
            auto prime_sets = get_prime_sets(num_digits, num_replace);
            for (const auto &set : prime_sets) {
                if (set.size() >= num_primes)
                    return set;
            }
        }
        ++num_digits;
    }
    return {};
}


int main()
{
    std::cout << "Hello World!\n";

    auto combos = get_combinations(5, 2);
    for (const auto &combo : combos) {
        for (const auto &el : combo)
            std::cout << el << " ";
        std::cout << std::endl;
    }

    {
    auto prime_set = get_prime_sets(6);
        for (const auto &prime : prime_set) {
            std::cout << prime << " ";
        }
        std::cout << std::endl;
    }

    {
        auto prime_set = get_prime_sets(7);
        for (const auto &prime : prime_set) {
            std::cout << prime << " ";
        }
        std::cout << std::endl;
    }

    {
        auto prime_set = get_prime_sets(8);
        for (const auto &prime : prime_set) {
            std::cout << prime << " ";
        }
        std::cout << std::endl;
    }
}
