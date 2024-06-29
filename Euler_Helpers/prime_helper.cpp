#include "prime_helper.h"

#include <math.h>

#include <algorithm>
#include <numeric>
#include <set>

#include "combinatorics.h"


PrimeHelper::PrimeHelper() {
    // factors_.push_back({ { 0,1 } });
    // factors_.push_back({ { 1,1 } });
    // factors_.push_back({ { 2,1 } });
    factors_[2] = { { 2,1 } };
    primes_.push_back(2);
}


bool PrimeHelper::is_prime(uint64_t value) {
    if (value >= sieve_.size())
        get_primes(value);

    return sieve_[value];
}


ULongLongVec PrimeHelper::get_primes(uint64_t max_value) {
    if (sieve_.size() == max_value) {
        // We already have the exact set of primes we need, return that vector.
        return primes_;
    }

    if (max_value < sieve_.size()) {
        // We already have the primes we need, copy them into return vector.
        auto max_predicate = [max_value](uint64_t value) { return value > max_value; };
        auto max_iter = std::find_if(primes_.begin(), primes_.end(), max_predicate);
        return ULongLongVec{ primes_.begin(), max_iter };
    }

    uint64_t old_max = sieve_.empty() ? -1 : static_cast<uint64_t>(sieve_.size())-1;
    // Resize the sieve, adding true values for all unexplored numbers to start.
    sieve_.reserve(max_value + 1);
    for (auto i = old_max+1; i < max_value + 1; ++i) {
        sieve_.push_back(1);
    }
    // Make sure 0 and 1 are not prime
    sieve_[0] = sieve_[1] = 0;
    // Update the composite-ness of all new numbers using known primes.
    for (auto p : primes_) {
        // Get first untested number that is a multiple of p.
        uint64_t first = ((old_max / p) + 1) * p;
        // Mark all multiples of p greater than old_max as composite.
        for (uint64_t mult = first; mult <= max_value; mult += p) {
            sieve_[mult] = 0;
        }
    }

    // Look for new primes that were out of bounds before.
    auto first_possible_prime = primes_.empty() ? 2 : primes_.back() + 1;
    double root = sqrt(max_value);
    for (uint64_t i = first_possible_prime; i <= root; ++i) {
        if (sieve_[i]) {
            for (uint64_t mult = 2 * i; mult <= max_value; mult += i) {
                sieve_[mult] = 0;
            }
        }
    }

    // Add any new primes to the list.
    uint64_t num_primes = std::accumulate(sieve_.begin(), sieve_.end(), 0ULL);
    primes_.reserve(num_primes);
    for (uint64_t i = first_possible_prime; i <= max_value; ++i) {
        if (sieve_[i]) {
            primes_.push_back(i);
        }
    }

    return primes_;
}


Factorization PrimeHelper::get_factorization(uint64_t number) {
    if (number >= sieve_.size()) {
        get_primes(static_cast<uint64_t>(number/2));
    }

    Factorization factors;

    for (auto p : primes_) {
        while ((number % p) == 0) {
            if (factors.find(p) == factors.end()) {
                factors[p] = 0;
            }
            factors[p] = factors.at(p) + 1;
            number /= p;
            if (number < p) {
                break;
            }
        }
    }

    if (factors.empty()) {
        factors[number] = 1;
    }

    return factors;
}


Factorization PrimeHelper::get_factorization_fast(uint64_t number) {
    if (0 == number) {
        return { };
    }

    // If we've already calculated this factorization, return it.
    if (factors_.find(number) != factors_.end()) {
        return factors_.at(number);
    }

    // Do we know enough primes to factor this number?
    {
        if (number > sieve_.size())
            get_primes(number);
        // If number is prime, then return add the simple factorization and return.
        if (sieve_[number]) {
            Factorization fact;
            fact[number] = 1;
            factors_[number] = fact;
            return fact;
        }
    }

    Factorization fact;
    double root = sqrt(number);

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


ULongLongVec PrimeHelper::get_proper_divisors(uint64_t number) {
    Factorization factors = get_factorization_fast(number);

    // Special case for p^n power of exponent.
    if (factors.size() == 1) {
        const auto &[prime, exponent] = *factors.cbegin();
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

    return {divisors.begin(), divisors.end()};
}
