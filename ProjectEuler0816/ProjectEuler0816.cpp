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

constexpr int64_t kMod = 50'515'093;

Points get_points(size_t num_pts) {
    int64_t s2n = 290797;
    int64_t s2n1 = (s2n * s2n) % kMod;

    Points ret;
    ret.reserve(num_pts);
    ret.emplace_back(Point{ .x = 1.0 * s2n, .y = 1.0 * s2n1 });

    for (size_t i = 1; i < num_pts; ++i) {
        s2n = (s2n1 * s2n1) % kMod;
        s2n1 = (s2n * s2n) % kMod;
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


using Bucket = std::vector<Point>;
using Row = std::vector<Bucket>;
using Grid = std::vector<Row>;


double min_bucket_distance_squared(const Bucket& bucket) {
    size_t num_pts = bucket.size();

    double ret = 1E100;

    for (size_t i1 = 0; i1 < num_pts; ++i1) {
        const Point& pt1 = bucket[i1];
        for (size_t i2 = i1 + 1; i2 < num_pts; ++i2) {
            const Point& pt2 = bucket[i2];
            double dist_sq = (pt1.x - pt2.x) * (pt1.x - pt2.x) + (pt1.y - pt2.y) * (pt1.y - pt2.y);
            ret = std::min(ret, dist_sq);
        }
    }

    return ret;
}


double min_bucket_distance_squared(const Bucket& bucket1, const Bucket& bucket2) {
    double ret = 1E100;

    for (size_t i1 = 0; i1 < bucket1.size(); ++i1) {
        const Point& pt1 = bucket1[i1];
        for (size_t i2 = 0; i2 < bucket2.size(); ++i2) {
            const Point& pt2 = bucket2[i2];
            double dist_sq = (pt1.x - pt2.x) * (pt1.x - pt2.x) + (pt1.y - pt2.y) * (pt1.y - pt2.y);
            ret = std::min(ret, dist_sq);
        }
    }

    return ret;
}


double solve_buckets(size_t num_pts) {

    const auto pts = get_points(num_pts);

    const size_t num_cols = std::sqrt(num_pts);
    const size_t num_rows = num_cols;

    const size_t bucket_size = kMod / num_cols + 1;

    Grid grid;
    grid.resize(num_rows);
    for (auto& row : grid) {
        row.resize(num_cols);
    }

    for (const auto& pt : pts) {
        size_t col = pt.x / bucket_size;
        size_t row = pt.y / bucket_size;

        grid[row][col].push_back(pt);
    }

    double ret = 1E100;

    // First process each bucket by itself.
    for (const auto& row : grid) {
        for (const auto& bucket : row) {
            ret = std::min(ret, min_bucket_distance_squared(bucket));
        }
    }

    // The 4 corners only have 3 neighbor grid cells
    ret = std::min(ret, min_bucket_distance_squared(grid[0][0], grid[0][1]));
    ret = std::min(ret, min_bucket_distance_squared(grid[0][0], grid[1][0]));
    ret = std::min(ret, min_bucket_distance_squared(grid[0][0], grid[1][1]));

    ret = std::min(ret, min_bucket_distance_squared(grid[num_cols - 1][0], grid[num_cols - 1][1]));
    ret = std::min(ret, min_bucket_distance_squared(grid[num_cols - 1][0], grid[num_cols - 2][0]));
    ret = std::min(ret, min_bucket_distance_squared(grid[num_cols - 1][0], grid[num_cols - 2][1]));

    ret = std::min(ret, min_bucket_distance_squared(grid[0][num_rows - 1], grid[0][num_rows - 2]));
    ret = std::min(ret, min_bucket_distance_squared(grid[0][num_rows - 1], grid[1][num_rows - 1]));
    ret = std::min(ret, min_bucket_distance_squared(grid[0][num_rows - 1], grid[1][num_rows - 2]));

    ret = std::min(ret, min_bucket_distance_squared(grid[num_cols - 1][num_rows - 1], grid[num_cols - 1][num_rows - 2]));
    ret = std::min(ret, min_bucket_distance_squared(grid[num_cols - 1][num_rows - 1], grid[num_cols - 2][num_rows - 1]));
    ret = std::min(ret, min_bucket_distance_squared(grid[num_cols - 1][num_rows - 1], grid[num_cols - 2][num_rows - 2]));

    // The edge buckets each have 5 neighbors
    for (size_t col = 1; col < num_cols - 1; ++col) {
        ret = std::min(ret, min_bucket_distance_squared(grid[0][col], grid[0][col - 1]));
        ret = std::min(ret, min_bucket_distance_squared(grid[0][col], grid[0][col + 1]));
        ret = std::min(ret, min_bucket_distance_squared(grid[0][col], grid[1][col - 1]));
        ret = std::min(ret, min_bucket_distance_squared(grid[0][col], grid[1][col]));
        ret = std::min(ret, min_bucket_distance_squared(grid[0][col], grid[1][col + 1]));

        ret = std::min(ret, min_bucket_distance_squared(grid[num_rows - 1][col], grid[num_rows - 1][col - 1]));
        ret = std::min(ret, min_bucket_distance_squared(grid[num_rows - 1][col], grid[num_rows - 1][col + 1]));
        ret = std::min(ret, min_bucket_distance_squared(grid[num_rows - 1][col], grid[num_rows - 2][col - 1]));
        ret = std::min(ret, min_bucket_distance_squared(grid[num_rows - 1][col], grid[num_rows - 2][col]));
        ret = std::min(ret, min_bucket_distance_squared(grid[num_rows - 1][col], grid[num_rows - 2][col + 1]));
    }

    for (size_t row = 1; row < num_rows - 1; ++row) {
        ret = std::min(ret, min_bucket_distance_squared(grid[row][0], grid[row - 1][0]));
        ret = std::min(ret, min_bucket_distance_squared(grid[row][0], grid[row + 1][0]));
        ret = std::min(ret, min_bucket_distance_squared(grid[row][0], grid[row - 1][1]));
        ret = std::min(ret, min_bucket_distance_squared(grid[row][0], grid[row][1]));
        ret = std::min(ret, min_bucket_distance_squared(grid[row][0], grid[row + 1][1]));

        ret = std::min(ret, min_bucket_distance_squared(grid[row][num_cols - 1], grid[row - 1][num_cols - 1]));
        ret = std::min(ret, min_bucket_distance_squared(grid[row][num_cols - 1], grid[row + 1][num_cols - 1]));
        ret = std::min(ret, min_bucket_distance_squared(grid[row][num_cols - 1], grid[row - 1][num_cols - 2]));
        ret = std::min(ret, min_bucket_distance_squared(grid[row][num_cols - 1], grid[row][num_cols - 2]));
        ret = std::min(ret, min_bucket_distance_squared(grid[row][num_cols - 1], grid[row + 1][num_cols - 2]));
    }

    // The rest of the buckets in the middle each ohave 8 neighbors
    for (size_t row = 1; row < num_rows - 1; ++row) {
        for (size_t col = 1; col < num_cols - 1; ++col) {
            ret = std::min(ret, min_bucket_distance_squared(grid[row][col], grid[row - 1][col - 1]));
            ret = std::min(ret, min_bucket_distance_squared(grid[row][col], grid[row - 1][col]));
            ret = std::min(ret, min_bucket_distance_squared(grid[row][col], grid[row - 1][col + 1]));
            ret = std::min(ret, min_bucket_distance_squared(grid[row][col], grid[row][col - 1]));
            ret = std::min(ret, min_bucket_distance_squared(grid[row][col], grid[row][col + 1]));
            ret = std::min(ret, min_bucket_distance_squared(grid[row][col], grid[row + 1][col - 1]));
            ret = std::min(ret, min_bucket_distance_squared(grid[row][col], grid[row + 1][col]));
            ret = std::min(ret, min_bucket_distance_squared(grid[row][col], grid[row + 1][col + 1]));
        }
    }

    return std::sqrt(ret);
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    auto pts = get_points(14);
    //    for (const auto& pt : pts) {
    //        std::cout << std::setprecision(14) << pt.x << "\t" << pt.y << std::endl;
    //    }
    //}

    //{
    //    std::cout << solve(14) << std::endl;
    //}

    //{
    //    std::cout << solve(2'000'000) << std::endl;
    //}

    {
        std::cout << std::setprecision(14) << solve_buckets(2'000'000) << std::endl;
    }
}
