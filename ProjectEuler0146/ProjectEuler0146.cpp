// The smallest positive integer for which the numbers
//     n^2 + 1, n^2 + 3, n^2 + 7, n^2 + 9, n^2 + 13, and n^2 + 27
// are consecutive primes is 10.
// The sum of all such integers below one-million is 1242490.
//
// What is the sum of all such integers n below 150 million?


#include <cmath>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <unordered_set>
#include <vector>

#include "prime_helper.h"


// All the constant addends are odd, so all the n values must be even to yield
// odd sums that have any hope of being prime.

// We know that all primes > 3 are equal to 1 or 5 mod 6:
//      n mod 6 in [0, 2, 4] -> n is even, not a prime
//      n mod 6 = 3 -> n = 0 mod 3, not a prime
// Look at the set of addends: {1, 3, 7, 9, 13, 27}.
// What if the first is 1 mod 6?
//      n^2 + 1 = 1 mod 6
//      n^2 + 3 = 3 mod 6 -> a multiple of 3, not prime
// What if the first is 5 mod 6?
//      n^2 + 1 = 5 mod 6
//      n^2 + 3 = 1 mod 6
//      n^2 + 7 = 5 mod 6
//      n^2 + 9 = 1 mod 6
//      n^2 + 13 = 5 mod 6
//      n^2 + 27 = 1 mod 6
// So:
//      n^2 + 1 = 5 mod 6
//      n^2 = 4 mod 6
// Look at 6x6 multiplication table mod 6:
//         0  1  2  3  4  5
//      0  0  0  0  0  0  0 
//      1  0  1  2  3  4  5
//      2  0  2  4  0  2  4
//      3  0  3  0  3  0  3
//      4  0  4  2  0  4  2
//      5  0  5  4  3  2  1
//
// So for n^2 = 4 mod 6, n = 2,4 mod 6
//


std::vector<uint64_t> solve(uint64_t max_value) {
    std::vector<uint64_t> ret;

    PrimeHelper helper;
    auto primes = helper.get_primes(max_value);

    std::vector other_addends{ 5, 11, 15, 17, 19, 21, 23, 25 };

    for (uint64_t n = 2; n <= max_value; n += 2) {
        uint64_t n2{ n * n };
        uint64_t s1{ n2 + 1 };
        uint64_t s2{ n2 + 3 };
        uint64_t s3{ n2 + 7 };
        uint64_t s4{ n2 + 9 };
        uint64_t s5{ n2 + 13 };
        uint64_t s6{ n2 + 27 };
        uint64_t max_root = std::sqrt(s6);

        bool composite{ false };

        for (const auto p : primes) {
            // Once we get to sqrt of largest potential prime, we can stop testing.
            if (p > max_root)
                break;

            // If any of the sums are multiples of a prime, then exit this for loop.
            if (s1 % p == 0) {
                composite = true;
                break;
            }
            if (s2 % p == 0) {
                composite = true;
                break;
            }
            if (s3 % p == 0) {
                composite = true;
                break;
            }
            if (s4 % p == 0) {
                composite = true;
                break;
            }
            if (s5 % p == 0) {
                composite = true;
                break;
            }
            if (s6 % p == 0) {
                composite = true;
                break;
            }
        }

        // If any of the 6 sums are composite, then go to next n
        if (composite)
            continue;

        // Next we have to test that there are not any other primes in the (s1, s6) range.
        for (const auto& i : other_addends) {
            composite = false;

            for (const auto p : primes) {
                // Once we get to sqrt of largest potential prime, we can stop testing.
                if (p > max_root)
                    break;

                if ((n2 + i) % p == 0) {
                    composite = true;
                    break;
                }
            }

            if (!composite) {
                std::cout << n << "^2 + " << i << " = " << n2 + i << " is prime" << std::endl;
                break;
            }
        }

        if (!composite)
            continue;

        // If we get here, then we have a valid n, print numbers and add to return value.
        std::cout << n << "\t" << s1 << "\t" << s2 << "\t" << s3 << "\t" << s4 << "\t" << s5 << "\t" << s6 << std::endl;
        ret.push_back(n);
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    {
        auto nums = solve(150'000'000);
        std::cout << "Sum = " << std::accumulate(nums.begin(), nums.end(), 0LL);
    }
}
