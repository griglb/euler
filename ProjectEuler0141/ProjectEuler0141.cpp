// A positive integer, n, is divided by d and the quotient and remainder are q and r respectively.
// In addition d, q, and r are consecutive positive integer terms in a geometric sequence,
// but not necessarily in that order.
//
// For example, 58 divided by 6 has quotient 9 and remainder 4.
// It can also be seen that are consecutive terms in a geometric sequence (common ratio 3/2).
// We will call such numbers, n, progressive.
//
// Some progressive numbers, such as 9 and 10404 = 102^2, happen to also be perfect squares.
// The sum of all progressive perfect squares below one hundred thousand is 124657.
//
// Find the sum of all progressive perfect squares below one trillion(10^12).


#include <cmath>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <vector>


// How to identify geometric sequence?
// Use sequence a, b, c, with scale factor k, so that
//      b = a * k
//      c = b * k = a * k^2
// Another way to look at is is that a * c = b^2 = a^2 * k^2.
// Note that we can identify 58 as progressive in 2 ways:
//      58 / 9 = 6 ; 58 % 9 = 4
//      58 / 6 = 9 ; 58 % 6 = 4
// This won't always be possible, if the remainder with the larger dividend is greater than the
// quotient, then you'll get a different remainder.  For example:
//      58 / 10 = 5 ; 58 % 10 = 8
//      58 / 5 = 11 ; 58 % 5 = 3

// The other examples are progressive as:
//      9 / 4 = 2 ; 9 % 4 = 1  -> (1, 2, 4) is geometric sequence
//  10404 /   =   ; 10404 %  =   -> ( , , ) is geometric sequence

// There are 6 possible sort orders for d, q, r:
//      d < q < r - this is not allowed, as the remainder must be less than dividend
//      d < r < q - this is not allowed, as the remainder must be less than dividend
//      q < d < r - this is not allowed, as the remainder must be less than dividend
//      q < r < d
//      r < d < q
//      r < q < d
// NOTE: r == 0 can never be part of a geometric sequence!
// NOTE: r == q can never be part of a geometric sequence, because that implies r == d with is impossible.


bool is_progressive(int64_t number) {
    int64_t max_d = std::sqrt(number);
    for (int64_t d = 2; d <= max_d; ++d) {
        const int64_t r = number % d;
        if (0 == r)
            continue;

        const int64_t q = number / d;
        if (q == r)
            continue;

        if (q < r) {
            // We have the q < r < d sequence
            if (r * r == q * d) {
                std::cout << number << " / " << d << " = " << q << " + " << r << std::endl;
                return true;
            }
        }
        else if (d < q) {
            // We have the r < d < q sequence
            if (d * d == r * q) {
                std::cout << number << " / " << d << " = " << q << " + " << r << std::endl;
                return true;
            }
        }
        else {
            // We have the r < q < d sequence
            if (q * q == r * d) {
                std::cout << number << " / " << d << " = " << q << " + " << r << std::endl;
                return true;
            }
        }
    }
    return false;
}


std::vector<int64_t> get_progressive_squares(int64_t max_n) {
    std::vector<int64_t> ret;

    for (int64_t n = 2; n <= max_n; ++n) {
        if (n % 1000 == 0)
            std::cout << n << "\t" << ret.size() << std::endl;
        if (is_progressive(n * n))
            ret.push_back(n * n);
    }

    return ret;
}

int main()
{
    std::cout << "Hello World!\n";

    //{
    //    std::vector<int64_t> values{ 58, 9, 10404, 57, 59, 8, 10, 10403, 10405 };
    //    for (const auto &val : values)
    //        std::cout << val << " is progressive = " << is_progressive(val) << std::endl;
    //}

    //{
    //    auto numbers = get_progressive_squares(1'000);
    //    for (const auto& n : numbers)
    //        std::cout << n << std::endl;
    //    std::cout << "There are " << numbers.size() << " progressive squares, summing to "
    //              << std::accumulate(numbers.begin(), numbers.end(), 0LL) << std::endl;
    //}

    {
        auto numbers = get_progressive_squares(1'000'000);
        //for (const auto& n : numbers)
        //    std::cout << n << std::endl;
        std::cout << "There are " << numbers.size() << " progressive squares, summing to "
            << std::accumulate(numbers.begin(), numbers.end(), 0LL) << std::endl;
    }

}


//       n            d          r          q           ratio
//             9          2         4           1         2
//         10404         72       144          36         2
//         16900         75       225          25         3
//         97344         92      1058           8        23/2
//        576081        360      1600          81        40/9
//       6230016       1472      4232         512        23/8
//       7322436       2420      3025        1936         5/4
//      12006225       2450      4900        1225         2
//      36869184       5760      6400        5184        10/9
//      37344321       5292      7056        3969         4/3
//      70963776       7452      9522        5832        23/18
//     196112016       4392     44652         432        61/6
//     256160025       6534     39204        1089         6
//    1361388609      30870     44100       21609        10/7
//    1380568336      25872     53361       12544        33/16
//    8534988225      75600    112896       50625       112/75
//    9729849600      78975    123201       50625       117/75
//   12551169024      70272    178608       27648        61/24
//   13855173264      83232    166464       41616         2
//   16394241600     104544    156816       69696         3/2
//  123383587600     202800    608400       67600         3
//  142965659664     355752    401868      314928      2257/1998
//  547674002500     658464    831744      521284        24/19
