// Using all of the digits 1 through 9 and concatenating them freely to form
// decimal integers, different sets can be formed.
// Interestingly with the set { 2, 5, 47, 89, 631 }, all of the elements
// belonging to it are prime.
//
// How many distinct sets containing each of the digits one through nine exactly
// once contain only prime elements ?

#include <iostream>
#include <set>


#include "prime_helper.h"


using PrimeSet = std::set<uint64_t>;
using PandigitalPrimeSets = std::set<PrimeSet>;
using Digits = std::set<uint16_t>;
struct WorkingSet {
    PrimeSet primes;
    Digits remaining;
};
using WorkingSets = std::set<WorkingSet>;


bool can_add_prime(uint64_t prime, Digits &digits) {
    bool can_use{ true };

    Digits tmp_digits{ digits };

    while (prime > 0) {
        int16_t dig = prime % 10;
        if (tmp_digits.count(dig) == 0) {
            can_use = false;
            break;
        }
        prime /= 10;
        tmp_digits.erase(dig);
    }
    // Current prime contains a digit already in the pandigital set, skip it.
    if (can_use)
        digits = tmp_digits;

    return can_use;
}


void return_prime_to_working_set(uint64_t prime, Digits &digits) {
    while (prime > 0) {
        digits.insert(prime % 10);
        prime /= 10;
    }
}


void try_primes(const ULongLongVec &primes, WorkingSet &work, PandigitalPrimeSets &answer) {
std::cout << "try_primes, primes = {";
for (const auto &p : work.primes)
    std::cout << p << ", ";
std::cout << "}, remaining digits = {";
for (const auto &d : work.remaining)
    std::cout << d << ", ";
std::cout << "}" << std::endl;

    uint64_t max_prime{ 1 };
    for (const auto &d : work.remaining)
        max_prime *= 10;

    for (auto p : primes) {
        if (p <= *work.primes.rbegin())
            continue;
        if (p > max_prime)
            break;

        // Check if the current prime contains only digits in the remaining set.
        if (!can_add_prime(p, work.remaining))
            continue;
std::cout << "adding " << p << " to working set" << std::endl;
        work.primes.insert(p);

        try_primes(primes, work, answer);
        if (work.remaining.empty())
            answer.insert(work.primes);

        work.primes.erase(p);
        return_prime_to_working_set(p, work.remaining);
    }
}


PandigitalPrimeSets solve_for(uint64_t max_value) {
    PandigitalPrimeSets ret;

    PrimeHelper helper;

    auto primes = helper.get_primes(max_value);

    for (const auto &p : primes) {
        WorkingSet work;
        work.primes.insert(p);
        work.remaining = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        if (can_add_prime(p, work.remaining))
            try_primes(primes, work, ret);
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    {
        auto sets = solve_for(100'000'000);
        for (const auto &s : sets) {
            for (const auto &p : s)
                std::cout << p << ", ";
            std::cout << std::endl;
        }
        std::cout << "There are " << sets.size() << " unique pandigital prime sets" << std::endl;
    }
}
