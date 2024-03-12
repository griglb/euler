// Both 169 and 961 are the square of a prime.  169is the reverse of 961.
//
// We call a number a reversible prime square if:
//      1. It is not a palindrome, and
//      2. It is the square of a prime, and
//      3. Its reverse is also the square of a prime.
//
// 169 and 961 are not palindromes, so both are reversible prime squares.
//
// Find the sum of the first 50 reversible prime squares.


#include <iostream>
#include <numeric>
#include <map>

#include "big_int.h"
#include "prime_helper.h"


void find_palindromes(int64_t max_prime) {
    PrimeHelper helper;
    const auto primes = helper.get_primes(max_prime);

    for (const auto& p : primes) {
        BigInt bi{ p * p };
        const auto digits = bi.get_digits();

        if (digits.front() != digits.back())
            continue;

        auto rev_digits = digits;
        std::reverse(rev_digits.begin(), rev_digits.end());
        if (rev_digits == digits) {
            std::cout << p << "^2 = " << bi << std::endl;
        }
    }
}


int64_t solve_it(int64_t max_prime) {
    PrimeHelper helper;
    const auto primes = helper.get_primes(max_prime);

    std::map<int64_t, int64_t> reverses;

    for (const auto& p : primes) {
        BigInt bi{ p * p };
        const auto digits = bi.get_digits();

        auto rev_digits = digits;
        std::reverse(rev_digits.begin(), rev_digits.end());
        if (rev_digits == digits) {
            std::cout << p << "^2 = " << bi << std::endl;
        }
        else {
            reverses[bi.to_int()] = BigInt{ rev_digits }.to_int();
        }
    }

    std::vector<int64_t> rev_prime_squares;
    for (const auto& [sq, rev] : reverses) {
        if (reverses.find(rev) != reverses.cend()) {
            rev_prime_squares.push_back(sq);
        }
    }

    for (const auto& sq : rev_prime_squares) {
        std::cout << sq << std::endl;
    }
    std::cout << "Found " << rev_prime_squares.size() << " reversible prime squares" << std::endl;
    return std::accumulate(rev_prime_squares.cbegin(), rev_prime_squares.cend(), int64_t{ 0 });
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    find_palindromes(1'000'000);
    //}

    {
        std::cout << "answer is " << solve_it(50'000'000);
    }
}
