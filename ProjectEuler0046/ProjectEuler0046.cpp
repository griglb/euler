// It was proposed by Christian Goldbach that every odd composite number can
// be written as the sum of a prime and twice a square.
//      9  = 7  + 2×1^2
//      15 = 7  + 2×2^2
//      21 = 3  + 2×3^2
//      25 = 7  + 2×3^2
//      27 = 19 + 2×2^2
//      33 = 31 + 2×1^2
// It turns out that the conjecture was false.
// What is the smallest odd composite that cannot be written as the sum of a prime and twice a square?


#include <math.h>

#include <iostream>
#include <set>

#include "prime_helper.h"


uint64_t smallest_composite() {
    constexpr uint64_t MaxSearch{ 10000 };

    PrimeHelper helper;
    auto tmp = helper.get_primes(MaxSearch);
    std::set<uint64_t> primes{ tmp.begin(), tmp.end() };
    // We are only processing odd composites, so only want odd primes.
    primes.erase(2);

    std::set<uint64_t> squares;
    for (uint64_t i = 1; i < sqrt(MaxSearch); ++i)
        squares.insert(i * i);

    for (uint64_t n = 3; n < MaxSearch; n += 2) {
        // Skip primes.
        if (primes.find(n) != primes.end())
            continue;
        bool found{ false };
        for (const auto& p : primes) {
            // No point in searching when current prime is greater than test number.
            if (p >= n)
                break;
            if (squares.find((n - p) / 2) != squares.end())
                found = true;
        }
        if (!found)
            return n;
    }

    return 0;
}


int main()
{
    std::cout << "Hello World!\n";

    std::cout << "Goldbach's other conjecture breaks down at: " << smallest_composite() << std::endl;
}
