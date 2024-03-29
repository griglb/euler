#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <algorithm>
#include <array>
#include <cmath>
#include <map>
#include <numeric>
#include <unordered_map>
#include <vector>

#include "combinatorics.h"


using ULongLongVec = std::vector<uint64_t>;
using Factorization = std::map<uint64_t, uint64_t>;
using KnownFactors = std::unordered_map<uint64_t, Factorization>;


template<size_t ChunkSize>
class ChunkedPrimeHelper {
public:
    ChunkedPrimeHelper()
     : sieve_(new std::array<bool, ChunkSize>{})
     , max_value_checked_(0) {
    }
    ~ChunkedPrimeHelper() = default;

    bool is_prime(uint64_t value) {
        if (primes_.empty() || (value > max_value_checked_)) {
            get_primes(value);
        }
        return std::binary_search(primes_.begin(), primes_.end(), value);
    }

    ULongLongVec get_primes(uint64_t max_value) {
        if (!primes_.empty() && (max_value <= max_value_checked_)) {
            // We already have the primes we need, copy them into return vector.
            auto max_predicate = [max_value](uint64_t value) { return value > max_value; };
            auto max_iter = std::find_if(primes_.begin(), primes_.end(), max_predicate);
            return ULongLongVec{ primes_.begin(), max_iter };
        }

        uint64_t sieve_min = 0;

        if (primes_.empty()) {
            // Reset sieve to all true
            sieve_->fill(true);
            // Make sure 0 and 1 are not prime
            (*sieve_)[0] = (*sieve_)[1] = 0;

            // Look for new primes that were out of bounds before.
            for (uint64_t i = 0; i < ChunkSize; ++i) {
                if ((*sieve_)[i]) {
                    for (uint64_t mult = 2 * i; mult < ChunkSize; mult += i) {
                        (*sieve_)[mult] = 0;
                    }
                }
            }

            // Add any new primes to the list.
            uint64_t num_primes = std::accumulate(sieve_->begin(), sieve_->end(), primes_.size());
            primes_.reserve(num_primes);
            for (uint64_t i = 0; i < ChunkSize; ++i) {
                if ((*sieve_)[i]) {
                    primes_.push_back(i + sieve_min);
                }
            }
            max_value_checked_ = ChunkSize - 1;
        }

        while (max_value_checked_ < max_value) {
            const uint64_t sieve_min = max_value_checked_ + 1;
            const uint64_t sieve_max = sieve_min + ChunkSize - 1;

            if (sieve_min > max_value)
                break;

            // Reset sieve to all true
            sieve_->fill(true);

            // Update the composite-ness of all new numbers using known primes.
            for (auto p : primes_) {
                // Get first untested number that is a multiple of p.
                uint64_t first = (((sieve_min - 1) / p) + 1) * p;
                // Mark all multiples of p greater than old_max as composite.
                for (uint64_t mult = first - sieve_min; mult < ChunkSize; mult += p) {
                    (*sieve_)[mult] = 0;
                }
            }

            // Add any new primes to the list.
            uint64_t num_primes = std::accumulate(sieve_->begin(), sieve_->end(), primes_.size());
            primes_.reserve(num_primes);
            for (uint64_t i = 0; i < ChunkSize; ++i) {
                if ((*sieve_)[i]) {
                    primes_.push_back(i + sieve_min);
                }
            }

            max_value_checked_ += ChunkSize;
        }

        auto max_predicate = [max_value](uint64_t value) { return value > max_value; };
        auto max_iter = std::find_if(primes_.begin(), primes_.end(), max_predicate);
        return ULongLongVec{ primes_.begin(), max_iter };
    }

    Factorization get_factorization(uint64_t number);

    Factorization get_factorization_fast(uint64_t number) {
        // If we've already calculated this factorization, return it.
        if (factors_.find(number) != factors_.end()) {
            return factors_.at(number);
        }

        // Do we know enough primes to factor this number?
        {
            if (primes_.empty() || (number > primes_.back()))
                get_primes(number);
            // If number is prime, then return add the simple factorization and return.
            if (std::binary_search(primes_.begin(), primes_.end(), number)) {
                Factorization fact;
                fact[number] = 1;
                factors_[number] = fact;
                return fact;
            }
        }

        Factorization fact;
        double root = std::sqrt(number);

        for (auto p : primes_) {
            if ((number % p) == 0) {
                fact = get_factorization_fast(number / p);
                if (fact.find(p) == fact.end()) {
                    fact[p] = 1;
                }
                else {
                    fact[p] = fact.at(p) + 1;
                }
                break;
            }
            if (p > root) {
                break;
            }
        }

        if (fact.empty()) {
            fact[number] = 1;
        }

        factors_[number] = fact;

        return factors_.at(number);
    }

    ULongLongVec get_proper_divisors(uint64_t number) {
        Factorization factors = get_factorization_fast(number);

        // Special case for p^n power of exponent.
        if (factors.size() == 1) {
            const auto& [prime, exponent] = *factors.cbegin();
            ULongLongVec ret;
            uint64_t val{ 1 };
            for (uint64_t exp = 0; exp < exponent; ++exp, val *= prime)
                ret.push_back(val);
            return ret;
        }

        std::vector<uint64_t> fact;
        for (const auto& [prime, exponent] : factors) {
            for (uint64_t i = 0; i < exponent; ++i) {
                fact.push_back(prime);
            }
        }

        std::set<int64_t> divisors;

        // 1 is always a proper divisor
        divisors.insert(1);

        // Iterate over different numbers of prime factors.
        // We can go to half the number of factors, as the complement
        // of the set of factors in the permutation is also a proper divisor.
        for (size_t num = 1; num <= fact.size() / 2; ++num) {
            CombinationList combo_list = get_combinations(fact.size(), num);
            for (const auto& combo : combo_list) {
                uint64_t product{ 1 };
                for (const auto& index : combo) {
                    product *= fact[index];
                }
                divisors.insert(product);
                divisors.insert(number / product);
            }
        }

        return { divisors.begin(), divisors.end() };
    }

private:
    std::array<bool, ChunkSize> *sieve_;
    uint64_t max_value_checked_;
    ULongLongVec primes_;
    KnownFactors factors_;
};

