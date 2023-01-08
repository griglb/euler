// The square root of 2 can be written as an infinite continued fraction.
//   sqrt(2) = 1 + 1 / (2 + 1 / (2 + 1 / (2 + 1 / (2 + 1  / (2 + ...)))))
// The infinite continued fraction can be written, sqrt(2) = [1; (2)], (2) indicates that 2 repeats ad infinitum.
// In a similar way, sqrt(23) = [4; (1, 3, 1, 8)].
// It turns out that the sequence of partial values of continued fractions for square roots provide
// the best rational approximations. Let us consider the convergents for sqrt(2).
//   1 + 1/2 = 3/2
//   1 + 1 / (2 + 1/2) = 7/5
//   1 + 1 / (2 + 1 / (2 + 1/2)) = 17/12
//   1 + 1 / (2 + 1 / (2 + 1 / (2 + 1/2))) = 41/29
// Hence the sequence of the first ten convergents for sqrt(2) are:
//   1, 3/2, 7/5, 17/12, 41/29, 99/70, 239/169, 577/408, 1393/985, 3363/2378, ...
// What is most surprising is that the important mathematical constant,
//   e = [2; 1, 2, 1, 1, 4, 1, 1, 6, 1, ... , 1, 2k, 1, ...].
// The first ten terms in the sequence of convergents for e are:
//   2, 3, 8/3, 11/4, 19/7, 87/32, 106/39, 193/71, 1264/465, 1457/536, ...
// The sum of digits in the numerator of the 10th convergent is 1 + 4 + 5 + 7 = 17.
// Find the sum of digits in the numerator of the 100th convergent of the continued fraction for e.

#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

#include "continued_fractions.h"


int main()
{
    std::cout << "Hello World!\n";

    {
        std::cout << "sqrt(2)" << std::endl;
        Fraction fraction{ 1, {} };
        for (int8_t i = 0; i < 10; ++i) {
            fraction.second.push_back(2);
            auto frac = get_convergent(fraction);
            std::cout << frac.first << " / " << frac.second << std::endl;
        }
    }

    {
        std::cout << "e" << std::endl;
        Repeat terms{ 1,2,1,1,4,1,1,6,1,1,8,1 };
        Fraction fraction{ 2, {} };
        for (const auto &t : terms) {
            fraction.second.push_back(t);
            auto frac = get_convergent(fraction);
            std::cout << frac.first << " / " << frac.second << std::endl;
        }
    }

    {
        std::vector<std::pair<BigInt, BigInt>> convergents{ {BigInt{2}, BigInt{1}} };
        Repeat terms;
        for (int8_t i = 1; i < 34; ++i) {
            terms.push_back(1);
            terms.push_back(2 * i);
            terms.push_back(1);
        };
        Fraction fraction{ 2, {} };
        for (const auto& t : terms) {
            fraction.second.push_back(t);
            convergents.push_back(get_convergent(fraction));
        }
        int16_t ind{ 0 };
        for (const auto& frac : convergents) {
            std::cout << ++ind << "\t" << frac.first << " / " << frac.second << std::endl;
        }
        auto digits = convergents[99].first.get_digits();
        uint64_t sum = std::accumulate(digits.begin(), digits.end(), 0ULL);
        std::cout << "sum = " << sum << std::endl;
    }
}

