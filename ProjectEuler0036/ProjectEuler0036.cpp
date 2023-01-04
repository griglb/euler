// The decimal number, 585 = 10010010012 (binary), is palindromic in both bases.
// Find the sum of all numbers, less than one million, which are palindromic in base 10 and base 2.
// (Please note that the palindromic number, in either base, may not include leading zeros.)


#include <algorithm>
#include <iostream>
#include <sstream>

#include "big_int.h"


bool is_palindrome(const BigInt &number) {
    auto digits = number.get_digits();
    auto flipped = digits;
    std::reverse(flipped.begin(), flipped.end());
    return digits == flipped;
}


BigInt get_binary_digits(int32_t number) {
    std::stringstream str;
    str << std::oct << number;
    BigInt ret;
    for (const auto octal_digit : str.str()) {
        ret *= 1000;
        switch (octal_digit) {
            case '0' :
            break;

            case '1' :
            ret += 1;
            break;

            case '2' :
            ret += 10;
            break;

            case '3' :
            ret += 11;
            break;

            case '4' :
            ret += 100;
            break;

            case '5' :
            ret += 101;
            break;

            case '6' :
            ret += 110;
            break;

            case '7' :
            ret += 111;
            break;

        }
    }
    return ret;
}


std::vector<int32_t> get_palindromes(int32_t max_val) {
    std::vector<int32_t> ret;
    for (int32_t i = 1; i < max_val; ++i) {
        if (is_palindrome(BigInt{ i }) && is_palindrome(get_binary_digits(i))) {
            ret.push_back(i);
        }
    }
    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    std::cout << is_palindrome(BigInt{ 660 }) << std::endl;
    std::cout << is_palindrome(BigInt{ 585 }) << std::endl;
    std::cout << get_binary_digits(585) << std::endl;
    std::cout << is_palindrome(get_binary_digits(585)) << std::endl;
    std::cout << get_binary_digits(7887) << std::endl;

    auto palindromes = get_palindromes(1'000'000);
    int32_t sum{ 0 };
    for (const auto& pal : palindromes) {
        std::cout << pal << " = " << get_binary_digits(pal) << std::endl;
        sum += pal;
    }
    std::cout << "sum = " << sum << std::endl;
}
