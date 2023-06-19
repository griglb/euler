// Consider the infinite polynomial series,
//      A_F(x) = x*F_1 + x^2*F_2 + x^3*F_3 + ...
// where F_k is the k'th term in the Fibonacci sequence : 1, 1, 2, 3, 5, 8, ...;
// that is, F_k = F_k-2 + F_k-1, and F_1 = F_2 = 1.
//
// For this problem we shall be interested in values of x for which F(x) is a positive integer.
// Surprisingly:
//      A_F(1/2) = (1/2) x 1 + (1/2)^2 x 1 + (1/2)^3 x 2 + (1/2)^4 x 3 + (1/2)^5 x 5 + ...
//               = 1/2 + 1/4 + 2/8 + 3/16 + 5/32 + ...
//               = 2
// The corresponding values of x for the first 4 natural numbers are shown below:
//                  x               A_F(x)
//              sqrt(2)-1             1
//                 1/2                2
//          (sqrt(13) - 2) / 3        3
//          (sqrt(89) - 5) / 8        4
//          (sqrt(34) - 3) / 5        5
//
// We shall call A_F(x) a golden nugget if x is rational, because they become increasingly rarer;
// for example, the 10th golden nugget is 74049690.
//
// Find the 15th golden nugget.


#include <cmath>
#include <iostream>
#include <tuple>
#include <vector>

#include "big_int.h"
#include "fraction.h"


// Look for a recurrence relation:
//      A_F(x) = x*F_1 + x^2*F_2 + x^3*F_3 + x^4*F_4 + x^5*F_5 + ...
//      A_F(x) = x*F_1 + x^2*(F_1 + 0) + x^3*(F_2 + F_1) + x^4*(F_3 + F_2) + x^5*(F_4 + F_3) + ...
//             = x + (x^2 + x^3)*F_1 + (x^3 + x^4)*F_2 + (x^4 + x^5)*F_3 + ...
//      A_F(x) = x + (x+x^2) * {x*F_1 + x^2*F_2 + x^3*F_3 + ... }
//      A_F(x) = x + (x+x^2) * A_F(x)
//      A_F(x) = x + x*A_F(x) + x^2*A_F(x)

// Call k = A_F(x):
//      k*x^2 + (k+1)*x - k = 0
//
// quadratic equation:
//      x = (-k-1 +- sqrt((k+1)^2 - 4*k*(-k))) / 2*k
//      x = (-k-1 +- sqrt(k^2 + 2*k + 1 + 4*k^2)) / 2*k
//      x = (-k-1 +- sqrt(5*k^2 + 2*k + 1)) / 2*k
//
//   k = 1 -> x = (-2 +- sqrt(5+2+1)) / 2 = -1 +- sqrt(2) = { sqrt(2) - 1, -sqrt(2) - 1 }
//   k = 2 -> x = (-3 +- sqrt(5*4 + 2*2 + 1)) / 4 = (-3 +- sqrt(25)) / 4 = { 1/2, -2 }
//   k = 3 -> x = (-4 +- sqrt(5*9 + 2*3 + 1)) / 6 = (-4 +- sqrt(52)) / 6 = { (sqrt(13)-2)/3, (-sqrt(13)-2)/3 }
//   k = 4 -> x = (-5 +- sqrt(5*16 + 2*4 + 1)) / 8 = (-5 +- sqrt(89)) / 8 = { (sqrt(89)-5)/8, (-sqrt(89)-5)/8 }
//   k = 5 -> x = (-6 +- sqrt(5*25 + 2*5 + 1)) / 10 = (-6 +- sqrt(136)) / 10 = { (sqrt(34)-3)/5, (-sqrt(34)-3)/5 }
// All 5 examples check out!
// Golden nuggets are found when the discriminant is a perfect square.

// Look at the first 10 golder nuggets:
// F_A(1 / 2) = 2
// F_A(3 / 5) = 15
// F_A(8 / 13) = 104
// F_A(21 / 34) = 714
// F_A(55 / 89) = 4895
// F_A(144 / 233) = 33552
// F_A(377 / 610) = 229970
// F_A(987 / 1597) = 1576239
// F_A(2584 / 4181) = 10803704
// F_A(6765 / 10946) = 74049690
//
// The numerator and denominator of x are adjacent Fibonacci numbers.
// It also turns out that the F_A(x) is the product of these same Fibonacci numbers.
// So the golden nuggets are of the form:
//      GN(n) = F_A( F_2n / F_2n+1 ) = F_2n * F_2n+1
//



// All the values of x can be reduced to the form (sqrt(A) - B) / C, so store in a tuple as <A, B, C>.
// A golden nugget will have a perfect square A, so it can be combined with B into B / C.
using Triplet = std::tuple<int64_t, int64_t, int64_t>;

Triplet get_x(int64_t k) {
    Triplet ret;

    int64_t A = 5 * k * k + 2 * k + 1;
    int64_t B = k + 1;
    int64_t C = 2 * k;

    double root_A = std::sqrt(A);
    uint64_t trunc_root_A = root_A;
    if (trunc_root_A * trunc_root_A == A) {
        // We have a perfect square, turn into a rational return value
        A = 0;
        B = root_A - k - 1;

        // Reduce the fraction
        auto div = gcd(B, C);
        if (div > 1) {
            B /= div;
            C /= div;
        }
    }
    //else {
    //    auto divBC = gcd(B, C);
    //    if (divBC > 1) {
    //        auto divAC = gcd(A, C * C);
    //        auto div = gcd(divBC, divAC);
    //        if (div > 1) {
    //            A /= div * div;
    //            B /= div;
    //            C /= div;
    //        }
    //    }
    //}

    return { A, B, C };
}


Triplet get_x(BigInt k) {
    Triplet ret;

//    BigInt A = 5 * k * k + 2 * k + 1;
    BigInt A = k * k;
    A *= 5;
    A += k;
    A += k;
    A += 1;

    BigInt B = k + 1ULL;
    BigInt C = k * 2ULL;

    BigInt root_A = A.sqrt();
    if (root_A * root_A == A) {
        // We have a perfect square, turn into a rational return value
        A = 0LL;
        B = root_A - k - 1ULL;

        // Reduce the fraction
        auto div = gcd(B, C);
        if (div > 1ULL) {
            B /= div;
            C /= div;
        }
    }
    //else {
    //    auto divBC = gcd(B, C);
    //    if (divBC > 1) {
    //        auto divAC = gcd(A, C * C);
    //        auto div = gcd(divBC, divAC);
    //        if (div > 1) {
    //            A /= div * div;
    //            B /= div;
    //            C /= div;
    //        }
    //    }
    //}

    return { A.to_int(), B.to_int(), C.to_int() };
}


// Repurpose Triplet = <A_F(x), B, C>
std::vector<Triplet> get_golden_nuggets(size_t how_many) {
    std::vector<Triplet> ret;

    for (int64_t k = 1; ret.size() < how_many; ++k) {
        const auto& [A, B, C] = k <= 500'000'000 ? get_x(k) : get_x(BigInt{ k });
        if (0 == A) {
            ret.push_back({ k, B, C });
        }
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    for (int64_t A_F = 1; A_F <= 50; ++A_F) {
    //        const auto& [A, B, C] = get_x(A_F);
    //        if (0 == A)
    //            std::cout << A_F << " = " << B << " / " << C << std::endl;
    //        else
    //            std::cout << "\t" << A_F << " = (sqrt(" << A << ") - " << B << ") / " << C << std::endl;
    //    }
    //}

    //{
    //    auto nuggets = get_golden_nuggets(15);
    //    for (const auto& [k, B, C] : nuggets) {
    //        std::cout << k << " = " << B << " / " << C << std::endl;
    //    }
    //}


    {
        int64_t num{ 1 };
        int64_t den{ 2 };

        for (int32_t n = 1; n <= 15; ++n) {
            std::cout << n << "\t" << num << " / " << den << " = " << num * den << std::endl;
            num += den;
            den += num;
        }
    }
}
