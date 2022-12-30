// If we take 47, reverse and add, 47 + 74 = 121, which is palindromic.

// Not all numbers produce palindromes so quickly. For example,

// 349 + 943 = 1292,
// 1292 + 2921 = 4213
// 4213 + 3124 = 7337

// That is, 349 took three iterations to arrive at a palindrome.

// Although no one has proved it yet, it is thought that some numbers, like 196, never produce a palindrome.
// A number that never forms a palindrome through the reverse and add process is called a Lychrel number.
// Due to the theoretical nature of these numbers, and for the purpose of this problem, we shall assume
// that a number is Lychrel until proven otherwise. In addition you are given that for every number below
// ten-thousand, it will either (i) become a palindrome in less than fifty iterations, or, (ii) no one,
// with all the computing power that exists, has managed so far to map it to a palindrome. In fact,
// 10677 is the first number to be shown to require over fifty iterations before producing a 
// palindrome: 4668731596684224866951378664 (53 iterations, 28-digits).

// Surprisingly, there are palindromic numbers that are themselves Lychrel numbers; the first example is 4994.

// How many Lychrel numbers are there below ten-thousand?

#include <algorithm>
#include <iostream>

#include "big_int.h"


bool is_palindrome(const BigInt& number) {
    const auto digits = number.get_digits();
    auto reverse_digits = digits;
    std::reverse(reverse_digits.begin(), reverse_digits.end());
    return digits == reverse_digits;
}


bool is_lychrel(uint64_t number) {
    BigInt curr_number {number};
    BigInt flip_number;

    for (int8_t i = 0; i < 55; ++i) {
        const auto digits = curr_number.get_digits();
        auto reverse_digits = digits;
        std::reverse(reverse_digits.begin(), reverse_digits.end());
        flip_number.set_digits(reverse_digits);
//        std::cout << curr_number << " + " << flip_number << " = ";
        curr_number += flip_number;
//        std::cout << curr_number << std::endl;

        if (is_palindrome(curr_number))
            return false;
    }

    return true;
}


uint64_t num_lychrel_numbers(uint64_t max_val) {
    uint64_t count {0};

    for (uint64_t num = 1; num < max_val; ++num) {
        if (is_lychrel(num))
            count++;
    }
    return count;
}

int main()
{
    std::cout << "Hello World!\n";

    std::cout << is_lychrel(196) << std::endl;
    std::cout << is_lychrel(349) << std::endl;
    std::cout << is_lychrel(4994) << std::endl;
    std::cout << is_lychrel(10677) << std::endl;

    std::cout << num_lychrel_numbers(10'000) << std::endl;
}
