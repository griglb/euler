// Let f(N) be the number of points with integer coordinates that are on a circle passing through
//      (0, 0), (N, 0), (0, N), and (N, N).
//
// It can be shown that f(10,000) = 36.
//
// What is the sum of all positive integers N <= 1E11 such that f(N) = 420?


#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>


using PointAB = std::pair<int64_t, int64_t>;
struct pair_hash {
    inline std::size_t operator()(const PointAB& pt) const {
        std::hash<int64_t> hasher;
        return hasher(pt.first) ^ hasher(pt.second);
    }
};
using CirclePoints = std::unordered_set<PointAB, pair_hash>;
using CirclesByN = std::map< int64_t, CirclePoints>;

// Answer is not 723294800

// With the circle going through the specified points, we know a few facts:
//      - it's center is at (N/2, N/2)
//      - it's radius is N/2 * sqrt(2)
//      - it passes through at least those 4 points with integer coordinates
//      - the equation for the circle is:
//          (x - N/2)^2 + (y - N/2)^2 = N^2 / 2
//
// Then the line from the center to any (x, y) conforms to the Pythagorean theorem:
//      a^2 + b^2 = c^2
// We can label:
//      a = +-(x - N/2)     &   b = +-(y - N/2)
// or
//      a = +-(y - N/2)     &   b = +-(x - N/2)
// In both cases:
//      c^2 = N^2 / 2
//      c = N/2 * sqrt(2)
//
// The values of a & b can be independently positive or negative, and the can be exchanged.
// This gives us these constraints for the bottom half of the first quadrant (below y = x):
//      0 < b < N / 2
//      0 < a < b
// We can then multiple the number of these points by 8 and add 4 to get the total number
// of integer points.

// Start with a normal Pythagorean triplet (a, b, c):
//      a^2 + b^2 = c^2
// The take sums and differences:
//      (a - b)^2 + (a + b)^2 = a^2 - 2ab + b^2 + a^2 + 2ab + b^2
//                            = 2 * a^2 + 2 * b^2
//                            = 2 * c^2
// This takes the integer triplet (a, b, c) and generates the non-integer triplet 
//      (a - b, a + b, c * sqrt(2))
// This means that all the integer triplets (a, b, c) will be on the circle when
//      c = N / 2
//
// For example, the classic (3, 4, 5) triangle will generate the triplet:
//      (4 + 3, 4 - 3, 5 * sqrt(2))
//      (7, 1, 5 * sqrt(2))
// This is the only point in the first octant that is on the circle for N = 10.
//

// Note that not all multiples of a triplet can be generated from integer m/n values.
// In fact, most multiples can't.  Let's return to m=2, n=1:
//      m = 2       n = 1       =>      a = 3       b = 4       c = 5
// If you double the m/n values, the a/b/c values go up by 4:
//      m = 4       n = 2       =>      a = 12      b = 16      c = 20
// This is because the expressions for a,b,c are all quadratic in terms of m,n.
// So how can you get a doubling or tripling of (3, 4, 5)?  You can't with integer
// m, n values.  So once you have used a given (m, n) to generate (a, b, c), you need
// to multiply those values by a scalar.  But only scalars that aren't perfect squares
// themselves, or else you risk repeating triplets.

// Start with the normal triplet generator:
//      a = m^2 - n^2
//      b = 2mn
//      c = m^2 + n^2
//

using Triplet = std::tuple<int64_t, int64_t, int64_t>;


std::vector<Triplet> get_triplets(int64_t m, int64_t n, int64_t max_k) {
    int64_t a = m * m - n * n;
    int64_t b = 2 * m * n;
    int64_t c = m * m + n * n;

    if (a > b) {
        auto tmp = a;
        a = b;
        b = tmp;
    }

    std::vector<Triplet> ret;
    for (int64_t k = 1; k <= max_k; ++k) {
        ret.push_back({ k * a, k * b, k * c });
    }

    return ret;
}


std::set<Triplet> find_triplets_smaller_than(int64_t max_perim) {
    std::set<Triplet> ret;
    for (int64_t m = 2; m < sqrt(max_perim); ++m) {
        for (int64_t n = 1; n < m; ++n) {
            int64_t max_k = max_perim / (m * m);
            std::vector<Triplet> triplets = get_triplets(m, n, max_k);
            for (const auto& [a, b, c] : triplets) {
                ret.insert({ a, b, c });
            }
        }
    }

    return ret;
}


std::vector<Triplet> find_points(int64_t N) {
    const int64_t c_sq = N * N / 2;
    const double c = std::sqrt(c_sq);

    std::unordered_set<int64_t> squares;
    for (int64_t i = 1; i <= c; ++i)
        squares.insert(i * i);

    std::vector<Triplet> ret;

    for (int64_t a = 1; a < c; ++a) {
        int64_t a_sq = a * a;
        int64_t b_sq = c_sq - a_sq;
        if (b_sq <= a_sq)
            break;
        if (squares.find(b_sq) != squares.cend()) {
            ret.push_back({ a, static_cast<int64_t>(std::sqrt(b_sq)), c_sq });
        }
    }

    return ret;
}


std::vector<int64_t> find_circles(int64_t max_radius, int64_t num_points) {
    if ((num_points - 4) % 8 > 0) {
        std::cerr << "Invalid number of points" << std::endl;
        return {};
    }

    const int64_t points_per_octant = (num_points - 4) / 8;

    std::unordered_set<int64_t> squares;
    for (int64_t i = 1; i <= max_radius / std::sqrt(2.0); ++i)
        squares.insert(i * i);

    std::vector<int64_t> ret;
    for (int64_t N = points_per_octant; N <= max_radius; ++N) {
        const int64_t c_sq = N * N / 2;
        const double c = std::sqrt(c_sq);

        int64_t count{ 0 };

        for (int64_t a = 1; a < c; ++a) {
            int64_t a_sq = a * a;
            int64_t b_sq = c_sq - a_sq;
            if (b_sq <= a_sq)
                break;
            if (squares.find(b_sq) != squares.cend()) {
                ++count;
                if (count > points_per_octant)
                    break;
            }
        }
        if (count == points_per_octant) {
            ret.push_back(N);
        }
    }

    return ret;
}


CirclesByN find_all_circles(int64_t max_radius) {
    CirclesByN ret;

    std::unordered_set<int64_t> squares;
    for (int64_t i = 1; i <= max_radius / std::sqrt(2.0); ++i)
        squares.insert(i * i);

    for (int64_t N = 2; N <= max_radius; N += 2) {
        const int64_t c_sq = N * N / 2;
        const double c = std::sqrt(c_sq);

        CirclePoints this_circle;

        for (int64_t a = 1; a < c; ++a) {
            int64_t a_sq = a * a;
            int64_t b_sq = c_sq - a_sq;
            if (b_sq <= a_sq)
                break;
            if (squares.find(b_sq) != squares.cend()) {
                this_circle.insert({ a, static_cast<int64_t>(std::sqrt(b_sq)) });
            }
        }
        if (!this_circle.empty()) {
            ret[N] = std::move(this_circle);
        }
    }

    return ret;
}


void AddPoint(CirclesByN& circles, int64_t N, const PointAB& point) {
    if (circles.find(N) == circles.cend())
        circles[N] = {};

    circles[N].insert(point);
}


using PointCounts = std::unordered_map<int64_t, int64_t>;
void AddPoint(PointCounts& circles, int64_t N) {
    if (circles.find(N) == circles.cend())
        circles[N] = 0;

    ++circles[N];
}


CirclesByN find_all_circles2(int64_t max_N) {
    CirclesByN ret;

    // m^2 + n^2 = c = N / sqrt(2)
    // largest m is when n = 1
    //      max(m) = sqrt(N / sqrt(2) - 1)
    const int64_t max_mn = std::sqrt(max_N / std::sqrt(2) - 1);

    std::unordered_set<int64_t> squares;
    for (int64_t i = 1; i <= std::sqrt(max_N); ++i)
        squares.insert(i * i);

    for (int64_t m = 2; m < max_mn; ++m) {
        for (int64_t n = 1; n < m; ++n) {
            // We don't the value of b, just whichever leg is larger
            const int64_t a = m * m - n * n;
            const int64_t b = 2 * m * n;
            const int64_t c = m * m + n * n;

            const int64_t longer = std::max(a, b);
            const int64_t shorter = std::min(a, b);

            const int64_t ap = longer - shorter;
            const int64_t bp = longer + shorter;
            const int64_t N = 2 * c;

            if (N > max_N / 2)
                break;

            AddPoint(ret, N, { ap, bp });
            for (int64_t k = 2; k <= max_N / N; ++k) {
                // Skip perfect square scalars, so we don't duplicate triplets.
                if (squares.find(k) != squares.cend())
                    continue;
                AddPoint(ret, k * N, { k * ap, k * bp });
            }
        }
    }

    return ret;
}


CirclesByN find_circles_with_points(const int64_t max_N, const int64_t num_points) {
    // m^2 + n^2 = c = cp / sqrt(2) = N / 2
    // largest m for a given c is when n = 1
    // smallest m for a given c is when n = m - 1
    //      max(m) = sqrt(N / 2 * sqrt(2))
    //      m^2 + (m-1)^2 = N / 2
//    const int64_t max_mn = std::sqrt(max_N / 2 - 1);
    const int64_t max_mn = std::sqrt(max_N / 2);
    std::cout << "max_mn = " << max_mn << std::endl;

    std::unordered_set<int64_t> squares;
    for (int64_t i = 1; i <= std::sqrt(max_N); ++i)
        squares.insert(i * i);

    using N_Values = std::unordered_set<int64_t>;
    CirclesByN working;
//    N_Values not_enough_points;
    CirclesByN just_right;
    N_Values too_many_points;

    for (int64_t m = 2; m < max_mn; ++m) {
        if (m % 1'000 == 0) std::cout << m << std::endl;
        while (!working.empty() && (m * m + 1 > working.cbegin()->first)) {
            const size_t num_pts = working.cbegin()->second.size();
            //if (num_pts < num_points)
            //    not_enough_points.insert(working.cbegin()->first);
            if (num_pts > num_points)
                too_many_points.insert(working.cbegin()->first);
            else if (num_pts == num_points) {
                std::cout << "moving " << working.cbegin()->first << std::endl;
                just_right[working.cbegin()->first] = std::move(working.cbegin()->second);
            }
            working.erase(working.cbegin());
        }
        for (int64_t n = 1; n < m; ++n) {
            // We don't the value of b, just whichever leg is larger
            const int64_t a = m * m - n * n;
            const int64_t b = 2 * m * n;
            const int64_t c = m * m + n * n;

            const int64_t longer = std::max(a, b);
            const int64_t shorter = std::min(a, b);

            const int64_t ap = longer - shorter;
            const int64_t bp = longer + shorter;
            const int64_t N = 2 * c;

            if (N > max_N)
                break;

            if (too_many_points.find(N) != too_many_points.cend())
                continue;
            AddPoint(working, N, { ap, bp });
            for (int64_t k = 2; k <= max_N / N; ++k) {
                // Skip perfect square scalars, so we don't duplicate triplets.
                if (squares.find(k) != squares.cend())
                    continue;
                if (too_many_points.find(k * N) != too_many_points.cend())
                    continue;
                AddPoint(working, k * N, { k * ap, k * bp });
            }
        }
    }

    squares.clear();

    for (const auto &[N, points] : working) {
        if (points.size() == num_points) {
            just_right[N] = std::move(points);
            std::cout << "moved " << N << std::endl;
        }
    }

    //for (const auto& N : not_enough_points) {
    //    if (working.find(N) != working.cend())
    //        working.erase(N);
    //}
    //for (const auto& N : too_many_points) {
    //    if (working.find(N) != working.cend())
    //        working.erase(N);
    //}

    //just_right.merge(working);

    return just_right;
}


std::vector<int64_t> find_circles_with_points2(const int64_t max_N, const int64_t num_points) {
    // m^2 + n^2 = c = cp / sqrt(2) = N / 2
    // largest m for a given c is when n = 1
    // smallest m for a given c is when n = m - 1
    //      max(m) = sqrt(N / 2 * sqrt(2))
    //      m^2 + (m-1)^2 = N / 2
    const int64_t max_mn = std::sqrt(max_N / 2);
    std::cout << "max_mn = " << max_mn << std::endl;

    std::unordered_set<int64_t> squares;
    for (int64_t i = 1; i <= std::sqrt(max_N); ++i)
        squares.insert(i * i);

    using N_Values = std::unordered_set<int64_t>;

    PointCounts working;
    std::vector<int64_t> just_right;
    N_Values too_many_points;

    for (int64_t m = 2; m < max_mn; ++m) {
        if (m % 1'000 == 0) std::cout << m << std::endl;
        while (!working.empty() && (m * m + 1 > working.cbegin()->first)) {
            const size_t num_pts = working.cbegin()->second;
            if (num_pts > num_points)
                too_many_points.insert(working.cbegin()->first);
            else
                just_right.push_back(working.cbegin()->first);
            working.erase(working.cbegin());
        }
        for (int64_t n = 1; n < m; ++n) {
            // We don't the value of b, just whichever leg is larger
            const int64_t a = m * m - n * n;
            const int64_t b = 2 * m * n;
            const int64_t c = m * m + n * n;

            const int64_t longer = std::max(a, b);
            const int64_t shorter = std::min(a, b);

            const int64_t ap = longer - shorter;
            const int64_t bp = longer + shorter;
            const int64_t N = 2 * c;

            if (N > max_N)
                break;

            if (too_many_points.find(N) != too_many_points.cend())
                continue;
            AddPoint(working, N);
            for (int64_t k = 2; k <= max_N / N; ++k) {
                // Skip perfect square scalars, so we don't duplicate triplets.
                if (squares.find(k) != squares.cend())
                    continue;
                if (too_many_points.find(k * N) != too_many_points.cend())
                    continue;
                AddPoint(working, k * N);
            }
        }
    }

    squares.clear();

    for (const auto& [N, count] : working) {
        if (count == num_points)
            just_right.push_back(N);
    }

    std::sort(just_right.begin(), just_right.end());

    return just_right;
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    auto triplets = find_triplets_smaller_than(5000);
    //    for (const auto& [a, b, c] : triplets)
    //        std::cout << a << "\t" << b << "\t" << c << std::endl;
    //}

//    {
////        auto triplets = find_points(10'000);
//        auto triplets = find_points(9962);
//        for (const auto& [a,b,c] : triplets) {
//            std::cout << a << "\t" << b << "\t" << c << std::endl;
//        }
//    }

    //{
    //    auto circles = find_circles(1'000'000, 420);
    //    for (const auto& N : circles) {
    //        std::cout << N << std::endl;
    //    }
    //    std::cout << "Found " << circles.size() << " circles" << std::endl;
    //}

    //{
    //    auto circles = find_all_circles(10'000);
    //    for (const auto& [N, points] : circles) {
    //        std::cout << N << ": ";
    //        for (const auto& [a, b] : points) {
    //            std::cout << "(" << a << ", " << b << "), ";
    //        }
    //        std::cout << std::endl;
    //    }
    //}

    //{
    //    auto circles = find_all_circles2(10'000);
    //    for (const auto& [N, points] : circles) {
    //        std::cout << N << ": ";
    //        for (const auto& [a, b] : points) {
    //            std::cout << "(" << a << ", " << b << "), ";
    //        }
    //        std::cout << std::endl;
    //    }
    //    const auto& points = circles[10'000];
    //    for (const auto& [a, b] : points) {
    //        std::cout << "(" << a << ", " << b << "), ";
    //    }
    //    std::cout << std::endl;
    //}

    //{
    //    auto circles = find_circles_with_points(10'000, 4);
    //    for (const auto& [N, points] : circles) {
    //        std::cout << N << ": ";
    //        for (const auto& [a, b] : points) {
    //            std::cout << "(" << a << ", " << b << "), ";
    //        }
    //        std::cout << std::endl;
    //    }
    //    std::cout << "Found " << circles.size() << " circles";
    //}

    {
//        auto circles = find_circles_with_points(100'000'000'000, 52);
//        auto circles = find_circles_with_points(1'000, 4);
        auto circles = find_circles_with_points(10'000'000, 52);
//        auto circles = find_circles_with_points(1'000'000, 52);
        std::cout << "done" << std::endl;
        for (const auto& [N, points] : circles) {
            std::cout << N << ": ";
            for (const auto& [a, b] : points) {
                std::cout << "(" << a << ", " << b << "), ";
            }
            std::cout << std::endl;
        }
        std::cout << "Found " << circles.size() << " circles" << std::endl;
        int64_t sum{ 0 };
        for (auto iter = circles.cbegin(); iter != circles.cend(); ++iter) {
            sum += iter->first;
        }
        std::cout << "Sum = " << sum << std::endl;
    }
return 0;
    {
//        auto circles = find_circles_with_points2(100'000'000'000, 52);
        auto circles = find_circles_with_points2(1'000'000, 52);
        for (const auto& N : circles) {
            std::cout << N << std::endl;
        }
        std::cout << "Found " << circles.size() << " circles" << std::endl;
        std::cout << "Sum = " << std::accumulate(circles.cbegin(), circles.cend(), int64_t{0}) << std::endl;
    }

}

// Circles with N < 1E6:
// 718250
// 939250