// The sum of the primes below 10 is 2 + 3 + 5 + 7 = 17.
// Find the sum of all the primes below two million.


#include <iostream>
#include <numeric>

#include "prime_helper.h"


int64_t get_prime_sum(int32_t max_value) {
    PrimeHelper helper;

    ULongLongVec primes = helper.get_primes(max_value);
    //for (const auto p : primes) {
    //    std::cout << p << " ";
    //}
    //std::cout << std::endl;
    std::cout << primes.size() << std::endl;

    return std::accumulate(primes.begin(), primes.end(), 0ULL);
}


int main()
{
    std::cout << "Hello World!\n";

    std::cout << get_prime_sum(10) << std::endl;
    std::cout << get_prime_sum(2'000'000) << std::endl;
}
