// It is easily proved that no equilateral triangle exists with integral length sides
// and integral area. However, the almost equilateral triangle 5-5-6 has an area of 12 square units.
//
// We shall define an almost equilateral triangle to be a triangle for which two sides are equal
// and the third differs by no more than one unit.
//
// Find the sum of the perimeters of all almost equilateral triangles with integral side lengths
// and area and whose perimeters do not exceed one billion (1,000,000,000).


#include <cmath>
#include <iostream>
#include <unordered_set>
#include <vector>

#include "big_int.h"
#include "continued_fractions.h"


// The hard way to calculate the area of the triangle is to construct its vertices, then get the
// vectors for two edges sharing a common vertex and take the magnitude of their cross product.

// The faster way to calculate the area of the triangle is to use Heron's formula, aka the
// semiperimeter formula:  area = sqrt(s * (s-a) * (s-b) * (s-c))  where s = (a + b + c)/2

// For almost equilateral triangles, we have one of 2 cases:
//  - b = a  &  c = a + 1
//  - b = a  &  c = a - 1

// Examine Heron's formula for both:
// Case 1, c = a + 1
//   s = (a + b + c) / 2 = (3a + 1) / 2
//   area = sqrt( (3a + 1)/2 * (a + 1)/2 * (a + 1)/2 * (a - 1)/2 )
//   area = sqrt( (3a+1)*(a+1)^2*(a-1) ) / 4
//   area = (a+1)/4 * sqrt( (3a+1)*(a-1) )
//
// Case 2, c = a - 1
//   s = (a + b + c) / 2 = (3a - 1) / 2
//   area = sqrt( (3a - 1)/2 * (a - 1)/2 * (a - 1)/2 * (a + 1)/2 )
//   area = (a-1)/4 * sqrt( (3a-1)*(a+1) )


using Triplet = std::tuple<uint64_t, uint64_t, uint64_t>;

std::vector<Triplet> get_almost_equilateral_triangles2(uint64_t max_perim) {
    std::vector<Triplet> ret;

    uint64_t max_a = max_perim / 3;
    for (uint64_t a = 3; a < max_a; a+=2) {
        double area_plus = (a + 1) / 4.0 * sqrt((3*a + 1) * (a - 1));
        if (area_plus == std::floor(area_plus))
            ret.push_back({ a, a, a + 1 });

        double area_minus = (a - 1) / 4.0 * sqrt((3 * a - 1) * (a + 1));
        if (area_minus == std::floor(area_minus))
            ret.push_back({ a, a, a - 1 });
    }

    return ret;
}


BigInt get_square_root(const BigInt & number) {
    // Now use Newton's method for the square root.
    //    f(x) = x^2 - a = 0
    //    x_n+1 = x_n - f(x_n)/f'(x_n)
    //    x_n+1 = x_n - (x_n^2 - a) / (2 * x_n)
    //    x_n+1 = x_n - x_n / 2 + a / (2 * x_n)
    //    x_n+1 = (x_n + a / x_n) / 2

    BigInt x_n{ number };
    for (int i = 0; i < 1000; ++i) {
        // We want x_n+1 = (x_n + (num / x_n) / 2
        // Can do this with a sequence of in-place operations:
        //    x_n+1 = num
        //    x_n+1 /= x_n
        //    x_n+1 += x_n
        //    x_n+1 /= 2
        BigInt x_np1{ number };
        x_np1 /= x_n;
        x_np1 += x_n;
        x_np1 /= 2;

//        std::cout << i << "\t" << x_np1 << std::endl;

        if (x_np1 == x_n)
            break;

        x_n = x_np1;
    }

    return x_n;
}


std::vector<Triplet> get_almost_equilateral_triangles(uint64_t max_perim) {
    std::vector<Triplet> ret;

    uint64_t max_a = max_perim / 3;
    for (uint64_t a = 3; a < max_a; a+=2) {
        if (a % 100'000 == 1) std::cout << a << std::endl;
        {
            BigInt t1{ a };
            t1 *= 3;
            t1 += 1;
            BigInt t2{ a };
            t2 -= 1;
            t1 *= t2;
//            BigInt root = get_square_root(t1);
//            BigInt square{ root };
//            square *= root;
//            if (square == t1) {
            if (t1.is_perfect_square()) {
                ret.push_back({ a, a, a + 1 });
                std::cout << a << "\t" << a << "\t" << a + 1 << std::endl;
            }
        }

        {
            BigInt t1{ a };
            t1 *= 3;
            t1 -= 1;
            BigInt t2{ a };
            t2 += 1;
            t1 *= t2;
//            BigInt root = get_square_root(t1);
//            BigInt square{ root };
//            square *= root;
//            if (square == t1) {
            if (t1.is_perfect_square()) {
                ret.push_back({ a, a, a - 1 });
                std::cout << a << "\t" << a << "\t" << a + 1 << std::endl;
            }
        }
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    {
        auto tris = get_almost_equilateral_triangles(1'000'000'000);
        uint64_t perim_sum{ 0 };
        for (const auto& [a, b, c] : tris) {
            uint64_t s = (a + b + c) / 2;
            uint64_t area = (s - a) * sqrt(s * (s - c));
            std::cout << a << "\t" << b << "\t" << c << "\t=\t" << area << std::endl;
            perim_sum += a + b + c;
        }
        std::cout << "perimeter sum = " << perim_sum << std::endl;
    }

    //{
    //    std::cout << "sqrt(3)" << std::endl;
    //    Fraction fraction{ 1, {} };
    //    for (int8_t i = 0; i < 10; ++i) {
    //        fraction.second.push_back(1);
    //        auto frac = get_convergent(fraction);
    //        std::cout << frac.first << " / " << frac.second << " = " << 1.0*frac.first.to_int() / frac.second.to_int() << std::endl;
    //        fraction.second.push_back(2);
    //        frac = get_convergent(fraction);
    //        std::cout << frac.first << " / " << frac.second << " = " << 1.0 * frac.first.to_int() / frac.second.to_int() << std::endl;
    //    }
    //}

}

// The only triangles are:
// 5            5           6           = 12
// 17           17          16          = 120
// 65           65          66          = 1848
// 241          241         240         = 25080
// 901          901         902         = 351780
// 3361         3361        3360        = 4890480
// 12545        12545       12546       = 68149872
// 46817        46817       46816       = 949077360
// 174725       174725      174726      = 13219419708
// 652081       652081      652080      = 184120982760
// 2433601      2433601     2433602     = 2564481115560
// 9082321      9082321     9082320     = 35718589344360
// 33895685     33895685    33895686    = 497495864091732
// 126500417    126500417   126500416   = 6929223155685600
