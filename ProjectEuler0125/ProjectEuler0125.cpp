// The palindromic number 595 is interesting because it can be written as the sum of consecutive squares:
//      6^2 + 7^2 + 8^2 + 9^2 + 10^2 + 11^2 + 12^2.
//
// There are exactly eleven palindromes below one-thousand that can be written as consecutive square sums,
// and the sum of these palindromes is 4164.  Note that 1 = 0^2 + 1^2 has not been included as this problem
// is concerned with the squares of positive integers.
//
// Find the sum of all the numbers less than 10^8 that are both palindromic
// and can be written as the sum of consecutive squares.


#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

#include "big_int.h"

// Is it faster to find all the palindromes and figure out which ones are sums of squares,
// or to iterate over all possible sums of squares and identify which are palindromes?
// For sums < 10^8, the numbers being squared are all < 10^4.

// The answer is not 2916867073.

// Closed form expression for sum of n^2:
//    n   n^2   sum 1..n of i^2
//    1     1          1
//    2     4          5
//    3     9         14
//    4    16         30
//    5    25         55
//    6    36         91
//    7    49        140
//    8    64        204
//    9    81        285
//   10   100        385

// Let F(x) = a*x^3 + b*x^2 + c*x + d
//    x               F(x)           sum
//    0                d               0?
//    1       a +    b +   c + d       1
//    2      8a +   4b +  2c + d       5
//    3     27a +   9b +  3c + d      14
//    4     64a +  16b +  4c + d      30
//    5    125a +  25b +  5c + d      55
//    6    216a +  36b +  6c + d      91
//    7    343a +  49b +  7c + d     140
//    8    512a +  64b +  8c + d     204
//    9    729a +  81b +  9c + d     285
//   10   1000a + 100b + 10c + d     385

// F(1) : a + b + c + d = 1
// d(2) = F(2) - F(1) : 7a + 3b + c = 4
// d(3) = F(3) - F(2) : 19a + 5b + c = 9
// dd(3) = d(3) - d(2) : 12a + 2b = 5
// d(3) = F(3) - F(2) : 19a + 5b + c = 9
// d(4) = F(4) - F(3) : 37a + 7b + c = 16
// dd(4) = d(4) - d(3) : 18a + 2b = 7
// dd(4) - dd(3) : 6a = 2
// a = 1/3
// dd(4) -> 18/3 + 2b = 7 -> b = (7 - 6) / 2 = 1/2
// d(2) -> 7/3 + 3/2 + c = 4 -> c = 24/6 - 14/6 - 9/6 = 1 /6
// F(1) -> 1/3 + 1/2 + 1/6 + d = 1 -> d = 0
//
// F(x) = 1/3 * x^3 + 1/2 * x^2 + x/6
// F(x) = (2*x^3 + 3*x^2 + x) / 6
// F(x) = x*(x+1)*(2x+1)/6
//   x   x+1   2x+1   F(x)
//   0    1      1      0
//   1    2      3      1
//   2    3      5      5
//   3    4      7     14
//   4    5      9     30
//   5    6     11     55
//   6    7     13     91
//   7    8     15    140
//   8    9     17    205
//   9   10     19    285
//  10   11     21    385



bool is_palindrome2(uint64_t number) {
    std::vector<uint16_t> digits;
    while (number > 0) {
        digits.push_back(number % 10);
        number /= 10;
    }

    std::vector<uint16_t> rev_digits{ digits };
    std::reverse(rev_digits.begin(), rev_digits.end());
    return digits == rev_digits;
}

bool is_palindrome(uint64_t number) {
    BigInt tmp{ number };
    const auto digits = tmp.get_digits();
    BigInt::Digits rev_digits{ digits };
    std::reverse(rev_digits.begin(), rev_digits.end());
    return digits == rev_digits;
}

std::vector<uint64_t> find_palindromes(uint64_t max_value) {
    std::vector<uint64_t> ret;

    uint64_t max_root = std::sqrt(max_value);

    for (uint64_t first = 1; first < max_root; ++first) {
        uint64_t sum{ first * first };
        for (uint64_t last = first + 1; last < max_root; ++last) {
            sum += last * last;
            if (sum > max_value)
                break;

            if (is_palindrome(sum)) {
                ret.push_back(sum);
                std::cout << first << "\t" << last << "\t" << sum << std::endl;
            }
        }
    }

    return ret;
}


std::vector<uint64_t> find_palindromes2(uint64_t max_value) {
    std::vector<uint64_t> ret;

    uint64_t max_root = std::sqrt(max_value);
std::cout << max_root << std::endl;
    std::vector<uint64_t> sums;
    sums.reserve(max_root + 1);
    for (uint64_t n = 0; n <= max_root; ++n)
        sums.push_back(n*(n+1)*(2*n+1)/6);

    for (uint64_t first = 0; first <= max_root; ++first) {
        for (uint64_t last = first+2; last <= max_root; ++last) {
            uint64_t partial_sum = sums[last] - sums[first];
            if (partial_sum >= max_value)
                break;
            if (is_palindrome(partial_sum)) {
                ret.push_back(partial_sum);
                std::cout << first << "\t" << last << "\t" << partial_sum << std::endl;
            }
        }
    }

    return ret;
}

int main()
{
    std::cout << "Hello World!\n";

    {
        auto pals = find_palindromes2(1000);
        for (const auto& p : pals) {
          std::cout << p << std::endl;
        }
        std::cout << "sum = " << std::accumulate(pals.begin(), pals.end(), 0ULL) << std::endl;
    }
//return 0;
    {
        auto pals = find_palindromes2(1e8);
        //for (const auto& p : pals) {
        //    std::cout << p << std::endl;
        //}
        std::cout << "sum = " << std::accumulate(pals.begin(), pals.end(), 0ULL) << std::endl;
    }
}
