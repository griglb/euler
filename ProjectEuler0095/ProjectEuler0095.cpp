// The proper divisors of a number are all the divisors excluding the number itself.
// For example, the proper divisors of 28 are 1, 2, 4, 7, and 14. As the sum of these
// divisors is equal to 28, we call it a perfect number.
//
// Interestingly the sum of the proper divisors of 220 is 284 and the sum of the
// proper divisors of 284 is 220, forming a chain of two numbers.
// For this reason, 220 and 284 are called an amicable pair.
//
// Perhaps less well known are longer chains.For example, starting with 12496, we form a chain of five numbers :
//      12496 -> 14288 -> 15472 -> 14536 -> 14264 (-> 12496 -> ...)
//
// Since this chain returns to its starting point, it is called an amicable chain.
//
// Find the smallest member of the longest amicable chain with no element exceeding one million.


#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "prime_helper.h"


std::vector<uint64_t> get_divisor_sums(uint64_t max_val) {
    std::vector<uint64_t> sums;

    PrimeHelper helper;
    helper.get_primes(max_val);

    sums.push_back(0);
    sums.push_back(0);

    for (uint64_t i = 2; i <= max_val; ++i) {
        if (i % 10000 ==0) std::cout << i << std::endl;
        uint64_t sum{ 0 };
        for (const auto& fact : helper.get_proper_divisors(i)) {
            sum += fact;
        }
        sums.push_back(sum > max_val ? 0 : sum);
    }

    return sums;
}


std::map<uint64_t, std::vector<uint64_t>> get_chains(uint64_t max_val) {
    std::map<uint64_t, std::vector<uint64_t>> ret;

    const auto sums = get_divisor_sums(max_val);

    for (uint64_t num = 2; num <= max_val; ++num) {
        if (num % 10000 == 0) std::cout << num << std::endl;
        std::set<uint64_t> used{ num };
        std::vector<uint64_t> chain{ num };
        uint64_t curr{ num };
        while (true) {
            uint64_t next = sums[curr];
            chain.push_back(next);
            if (used.find(next) != used.end())
                break;
            used.insert(next);
            curr = next;
        }
        if (chain.back() == num) {
            ret[num] = chain;
        }
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    auto sums = get_divisor_sums(1'000);
    //    int64_t ind{ 0 };
    //    for (const auto& s : sums)
    //        std::cout << ind++ << "\t" << s << std::endl;
    //}

    {
        auto chains = get_chains(1'000'000);
        std::vector<uint64_t> longest;
        for (const auto& [num, chain] : chains) {
            std::cout << num << " : ";
            for (const auto& n : chain)
                std::cout << n << " -> ";
            std::cout << std::endl;
            if (chain.size() > longest.size())
                longest = chain;
        }

        std::cout << "longest chain:" << std::endl;
        for (const auto& n : longest)
            std::cout << n << " -> ";
        std::cout << std::endl;
    }

}
