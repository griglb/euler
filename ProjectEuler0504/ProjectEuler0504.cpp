// Let ABCD be a quadrilateral whose vertices are lattice points lying on the
// coordinate axes as follows:
//      A(a,0), B(0,b), C(-c,0), D(0,-d)
//      where 1 <= a,b,c,d <= m
//      and   a,b,c,d,m are integers
//
// It can be shown that for m=4 there are exactly 256 valid ways to construct
// ABCD.  Of these 256 quadrilaterals, 42 of them strictly contain a square
// number of lattice points.
//
// How many quadrilaterals ABCD strictly contain a square number of lattice
// points for m=100?


#include <array>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <unordered_set>

// We can define ABCD with 4 cutting planes, the intersection of which defines
// its interior.
// They underlined the word strict, which I think means the lattice points
// must be inside the quad, not on the edges to count.  Testing m=4 will prove this.

// The signed distance from a line through (x1, y1) and (x2, y2) to a point (x,y) is:
//      d = (x - x1)(y2 - y1) - (y - y1)(x2 - x1)

// Let's use this to define the 4 cutting planes:
// Edge AB, connecting (a, 0) and (0, b):
//      d = (x - a)(b - 0) - (y - 0)(0 - a)
//      d = (x - a)b + ya
//      d = xb + ya - ab
//      (0, 0) is inside, so d = -ab => negative is inside
//
// Edge BC, connecting (0, b) and (-c, 0):
//      d = (x + 0)(0 - b) - (y - b)(-c - 0)
//      d = -xb + c(y - b)
//      d = -xb + cy - bc
//      (0, 0) is inside, so d = -bc => negative is inside
//
// Edge CD, connecting (-c, 0) and (0, -d):
//      d = (x + c)(-d - 0) - (y - 0)(0 + c)
//      d = -d(x + c) - yc
//      d = -dx - yc - cd
//      (0, 0) is inside, so d = -cd => negative is inside
//
// Edge DA, connecting (0, -d) and (a, 0):
//      d = (x - 0)(0 + d) - (y + d)(a - 0)
//      d = xd - a(y + d)
//      d = xd - ay - ad
//      (0, 0) is inside, so d = -ad => negative is inside
//

int64_t count_quads(int m) {
    int64_t count = 0;

    std::unordered_set<int32_t> squares;
    for (int i = 1; i <= 2 * m + 1; ++i) {
        squares.insert(i * i);
    }

    for (int a = 1; a <= m; ++a) {
        for (int b = 1; b <= m; ++b) {
            std::cout << a << ", " << b << ", " << count << std::endl;
            for (int c = 1; c <= m; ++c) {
                for (int d = 1; d <= m; ++d) {
                    std::array<int32_t, 3> ab = { b, a, -a * b };
                    std::array<int32_t, 3> bc = { -b, c, -b * c };
                    std::array<int32_t, 3> cd = { -d, -c, -c * d };
                    std::array<int32_t, 3> da = { d, -a, -a * d };

                    int32_t num_pts = 0;
                    for (int x = -c; x <= a; ++x) {
                        for (int y = -d; y <= b; ++y) {
                            int32_t d1 = ab[0] * x + ab[1] * y + ab[2];
                            int32_t d2 = bc[0] * x + bc[1] * y + bc[2];
                            int32_t d3 = cd[0] * x + cd[1] * y + cd[2];
                            int32_t d4 = da[0] * x + da[1] * y + da[2];
                            if ((d1 < 0) && (d2 < 0) && (d3 < 0) && (d4 < 0)) {
                                ++num_pts;
                            }
                        }
                    }

                    if (squares.count(num_pts) > 0) {
                        ++count;
                    }
                }
            }
        }
    }

    return count;
}

int main() {
    std::cout << "Hello World!\n";

    std::cout << "m = 4, count = " << count_quads(4);
    std::cout << "m = 100, count = " << count_quads(100);
}
