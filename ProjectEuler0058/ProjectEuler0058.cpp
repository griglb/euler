// Starting with 1 and spiralling anticlockwise in the following way, a square spiral with side length 7 is formed.

// 37 36 35 34 33 32 31
// 38 17 16 15 14 13 30
// 39 18  5  4  3 12 29
// 40 19  6  1  2 11 28
// 41 20  7  8  9 10 27
// 42 21 22 23 24 25 26
// 43 44 45 46 47 48 49

// It is interesting to note that the odd squares lie along the bottom right diagonal,
// but what is more interesting is that 8 out of the 13 numbers lying along both diagonals are prime; that is, a ratio of 8/13 ≈ 62%.

// If one complete new layer is wrapped around the spiral above, a square spiral with side length 9 will be formed.
// If this process is continued, what is the side length of the square spiral for which the ratio of primes along both
// diagonals first falls below 10%?

#include <iostream>
#include <unordered_set>

#include "prime_helper.h"

// Diagonal values are:
//  NE   NW   SW   SE
//   3    5    7    9  - add 2
//  13   17   21   25  - add 4
//  31   37   43   49  - add 6
//  57   65   73   81  - add 8


std::vector<uint64_t> get_diagonals(uint64_t radius) {
    std::vector<uint64_t> ret;

    uint64_t value{1};
    for (uint64_t r = 1; r <= radius; ++r) {
        value += r + r;
        ret.push_back(value);
        value += r + r;
        ret.push_back(value);
        value += r + r;
        ret.push_back(value);
        value += r + r;
        ret.push_back(value);
    }

    return ret;
}


uint64_t get_answer() {
    PrimeHelper helper;

    auto tmp = helper.get_primes(700'000'000);
    std::unordered_set<uint64_t> primes(tmp.begin(), tmp.end());

    uint64_t value{1};
    uint64_t r{1};
    uint64_t num_diag{1 };
    uint64_t num_prime{0};
    while (true) {
        value += r + r;
        std:: cout << value << " ";
        if (primes.find(value) != primes.end()) {
            ++num_prime;
            std::cout << "Y\t";
        }
        else
            std::cout << "N\t";

        value += r + r;
        std:: cout << value << " ";
        if (primes.find(value) != primes.end()) {
            ++num_prime;
            std::cout << "Y\t";
        }
        else
            std::cout << "N\t";

        value += r + r;
        std:: cout << value << " ";
        if (primes.find(value) != primes.end()) {
            ++num_prime;
            std::cout << "Y\t";
        }
        else
            std::cout << "N\t";

        value += r + r;
        std:: cout << value << " ";
        if (primes.find(value) != primes.end()) {
            ++num_prime;
            std::cout << "Y\t";
        }
        else
            std::cout << "N\t";

        num_diag += 4;
        if (num_diag > 10 * num_prime)
            return 2 * r + 1;
        r += 1;
        std::cout << r << "\t" << num_prime << "\t" << num_diag << std::endl;
    }
}


int main()
{
    std::cout << "Hello World!\n";

    {
        auto diagonals = get_diagonals(4);
        for (const auto &diag : diagonals)
            std::cout << diag << std::endl;
    }

    std::cout << get_answer() << std::endl;
}
