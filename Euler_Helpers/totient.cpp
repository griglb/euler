#include "totient.h"

#include <unordered_set>


uint64_t Phi::operator()(uint64_t n) const {
    uint64_t ret{ 1 };
    auto factors = helper.get_factorization_fast(n);
    for (const auto& [prime, exp] : factors) {
        for (uint64_t e = 1; e < exp; ++e)
            ret *= prime;
        ret *= prime - 1;
    }
    return ret;
}


uint64_t Phi::naive2(uint64_t n) const {
    auto factors = helper.get_factorization_fast(n);
    if ((factors.size() == 1) && (factors.begin()->second == 1)) {
        // n is prime, return n-1 for all positive numbers < n
        return n - 1;
    }

    // Do this as a sieve, instead of the O(n^2) iteration over all numbers in [2, n).
    std::unordered_set<uint64_t> candidates;
    for (uint64_t i = 1; i < n; ++i)
        candidates.insert(i);
    for (auto iter : factors) {
        for (uint64_t i = iter.first; i < n; i += iter.first)
            candidates.erase(i);
    }
    return candidates.size();
}
