// If a box contains twenty-one coloured discs, composed of fifteen blue discsand six red discs,
// and two discs were taken at random, it can be seen that the probability of taking
// two blue discs, P(BB) = (15 / 21)*(14 / 20) = 1 / 2.
//
// The next such arrangement, for which there is exactly 50% chance of taking two blue discs
// at random, is a box containing eighty-five blue discs and thirty-five red discs.
//
// By finding the first arrangement to contain over 10^12 = 1,000,000,000,000 discs in total,
// determine the number of blue discs that the box would contain.


#include <cmath>
#include <iostream>

#include "fraction.h"


// When there are N_B blue discs and N_R red discs, then the probability of 2 blues is:
//      P(BB) = (N_B / (N_B + N_R)) * ((N_B - 1) / (N_B + N_R - 1))
// Let N_B = x, (N_B + N_R) = y, the equation becomes
//      P(BB) = x(x-1) / y(y-1)

// First example - 15 blue, 6 red
//      P(BB) = 15/21 * 14/20
//            = 15/20 * 14/21
//            = 3/4 * 2/3
//            = 1/2
//
// Second example - 85 blue, 35 red
//      P(BB) = 85/120 * 84/119
//            = 85/119 * 84/120
//            = 5/7 * 7/10
//            = 1/2
//
// GCD(85,119) = GCD(34,85) = GCD(34,51) = GCD(17,34) = 17
// GCD(84,120) = GCD(36,84) = GCD(36,48) = GCD(12,36) = GCD(12,24) = 12


void find_answer() {
    double blue_percent = 1.0 / std::sqrt(2.0);

    for (int64_t tot = 1'006'000'000'000; tot < 2'000'000'000'000; ++tot) {
        int64_t blue = std::round(tot * blue_percent);

        if (tot % 100'000 == 0)
            std::cout << tot << "\t" << blue << "\t " << tot - blue << std::endl;

        const auto gcd1 = gcd(blue, tot - 1);
        const auto gcd2 = gcd(blue - 1, tot);

        auto num1 = blue / gcd1;
        auto den1 = (tot - 1) / gcd1;
        auto num2 = (blue - 1) / gcd2;
        auto den2 = tot / gcd2;

        const auto gcd3 = gcd(num1, den2);
        const auto gcd4 = gcd(num2, den1);

        if ((gcd3 != num1) || (gcd4 != num2))
            continue;

        std::cout << "B: " << blue << "\tR: " << tot - blue << "\ttotal: " << tot << std::endl;
        // answer is B: 756872327473 R: 313506783024 total: 1070379110497
        int16_t foo{ 0 };
        std::cin >> foo;
        return;
    }
}


int main()
{
    std::cout << "Hello World!\n";

    std::cout << gcd(85, 119) << std::endl;
    std::cout << gcd(84, 120) << std::endl;
    //std::cout << gcd(707106781190, 1'000'000'000'006) << std::endl;

    find_answer();
}
