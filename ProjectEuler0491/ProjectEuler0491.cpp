// We call a positive integer double pandigital if it uses all the digits 0 to 9
// exactly twice (with no leading zero).
// For example, 40561817703823564929 is one such number.
//
// How many double pandigital numbers are divisible by 11?


#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

#include "combinatorics.h"


// As it turns out, that example number 40561817703823564929 is a multiple of 11.

// A trick for determining if a number is a multiple of 11 is to do the difference
// sum.  For the number abcdef, the alternating sum is a - b + c - d + e - f.  If
// this is divisible by 11 (or 0), the the number is divisible by 11.

// For a 20 digit number, we have 10 digits being added and 10 subtracted.  So we
// can split the 20 digits into two sets of 10 and then calculate the alternating
// sum is a multiple of 11.  If so, then we need to calculate how many permutations
// of those sets of digits exist.  If not, we skip that partition.
// The are C(20, 10) = 20! / (10! * 10!) = 184756 partitions to consider.`


// The get_combinations() function will use indices [0, 19], so we can convert them
// to the single digits with % 10.
// To avoid double counting, we will treat the 10 values as the set of digits that
// are added to the alternating sum, and the rest will be subtracted.  We know that
// the sum of all 20 sigits = 2*T(9) = 90.  So the sum of the subtractions is 90 minus
// the sum of the additions.

// Just calling get_combinations() will cause a lot of duplicate counting of sets of
// digits.  The sets {0, 1, 2, 3, 4, 5, 6, 7, 8, 9} and {0, 1, 2, 3, 4, 5, 6, 7, 8, 19}
// look different, but turn into the same pandigital set of digits.
// So we need to prune the sets to not repeat different sets with the same digits.
// 


bool is_multiple_digit_set(const Combination& adders) {
    const int64_t sum_add = std::accumulate(adders.begin(), adders.end(), 0ll,
        [](const int64_t& sum, const uint64_t& value) -> int64_t {
            return sum + (value % 10);
        });
    //std::cout << sum_add << std::endl;

    const int64_t sum_sub = 90 - sum_add;
    const int64_t alt_sum = sum_add - sum_sub;

    return 0 == (alt_sum % 11);
}


template <typename coll>
uint64_t number_permutations(const coll& adders) {
    constexpr uint64_t nine_factorial = 9 * 8 * 7 * 6 * 5 * 4 * 3 * 2;
    constexpr uint64_t ten_factorial = 10 * nine_factorial;
    const uint64_t pow_two[] = { 1, 2, 4, 8, 16, 32 };

    std::vector<int32_t> add_counts(10, 0);
    std::vector<int32_t> sub_counts(10, 2);

    for (const auto& value : adders) {
        const uint64_t digit = value % 10;
        ++add_counts[digit];
        --sub_counts[digit];
    }

    const int64_t num_add_pairs = std::count(add_counts.begin(), add_counts.end(), 2ll);
    uint64_t num_add_perms;
    if (0 == add_counts[0]) {
        // No 0s, so 10! permutations before duplicates.
        num_add_perms = ten_factorial / pow_two[num_add_pairs];
    }
    else if (1 == add_counts[0]) {
        // There is one 0, so first digit has 9 options, followed by 9! permutations.
        num_add_perms = 9 * nine_factorial / pow_two[num_add_pairs];
    }
    else {
        // There are two 0s, so first digit has 8 options, followed by 9! permutations.
        num_add_perms = 8 * nine_factorial / pow_two[num_add_pairs];
    }

    const int64_t num_sub_pairs = std::count(sub_counts.begin(), sub_counts.end(), 2);
    const uint64_t num_sub_perms = ten_factorial / pow_two[num_sub_pairs];

    return num_add_perms * num_sub_perms;
}


uint64_t solve_it() {
    uint64_t ret{ 0 };

    using DigitSet = std::multiset<uint64_t>;
    using UniqueDigits = std::set<DigitSet>;

    auto combos = get_combinations(20, 10);
    UniqueDigits uniques;
    for (const auto& combo : combos) {
        if (is_multiple_digit_set(combo)) {
            DigitSet new_set;
            for (const auto& value : combo) {
                new_set.insert(value % 10);
            }
            uniques.insert(new_set);
        }
    }

    for (const auto& digits : uniques) {
        ret += number_permutations(digits);
    }

    return ret;
}

int main()
{
    std::cout << "Hello World!\n";

    //{
    //    auto combos = get_combinations(20, 10);
    //    std::cout << "Created " << combos.size() << " combinations" << std::endl;
    //}

    //{
    //    std::cout << is_multiple_digit_set({ 4, 5, 1, 11, 7, 3, 12, 15, 14, 2 }) << std::endl;
    //}

    //{
    //    std::vector<Combination> good_sets;
    //    auto combos = get_combinations(20, 10);
    //    for (const auto& combo : combos) {
    //        if (is_multiple_digit_set(combo)) {
    //            good_sets.push_back(combo);
    //        }
    //    }
    //    std::cout << "Found " << good_sets.size() << " sets" << std::endl;
    //}

    {
        std::cout << solve_it() << std::endl;
    }
}
