// It is possible to show that the square root of two can be expressed as an infinite continued fraction.
//  sqrt(2) = 1 + 1 / (2 + 1 / (2 + 1 / (2 + 1 / (2 + ...))))

// By expanding this for the first four iterations, we get:
//   1 + 1/2 = 3/2 = 1.5
//   1 + 1 / (2 + 1/2) = 7/5 = 1.4
//   1 + 1 / (2 + 1 / (2 + 1/2)) = 17/12 = 1.41666...
//   1 + 1 / (2 + 1 / (2 + 1 / (2 + 1/2))) = 41/29 = 1.41379...
// The next three expansions are 99/70, 239/169, and 577/408,
// but the eighth expansion, 1393/895, is the first example where the number
// of digits in the numerator exceeds the number of digits in the denominator.

// In the first one-thousand expansions, how many fractions contain a numerator with more digits than the denominator?

#include <algorithm>
#include <iostream>

#include "big_int.h"


using Fraction = std::pair<BigInt, BigInt>;


std::vector<Fraction> get_expansions(int16_t num) {
    std::vector<Fraction> ret;

    Fraction curr_val{3, 2};
    ret.push_back(curr_val);
    for (int16_t i = 1; i < num; ++i) {
        curr_val.first += curr_val.second;
        auto tmp = curr_val.first;
        curr_val.first = curr_val.second;
        curr_val.second = tmp;
        curr_val.first += curr_val.second;
        ret.push_back(curr_val);
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    {
        auto expansions = get_expansions(8);
        for (const auto &fract : expansions) {
            std::cout << fract.first << " / " << fract.second << std::endl;
        }
    }

    {
        auto expansions = get_expansions(1000);
        int16_t count{0};
        for (const auto &fract : expansions) {
            if (fract.first.get_digits().size() > fract.second.get_digits().size())
                ++count;
        }
        std::cout << count << std::endl;
    }
}
