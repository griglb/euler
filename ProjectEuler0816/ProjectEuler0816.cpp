// We create an array of points P_n in a two dimensional plane using the following random number generator:
//      s_0 = 290797
//      s_n+1 = s_n^2 mod 50515093
//
//      P_n = (s_2n, s_2n+1)
//
// Let d(k) be the shortest distance of any two(distinct) points among P_0, ..., P_k-1.
// E.g. d(14) = 546446.466846479.
//
// Find d(2000000).  Give your answer rounded to 9 places after the decimal point.


#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

struct Point {
    double x;
    double y;
};
using Points = std::vector<Point>;

Points get_points(size_t num_pts) {
    int64_t s2n = 290797;
    int64_t s2n1 = (s2n * s2n) % 50515093;

    Points ret;
    ret.reserve(num_pts);
    ret.emplace_back(Point{ .x = 1.0 * s2n, .y = 1.0 * s2n1 });

    for (size_t i = 1; i < num_pts; ++i) {
        s2n = (s2n1 * s2n1) % 50515093;
        s2n1 = (s2n * s2n) % 50515093;
        ret.emplace_back(Point{ .x = 1.0 * s2n, .y = 1.0 * s2n1 });
    }

    return ret;
}


double solve(size_t num_pts) {
    const auto pts = get_points(num_pts);

    double ret = 1E100;

    for (size_t i1 = 0; i1 < num_pts; ++i1) {
        const Point& pt1 = pts[i1];
        for (size_t i2 = i1+1; i2 < num_pts; ++i2) {
            const Point& pt2 = pts[i2];
            double dist_sq = (pt1.x - pt2.x) * (pt1.x - pt2.x) + (pt1.y - pt2.y) * (pt1.y - pt2.y);
            ret = std::min(ret, dist_sq);
        }
    }

    return std::sqrt(ret);
}


int main()
{
    std::cout << "Hello World!\n";

    {
        auto pts = get_points(14);
        for (const auto& pt : pts) {
            std::cout << std::setprecision(14) << pt.x << "\t" << pt.y << std::endl;
        }
    }

    {
        std::cout << solve(14) << std::endl;
    }

    {
        std::cout << solve(2'000'000) << std::endl;
    }
}
