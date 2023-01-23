// A spider, S, sits in one corner of a cuboid room, measuring 6 by 5 by 3,
// and a fly, F, sits in the opposite corner.
// By travelling on the surfaces of the room the shortest "straight line"
// distance from S to F is 10 and the path is shown on the diagram.
//
// However, there are up to three "shortest" path candidates for any given
// cuboid and the shortest route doesn't always have integer length.
//
// It can be shown that there are exactly 2060 distinct cuboids, ignoring rotations,
// with integer dimensions, up to a maximum size of M by M by M, for which the
// shortest route has integer length when M = 100. This is the least value of M for
// which the number of solutions first exceeds two thousand;
// the number of solutions when M = 99 is 1975.
//
// Find the least value of M such that the number of solutions first exceeds one million.

#include <algorithm>
#include <cmath>
#include <iostream>
#include <set>


double shortest_distance(int16_t w, int16_t h, int16_t d) {
    // The shortest path can follow one of 3 paths
    // 1) on wh plane to x, then on hd plane to F
    //      (0, 0, 0) -> (x, h, 0) -> (w, h, d)
    //   or (0, 0, 0) -> (w, x, 0) -> (w, h, d)
    // 2) on wd plane to x, then on wh plane to F
    //      (0, 0, 0) -> (x, 0, d) -> (w, h, d)
    //   or (0, 0, 0) -> (w, 0, x) -> (w, h, d)
    // 3) on hd plane to x, then on wh plane to F
    //      (0, 0, 0) -> (0, x, d) -> (w, h, d)
    //   or (0, 0, 0) -> (0, h, x) -> (w, h, d)

    // Calculate min for each of the 6 equations
    //      (0, 0, 0) -> (x, h, 0) -> (w, h, d)
    //    dist = sqrt(x^2 + h^2) + sqrt((w-x)^2 + d^2)
    // The extrema happen when 1st derivative is 0
    //    d dist/dx = (1/2 * 1/sqrt(x^2 + h^2) * 2x) + (1/2 * 1/sqrt((w-x)^2 + d^2) * 2*(x-w))
    //    d dist/dx = x/sqrt(x^2 + h^2) + (x-w)/sqrt(w-x)^2 + d^2)
    //    x*sqrt((w-x)^2 + d^2) = (w-x)*sqrt(x^2 + h^2)
    // square both sides
    //    x^2 * ((w-x)^2 + d^2) = (w-x)^2 * (x^2 + h^2)
    //    x^2 d^2 = (w-x)^2 h^2
    //    d^2*x^2 = h^2*x^2 - 2*w*h^2*x + h^2*w^2
    //    (h^2 - d^2) x^2 - 2*w*h^2*x + h^2*w^2 = 0
    //  A = h^2 - d^2 ; B = -2*w*h^2 ; C = h^2*w^2
    //  From quadratic equation:
    //    x = -B/2A +- sqrt(B^2/4A^2 - C/A)
    //    x = 2*w*h^2/2(h^2 - d^2) +- sqrt(4*w^2*h^4/4(h^2 - d^2)^2 - (h^2*w^2/(h^2 - d^2)))

// w = 6; h = 5; d = 3
// A = 25-9 = 16 ; B = -2*6*25 = -300 ; C = 25*9 = 225
// x = 300/32 +- sqrt(90000/4*256 - 225/16)
// x = 75/8 +- sqrt()
    return 0;
}


double shortest_path(int16_t w, int16_t h, int16_t d) {
    // The problem mentions "straight line", in quotes like that, so it implies
    // unfolding the walls to make them flat.  In the case of the 6 x 5 x 3 room,
    // we unfold the two walls on the longest edge, which turns it into a 6 x 8
    // rectagle, which is the orthogonal legs of the famous (6, 8, 10) Pythagorean
    // triple.  So we don't need any calculus of variations to find the point which
    // would make Snell's Law apply, we just need to try all 3 unfoldings to see
    // if any result in a integer length diagonal, if it is the shortest diagonal.
    // So we need to test 3 sums:
    //   w^2 + (h + d)^2
    //   h^2 + (w + d)^2
    //   d^2 + (w + h)^2
    double diag1sq = w*w + (h+d)*(h+d);
    double diag2sq = h*h + (w+d)*(w+d);
    double diag3sq = d*d + (w+h)*(w+h);

    double min_diagsq = std::min(diag1sq, std::min(diag2sq, diag3sq));
    return std::sqrt(min_diagsq);
}


int32_t num_integral_paths(int16_t max_len) {
    int32_t count{ 0 };
    std::set<int32_t> squares;
    for (int32_t i = 1; i < 3*max_len; ++i)
        squares.insert(i * i);

    for (int32_t w = 1; w <= max_len; ++w) {
        for (int32_t h = w; h <= max_len; ++h) {
            for (int32_t d = h; d <= max_len; ++d) {
                double diag1sq = w*w + (h+d)*(h+d);
                double diag2sq = h*h + (w+d)*(w+d);
                double diag3sq = d*d + (w+h)*(w+h);

                double min_diagsq = std::min(diag1sq, std::min(diag2sq, diag3sq));
                if (squares.find(min_diagsq) != squares.end())
                    ++count;
            }
        }
    }

    return count;
}


int main()
{
    std::cout << "Hello World!\n";

    std::cout << shortest_path(6, 5, 3) << std::endl;

    std::cout << num_integral_paths(99) << std::endl;
    std::cout << num_integral_paths(100) << std::endl;

    std::cout << num_integral_paths(1818) << std::endl;
}
