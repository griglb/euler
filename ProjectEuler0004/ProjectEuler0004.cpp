// A palindromic number reads the same both ways.
// The largest palindrome made from the product of two 2-digit numbers is 9009 = 91 × 99.
// Find the largest palindrome made from the product of two 3-digit numbers.


#include <math.h>

#include <iostream>
#include <sstream>
#include <string>


bool is_palindrome(int32_t number) {
    std::stringstream str;
    str << number;
    std::string digits = str.str();
    std::string reversed_digits(digits.length(), '0');
    std::copy(digits.rbegin(), digits.rend(), reversed_digits.begin());
    return digits == reversed_digits;
}


int32_t get_largest_palindrome(int8_t digits) {
    int32_t start = static_cast<int32_t>(pow(10, digits-1));
    int32_t end = start * 10;
    int32_t largest = 0;

    std::cout << start << "->" << end << std::endl;
    for (int32_t a = start; a < end; ++a) {
        for (int32_t b = start; b < end; ++b) {
            int32_t c = a * b;
            if (is_palindrome(c)) {
                if (c > largest)
                    largest = c;
            }
        }
    }

    return largest;
}


int main()
{
    std::cout << "Hello World!\n";
    std::cout << get_largest_palindrome(2) << std::endl;
    std::cout << get_largest_palindrome(3) << std::endl;
}
