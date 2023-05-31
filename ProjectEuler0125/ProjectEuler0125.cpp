// The palindromic number 595 is interesting because it can be written as the sum of consecutive squares:
//      6^2 + 7^2 + 8^2 + 9^2 + 10^2 + 11^2 + 12^2.
//
// There are exactly eleven palindromes below one-thousand that can be written as consecutive square sums,
// and the sum of these palindromes is 4164.  Note that 1 = 0^2 + 1^2 has not been included as this problem
// is concerned with the squares of positive integers.
//
// Find the sum of all the numbers less than 10^8 that are both palindromic
// and can be written as the sum of consecutive squares.


#include <iostream>
#include <numeric>
#include <vector>


// Is it faster to find all the palindromes and figure out which ones are sums of squares,
// or to iterate over all possible sums of squares and identify which are palindromes?
// For sums < 10^8, the numbers being squared are all < 10^4.


bool is_palindrome(uint64_t number) {
    std::vector<uint16_t> digits;
    while (number > 0) {
        digits.push_back(number % 10);
        number /= 10;
    }

    std::vector<uint16_t> rev_digits{ digits };
    std::reverse(rev_digits.begin(), rev_digits.end());
    return digits == rev_digits;
}


std::vector<uint64_t> find_palindromes(uint64_t max_value) {
    std::vector<uint64_t> ret;

    uint64_t max_root = sqrt(max_value);

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


int main()
{
    std::cout << "Hello World!\n";

    {
        auto pals = find_palindromes(1000);
        //for (const auto& p : pals) {
        //   std::cout << p << std::endl;
        //}
        std::cout << "sum = " << std::accumulate(pals.begin(), pals.end(), 0ULL) << std::endl;
    }

    {
        auto pals = find_palindromes(1e8);
        //for (const auto& p : pals) {
        //    std::cout << p << std::endl;
        //}
        std::cout << "sum = " << std::accumulate(pals.begin(), pals.end(), 0ULL) << std::endl;
    }
}
