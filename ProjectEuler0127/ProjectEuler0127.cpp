// The radical of n, rad(n), is the product of distinct prime factors of.
// For example, 504 = 2^3 x 3^2 x 7, so rad(504) = 2 x 3 x 7 = 42.
//
// We shall define the triplet of positive integers (a, b, c) to be an abc-hit if:
//      1. gcd(a, b) = gcd(a, c) = gcd(b, c) = 1
//      2. a < b
//      3. a + b = c
//      4. rad(abc) < c
//
// For example, (5, 27, 32) is an abc - hit, because:
//      1. gcd(5, 27) = gcd(5, 32) = gcd(27, 32) = 1
//      2. 5 < 27
//      3. 5 + 27 = 32
//      4. rad(4320) = 30 < 32
//
// It turns out that abc - hits are quite rare and there are only thirty-one abc-hits for c < 1000,
// with sum(c) = 12523.
//
// Find sum(c) for c < 120,000.


#include <iostream>
#include <tuple>
#include <vector>

#include "fraction.h"
#include "prime_helper.h"


// Through brute force analysis in Libre Office Calc, the 31 abc_hits under 1000 are:
//    1     8       9
//    5     27      32
//    1     48      49
//    1     63      64
//    1     80      81
//    32    49      81
//    4     121     125
//    3     125     128
//    1     224     225
//    1     242     243
//    2     243     245
//    7     243     250
//    13    243     256
//    81    175     256
//    1     288     289
//    100   243     343
//    32    343     375
//    5     507     512
//    169   343     512
//    1     512     513
//    27    512     539
//    1     624     625
//    49    576     625
//    81    544     625
//    1     675     676
//    1     728     729
//    25    704     729
//    104   625     729
//    200   529     729
//    1     960     961
//    343   625     968

// It can be shown that gcd(a,b) == gcd(a,c) == gcd(b,c) when a + b = c
// One identity for GCD is that gcd(x,y) == gcd(x, y-x)
// Substitute in a, b, or for x&y, you get:
//      gcd(a,c) == gcd(a,c-a) == gcd(a,b)
//      gcd(b,c) == gcd(b,c-b) == gcd(b,a)
// So we don't need to calculate all 3 GCDs, only 1.


using ABC_Hit = std::tuple<int64_t, int64_t, int64_t>;


int64_t rad1(int64_t n, PrimeHelper &helper) {
    int64_t ret{ 1 };

    auto factors = helper.get_factorization_fast(n);
    for (const auto& [prime, exp] : factors)
        ret *= prime;

    return ret;
}


std::vector<ABC_Hit> get_abc_hits1(int64_t max_value) {
    PrimeHelper helper;
    helper.get_primes(max_value * max_value * max_value / 4);

    std::vector<ABC_Hit> ret;

    for (int64_t c = 3; c <= max_value; ++c) {
        int64_t max_a = (c + 1) / 2;
        std::cout << c << std::endl;
        for (int64_t a = 1; a < max_a; ++a) {
            int64_t b = c - a;
            if ((gcd(a, b) > 1) || (gcd(a, c) > 1) || (gcd(b, c) > 1))
                continue;
            if (rad1(a * b * c, helper) < c)
                ret.push_back({ a, b, c });
        }
    }

    return ret;
}


int64_t rad(int64_t a, int64_t b, int64_t c, PrimeHelper& helper) {
    int64_t ret{ 1 };

    auto a_factors = helper.get_factorization_fast(a);
    auto b_factors = helper.get_factorization_fast(b);
    auto c_factors = helper.get_factorization_fast(c);

    for (const auto& [prime, exp] : b_factors) {
        if (a_factors.find(prime) == a_factors.end())
            a_factors[prime] = exp;
    }
    for (const auto& [prime, exp] : c_factors) {
        if (a_factors.find(prime) == a_factors.end())
            a_factors[prime] = exp;
    }

    for (const auto& [prime, exp] : a_factors)
        ret *= prime;

    return ret;
}


std::vector<ABC_Hit> get_abc_hits(int64_t max_value) {
    PrimeHelper helper;
    helper.get_primes(max_value);

    std::vector<ABC_Hit> ret;

    for (int64_t c = 3; c <= max_value; ++c) {
        int64_t max_a = (c + 1) / 2;
        auto c_factors = helper.get_factorization_fast(c);
        if (c % 1000 == 0)
            std::cout << c << "\t" << ret.size() << std::endl;
        for (int64_t a = 1; a < max_a; ++a) {
            bool has_factor{ false };
            for (const auto& [prime, exp] : c_factors) {
                if (a % prime == 0)
                    has_factor = false;
            }
            if (has_factor)
                continue;

            int64_t b = c - a;
            if (gcd(a, b) > 1)
                continue;
            if (rad(a, b, c, helper) < c)
                ret.push_back({ a, b, c });
        }
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    auto hits = get_abc_hits(1000);
    //    int64_t c_sum{ 0 };
    //    for (const auto& [a, b, c] : hits) {
    //        std::cout << a << "\t" << b << "\t" << c << std::endl;
    //        c_sum += c;
    //    }
    //    std::cout << hits.size() << " abc_hits, sum of c = " << c_sum << std::endl;
    //}
//return 0;
    {
        auto hits = get_abc_hits(120'000);
        int64_t c_sum{ 0 };
        for (const auto& [a, b, c] : hits) {
            std::cout << a << "\t" << b << "\t" << c << std::endl;
            c_sum += c;
        }
        std::cout << hits.size() << " abc_hits, sum of c = " << c_sum << std::endl;
    }
}
