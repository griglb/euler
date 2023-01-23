// The smallest number expressible as the sum of a prime square, prime cube, and prime fourth power is 28.
// In fact, there are exactly four numbers below fifty that can be expressed in such a way :
//      28 = 2^2 + 2^3 + 2^4
//      33 = 3^2 + 2^3 + 2^4
//      49 = 5^2 + 2^3 + 2^4
//      47 = 2^2 + 3^3 + 2^4
//
// How many numbers below fifty million can be expressed as the sum of a prime square, prime cube, and prime fourth power?


#include <cmath>
#include <iostream>
#include <set>

#include "prime_helper.h"


uint64_t num_sums(uint64_t max_value) {
    PrimeHelper helper;

    auto primes = helper.get_primes(max_value);

    std::set<uint64_t> squares, cubes, quads;
    for (const auto &p : primes) {
        auto prod = p * p;
        if (prod > max_value)
            break;
        squares.insert(prod);
        prod *= p;
        if (prod < max_value) {
            cubes.insert(prod);
            prod *= p;
            if (prod < max_value)
                quads.insert(prod);
        }
    }

    std::set<uint64_t> sums;

    for (const auto & p2 : squares) {
        for (const auto & p3 : cubes) {
            if (p2 + p3 > max_value)
                break;

            for (const auto & p4 : quads) {
                auto sum = p2 + p3 + p4;
                if (sum > max_value)
                    break;
                sums.insert(sum);
            }
        }
    }

    // for (const auto &s : sums)
    //     std::cout << s << std::endl;

    return sums.size();
}


int main()
{
    std::cout << "Hello World!\n";

    std::cout << num_sums(50) << std::endl;
    std::cout << num_sums(50'000'000) << std::endl;
}
