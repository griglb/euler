// For a given integer R consider all primitive Pythagorean triangles that can fit inside,
// without touching, a circle with radius R.
// Define F(R) to be the largest inradius of those triangles.
// You are given F(100) = 36.
// Find  F(1018).


#include <cmath>
#include <iostream>

#include "gcd.h"


// What is a primitive Pythagorean triplet?
// One where gcd(a, b) = gcd(a, c) = gcd(b, c) = 1

// The generator for Pythagorean triplets uses two parameters m & n, where m > n.
// The expressions are:
//          a = m^2 - n^2
//          b = 2mn
//          c = m^2 + n^2

// The next part is the inradius of the triangles.  The general equation is:
//          inradius = 1/2 * sqrt{ (b+c-a)(c+a-b)(a+b-c) / (a+b+c) }
// Plug in the expressions from generator:
//          inradius = 1/2 * sqrt{ (2mn + m^2 + n^2 - m^2 + n^2)(m^2 + n^2 + m^2 - n^2 - 2mn)
//                                *(m^2 - n^2 + 2mn - m^2 - n^2) / (m^2 - n^2 + 2mn + m^2 + n^2) }
//          inradius = 1/2 * sqrt{ (2mn + 2n^2)(2m^2 - 2mn)(2mn - 2n^2) / (2m^2 + 2mn) }
//          inradius = 1/2 * sqrt{ 2n(m + n)2m(m - n)2n(m - n) / 2m(m + n) }
//          inradius = 1/2 * sqrt{ 4n^2(m - n)^2 }
//          inradius = 2n(m - n)

// When can a triangle fit in a circle?  For right triangles, we know that the hypotenuse
// is the diameter of the circumcircle, so the contraint is that the hypotenuse must be
// less than the diameter:
//          m^2 + n^2 < 2R

// The filtering out of non-primitive triangles is the only challenge.  We can
// immediately filter out all (m, n) pairs where m+n is even:
//          a = m^2 - n^2 = (m + n)(m - n)
//          b = 2mn
// if (m + n) is even, then gcd(a, b) >= 2.

// How to define the problem space:
// To perform the search, we can loop over m and then n or n and then m.
// It is better to do the latter, n and then m.
// Let us return to the example of R = 100, and first try m then n.
// We know that m^2 + n^2 < 200, so max(m) = 14 to allow the (14, 1) pair.
// The follow pairs are valid:
// For m = 14, the only valid n = 1.
// For m = 13, the valid n's are in [1, 5].
// For m = 12, the valid n's are in [1, 7].
// For m = 11, the valid n's are in [1, 8].
// For m <= 10, the valid n's are in [1, m-1].
// Look at the values of the inradius for these pairs:
// m/n   1   2   3   4   5   6   7   8   9  10  11  12  13
//  14  13\\24  33  40  45  48  49  48  45  40  33  24  13
//  13  12  22  30  36  40\\42  42  40  36  30  22  12
//  12  11  20  27  32  35  36  35\\32  27  20  11
//  11  10  18  24  28  30  30  28  24\\18  10
//  10   9  16  21  24  25  24  21  16   9
//   9   8  14  18  20  20  18  14   8
//   8   7  12  15  16  15  12   7
//   7   6  10  12  12  10   6
//   6   5   8   9   8   5
//   5   4   6   6   4
//   4   3   4   3
//   3   2   2
//   2   1
// We can see that each row contains values on a parabola, with a maximum
// value near n = m/2.  This makes knowing which values of n to search for
// a given m a little challenging.
// If iterate over n first and then m, then we look at the columns instead
// of the rows.  Here we notice that the values always increase with
// increasing m.  So if we start at the largest possible m for a given n,
// then we only have to search until we get to an inradius less than the
// current largest known value.
//

// For a given R, we can establish a maximum value for n.  We know that:
//          m > n
//          m^2 + n^2 < 2R
// The smallest value for m = n + 1
//          (n+1)^2 + n^2 < 2R
//          2n^2 + 2n + 1 < 2R
//          n^2 < R
// Start with an upper bound of sqrt(R), then decrement until until we satisfy
//          2n^2 + 2n + 1 < 2R

// For a given R and n, we can calculate a maximum value for m:
//          m > n
//          m^2 + n^2 < 2R
// Start with the upper bound of
//          m = sqrt(2R - n^2)
// It is possible that (m, n, 2R) is a Pythagorean triplet, but we have to
// exclude this as it would touch the circle.  So we must decrement m in this
// case to make the triangle fit in the circle without touching.
// We can also decrement m to make sure m + n is odd, and then search
// by decrementing m by 2 instead of 1 to avoid all even sums.


int64_t max_n(const int64_t R) {
    int64_t n = std::sqrt(R);
    while (n * n + n + 0.5 > R)
        --n;
    return n;
}


int64_t max_m(const int64_t R, const int64_t n) {
    double upper_bound = std::sqrt(2 * R - n * n);
    int64_t m = upper_bound;
    if (m == upper_bound)
        --m;

    // We know m+n can't be even, so decrement m if this is the case
    if (0 == (m + n) % 2)
        --m;

    return m;
}


int64_t solve_it(const int64_t R) {
    int64_t ret = 0;

    const int64_t nMax = max_n(R);

    for (int64_t n = 1; n <= nMax; ++n) {
        if (0 == n % 100'000)
            std::cout << n << std::endl;
        const int64_t n2 = n * n;

        const int64_t mMax = max_m(R, n);
        //const int64_t max_inrad = n * (mMax - n);
        //if (max_inrad <= ret)
        //    break;
        for (int64_t m = mMax; m > n; m -= 2) {
            const int64_t m2 = m * m;

            const int64_t inrad = n * (m - n);
            if (inrad < ret) {
                break;
            }

            const int64_t a = m2 - n2;
            const int64_t b = 2 * m * n;
            const int64_t c = m2 + n2;

            bool primitive = (gcd(a, b) == 1) && (gcd(a, c) == 1) && (gcd(b, c) == 1);

//            std::cout << m << "\t" << n << "\t" << a << "\t" << b << "\t" << c << "\t" << inrad << "\t" << primitive << std::endl;

            if (primitive && (inrad > ret)) {
                ret = inrad;
                std::cout << m << "\t" << n << "\t" << a << "\t" << b << "\t" << c << "\t" << inrad << std::endl;
                break;
            }
        }
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    {
        std::cout << "R = 100, max n = " << max_n(100) << std::endl;
        std::cout << "R = 10^18, max n = " << max_n(1'000'000'000'000'000'000) << std::endl;
    }
    {
        std::cout << "R = 100, max n = " << max_n(100) << ", max m = " << max_m(100, max_n(100)) << std::endl;
        std::cout << "R = 10^18, max n = " << max_n(1'000'000'000'000'000'000)
                  << ", max m = " << max_m(1'000'000'000'000'000'000, max_n(1'000'000'000'000'000'000)) << std::endl;
    }

    {
        auto ret = solve_it(100);
        std::cout << "F(100) = " << ret << std::endl;
    }
    {
        auto ret = solve_it(1'000'000'000'000'000'000);
        std::cout << "F(1'000'000'000'000'000'000) = " << ret << std::endl;
    }
}
