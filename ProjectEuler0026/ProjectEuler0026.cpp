// A unit fraction contains 1 in the numerator.
// The decimal representation of the unit fractions with denominators 2 to 10 are given:
//     1/2	= 	0.5
//     1/3	= 	0.(3)
//     1/4	= 	0.25
//     1/5	= 	0.2
//     1/6	= 	0.1(6)
//     1/7	= 	0.(142857)
//     1/8	= 	0.125
//     1/9	= 	0.(1)
//     1/10	= 	0.1 
// Where 0.1(6) means 0.166666..., and has a 1-digit recurring cycle.
// It can be seen that 1/7 has a 6-digit recurring cycle.
// Find the value of d < 1000 for which 1/d contains the longest recurring cycle in its decimal fraction part.


#include <iostream>
#include <utility>
#include <vector>

#include "big_int.h"


// Function that performs long division for numerator/denominator.
// The return is a pair with the non-repeating and repeating parts of the decimal.
std::pair<BigInt::Digits, BigInt::Digits> get_decimal(int16_t numerator, int16_t denominator) {
    if (numerator < 0) {
        throw "Numerator must be non-negative.";
    }
    if (denominator <= 0) {
        throw "Denominator must be positive.";
    }
    // Special case of 0/x == 0.0
    if (0 == numerator) {
        return std::make_pair(BigInt::Digits{ 0 }, BigInt::Digits{ });
    }
    if (numerator == denominator) {
        return std::make_pair(BigInt::Digits{ 1 }, BigInt::Digits{ });
    }
    // Store numerator and denominator in BigInts so they can have as many digits as needed.
    BigInt num{ numerator };
    BigInt den{ denominator };
    // Store the full set of decimal digits in a BigInt, it will be split into repeating and non-repeating parts below.
    BigInt decimal;
    // Keep track of all the remainders left after calculating the next digit, so we can identify the repeat.
    std::vector<std::pair<int16_t, BigInt>> remainders;

    while (true) {
        // Add a 0 to numerator for next decimal digit.
        num *= 10;
        // Calculate the next digit through repeated subtractions of denominator from numerator.
        int16_t quotient{ 0 };
        while (num >= den) {
            num -= den;
            quotient++;
        }
        // Add next digit to the output decimal.
        decimal *= 10;
        decimal += quotient;
        // If the decimal terminates, then there will not be any remainder, so return as non-repeating decimal.
        if (num == 0) {
            return std::make_pair(decimal.get_digits(), BigInt::Digits{ });
        }
        // Look to see if the remainder has been seen before.
        bool repeating{ false };
        for (const auto& old_num : remainders) {
            if ((old_num.first == quotient) && (old_num.second == num)) {
                repeating = true;
                break;
            }
        }
        remainders.push_back({ quotient, num});
        if (repeating)
            break;
    }

    // Split the decimal into non-repeating and repeating sections.
    BigInt::Digits non_rep;
    BigInt::Digits rep;

    auto last_quotient = remainders.back().first;
    auto last_remainder = remainders.back().second;
    auto rep_iter = remainders.begin();
    for (; rep_iter != remainders.end(); ++rep_iter) {
        if ((rep_iter->first == last_quotient) && (rep_iter->second == last_remainder))
            break;
    }
    for (auto iter = remainders.begin(); iter != rep_iter; ++iter) {
        non_rep.push_back(iter->first);
    }
    for (auto iter = rep_iter; iter != remainders.end(); ++iter) {
        rep.push_back(iter->first);
    }
    rep.pop_back();

    return std::make_pair(non_rep, rep);
}


int main()
{
    std::cout << "Hello World!\n";

    int16_t longest_inverse{ 0 };
    BigInt::Digits longest_repeat;

    for (int16_t i = 2; i < 1000; ++i) {
        auto decimal = get_decimal(1, i);
        std::cout << i << "\t";
        for (const auto& digit : decimal.first)
            std::cout << static_cast<int16_t>(digit);
        std::cout << "\t";
        for (const auto& digit : decimal.second)
            std::cout << static_cast<int16_t>(digit);
        std::cout << "\t" << decimal.second.size() << std::endl;
        if (decimal.second.size() > longest_repeat.size()) {
            longest_inverse = i;
            longest_repeat = decimal.second;
        }
    }

    std::cout << std::endl;
    for (const auto& digit : longest_repeat)
        std::cout << static_cast<int16_t>(digit);
    std::cout << std::endl;
    std::cout << longest_inverse << "\t" << longest_repeat.size() << std::endl;
}
