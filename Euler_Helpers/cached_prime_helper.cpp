#include "cached_prime_helper.h"

#include <fstream>
#include <numeric>


CachedPrimeHelper::CachedPrimeHelper() : sieve_(new std::array<bool, kCacheSize>{}) {
}


ULongLongVec CachedPrimeHelper::get_primes(uint64_t max_value) {
    if (!primes_.empty() && (max_value <= primes_.back())) {
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
        for (uint64_t i = 0; i < kCacheSize; ++i) {
            if ((*sieve_)[i]) {
                for (uint64_t mult = 2 * i; mult < kCacheSize; mult += i) {
                    (*sieve_)[mult] = 0;
                }
            }
        }

        // Add any new primes to the list.
        uint64_t num_primes = std::accumulate(sieve_->begin(), sieve_->end(), primes_.size());
        primes_.reserve(num_primes);
        for (uint64_t i = 0; i < kCacheSize; ++i) {
            if ((*sieve_)[i]) {
                primes_.push_back(i + sieve_min);
            }
        }
    }

    while (true) {
        const uint64_t sieve_min = ((primes_.back() / kCacheSize) + 1) * kCacheSize;
        const uint64_t sieve_max = sieve_min + kCacheSize - 1;

        if (sieve_min > max_value)
            break;

        // Reset sieve to all true
        sieve_->fill(true);

        // Update the composite-ness of all new numbers using known primes.
        for (auto p : primes_) {
            // Get first untested number that is a multiple of p.
            uint64_t first = (((sieve_min - 1) / p) + 1) * p;
            // Mark all multiples of p greater than old_max as composite.
            for (uint64_t mult = first - sieve_min; mult < kCacheSize; mult += p) {
                (*sieve_)[mult] = 0;
            }
        }

        // Add any new primes to the list.
        uint64_t num_primes = std::accumulate(sieve_->begin(), sieve_->end(), primes_.size());
        primes_.reserve(num_primes);
        for (uint64_t i = 0; i < kCacheSize; ++i) {
            if ((*sieve_)[i]) {
                primes_.push_back(i + sieve_min);
            }
        }
    }

    auto max_predicate = [max_value](uint64_t value) { return value > max_value; };
    auto max_iter = std::find_if(primes_.begin(), primes_.end(), max_predicate);
    return ULongLongVec{ primes_.begin(), max_iter };
}