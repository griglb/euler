// The number 13521270961 is the first number that has exactly 25 divisors and
// the sum of those divisors is divisible by 25.
// Find the sum of all numbers less than 10^15 that have exactly 25 divisors
// and the sum of those divisors is divisible by 25.


#include <iostream>

#include "prime_helper.h"


// We know from previous problems that the number of factors is related to the
// exponents in the prime factorization.
// If N = p^a * q^b * ... s^d
// then the number of factors is (a+1)*(b+1)*...*(d+1)
//
// In order to have 25 factors, this means one of two factorizations:
//      N = p^24
//      N = p^4 * q^4
//
// The given number 13521270961 falls into the second category, being
// equal to 11^4 * 31^4.

// So a constructive approach makes this simple.
// For N = p^24, the only primes that don't exceed 10^15 are 2 and 3,
// as 5^24 = 5.9E16.
// We can quickly sum the powers of 2 or 3 from 0 to 24, and prove
// that neither of them is a multiple of 25.
//
// That leaves only p^4 * q^4.
// Start with p=2, q=3
//

// For N = p^4 * q^4, we have these factors:
//      1
//      p
//      q
//      p^2
//      pq
//      q^2
//      p^3
//      p^2 q
//      p q^2
//      q^3
//      p^4
//      p^3 q
//      p^2 q^2
//      p q^3
//      q^4
//      p^4 q
//      p^3 q^2
//      p^2 q^3
//      p q^4
//      p^4 q^2
//      p^3 q^3
//      p^2 q^4
//      p^4 q^3
//      p^3 q^4
//      p^4 q^4

bool is_25_divisor(int64_t p, int64_t q) {
    const int64_t p_powers[] = { 1, p, p * p, p * p * p, p * p * p * p };
    const int64_t q_powers[] = { 1, q, q * q, q * q * q, q * q * q * q };

    int64_t sum = 0;
    for (const int64_t pp : p_powers) {
        for (const int64_t qq : q_powers) {
            sum += pp * qq;
            //std::cout << "product = " << pp * qq << ", sum = " << sum << std::endl;
        }
    }
    //std::cout << "sum = " << sum << std::endl;
    return 0 == sum % 25;
}


// Test all prime pairs (p, q) such that p < q and p^4 * q^4 < 10^15.
// In order to test all possible primes that could lead to a product up to 10^15,
// we need to know all primes up to q = (10^15 / 2^4)^(1/4) = 2811.7
// Though the largest p < 100.  If p = q = 100 = 10^2, p^4 = q^4 = 10^8, p^4 * q^4 = 10^16.
int64_t solve_it() {
    PrimeHelper helper;
    auto primes = helper.get_primes(2812);
    std::cout << "Using " << primes.size() << " primes up to " << primes.back() << std::endl;

    int64_t sum = 0;
    for (int64_t p : primes) {
        if (p >= 100) {
            break;
        }
        const int64_t max_q = sqrt(sqrt(1E15 / p / p / p / p));
        std::cout << "p = " << p << ", max_q = " << max_q << std::endl;
        for (int64_t q : primes) {
            if (q <= p) {
                continue;
            }
            if (q > max_q) {
                break;
            }
            if (is_25_divisor(p, q)) {
                sum += p * p * p * p * q * q * q * q;
                std::cout << "p = " << p << ", q = " << q << ", N = " << p * p * p * p * q * q * q * q << std::endl;
            }
        }
    }

    std::cout << "The answer is " << sum << std::endl;
}

int main()
{
    std::cout << "Hello World!\n";

    std::cout << "11, 31 = " << is_25_divisor(11, 31) << std::endl;

    solve_it();
}
