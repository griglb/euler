// In a 3x2 cross-hatched grid, a total of 37 different rectangles could be situated
// within that grid as indicated in the sketch.
//
//      +---+---+---+       +---+---+---+       +---+---+---+       +---+---+---+
//      |\@/|\ /|\ /|       |\@/|\@/|\ /|       |\ /|\ /|\ /|       |\ /|\ /|\ /|
//      |@x@| x | x |       |@x@|@x@| x |       | x | x | x |       | x@|@x | x |
//      |/@\|/ \|/ \|       |/@\|/@\|/ \|       |/@\|/ \|/ \|       |/@\|/@\|/ \|
//      +---+---+---+ 6     +---+---+---+ 2     +---+---+---+ 7     +---+---+---+ 2
//      |\ /|\ /|\ /|       |\@/|\@/|\ /|       |\@/|\ /|\ /|       |\@/|\@/|\ /|
//      | x | x | x |       |@x@|@x@| x |       | x | x | x |       | x@|@x | x |
//      |/ \|/ \|/ \|       |/@\|/@\|/ \|       |/ \|/ \|/ \|       |/ \|/ \|/ \|
//      +---+---+---+       +---+---+---+       +---+---+---+       +---+---+---+
// 
//
//      +---+---+---+       +---+---+---+       +---+---+---+       +---+---+---+
//      |\@/|\@/|\ /|       |\@/|\ /|\ /|       |\ /|\ /|\ /|       |\ /|\ /|\ /|
//      |@x@|@x@| x |       |@x@| x | x |       | x@|@x | x |       | x | x | x |
//      |/@\|/@\|/ \|       |/@\|/ \|/ \|       |/@\|/ \|/ \|       |/@\|/ \|/ \|
//      +---+---+---+ 4     +---+---+---+ 3     +---+---+---+ 4     +---+---+---+ 4
//      |\ /|\ /|\ /|       |\@/|\ /|\ /|       |\@/|\ /|\ /|       |\@/|\ /|\ /|
//      | x | x | x |       |@x@| x | x |       | x | x | x |       | x@|@x | x |
//      |/ \|/ \|/ \|       |/@\|/ \|/ \|       |/ \|/ \|/ \|       |/ \|/ \|/ \|
//      +---+---+---+       +---+---+---+       +---+---+---+       +---+---+---+
// 
//
//      +---+---+---+       +---+---+---+       +---+---+---+       +---+---+---+
//      |\@/|\@/|\@/|       |\@/|\@/|\@/|       |\ /|\ /|\ /|       |\ /|\ /|\ /|
//      |@x@|@x@|@x@|       |@x@|@x@|@x@|       | x | x@|@x |       | x@|@x | x |
//      |/@\|/@\|/@\|       |/@\|/@\|/@\|       |/ \|/@\|/ \|       |/ \|/@\|/ \|
//      +---+---+---+ 2     +---+---+---+ 1     +---+---+---+ 1     +---+---+---+ 1
//      |\ /|\ /|\ /|       |\@/|\@/|\@/|       |\ /|\@/|\ /|       |\ /|\@/|\ /|
//      | x | x | x |       |@x@|@x@|@x@|       | x@|@x | x |       | x | x@|@x |
//      |/ \|/ \|/ \|       |/@\|/@\|/@\|       |/ \|/ \|/ \|       |/ \|/ \|/ \|
//      +---+---+---+       +---+---+---+       +---+---+---+       +---+---+---+
//
// There are 5 grids smaller than 3x2, vertical and horizontal dimensions being important,
//      1x1, 2x1, 3x1, 1x2, and 2x2.
// If each of them is cross-hatched, the following number of different rectangles could be
// situated within those smaller grids:
//      1x1     1
//      2x1     4
//      3x1     8
//      1x2     4
//      2x2    18
// Adding those to the 37 of the 3x2 grid, a total of 72 different rectangles could be
// situated within 3x2 and smaller grids.
//
// How many different rectangles could be situated within 47x43 and smaller grids?


#include <iostream>
#include <map>
#include <numeric>
#include <utility>
#include <vector>


// Consider the counts of each sized rectangle in each of the grids up to 3x2:
//  rect     1x1     2x1     3x1     1x2     2x2     3x2
//  1x1       1       2       3       2       4       6
//  2x1       0       1       2       0       2       4
//  3x1       0       0       1       0       0       2
//  1x2       0       0       0       1       2       3
//  2x2       0       0       0       0       1       2
//  3x2       0       0       0       0       0       1
//  d1x1      0       1       2       1       4       7
//  d2x1      0       0       0       0       2       4
//  d3x1      0       0       0       0       0       1
//  d2x2      0       0       0       0       1       2
//  d1x2      0       0       0       0       2       4
//  d1x3      0       0       0       0       0       1
// ------    ---     ---     ---     ---    ----    ----
//  total     1       4       8       4      18      37

// The axis-aligned rectangles are easier to analyze and count.  For a given rectangle
// sized MxN, it can include any axis-aligned rectangles up to the size MxN.
// For a rectangle of size kxl, it can be positioned in the upper left corner of the MxN
// rectangle and then translated horizontally or vertically.  These orthogonal translations
// can be decoupled, so that the total number of positions is the product of the number
// of horizontal and vertical positions.  The number of positions in each dimension is
// the number of empty squares in that dimension plus 1.  So a kxl rectangle on an MxN
// rectangle can have
//      (M - k + 1) * (N - l + 1)
// different positions.
// If we look to sum how many positions a kxl rectangle can have in an MxN or smaller
// rectangle, then it is the sum of that expression for all MxN up to the max size.
// As the table above shows, across a row you see a sequence 0s and then an arithmetic
// sequence with an increment that grows with each new sequence.  So we have a sequence
// of scaled sequences, which is the product of two sequences that yield triangle numbers.
// The total sum of a row is the product of two triangle numbers:
//      total = {(M - k + 1) * (M - k + 2) / 2} * {(N - l + 1) * (N - l + 2) / 2}
 
// The diagonal rectangles are a little harder to analyze and count.
// We can count them either along the diagonals, or along the axes.  For the latter,
// there are two rectilinear arrays of diamonds - those with a vertical divider in the
// uppermost cell and those with a horizontal divider.
//
// For a MxN rectangle, there are (M-1)*N 1x1 diamonds with a vertical divider and
// M*(N-1) 1x1 diamonds with a horizontal divider.  So the total number of 1x1 diamonds is:
//      d1x1 = (M-1)*N + M*(N-1) = 2*M*N - M - N
//
// How to enumerate the 1x1 diamonds using the diagonals?  First we need to count the
// diagonals.  There are M-1 that end on the top edge, and N-1 that end on the right edge,
// so the total number of diagonals is:
//      num_diag = M + N - 2
// How many 1x1 diamonds can fit on each diagonal?  The first and last diagonal will each
// have only 2 diamonds in them, one of each orientation.  Adjacent rows will add 2 more,
// until you reach the maximum number of diamonds in a diagonal.  This is:
//      max_diamonds = 2 * min(M, N) - 1

// Let's use the 3x2 rectangle above, we know the following:
//      num_diag = 3 + 2 - 2 = 3
//      max_diamonds = 2 * min(3, 2) - 1 = 4 - 1 = 3
// So the cardinality of each diagonal is:
//      min([2, 4, 2], 3) = [2, 3, 2]
// This yields 7 diamonds, which matches the number in the problem statement and what is
// calculated above:
//      d1x1 = 2*M*N - M - N = 2*3*2 - 3 - 2 = 7

// Let's try a bigger rectangle, 7x3
// 
//      +---+---+---+---+---+---+---+
//      |\ /|\ /|\ /|\ /|\ /|\ /|\ /|
//      | x | x | x | x | x | x | x |
//      |/ \|/ \|/ \|/ \|/ \|/ \|/ \|
//      +---+---+---+---+---+---+---+
//      |\ /|\ /|\ /|\ /|\ /|\ /|\ /|
//      | x | x | x | x | x | x | x |
//      |/ \|/ \|/ \|/ \|/ \|/ \|/ \|
//      +---+---+---+---+---+---+---+
//      |\ /|\ /|\ /|\ /|\ /|\ /|\ /|
//      | x | x | x | x | x | x | x |
//      |/ \|/ \|/ \|/ \|/ \|/ \|/ \|
//      +---+---+---+---+---+---+---+
//
// Let's verify the 2 ways of counting 1x1 diamonds yield the same answer.
//      d1x1 = 2*M*N - M - N = 2*7*3 - 7 - 3 = 32
//
//      num_diag = M + N - 2 = 7 + 3 - 2 = 8
//      max_diamonds = 2 * min(M, N) - 1 = 2 * min(7, 3) - 1 = 6 - 1 = 5
// The diagonal counts then are:
//      min([2, 4, 6, 8, 8, 6, 4, 2], 5) = [2, 4, 5, 5, 5, 5, 4, 2]
// Sum these up, you get 2 + 4 + 5 + 5 + 5 + 5 + 4 + 2 = 32

// That gives the counts for 1x1 diamonds, what about other sizes?
// The next smallest is 1x2, which is the SW-NE oriented one.
// If we use the diagonals, then we can see that the number of 1x2
// diamonds on each diagonal is 1 less than the numbder of 1x1 diamonds.
//
// Going back to 3x2 rectangle, the 1x1 diagonals have these counts:
//      [2, 3, 2]
// Substract 1 from each of these, we get:
//      [1, 2, 1]
// Add it up, you get the number of places (4) the 1x2 diamond can fit, which matches.
// This can be extrapolated for other 1xk or kx1 sized diamonds:
//      1x1 = [2, 3, 2]
//      1x2 = [1, 2, 1]
//      1x3 = [0, 1, 0]
//      1x4 = [0, 0, 0]
//      2x1 = [1, 2, 1]
//      3x1 = [0, 1, 0]
//      4x1 = [0, 0, 0]
// Note we don't go into negative numbers, we can't have a negative count, so a max
// is involved there to keep the values non-negative.

// This can be extended into larger diamonds.  Let's look at 2x2 next.
// First, the number of diagonals is decreased by 1.  This is because there
// needs to be 2 diagonals involved in any 2x2 diamond, so the number of
// diagonals that can contain the upper row in the diamond is one less than
// the total number of diagonals.
// Second, the max number of diamonds in a diagonal is 2 less than before.  We have
// two adjacent diagonals, which are offset by 1, so the common length is 1 less.
// But there needs to be 2 columns for the diamond, so we subtract another 1.
// So we now have:
//      num_diag = M + N - 3
//      max_diamonds = 2 * min(M, N) - 3
// How to calculate the counts on each diagonal?  We start with the 1x2 counts
// and remove one of the largest values from the middle to account for the extra
// column of cells.
//
// For the 3x2 rectangle, this means starting with [1, 2, 1] and removing the 2
// in the middle, yielding [1, 1], which gives the 2 positions in the problem statement.
//
// For the 7x3, we go through this process:
//      d1x1 = [2, 4, 5, 5, 5, 5, 4, 2]
//      d1x2 = [1, 3, 4, 4, 4, 4, 3, 1]
//
//      num_diag = M + N - 3 = 7 + 3 - 3 = 7
//      max_diamonds = 2 * min(M, N) - 3 = 2 * min(7, 3) - 3 = 6 - 3 = 3
//      d2x2 = min([1, 3, 4, 4, 4, 3, 1], 3) = [1, 3, 3, 3, 3, 3, 1]
//
// Counting the other way, we have 2 rectilinear grids of diamonds, with one less
// row and column than before:
//      d2x2 = (M-1)*(N-1) + (M-1)*(N-2)
//      d2x2 = M*N - M - 2*N + 2 + M*N - 2*M - N + 2
//      d2x2 = 2*M*N - 3*M - 3*N + 4
// For 3x2, this yields:
//      d2x2 = 2*3*2 - 3*3 - 3*2 + 4 = 12 - 9 - 6 + 4 = 1
//

// Let's look at the number of positions for each rectangle for different sized grids:
//      d1x1  M  1  2  3  4  5  6  7
//          N 1  0  1  2  3  4  5  6    (M-1)*N + M*(N-1)
//            2  1  4  7 10 13 16 19
//            3  2  7 12 17 22 27 32
//            4  3 10 17 24 31 38 45
//            5  4 13 22 31 40 49 58
//
//      d1x2  M  1  2  3  4  5  6  7
//          N 1  0  0  0  0  0  0  0    2*(M-1)*(N-1)
//            2  0  2  4  6  8 10 12
//            3  0  4  8 12 16 20 24
//            4  0  6 12 18 24 30 36
//            5  0  8 16 24 32 40 48
//
//      d1x3  M  1  2  3  4  5  6  7
//          N 1  0  0  0  0  0  0  0    max((M-2)*(N-1) + (M-1)*(N-2), 0)
//            2  0  0  1  2  3  4  5
//            3  0  1  4  7 10 13 16
//            4  0  2  7 12 17 22 27
//            5  0  3 10 17 24 31 38
//
//      d1x4  M  1  2  3  4  5  6  7
//          N 1  0  0  0  0  0  0  0    max(2*(M-2)*(N-2), 0)
//            2  0  0  0  0  0  0  0
//            3  0  0  2  4  6  8 10
//            4  0  0  4  8 12 16 20
//            5  0  0  6 12 18 24 30
//
//      d2x2  M  1  2  3  4  5  6  7
//          N 1  0  0  0  0  0  0  0    (M-1)*(N-1) + max((M-2)*(N-2), 0)
//            2  0  1  2  3  4  5  6
//            3  0  2  5  8 11 14 17
//            4  0  3  8 13 18 23 28
//            5  0  4 11 18 25 32 39
//
//      d2x3  M  1  2  3  4  5  6  7
//          N 1  0  0  0  0  0  0  0    max(2*(M-2)*(N-2), 0)
//            2  0  0  0  0  0  0  0
//            3  0  0  2  4  6  8 10
//            4  0  0  4  8 12 16 20
//            5  0  0  6 12 18 24 30
//
//      d3x3  M  1  2  3  4  5  6  7
//          N 1  0  0  0  0  0  0  0    max((M-3)*(N-2) + (M-2)*(N-3), 0)
//            2  0  0  0  0  0  0  0
//            3  0  0  0  1  2  3  4
//            4  0  0  1  4  7 10 13
//            5  0  0  2  7 12 17 22
//
//      d3x4  M  1  2  3  4  5  6  7
//          N 1  0  0  0  0  0  0  0    
//            2  0  0  0  0  0  0  0
//            3  0  0  0  0  0  0  0
//            4  0  0  0  2  4  6  8
//            5  0  0  0  4  8 12 16
//
//      d4x4  M  1  2  3  4  5  6  7
//          N 1  0  0  0  0  0  0  0    max((m-3)*(n-3), 0) + max((m-4)*(n-4), 0)
//            2  0  0  0  0  0  0  0
//            3  0  0  0  0  0  0  0
//            4  0  0  0  1  2  3  4
//            5  0  0  0  2  5  8 11
//
// We can see patterns emerge:
//   - There are 2 subsets of counts, based on whether the smaller rectangle dimension is even or odd
//   - The pattern for d(k+2)x(l+2) is the same as dkxl shifted by 2 in each dimension
//     - For example, d3x3 is d1x1 with 2 rows and columns of 0s to start,
//       same for d3x4 vs d1x2 or d4x4 vs d2x2
//   - dkxl and dlxk have the same counts
// All we need to do is calculate all the d1xY and d2xY patterns and we can then know all the other values
// with the shifts.
//



using Rectangle = std::pair<int64_t, int64_t>;
using Diagonals = std::map<Rectangle, std::vector<int64_t>>;

using Row = std::vector<uint64_t>;
using Array = std::vector<Row>;
using AllCounts = std::map < Rectangle, Array >;  // key is rectangle size, value is counts for MxN grid dims


Diagonals get_diagonals(const Rectangle& grid) {
    Diagonals ret;

    const auto& [M, N] = grid;
    const int64_t min_dim = std::min(M, N);
    const int64_t max_dim = std::max(M, N);

    // First do all the d1xY diagonal counts.
    {
        const int64_t num_diag = M + N - 2;
        const int64_t max_diamonds = M  == N ? 2 * min_dim - 2 : 2 * min_dim - 1;

        std::vector<int64_t> diag_counts(num_diag, 0);
        for (int64_t i = 0; i <= num_diag / 2; ++i) {
            diag_counts[i] = diag_counts[num_diag - i - 1] = std::min(max_diamonds, 2 * (i + 1));
        }
        ret[{1, 1}] = diag_counts;

        for (int16_t k = 2; k <= max_diamonds; ++k) {
            for (auto& el : diag_counts) {
                el = std::max(int64_t(0), el - 1);
            }

            ret[{1, k}] = diag_counts;
        }
    }

    // Then do the d2xY diagonal counts.
    {
        const int64_t num_diag = M + N - 3;
        const int64_t max_diamonds = 2 * min_dim - 3;

        std::vector<int64_t> diag_counts(num_diag, 0);
        if (num_diag > 0) {
            for (int64_t i = 0; i <= num_diag / 2; ++i) {
                diag_counts[i] = diag_counts[num_diag - i - 1] = std::min(max_diamonds, 2 * i + 1);
            }
        }
        ret[{2, 2}] = diag_counts;

        for (int16_t k = 3; k <= max_diamonds+1; ++k) {
            for (auto& el : diag_counts) {
                el = std::max(int64_t(0), el - 1);
            }

            ret[{2, k}] = diag_counts;
        }
    }

    return ret;
}


std::map<Rectangle, uint64_t> get_counts_for_grid(const Rectangle& grid) {
    std::map<Rectangle, uint64_t> ret;

    const auto& [M, N] = grid;
    const int64_t min_dim = std::min(M, N);
    const int64_t max_dim = std::max(M, N);

    // Start with the easier axis-aligned rectangles
    for (int16_t k = 1; k <= M; ++k) {
        for (int16_t l = 1; l <= N; ++l) {
            ret[{k, l}] = (M - k + 1) * (N - l + 1);
        }
    }

    // Special case for grid = {1, 1}, which has no diagonals
    if ((1 == M) && (1 == N))
        return ret;

    // Then do diamonds
    auto diags = get_diagonals(grid);

    // Prime the pumps for the 1-by and 2-by diagonal rectangles.
    {
        const int64_t max_diamonds = M == N ? 2 * min_dim - 2 : 2 * min_dim - 1;
        for (int16_t k = 1; k <= max_diamonds; ++k) {
            const auto& counts = diags.at({ 1, k });
            ret[{-1, -k}] = ret[{-k, -1}] = std::accumulate(counts.cbegin(), counts.cend(), 0LL);
        }
    }
    {
        const int64_t max_diamonds = 2 * min_dim - 3;
        for (int16_t k = 1; k <= max_diamonds; ++k) {
            const auto& counts = diags.at({ 2, k+1 });
            ret[{-2, -k-1}] = ret[{-k-1, -2}] = std::accumulate(counts.cbegin(), counts.cend(), 0LL);
        }
    }

    return ret;
}


AllCounts get_counts_up_to_grid(const Rectangle& max_grid) {
    AllCounts ret;

    const auto& [M, N] = max_grid;
    const int64_t min_dim = std::min(M, N);
    const int64_t max_dim = std::max(M, N);

    // Create an MxN array of 0s to use as starting point for new elements of ret.
    Array zeros;
    zeros.resize(N);
    for (auto& row : zeros)
        row.resize(M);

    // Populate all the count arrays for rectangles of size 1xY, Yx1, 2xY, and Yx2.
    for (int16_t l = 1; l <= N; ++l) {
        for (int16_t k = 1; k <= M; ++k) {
            const auto counts = get_counts_for_grid({ k, l });

            for (const auto &[rect, count] : counts) {
                if (ret.find(rect) == ret.end())
                    ret[rect] = zeros;

                ret[rect][l-1][k-1] = count;
            }
        }
    }


    // Calculate the larger rectangle counts by translating the smaller ones
    {
        const int64_t max_diamonds = M == N ? 2 * min_dim - 2 : 2 * min_dim - 1;
        for (int16_t k = 3; k <= max_diamonds; ++k) {
            for (int16_t l = k; l <= max_diamonds; ++l) {
                const auto iter = ret.find({ -k + 2, -l + 2 });
                if (iter == ret.cend())
                    continue;
                const auto& base_counts = iter->second;
                Array shifted = zeros;
                for (int16_t i = 2; i < M; ++i) {
                    for (int16_t j = 2; j < N; ++j) {
                        shifted[j][i] = base_counts[j - 2][i - 2];
                    }
                }
                if (shifted[N-1][M-1] > 0)
                    ret[{-k, -l}] = ret[{-l, -k}] = shifted;
            }
        }
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    Diagonals diags = get_diagonals({ 3, 3 });
    //    for (const auto& [rect, counts] : diags) {
    //        std::cout << "(" << rect.first << ", " << rect.second << ") = ";
    //        for (const auto& c : counts)
    //            std::cout << c << ", ";
    //        std::cout << std::endl;
    //    }
    //}

    //{
    //    auto counts = get_counts_for_grid({ 4, 3 });
    //    for (const auto &[rect, count] : counts) {
    //        std::cout << "(" << rect.first << ", " << rect.second << ") = " << count << std::endl;
    //    }
    //}

    //{
    ////    auto allCounts = get_counts_up_to_grid({ 3, 2 });
    //    auto allCounts = get_counts_up_to_grid({ 7, 5 });
    //    uint64_t total{ 0 };
    //    for (const auto& [rect, counts] : allCounts) {
    //        for (const auto& row : counts)
    //            total += std::accumulate(row.begin(), row.end(), 0ULL);

    //        if ((rect.first > 0) || (rect.second > 0))
    //            continue;
    //        std::cout << "(" << rect.first << ", " << rect.second << "):" << std::endl;
    //        std::cout << "\t  M";
    //        for (int16_t m = 1; m <= counts.begin()->size(); ++m)
    //            std::cout << "  " << m;
    //        std::cout << std::endl;
    //        for (int16_t n = 0; n < counts.size(); ++n) {
    //            std::cout << (n == 0 ? "\tN " : "\t  ") << n + 1;
    //            for (int16_t m = 0; m < counts.begin()->size(); ++m)
    //                std::cout << "  " << counts[n][m];
    //            std::cout << std::endl;
    //        }
    //    }
    //    std::cout << "The total of all " << allCounts.size() << " different sized rectangles is " << total << std::endl;
    //}

    {
        //auto allCounts = get_counts_up_to_grid({ 3, 2 });
        //auto allCounts = get_counts_up_to_grid({ 7, 5 });
        auto allCounts = get_counts_up_to_grid({ 47, 43 });
        uint64_t total{ 0 };
        for (const auto& [rect, counts] : allCounts) {
            for (const auto& row : counts)
                total += std::accumulate(row.begin(), row.end(), 0ULL);
        }
        std::cout << "The total of all " << allCounts.size() << " different sized rectangles is " << total << std::endl;
    }
}
