// We shall say that an n-digit number is pandigital if it makes
// use of all the digits 1 to n exactly once.
// For example, 2143 is a 4-digit pandigital and is also prime.
// What is the largest n-digit pandigital prime that exists?


#include <iostream>
#include <set>

#include "big_int.h"
#include "prime_helper.h"


std::vector<uint64_t> get_pandigital_primes(int64_t max_val) {
    std::vector<uint64_t> ret;

    PrimeHelper helper;
    auto primes = helper.get_primes(max_val);

    std::vector<std::multiset<int8_t>> pan_digits{ { },
                                                   { 1 },
                                                   { 1, 2 },
                                                   { 1, 2, 3 },
                                                   { 1, 2, 3, 4 },
                                                   { 1, 2, 3, 4, 5 },
                                                   { 1, 2, 3, 4, 5, 6 },
                                                   { 1, 2, 3, 4, 5, 6, 7 },
                                                   { 1, 2, 3, 4, 5, 6, 7, 8 },
                                                   { 1, 2, 3, 4, 5, 6, 7, 8, 9 } };

    for (const auto& p : primes) {
        BigInt tmp{ p };
        const auto digits = tmp.get_digits();
        const std::multiset<int8_t> prime_digits{ digits.begin(), digits.end() };

        if (prime_digits == pan_digits[digits.size()])
            ret.push_back(p);
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    auto primes = get_pandigital_primes(1'000'000'000);
    for (const auto& p : primes) {
        std::cout << p << std::endl;
    }
}
