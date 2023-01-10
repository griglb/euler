#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <map>
#include <unordered_map>
#include <vector>

using ULongLongVec = std::vector<uint64_t>;
using Factorization = std::map<uint64_t, uint64_t>;
using KnownFactors = std::unordered_map<uint64_t, Factorization>;


class PrimeHelper {
public:
    PrimeHelper();
    ~PrimeHelper() = default;

    ULongLongVec get_primes(uint64_t max_value);

    Factorization get_factorization(uint64_t number);
    Factorization get_factorization_fast(uint64_t number);

    ULongLongVec get_proper_divisors(uint64_t number);

private:
    ULongLongVec sieve_;
    ULongLongVec primes_;
    KnownFactors factors_;
};

