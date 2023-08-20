// A triangular pyramid is constructed using spherical balls so that each ball
// rests on exactly three balls of the next lower level.
//
//      n = 0                       1
// 
//      n = 1                       1
//                                1   1
// 
//      n = 2                       1
//                                2   2
//                              1   2   1
// 
//      n = 3                       1
//                                3   3
//                              3   6   3
//                            1   3   3   1
// 
// Then, we calculate the number of paths leading from the apex to each position:
//
// A path starts at the apexand progresses downwards to any of the three spheres
// directly below the current position.
//
// Consequently, the number of paths to reach a certain position is the sum of the
// numbers immediately above it (depending on the position, there are up to three
// numbers above it).
//
// The result is Pascal's pyramid and the numbers at each level n are the coefficients
// of the trinomial expansion (x + y + z)^ n.
//
// How many coefficients in the expansion of (x + y + z)^200,000 are multiples of 10^12?


#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

#include "big_int.h"
#include "prime_helper.h"


// This problem brings back fond memories of sophomore year of high school when I
// discovered Pascal's Pyramid on my own.  I knew about the relationship between
// the binomial expansion and Pascal's Triangle, and asked myself if there was some
// analog for the trionmial expansion.  As it turns out, there is, though I did not
// know it was called Pascal's Pyramid.
// I manually figured out how the numbers are structured as layers of a tetrahedron,
// and that each number is the sum of the 1-3 numbers above it in an inverse tetrahedron.
// It may be 33 years since I figured this out, but I still remember the pattern, which
// is the product of two combinatorials.  I wish I still had my old Pascal code, where
// I calculated all the coefficients for up to septinomial expansion, the 7-dimensional
// hypersimplex analog of Pascal's Pyramid.

// Compare the layers of Pascal's Pyramid with the Pascal's Triangle of the same size,
// and their ratios:
//      n = 0                       1                           1                           1
//
//      n = 1                       1                           1                           1
//                                1   1                       1   1                       1   1
// 
//      n = 2                       1                           1                           1
//                                2   2                       1   1                       2   2
//                              1   2   1                   1   2   1                   1   1   1
// 
//      n = 3                       1                           1                           1
//                                3   3                       1   1                       3   3
//                              3   6   3                   1   2   1                   3   3   3
//                            1   3   3   1               1   3   3   1               1   1   1   1
//
//      n = 4                       1                           1                           1
//                                4   4                       1   1                       4   4
//                              6  12   6                   1   2   1                   6   6   6
//                            4  12  12   4               1   3   3   1               4   4   4   4
//                          1   4   6   4   1           1   4   6   4   1           1   1   1   1   1
//
//      n = 5                       1                           1                           1
//                                5   5                       1   1                       5   5
//                             10  20  10                   1   2   1                  10  10  10
//                           10  30  30  10               1   3   3   1              10  10  10  10
//                          5  20  30  20   5           1   4   6   4   1           5   5   5   5   5
//                        1   5  10  10   5   1       1   5  10  10   5   1       1   1   1   1   1   1
//
//      n = 6                       1                           1                           1
//                                6   6                       1   1                       6   6
//                             15  30  15                   1   2   1                  15  15  15
//                           20  60  60  20               1   3   3   1              20  20  20  20
//                         15  60  90  60  15           1   4   6   4   1          15  15  15  15  15
//                        6  30  60  60  30   6       1   5  10  10   5   1       6   6   6   6   6   6
//                      1   6  15  20  15   6   1   1   6  15  20  15   6   1   1   1   1   1   1   1   1
//
// We can see how the n'th layer of Pascal's Pyramid is a scaled version of the Pascal's Triangle
// of the same size.  The scaling is not completely homogeneous across the whole layer, but is
// constant across a row in the layer.  The scale factors for each row are the same as the last
// row of Pascal's Triangle.  This makes sense based on the correlation to the trinomal expansion.
// It doesn't matter if we do (x + y + z)^n or (z + x + y)^n, we should get the same final sum,
// so there is a 120 degree rotational symmetry to the pyramid layers.  If we rotate either CW or
// CCW by 120 degrees, the bottom row becomes the diagonal edge.  In Pascal's Triangle, these edges
// are all 1's, so the row-wise scale factor is equal to the bottom row of the Triangle.

// There are multiple ways to calculate Pascal's Triangle.
// The simplest is through summation, using the row above to calculate the next row.  We can do
// this, but for (x + y + z)^200000 the number of terms in the triangle is the 200,000th triangular
// number T(n), where T(n) = n * (n + 1) / 2.
//      T(200000) = 200000 * 200001 / 2 = 20,000,100,000
// This size is almost tractable, if we could use uint64_t values then we would need 160GB of memory.
// With the rotational symmetry, we could get away with only calculating one third of the values,
// which is ~54GB.  The problem is that we need many more bits than uint64_t to store all the values
// exactly.  Even for the non-scaled Pascal's Triangle, the 69th row is the first one that exceeds
// the uint64_t range.  So we could switch to BigInt, but then need way more memory to solve the problem.
// One could argue that we don't need to store the entire Triangle, only 2 rows at a time.  This would
// then involve 2 passes, one to calculate the last row, and another to use the last row to scale all
// the other rows and count the number of elements that are multiples of 10^12.

// The next way to calculate the values in a row of Pascal's Triangle is with combinatorics.  If we
// are on row R, then we know there are R elements in the row, and they can be calculated as the number
// of combinations C(R-1, i) for the i'th element in the row (using 0-based indexing).  Consider the 5th
// row, which has the values { 1, 4, 6, 4, 1 }:
//      C(4, 0) = 4! / (0! * (4-0)!) = 4! / 4! = 1
//      C(4, 1) = 4! / (1! * (4-1)!) = 4! / 3! = 4
//      C(4, 2) = 4! / (2! * (4-2)!) = 4! / (2! * 2!) = 4 * 3 / 2 = 6
//      C(4, 3) = 4! / (3! * (4-3)!) = 4! / 3! = 4
//      C(4, 4) = 4! / (4! * (4-4)!) = 4! / 4! = 1
// This works, but still leads to the very large numbers if we multiply everything out.

// A related approach uses a trick about the ratios of adjacent elements in the same row of Pascal's
// Triangle.  Look at the combinations expanded out, instead of using factorials:
//      4 * 3 * 2 * 1   4 * 3 * 2 * 1   4 * 3 * 2 * 1   4 * 3 * 2 * 1   4 * 3 * 2 * 1
//      ------------- , ------------- , ------------- , ------------- , -------------
//      4 * 3 * 2 * 1   1 * 3 * 2 * 1   2 * 1 * 2 * 1   3 * 2 * 1 * 1   4 * 3 * 2 * 1
//
// The numerators never change, but the denominators do, but always with 4 terms in the product.
// Look at how to go from one term P(n) to the next:
//      P(0) = 1
//      P(1) = P(0) * 4 / 1
//      P(2) = P(1) * 3 / 2
//      P(3) = P(2) * 2 / 3
//      P(4) = P(3) * 1 / 4
// If you are on row R, then there is the recurrence relation:
//      P(n+1) = P(n) * (R - n - 1) / (n + 1)
// This still involves calculating very large products, or does it?

// We are looking for very specific numbers, those that are multiples of 10^12.  We don't need to
// calculate the exact numbers in Pascal's Pyramid to do this, all we need to do is count how many
// times 2 and 5 show up in the prime factorization of the elements.  If we have 12 or more 2's and
// 12 or more 5's as factors, then we are guaranteed to have a product that is a multiple of 10^12.
// The recurrence relation can help us keep track of the counts of 2s and 5s in each term in the Pyramid.
//


using Count = int64_t;  // Keep signed to identify bugs when we hit negative counts
using RowCounts = std::vector<Count>;

struct Factors {
    Count pow2;
    Count pow5;
};
using FactorCounts = std::vector<Factors>;


std::ostream& operator<<(std::ostream& str, std::vector<BigInt> row) {
    for (const auto& val : row) {
        if (val == 0)
            break;
        std::cout << val << "\t";
    }
    std::cout << std::endl;
    return str;
}

void calculate_rows(int64_t num_rows) {
    std::vector<BigInt> row_a, row_b;
    row_a.resize(num_rows);
    row_b.resize(num_rows);

    row_a[0] = BigInt{ 1 };
    row_b[0] = row_b[1] = BigInt{ 1 };

    std::cout << row_a << std::endl << row_b << std::endl;

    for (int64_t row = 3; row <= num_rows; row += 2) {
        if (row % 1000 == 1)
            std::cout << row << std::endl;

        for (size_t i = 1; i < row_b.size(); ++i) {
            row_a[i] = row_b[i - 1] + row_b[i];
        }
        row_a[row - 1] = BigInt{ 1 };

        for (size_t i = 1; i < row_a.size(); ++i) {
            row_b[i] = row_a[i - 1] + row_a[i];
        }
        row_b[row] = BigInt{ 1 };

    }

    std::cout << row_a << std::endl;
//    std::cout << row_b << std::endl;

    size_t max_pow10{ 0 };
    for (const auto& val : row_a) {
        const auto& digits = val.get_digits();
        size_t pow10{ 0 };
        for (auto iter = digits.crbegin(); iter != digits.crend(); ++iter) {
            if (*iter == 0)
                ++pow10;
            else
                break;
        }
        max_pow10 = std::max(max_pow10, pow10);
    }

    std::cout << "max power of 10 = " << max_pow10 << std::endl;
}


// Use 1-based indexing, so that size of row equals row index.
FactorCounts get_factors(int64_t row, PrimeHelper &helper) {
    FactorCounts ret;
    ret.resize(row);

    helper.get_primes(row);

    ret[0] = { 0, 0 };
    for (int64_t n = 1; n < row; ++n) {
        ret[n] = ret[n - 1];

        auto den_factors = helper.get_factorization_fast(n);
        auto num_factors = helper.get_factorization_fast(row - n);

        if (num_factors.find(2) != num_factors.cend())
            ret[n].pow2 += num_factors.at(2);
        if (num_factors.find(5) != num_factors.cend())
            ret[n].pow5 += num_factors.at(5);

        if (den_factors.find(2) != den_factors.cend())
            ret[n].pow2 -= den_factors.at(2);
        if (den_factors.find(5) != den_factors.cend())
            ret[n].pow5 -= den_factors.at(5);
    }

    return ret;
}


RowCounts solve(int64_t num_rows) {
    RowCounts ret;
    ret.resize(num_rows + 1);

    PrimeHelper helper;
    helper.get_primes(num_rows+1);

    FactorCounts scale = get_factors(num_rows+1, helper);

    for (int64_t row = 0; row <= num_rows; ++row) {
        const auto& [scale2, scale5] = scale[row];
        auto& count = ret[row];

        if (row % 1000 == 0)
            std::cout << row << std::endl;

        //auto max_factors = helper.get_factorization_fast(row);
        //Count max_pow2{ 0 };
        //Count max_pow5{ 0 };
        //if (max_factors.find(2) != max_factors.cend())
        //if (max_factors.find(5) != max_factors.cend())
        //    max_pow5 = max_factors.at(5);
        //if ((scale2 + max_pow2 < 12) || (scale5 + max_pow5 < 12))
        //    continue;

        //std::cout << "Row " << row << " should have some values" << std::endl;

        auto factors = get_factors(row+1, helper);

        for (const auto& [pow2, pow5] : factors) {
            if ((scale2 + pow2 >= 12) && (scale5 + pow5 >= 12))
                ++count;
        }
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

//    {
//        calculate_rows(1002);
////        return 0;
//    }

//    {
//        PrimeHelper helper;
//        auto factors = get_factors(200001, helper);
//        Count max_pow2{ 0 };
//        Count max_pow5{ 0 };
//        Count max_pow10{ 0 };
//        for (const auto& [pow2, pow5] : factors) {
////            std::cout << "2^" << pow2 << " * 5^" << pow5 << std::endl;
//            max_pow2 = std::max(max_pow2, pow2);
//            max_pow5 = std::max(max_pow5, pow5);
//            max_pow10 = std::max(max_pow10, std::min(pow2, pow5));
//        }
//        std::cout << "Max power of 2 = " << max_pow2 << ", max power of 5 = " << max_pow5 << ", max power of 10 = " << max_pow10 << std::endl;
//        return 0;
//    }

    //{
    //    PrimeHelper helper;
    //    auto factors = get_factors(9, helper);
    //    Count max_pow2{ 0 };
    //    Count max_pow5{ 0 };
    //    Count max_pow10{ 0 };
    //    for (const auto& [pow2, pow5] : factors) {
    //        std::cout << "2^" << pow2 << " * 5^" << pow5 << std::endl;
    //        max_pow2 = std::max(max_pow2, pow2);
    //        max_pow5 = std::max(max_pow5, pow5);
    //        max_pow10 = std::max(max_pow10, std::min(pow2, pow5));
    //    }
    //    std::cout << "Max power of 2 = " << max_pow2 << ", max power of 5 = " << max_pow5 << ", max power of 10 = " << max_pow10 << std::endl;
    //}

    {
        //auto counts = solve(10);
        auto counts = solve(200'000);
        int64_t non_zero{ 0 };
        for (const auto& c : counts) {
            non_zero += c > 0;
        }
        std::cout << "There are " << non_zero << " rows with multiples of 10^12 in them, for a total of "
                 << std::accumulate(counts.begin(), counts.end(), Count{ 0 }) << std::endl;;
    }
}


// There are 114225 rows with multiples of 10^12 in them, for a total of 479742450
