// Consider the infinite polynomial series,
//      A_G(x) = x*G_1 + x^2*G_2 + x^3*G_3 + ...
// where G_k is the k'th term of the second order recurrence relation,
//      G_k = G_k-1 + G_k-2
// and
//      G_1 = 1
//      G_2 = 4
// that is,
//      1, 4, 5, 9, 14, 23, ...
//
// For this problem we shall be concerned with values of x for which A_G(X)
// is a positive integer.
//
// The corresponding values of x for the first five natural numbers are shown below.
//                  x               A_F(x)
//           (sqrt(5) - 1) / 4         1
//                2 / 5                2
//          (sqrt(22) - 2) / 6         3
//         (sqrt(137) - 5) / 14        4
//                1 / 2                5
// 
// We shall call A_G(x) a golden nugget if is rational, because they become increasingly rarer;
// for example, the 20th golden nugget is 211345365.
//
// Find the sum of the first thirty golden nuggets.


#include <cmath>
#include <cstdint>
#include <iostream>
#include <tuple>
#include <vector>

#include "gcd.h"


// Look for a recurrence relation:
//      A_G(x) = x*G_1 + x^2*G_2 + x^3*G_3 + x^4*G_4 + x^5*G_5 + ...
//      A_G(x) = x*G_1 + x^2*(G_1 + 3) + x^3*(G_2 + G_1) + x^4*(G_3 + G_2) + x^5*(G_4 + G_3) + ...
//             = x + 3*x^2 + (x^2 + x^3)*G_1 + (x^3 + x^4)*G_2 + (x^4 + x^5)*G_3 + ...
//      A_G(x) = x + 3*x^2 + (x + x^2) * {x*G_1 + x^2*G_2 + x^3*G_3 + ... }
//      A_G(x) = x + 3*x^2 + (x + x^2) * A_G(x)
//      A_G(x) = x + 3*x^2 + x*A_G(x) + x^2*A_G(x)

// Call k = A_G(x):
//      (k+3)*x^2 + (k+1)*x - k = 0
//
// quadratic equation:
//      x = (-k-1 +- sqrt((k+1)^2 - 4*(k+3)*(-k))) / 2*(k+3)
//      x = (-k-1 +- sqrt(k^2 + 2*k + 1 + 4*k^2 + 12*k)) / 2*(k+3)
//      x = (-k-1 +- sqrt(5*k^2 + 14*k + 1)) / 2*(k+3)
//
//   k = 1 -> x = (-2 +- sqrt(5+14+1)) / 8 = (-1 +- sqrt(5)) / 4 = { (sqrt(5) - 1)/4, (-sqrt(5) - 1)/4 }
//   k = 2 -> x = (-3 +- sqrt(5*4 + 14*2 + 1)) / 10 = (-3 +- sqrt(49)) / 10 = { 2/5, -1 }
//   k = 3 -> x = (-4 +- sqrt(5*9 + 14*3 + 1)) / 12 = (-4 +- sqrt(88)) / 12 = { (sqrt(22)-2)/6, (-sqrt(22)-2)/6 }
//   k = 4 -> x = (-5 +- sqrt(5*16 + 14*4 + 1)) / 14 = (-5 +- sqrt(137)) / 14 = { (sqrt(137)-5)/14, (-sqrt(137)-5)/14 }
//   k = 5 -> x = (-6 +- sqrt(5*25 + 14*5 + 1)) / 16 = (-6 +- sqrt(196)) / 16 = { 1/2, -5/4 }
// All 5 examples check out!
// Golden nuggets are found when the discriminant is a perfect square.


// Look at the first 20 golden nuggets:
// A_G(2 / 5) = 2
// A_G(1 / 2) = 5
// A_G(7 / 12) = 21
// A_G(3 / 5) = 42
// A_G(19 / 31) = 152
// A_G(8 / 13) = 296
// A_G(50 / 81) = 1050
// A_G(21 / 34) = 2037
// A_G(131 / 212) = 7205
// A_G(55 / 89) = 13970
// A_G(343 / 555) = 49392
// A_G(144 / 233) = 95760
// A_G(898 / 1453) = 338546
// A_G(377 / 610) = 656357
// A_G(2351 / 3804) = 2320437
// A_G(987 / 1597) = 4498746
// A_G(6155 / 9959) = 15904520
// A_G(2584 / 4181) = 30834872
// A_G(16114 / 26073) = 109011210
// A_G(6765 / 10946) = 211345365

// These alternate between 2 subsets:
// A_G(2 / 5)           = 2         = 2 * 1         = 2 * (3 * 2 + 5) / 11
// A_G(7 / 12)          = 21        = 7 * 3         = 7 * (3 * 7 + 12) / 11
// A_G(19 / 31)         = 152       = 19 * 8        = 19 * (3 * 19 + 31) / 11
// A_G(50 / 81)         = 1050      = 50 * 21       = 50 * (3 * 50 + 81) / 11
// A_G(131 / 212)       = 7205      = 131 * 55      = 131 * (3 * 131 + 212) / 11
// A_G(343 / 555)       = 49392     = 343 * 144     = 343 * (3 * 343 + 555) / 11
// A_G(898 / 1453)      = 338546    = 898 * 377     = 898 * (3 * 898 + 1453) / 11
// A_G(2351 / 3804)     = 2320437   = 2351 * 987    = 2351 * (3 * 2351 + 3804) / 11
// A_G(6155 / 9959)     = 15904520  = 6155 * 2584   = 6155 * (3 * 6155 + 9959) / 11
// A_G(16114 / 26073)   = 109011210 = 16114 * 6765  = 16114 * (3 * 16114 + 26073) / 11
// and
// A_G(1 / 2)           = 5         = 1 * 5         = 1 * (3 * 1 + 2)
// A_G(3 / 5)           = 42        = 3 * 14        = 3 * (3 * 3 + 5)
// A_G(8 / 13)          = 296       = 8 * 37        = 8 * (3 * 8 + 13)
// A_G(21 / 34)         = 2037      = 21 * 97       = 21 * (3 * 21 + 34)
// A_G(55 / 89)         = 13970     = 55 * 254      = 55 * (3 * 55 + 89)
// A_G(144 / 233)       = 95760     = 144 * 665     = 144 * (3 * 144 + 233)
// A_G(377 / 610)       = 656357    = 377 * 1741    = 377 * (3 * 377 + 610)
// A_G(987 / 1597)      = 4498746   = 987 * 4558    = 987 * (3 * 987 + 1597)
// A_G(2584 / 4181)     = 30834872  = 2584 * 11933  = 2584 * (3 * 2584 + 4181)
// A_G(6765 / 10946)    = 211345365 = 6765 * 31241  = 6765 * (3 * 6765 + 10946)
//
// The first set follows the pattern where if x_n = M / N, then x_n+1 = (M + N) / (M + 2*N):
//      7 = 2 + 5 ; 12 = 7 + 5
//     19 = 7 + 12 ; 31 = 19 + 12
//     50 = 19 + 31 ; 81 = 50 + 31
//    131 = 50 + 81 ; 212 = 131 + 81
// The second set is created by ratios of Fibonacci numbers, just like in Problem 137,
// though the A_G() values are not the product of these 2 numbers.
// These also follow the recurrence relationship x_n = M / N -> X_n+1 = (M + N) / (M + 2*N),
// just with a different M_0, N_0 pair.
// Is there a way to calculate A_G(x) from x?

//      (k+3)*x^2 + (k+1)*x - k = 0
//      k * (x^2 + x - 1) = -3*x^2 - x
//      k = (3 * x^2 + x) / (1 - x - x^2)

//      k = {3 * (144 / 233)^2 + (144 / 233)} / {1 - (144 / 233) - (144 / 233)^2}
//      k = {(3 * 144^2 + 144 * 233) / 233^2} / {(233^2 - 144 * 233 - 144^2) / 233^2}
//      k = (3 * 144^2 + 144 * 233) / (233^2 - 144 * 233 - 144^2)
//      k = (62208 + 33552) / (54289 - 33552 - 20736)
//      k = 95760 / 1

// Let k = M / N, we then have:
//      k = {3 * (M / N)^2 + (M / N)} / {1 - (M / N) - (M / N)^2}
//      k = {(3 * M^2 + M*N) / N^2} / {(N^2 - M * N - M^2) / N^2}
//      k = M * (3 * M + N) / (N^2 - M*N - M^2)
//
// For either subset, using the recursion scheme, we can reduce further:
//      k = M * (3 * M + N) / ((M' + 2*N')^2 - (M' + N')*(M' + 2*N') - (M' + N')^2)
//      k = M * (3 * M + N) / (M'^2 + 4M'N' + 4N'^2 - (M'^2 + 3M'N' + 2N'^2) - (M'^2 + 2M'N' + N'^2))
//      k = M * (3 * M + N) / (N'^2 - M'N' - M'^2))
// The denominator will recurse to M_0 & N_0.
// For the first set, M_0 = 2, N_0 = 5 -> N^2 - MN - M^2 = 25 - 10 - 4 = 11
// For the second set, M_0 = 1, N_0 = 2 -> N^2 - MN - M^2 = 4 - 2 - 1 = 1



// All the values of x can be reduced to the form (sqrt(A) - B) / C, so store in a tuple as <A, B, C>.
// A golden nugget will have a perfect square A, so it can be combined with B into B / C.
using Triplet = std::tuple<int64_t, int64_t, int64_t>;

Triplet get_x(int64_t k) {
    Triplet ret;

    int64_t A = 5 * k * k + 14 * k + 1;
    int64_t B = k + 1;
    int64_t C = 2 * k + 6;

    double root_A = std::sqrt(A);
    int64_t trunc_root_A = root_A;
    if (trunc_root_A * trunc_root_A == A) {
        // We have a perfect square, turn into a rational return value
        A = 0;
        B = trunc_root_A - k - 1;

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


// Repurpose Triplet = <A_G(x), B, C>
std::vector<Triplet> get_golden_nuggets_brute(size_t how_many) {
    std::vector<Triplet> ret;

    for (int64_t k = 1; ret.size() < how_many; ++k) {
//        const auto& [A, B, C] = k <= 500'000'000 ? get_x(k) : get_x(BigInt{ k });
        const auto& [A, B, C] = get_x(k);
        if (0 == A) {
            ret.push_back({ k, B, C });
            std::cout << k << " = " << B << " / " << C << std::endl;
        }
    }

    return ret;
}


// Repurpose Triplet = <A_G(x), B, C>
std::vector<Triplet> get_golden_nuggets(size_t how_many) {
    std::vector<Triplet> ret;

    // A_G(2 / 5) = 2
    // A_G(1 / 2) = 5

    int64_t odd_M{ 2 };
    int64_t odd_N{ 5 };
    int64_t even_M{ 1 };
    int64_t even_N{ 2 };

    for (size_t k = 1; k <= how_many; k += 2) {
        int64_t odd_A_G = odd_M * (3 * odd_M + odd_N) / 11;
        ret.push_back({ odd_A_G, odd_M, odd_N });

        int64_t even_A_G = even_M * (3 * even_M + even_N);
        ret.push_back({ even_A_G, even_M, even_N });

        odd_M += odd_N;
        odd_N += odd_M;
        even_M += even_N;
        even_N += even_M;
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    for (int64_t A_G = 1; A_G <= 50; ++A_G) {
    //        const auto& [A, B, C] = get_x(A_G);
    //        if (0 == A)
    //            std::cout << A_G << " = " << B << " / " << C << std::endl;
    //        else
    //            std::cout << "\t" << A_G << " = (sqrt(" << A << ") - " << B << ") / " << C << std::endl;
    //    }
    //}

    //{
    //    auto nuggets = get_golden_nuggets_brute(20);
    //    for (const auto& [k, B, C] : nuggets) {
    //        std::cout << k << " = " << B << " / " << C << std::endl;
    //    }
    //}

    {
        auto nuggets = get_golden_nuggets(30);
        int64_t sum{ 0 };
        for (const auto& [k, M, N] : nuggets) {
            std::cout << k << " = " << M << " / " << N << std::endl;
            sum += k;
        }
        std::cout << "Sum = " << sum << std::endl;
    }
}


// 2 = 2 / 5
// 5 = 1 / 2
// 21 = 7 / 12
// 42 = 3 / 5
// 152 = 19 / 31
// 296 = 8 / 13
// 1050 = 50 / 81
// 2037 = 21 / 34
// 7205 = 131 / 212
// 13970 = 55 / 89
// 49392 = 343 / 555
// 95760 = 144 / 233
// 338546 = 898 / 1453
// 656357 = 377 / 610
// 2320437 = 2351 / 3804
// 4498746 = 987 / 1597
// 15904520 = 6155 / 9959
// 30834872 = 2584 / 4181
// 109011210 = 16114 / 26073
// 211345365 = 6765 / 10946
