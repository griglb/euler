// In a triangular array of positive and negative integers, we wish to find a
// sub-triangle such that the sum of the numbers it contains is the smallest possible.
//
// In the example below, it can be easily verified that the marked triangle satisfies
// this condition having a sum of -42.
// 
//                                          15
//                                               ^
//                                      -14    /-7 \
//                                           /       \
//                                  20     /-13     -5 \
//                                       /               \
//                              -3     /  8     23     -26 \
//                                   /                       \
//                          1      /-4      -5     -18      5  \
//                               /_______________________________\
//                      -16     31        2      9      28      3
//
// We wish to make such a triangular array with one thousand rows, so we generate
// 500500 pseudo-random numbers s_k in the range +-2^19, using a type of random number
// generator (known as a Linear Congruential Generator) as follows:
//
//          t := 0
//          for k = 1 up to k = 500500:
//              t := (615949 * t + 797807) modulo 2^20
//              s_k := t - 2^19
//
// Thus:
//      s_1 = 273519
//      s_2 = -153582
//      s_3 = 450905 etc
//
// Our triangular array is then formed using the pseudo-random numbers thus:
//                                       s_1
//                                    s_2   s_3
//                                 s_4   s_5   s_6
//                              s_7   s_8   s_9   s_10
//                                       ...
//
// Sub-triangles can start at any element of the array and extend down as far as we
// like (taking-in the two elements directly below it from the next row, the three
// elements directly below from the row after that, and so on).
// The "sum of a sub-triangle" is defined as the sum of all the elements it contains.
// Find the smallest possible sub-triangle sum.


#include <cstdint>
#include <iostream>
#include <vector>


// This is similar to Problem 149, but with only horizontal summing across the
// rows of the triangle needed.


using Row = std::vector<int64_t>;
using Triangle = std::vector<Row>;


std::vector<int64_t> get_values() {
    constexpr int64_t kMod{ 1 << 20 };
    constexpr int64_t kOffset{ 1 << 19 };
    constexpr size_t kTriangleSize{ 500'500 };

    std::vector<int64_t> ret(kTriangleSize, 0);

    int64_t t{ 0 };
    for (int64_t k = 0; k < kTriangleSize; ++k) {
        t = (615'949 * t + 797'807) % kMod;
        ret[k] = t - kOffset;
    }

    return ret;
}


std::vector<int64_t> get_values_test() {
    return { 15,
             -14, -7,
             20, -13, -5,
             -3, 8, 23, -26,
             1, -4, -5, -18, 5,
             -16, 31, 2, 9, 28, 3 };
}


Triangle get_row_sums() {
    constexpr size_t kNumRows{ 1'000 };
//    constexpr size_t kNumRows{ 6 };

    Triangle tri;
    tri.resize(kNumRows + 1);  // Add extra row to make 1-based indexing

    const auto values = get_values();
    auto iter = values.cbegin();

    for (size_t r = 1; r <= kNumRows; ++r) {
        Row& row = tri[r];
        row.resize(r + 1);  // Add extra column to homogenize adding sums involving right element in row.

        row[1] = *iter++;

        for (size_t c = 1; c < r; ++c)
            row[c + 1] = row[c] + *iter++;
    }

    return tri;
}


int64_t solve() {
    int64_t min_sum{ 999'999'999'999 };

    const auto tri = get_row_sums();

    for (size_t top_row = 1; top_row < tri.size(); ++top_row) {
        for (size_t vert_col = 1; vert_col <= top_row; ++vert_col) {
            int64_t sum{ 0 };
            for (size_t row = top_row; row < tri.size(); ++row) {
                sum += tri[row][vert_col + row - top_row] - tri[row][vert_col - 1];
                if (sum < min_sum) {
                    min_sum = sum;
                    std::cout << "New minimum sum: " << min_sum << std::endl;
                }
            }
        }
    }

    return min_sum;
}


int main()
{
    std::cout << "Hello World!\n";

    {
        auto vals = get_values();
        auto tri = get_row_sums();
    }

    std::cout << "The answer is " << solve() << std::endl;
}
