// Consider the diophantine equation 1/a + 1/b = p/10^n with a, b, p, n positive
// integers and a <= b.
// For n=1 this equation has 20 solutions that are listed below:
//
//      1   1   20      1   1   15      1   1   12      1    1   11     1   1   10
//      - + - = --      - + - = --      - + - = --      - + -- = --     - + - = --
//      1   1   10      1   2   10      1   5   10      1   10   10     2   2   10
//
//      1   1    7      1    1    6     1   1    5      1    1    4     1   1    5
//      - + - = --      - + -- = --     - + - = --      - + -- = --     - + - = --
//      2   5   10      2   10   10     3   6   10      3   15   10     4   4   10
//
//      1    1    3     1   1    4      1    1    3     1    1    2      1    1    2
//      - + -- = --     - + - = --      - + -- = --     - + -- = --     -- + -- = --
//      4   20   10     5   5   10      5   10   10     6   30   10     10   10   10
//
//       1     1    1    1    1    1     1    1    1     1    1    1      1    1    1
//      -- + --- = --   -- + -- = --    -- + -- = --    -- + -- = --     -- + -- = --
//      11   110   10   12   60   10    14   35   10    15   30   10     20   20   10
//
// How many solutions has this equation for 1 <= n <= 9?


#include <array>
#include <chrono>
#include <iostream>
#include <map>
#include <set>
#include <thread>
#include <tuple>

#include "fraction.h"


using namespace std::chrono_literals;


// The challenging part of this is the improper fractions, which is 11 of the 20
// solutions for n=1.

// Since a <= b, we know that a is in [1, 2*10^n].  If a > 2*10^n, then there's no way
// for the sum to be as large as 1/10^n.
// We also know that is a > 10^n, then the sum must be equal to 1/10^n, there's no
// way to be as large as 2/10^n.

// This lets us split the search into a couple subsets:
//      a = 1                   => b in [1, 10^n]   p in [1 + 10^n, 2*10^n]
//      a in [2, 10^n)          => b in [a, inf)    p in [2, 10^n]
//      a = 10^n                => b = a            p = 2
//      a in (10^n, 2*10^n)     => b in (a, inf)    p = 1
//      a = 2*10^n              => b = a            p = 1

// There 5 subsets have O(N), O(N^2), O(1), O(N), and O(1) complexity.  The quadratic
// subset is the one that doesn't scale well to 10^9.
// This subset could involve O(N) loops over both a and p, how to avoid that?
//

//1 :
//    1       1       20
//    1       2       15
//    1       5       12
//    1       10      11
//    2       2       10
//    2       5       7
//    2       10      6
//    3       6       5
//    3       15      4
//    4       4       5
//    4       20      3
//    5       5       4
//    5       10      3
//    6       30      2
//    10      10      2
//    11      110     1
//    12      60      1
//    14      35      1
//    15      30      1
//    20      20      1
//2 :
//    1       1       200
//    1       2       150
//    1       4       125
//    1       5       120
//    1       10      110
//    1       20      105
//    1       25      104
//    1       50      102
//    1       100     101
//    2       2       100
//    2       4       75
//    2       5       70
//    2       10      60
//    2       20      55
//    2       25      54
//    2       50      52
//    2       100     51
//    3       6       50
//    3       15      40
//    3       60      35
//    3       150     34
//    4       4       50
//    4       5       45
//    4       10      35
//    4       20      30
//    4       25      29
//    4       50      27
//    4       100     26
//    5       5       40
//    5       10      30
//    5       20      25
//    5       25      24
//    5       50      22
//    5       100     21
//    6       12      25
//    6       30      20
//    6       75      18
//    6       300     17
//    7       140     15
//    8       8       25
//    8       40      15
//    8       200     13
//    10      10      20
//    10      20      15
//    10      25      14
//    10      50      12
//    10      100     11
//    11      110     10
//    12      15      15
//    12      60      10
//    12      150     9
//    13      325     8
//    14      35      10
//    15      30      10
//    15      75      8
//    15      300     7
//    17      850     6
//    18      225     6
//    20      20      10
//    20      25      9
//    20      50      7
//    20      100     6
//    21      420     5
//    22      220     5
//    24      120     5
//    25      25      8
//    25      50      6
//    25      100     5
//    26      650     4
//    28      70      5
//    30      60      5
//    30      150     4
//    34      1700    3
//    35      700     3
//    36      45      5
//    36      450     3
//    40      40      5
//    40      200     3
//    50      50      4
//    50      100     3
//    51      2550    2
//    52      1300    2
//    54      675     2
//    55      550     2
//    60      75      3
//    60      300     2
//    70      175     2
//    75      150     2
//    100     100     2
//    101     10100   1
//    102     5100    1
//    104     2600    1
//    105     2100    1
//    108     1350    1
//    110     1100    1
//    116     725     1
//    120     600     1
//    125     500     1
//    140     350     1
//    150     300     1
//    180     225     1
//    200     200     1



using Value = int64_t;
using ABP = std::tuple<Value, Value, Value>;
using SameSum = std::set<ABP>;
using Solution = std::map<Value, SameSum>;


Solution brute_force(int16_t max_n) {
    Solution ret;

    // The debate is to iterate over b or p:
    //   If we iterate over b, then we have to figure out how to identify sums
    // that can be expressed as p/10^n.
    //   If we iterate over p, then we can calculate b and see if its numerator = 1.

    for (int16_t n = 1; n <= max_n; ++n) {
        ret[n] = {};
        auto& triplets = ret.at(n);

        Value ten_to_n{ 1 };
        for (int16_t i = 0; i < n; ++i)
            ten_to_n *= 10;
        Value two_ten_to_n{ 2 * ten_to_n };

        // Start with a = 1
        //   p is in [1 + 10^n, 2*10^n], b should be in [1, 10^n]
        {
            const Fraction a{ 1, 1 };
            for (Value p = ten_to_n + 1; p <= two_ten_to_n; ++p) {
                Fraction sum{ p, ten_to_n };
                sum -= a;
                const auto& [num, den] = sum.get_components();
                if (1 == num) {
                    triplets.insert({ 1, den, p });
                }
            }
        }
        
        // Next a in [2, 10^n)
        //   p is in [2, 10^n], b >= a
        {
            for (Value a = 2; a < ten_to_n; ++a) {
                const Fraction a_frac{ 1, a };
                for (Value p = 2; p <= ten_to_n; ++p) {
                    Fraction sum{ p, ten_to_n };
                    sum -= a_frac;
                    const auto& [num, den] = sum.get_components();
                    if ((1 == num) && (den >= a)) {
                        triplets.insert({ a, den, p });
                    }
                }
            }
        }

        // Next special case of a = b = 10^n, p = 2
        triplets.insert({ ten_to_n, ten_to_n, 2 });

        // Next a in (10^n, 2*10^n)
        //   p = 1, b >= a
        {
            for (Value a = ten_to_n; a < two_ten_to_n; ++a) {
                Fraction sum{ 1, ten_to_n };
                Fraction a_frac{ 1, a };
                sum -= a_frac;
                const auto& [num, den] = sum.get_components();
                if (1 == num) {
                    triplets.insert({ a, den, 1 });
                }
            }
        }

        // Next special case of a = b = 2*10^n, p = 1
        triplets.insert({ 2*ten_to_n, 2*ten_to_n, 1 });
    }

    return ret;
}


Solution brute_force2(int16_t max_n) {
    Solution ret;

    // The debate is to iterate over b or p:
    //   If we iterate over b, then we have to figure out how to identify sums
    // that can be expressed as p/10^n.
    //   If we iterate over p, then we can calculate b and see if its numerator = 1.

    for (int16_t n = 1; n <= max_n; ++n) {
        ret[n] = {};
        auto& triplets = ret.at(n);

        Value ten_to_n{ 1 };
        for (int16_t i = 0; i < n; ++i)
            ten_to_n *= 10;
        Value two_ten_to_n{ 2 * ten_to_n };

        // Start with a = 1
        //   p is in [1 + 10^n, 2*10^n], b should be in [1, 10^n]
        {
            const Fraction a{ 1, 1 };
            for (Value p = ten_to_n + 1; p <= two_ten_to_n; ++p) {
                Fraction sum{ p, ten_to_n };
                sum -= a;
                const auto& [num, den] = sum.get_components();
                if (1 == num) {
                    triplets.insert({ 1, den, p });
                }
            }
        }

        // Next a in [2, 10^n)
        //   p is in [2, 10^n], b >= a
        // For a given value of a, we know that the sum is in [1/a, 2/a).
        // This can be used to constrain p:
        //    2/a <= p/10^n < 1/a
        //    2*10^n/a <= p < 10^n/a
        {
            for (Value a = 2; a < ten_to_n; ++a) {
                const Fraction a_frac{ 1, a };
                Value max_p = two_ten_to_n / a;
                for (Value p = ten_to_n / a; p <= max_p; ++p) {
                    Fraction sum{ p, ten_to_n };
                    sum -= a_frac;
                    const auto& [num, den] = sum.get_components();
                    if ((1 == num) && (den >= a)) {
                        triplets.insert({ a, den, p });
                    }
                }
            }
        }

        // Next special case of a = b = 10^n, p = 2
        triplets.insert({ ten_to_n, ten_to_n, 2 });

        // Next a in (10^n, 2*10^n)
        //   p = 1, b >= a
        {
            for (Value a = ten_to_n; a < two_ten_to_n; ++a) {
                Fraction sum{ 1, ten_to_n };
                Fraction a_frac{ 1, a };
                sum -= a_frac;
                const auto& [num, den] = sum.get_components();
                if (1 == num) {
                    triplets.insert({ a, den, 1 });
                }
            }
        }

        // Next special case of a = b = 2*10^n, p = 1
        triplets.insert({ 2 * ten_to_n, 2 * ten_to_n, 1 });
    }

    return ret;
}


struct ThreadParams {
    const Value n;
    const Value thread_index;
    const Value num_threads;
    SameSum& triplets;
};


void thread_func(const ThreadParams& params) {
    std::cout << "\tthread_func, thread id = " << std::this_thread::get_id()
              << ", n = " << params.n
              << ", thread_index = " << params.thread_index
              << std::endl;

    Value ten_to_n{ 1 };
    for (int16_t i = 0; i < params.n; ++i)
        ten_to_n *= 10;
    Value two_ten_to_n{ 2 * ten_to_n };

    // Start with a = 1
    //   p is in [1 + 10^n, 2*10^n], b should be in [1, 10^n]
    {
        const Fraction a{ 1, 1 };
        for (Value p = ten_to_n + 1 + params.thread_index; p <= two_ten_to_n; p += params.num_threads) {
            Fraction sum{ p, ten_to_n };
            sum -= a;
            const auto& [num, den] = sum.get_components();
            if (1 == num) {
                params.triplets.insert({ 1, den, p });
            }
        }
    }

    // Next a in [2, 10^n]
    //   p is in [2, 10^n], b >= a
    // For a given value of a, we know that the sum is in [1/a, 2/a).
    // This can be used to constrain p:
    //    2/a <= p/10^n < 1/a
    //    2*10^n/a <= p < 10^n/a
    {
        for (Value a = 2 + params.thread_index; a <= ten_to_n; a += params.num_threads) {
            const Fraction a_frac{ 1, a };
            Value max_p = two_ten_to_n / a;
            for (Value p = ten_to_n / a; p <= max_p; ++p) {
                Fraction sum{ p, ten_to_n };
                sum -= a_frac;
                const auto& [num, den] = sum.get_components();
                if ((1 == num) && (den >= a)) {
                    params.triplets.insert({ a, den, p });
                }
            }
        }
    }

    // Next a in (10^n, 2*10^n]
    //   p = 1, b >= a
    {
        for (Value a = ten_to_n + params.thread_index; a <= two_ten_to_n; a += params.num_threads) {
            Fraction sum{ 1, ten_to_n };
            Fraction a_frac{ 1, a };
            sum -= a_frac;
            const auto& [num, den] = sum.get_components();
            if (1 == num) {
                params.triplets.insert({ a, den, 1 });
            }
        }
    }
}


Solution threaded_brute_force2(int64_t max_n) {
    constexpr int64_t kNumThreads{ 16 };

    Solution ret;

    for (int16_t n = 1; n <= max_n; ++n) {
        if (n < 6) {
            ret[n] = {};
            ThreadParams params{ .n = n,
                                 .thread_index = 0,
                                 .num_threads = 1,
                                 .triplets = ret.at(n) };
            thread_func(params);

            continue;
        }

        std::array<std::thread, kNumThreads> threads;
        std::array<SameSum, kNumThreads> all_sums;

        for (int64_t ind = 0; ind < kNumThreads; ++ind) {
            ThreadParams params{ .n = n,
                                 .thread_index = ind,
                                 .num_threads = kNumThreads,
                                 .triplets = all_sums[ind] };
            threads[ind] = std::thread(thread_func, params);
            std::this_thread::sleep_for(5ms);
        }

        for (auto& th : threads)
            th.join();

        SameSum triplets;
        for (const auto &thread_triplets : all_sums) {
            triplets.insert(thread_triplets.cbegin(), thread_triplets.cend());
        }
        ret[n] = std::move(triplets);
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    auto sol = brute_force(4);
    //    for (const auto& [n, triplets] : sol) {
    //        std::cout << n << " : " << std::endl;
    //        for (const auto& [a, b, p] : triplets) {
    //            std::cout << "\t" << a << "\t" << b << "\t" << p << std::endl;
    //        }
    //    }
    //    Value sum{ 0 };
    //    for (const auto& [n, triplets] : sol) {
    //        std::cout << "n = " << n << " has " << triplets.size() << " triplets" << std::endl;
    //        sum += triplets.size();
    //    }
    //    std::cout << "There are " << sum << " total solutions" << std::endl;
    //}

    //{
    //    auto sol = brute_force2(4);
    //    for (const auto& [n, triplets] : sol) {
    //        std::cout << n << " : " << std::endl;
    //        for (const auto& [a, b, p] : triplets) {
    //            std::cout << "\t" << a << "\t" << b << "\t" << p << std::endl;
    //        }
    //    }
    //    Value sum{ 0 };
    //    for (const auto& [n, triplets] : sol) {
    //        std::cout << "n = " << n << " has " << triplets.size() << " triplets" << std::endl;
    //        sum += triplets.size();
    //    }
    //    std::cout << "There are " << sum << " total solutions" << std::endl;
    //}

    {
        auto sol = threaded_brute_force2(9);
        //for (const auto& [n, triplets] : sol) {
        //    std::cout << n << " : " << std::endl;
        //    for (const auto& [a, b, p] : triplets) {
        //        std::cout << "\t" << a << "\t" << b << "\t" << p << std::endl;
        //    }
        //}
        Value sum{ 0 };
        for (const auto& [n, triplets] : sol) {
            std::cout << "n = " << n << " has " << triplets.size() << " triplets" << std::endl;
            sum += triplets.size();
        }
        std::cout << "There are " << sum << " total solutions" << std::endl;
    }
}


// n = 1 has 20 triplets
// n = 2 has 102 triplets
// n = 3 has 356 triplets
// n = 4 has 958 triplets
// n = 5 has 2192 triplets
// n = 6 has 4456 triplets
// n = 7 has 8260 triplets
// n = 8 has 14088 triplets
// There are 30432 total solutions
