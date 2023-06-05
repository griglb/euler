// Considering 4 - digit primes containing repeated digits it is clear that they cannot
// all be the same : 1111 is divisible by 11, 2222 is divisible by 22, and so on.
// But there are nine 4 - digit primes containing three ones :
//      1117, 1151, 1171, 1181, 1511, 1811, 2111, 4111, 8111
//
// We shall say that M(n, d) represents the maximum number of repeated digits for an
// n-digit prime where d is the repeated digit, N(n, d) represents the number of such
// primes, and S(n, d) represents the sum of these primes.
//
// So M(4, 1) = 3 is the maximum number of repeated digits for a 4-digit prime where
// one is the repeated digit, there are N(4, 1) = 9 such primes, and the sum of these
// primes is S(4, 1) = 22275. It turns out that for d = 0, it is only possible to have
// M(4, 0) = 2 repeated digits, but there are N(4, 0) = 13 such cases.
//
// In the same way we obtain the following results for 4 - digit primes.
//              Digit, d    M(4, d)     N(4, d)     S(4, d)
//                 0            2           13       67061
//                 1            3            9       22275
//                 2            3            1        2221
//                 3            3           12       46214
//                 4            3            2        8888
//                 5            3            1        5557
//                 6            3            1        6661
//                 7            3            9       57863
//                 8            3            1        8887
//                 9            3            7       48073
//
// For d = 0 to 9, the sum of all S(4, d) is 273700.
//
// Find the sum of all S(10, d).

#include <array>
#include <iostream>
#include <numeric>
#include <vector>

#include "big_int.h"
#include "chunked_prime_helper.h"


using DigitCounts = std::array<uint64_t, 10>;
using DigitPrimes = std::array<ULongLongVec, 10>;

uint64_t S(int16_t num_digits) {
    uint64_t min_val{ 1 };
    for (int16_t i = 1; i < num_digits; ++i)
        min_val *= 10;
    uint64_t max_val{ min_val * 10 };
    std::cout << min_val << "\t" << max_val << std::endl;

    ULongLongVec primes;
    {
        ChunkedPrimeHelper<1'000'000'000> helper;
        primes = helper.get_primes(max_val);
        std::cout << "Got " << primes.size() << " primes" << std::endl;
    }

    DigitCounts M{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    DigitPrimes N;

    for (const auto& p : primes) {
        // Skip primes without enough digits.
        if (p < min_val)
            continue;

        auto digits = BigInt{ p }.get_digits();
        DigitCounts counts{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        for (const auto& dig : digits)
            ++counts[dig];

        for (int8_t d = 0; d < 10; ++d) {
            if (counts[d] > M[d]) {
                M[d] = counts[d];
                N[d].clear();
            }
            if (counts[d] == M[d]) {
                N[d].push_back(p);
            }
        }
    }

    uint64_t sum{ 0 };
    for (int16_t d = 0; d < 10; ++d) {
        uint64_t S = std::accumulate(N[d].begin(), N[d].end(), 0ULL);
        std::cout << d << "\t" << M[d] << "\t" << N[d].size() << "\t" << S << std::endl;
        sum += S;
    }

    return sum;
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    PrimeHelper helper1;
    //    auto primes1 = helper1.get_primes(2000);

    //    CachedPrimeHelper helper2;
    //    auto primes2 = helper2.get_primes(1100);


    //    for (size_t i = 0; i < primes2.size(); ++i)
    //        std::cout << primes1[i] << "\t" << primes2[i] << std::endl;
    //}

    {
//        std::cout << S(4) << std::endl;
        std::cout << S(10) << std::endl;
    }
}
