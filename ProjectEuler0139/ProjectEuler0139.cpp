// Let (a, b, c) represent the three sides of a right angle triangle with integral length sides.
// It is possible to place four such triangles together to form a square with length c.
//
// For example, (3, 4, 5) triangles can be placed together to form a 5 by 5 square with a
// 1 by 1 hole in the middle and it can be seen that the by square can be tiled with
// twenty-five 1 by 1 squares.
// 
// However, if (5, 12, 13) triangles were used then the hole would measure 7 by7 and these could
// not be used to tile the 13 by 13 square.
//
// Given that the perimeter of the right triangle is less than one-hundred million, how many
// Pythagorean triangles would allow such a tiling to take place?


#include <cmath>
#include <cstdint>
#include <iostream>
#include <tuple>
#include <utility>
#include <vector>

#include "fraction.h"


// One way to calculate the hole side h is by area.  We know the square is c^2 in size, and the
// 4 triangles are each a*b/2 in size, totalling 2*a*b.  So the hole size h^2 can be calculated as:
//      h^2 = c^2 - 2*a*b
//      h^2 = a^2 + b^2 - 2*a*b
//      h^2 = (a - b)^2
//      h = a - b

// We only want to count triangles when c % h = 0.
// We can use the Pythagorean generator:
//      P(m, n) = { m^2 - n^2, 2mn, m^2 + n^2 }
// Like Problem 138, scaling an unviable triangle will not yield a viable triangle.
// Unlike Problem 138, scaling a viable triangle will yield a family of viable triangles.
// We know (3, 4, 5) is viable, so are (6, 8, 10), (9, 12, 15), (12, 16, 20), ...
// All scaling does is change the size of the squares, but this family is all tiled by a 5x5
// array of hole squares.
// So once we find a viable triangle, we can calculate the maximum scale factor and that is
// the size of this family - 100,000,000 / (a + b + c).


using Triplet = std::tuple<int64_t, int64_t, int64_t>;
using Family = std::pair<Triplet, int64_t>;
using Families = std::vector<Family>;

Families get_families(int64_t max_perimeter) {
    Families ret;

    int64_t max_m = std::sqrt(max_perimeter);
    for (int64_t m = 2; m < max_m; ++m) {
        for (int64_t n = 1; n < m; ++n) {
            int64_t a = m * m - n * n;
            int64_t b = 2 * m * n;
            int64_t c = m * m + n * n;

            if (gcd(a, b) > 1)
                continue;

            int64_t h = a > b ? a - b : b - a;

            if (c % h > 0)
                continue;

            int64_t p = a + b + c;
            if (p > max_perimeter)
                continue;

            ret.push_back({ {a, b, c}, max_perimeter / p });
        }
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    auto families = get_families(100);
    //    for (const auto& [tri, count] : families) {
    //        const auto& [a, b, c] = tri;

    //        std::cout << "( " << a << ", " << b << ", " << c << " ) - " << count << std::endl;
    //    }
    //}

    {
        auto families = get_families(100'000'000);
        int64_t sum{ 0 };
        for (const auto& [tri, count] : families) {
            const auto& [a, b, c] = tri;
            sum += count;
            std::cout << "( " << a << ", " << b << ", " << c << " ) - " << count << std::endl;
        }
        std::cout << "The " << families.size() << " families have " << sum << " triangles in them" << std::endl;
    }

}
