// The prime 41, can be written as the sum of six consecutive primes:
//          41 = 2 + 3 + 5 + 7 + 11 + 13
// This is the longest sum of consecutive primes that adds to a prime below one-hundred.
// The longest sum of consecutive primes below one-thousand that adds to a prime,
// contains 21 terms, and is equal to 953.
// Which prime, below one-million, can be written as the sum of the most consecutive primes?


#include <iostream>
#include <set>

#include "prime_helper.h"


uint64_t get_sum_prime(int32_t max_val) {
    PrimeHelper helper;

    auto tmp = helper.get_primes(max_val);
    std::set<uint64_t> primes{ tmp.begin(), tmp.end() };

    int32_t most_terms{ 0 };
    uint64_t first_prime{ 0 };
    uint64_t ret{ 0 };

    for (auto iter = primes.begin(); iter != primes.end(); ++iter) {
        uint64_t sum{ *iter };
        auto iter2 = iter;
        int32_t num_primes{ 2 };
        for (++iter2; iter2 != primes.end(); ++iter2, ++num_primes) {
            sum += *iter2;
            if (sum > max_val)
                break;
            if (primes.find(sum) == primes.end())
                continue;
            if (num_primes > most_terms) {
                most_terms = num_primes;
                first_prime = *iter;
                ret = sum;
            }
        }
    }

    std::cout << "longest sum has " << most_terms << " terms starting at " << first_prime << std::endl;
    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    std::cout << get_sum_prime(100) << std::endl;
    std::cout << get_sum_prime(1'000) << std::endl;
    std::cout << get_sum_prime(1'000'000) << std::endl;
}
