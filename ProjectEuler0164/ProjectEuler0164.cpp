// How many 20 digit numbers (without any leading zero) exist such that no
// three consecutive digits of have a sum greater than 9?

#include <iostream>
#include <numeric>
#include <vector>

// Let's start by enumerating the 3 digit sequences that have a sum < 10:
//      000, 001, 002, 003, 004, 005, 006, 007, 008, 009
//      010, 011, 012, 013, 014, 015, 016, 017, 018
//      020, 021, 022, 023, 024, 025, 026, 027
//      030, 031, 032, 033, 034, 035, 036
//      040, 041, 042, 043, 044, 045
//      050, 051, 052, 053, 054
//      060, 061, 062, 063
//      070, 071, 072
//      080, 081
//      090
//      100, 101, 102, 103, 104, 105, 106, 107, 108
//      110, 111, 112, 113, 114, 115, 116, 117
//      120, 121, 122, 123, 124, 125, 126
//      130, 131, 132, 133, 134, 135
//      140, 141, 142, 143, 144
//      150, 151, 152, 153
//      160, 161, 162
//      170, 171
//      180
//      200, 201, 202, 203, 204, 205, 206, 207
//      210, 211, 212, 213, 214, 215, 216
//      220, 221, 222, 223, 224, 225
//      230, 231, 232, 233, 234
//      240, 241, 242, 243
//      250, 251, 252
//      260, 261
//      270
//      300, 301, 302, 303, 304, 305, 306
//      310, 311, 312, 313, 314, 315
//      320, 321, 322, 323, 324
//      330, 331, 332, 333
//      340, 341, 342
//      350, 351
//      360
//      400, 401, 402, 403, 404, 405
//      410, 411, 412, 413, 414
//      420, 421, 422, 423
//      430, 431, 432
//      440, 441
//      450
//      500, 501, 502, 503, 504
//      510, 511, 512, 513
//      520, 521, 522
//      530, 531
//      540
//      600, 601, 602, 603
//      610, 611, 612
//      620, 621
//      630
//      700, 701, 702
//      710, 711
//      720
//      800, 801
//      810
//      900
// The sequences were organzied like this to show that the number of sequences that
// start with the same digit is a triangle number T(10-n) = n*(n+1)/2.
// So for 3 digit sequences, we have 55 + 45 + 36 + 28 + 21 + 15 + 10 + 6 + 3 + 1 = 220.

// How to construct longer sequences?  We can look at a 4 digit number ABCD as being
// comprised of two 3 digit sequences - ABC and BCD.  Both of these must be in the set of
// 220 valid 3 digit numbers, but the values of A & D are completely decoupled.

// First tried a 10x10 matrix showing how many 3-digit sequences can start with the
// digits corresponding to row and column.  This yields the correct answer for 3-digit
// numbers but not longer sequences.
//
//       0  1  2  3  4  5  6  7  8  9
//    0 10  9  8  7  6  5  4  3  2  1
//    1  9  8  7  6  5  4  3  2  1  0
//    2  8  7  6  5  4  3  2  1  0  0
//    3  7  6  5  4  3  2  1  0  0  0
//    4  6  5  4  3  2  1  0  0  0  0
//    5  5  4  3  2  1  0  0  0  0  0
//    6  4  3  2  1  0  0  0  0  0  0
//    7  3  2  1  0  0  0  0  0  0  0
//    8  2  1  0  0  0  0  0  0  0  0
//    9  1  0  0  0  0  0  0  0  0  0

// Need to use a 100x100 adjacency matrix instead.  The rows are indexed by the
// first 2 digits, and the columns by last 2 digits.  The adjacency matrix will
// 0 if the combination of row and column don't share a common digit, or if they
// do but the sum is >9.  When they do match the digit and sum <10, then the
// matrix will have a 1 value in that cell.  So for a 3-digit sequence ABC, the only
// cells with a 1 value are those in row (10*A+B) and column (10*B+C) where
//      A + B + C < 10

// This will result in 220 of the 10,000 cells in the matrix with a value of 1,
// so the sum of all the matrix cells is the number of 3-digit sequences that
// match this pattern.
// Another way of interpreting the adjacency matrix is that you have a graph with 100
// nodes, numbered [00, 99], and the matrix has a 1 if and only if there is an edge
// between the nodes that correspond to its row and column indices.  When no edge is
// present, then it has a 0 value.
// For example, if you want the 3-digit number 456, you can inspect the graph to
// see if nodes 45 and 56 are connected, which corresponds to if the matrix has a
// 1 at cell [45, 56].

// How to extend this to larger numbers/longer sequences?  For a 4-digit number like
// 4567, we can look for a connected path from 45 -> 56 -> 67.  This would involve
// seeing if both matrix cells [45, 56] and [56, 67] have a 1 value in them.  If they
// do, then the number qualifies, otherwise it doesn't
// For 20-digit numbers, this means looking for 18 edges in the graph.  While this is
// doable, it doesn't scale well to testing all 10^20 numbers.

// Instead, what does it mean if you multiply the adjacency matrix by itself?  Think
// about the mechanics of matrix multiplication.  For any cell in the squared matrix,
// you take the dot product of a row and a column - for cell [AB, CD] you get:
//     square[AB, CD] =  sum   { adj[AB, i] * adj[i, CD] }
//                     i=0..99
//
// So the value in the square matrix tells you how many 4-digit numbers of the form
// ABCD are valid.  In the case of 4-digit numbers, there is still at most only 1
// valid path from AB -> CD, because of the shared overlap of BC.  But for higher
// powers of the matrix and longer paths we start to get bigger numbers in the matrix.
// When you sum all the cells in a matrix power N, you get the total number of valid
// (N+2)-digit numbers.  So for 20-digit numbers we just need to sum all the terms
// in the 18th power of the adjacency matrix.

// There is one gotcha to this logic.  The problem states that we only want 20-digit
// numbers that do not have leading 0s.  To accommodate this, we need a modified
// version of the adjacency matrix, where the first 10 rows are all 0s.  This means
// that there are no valid paths that start at nodes 00 through 09.  So the final
// answer is the sum of all the cells in the modified adjacency matrix multiplied by
// the 17th power of the normal adjacency matrix.


uint64_t brute_force(int16_t num_digits) {
    uint64_t ret{ 0 };

    int64_t max_val{ 1 };
    for (int16_t n = 0; n < num_digits; ++n)
        max_val *= 10;

    std::cout << max_val << std::endl;

    for (int64_t n = max_val / 10; n < max_val; ++n) {
        int64_t tmp{ n };

        int64_t d_2{ tmp % 10 };
        tmp /= 10;
        int64_t d_1{ tmp % 10 };
        tmp /= 10;
        int64_t d_0{ tmp % 10 };
        tmp /= 10;

        int64_t sum{ d_0 + d_1 + d_2 };
        if (sum > 9)
            continue;

        for (int16_t d = 0; d < num_digits; ++d) {
            d_0 = tmp % 10;

            sum += d_0 - d_2;
            if (sum > 9)
                break;

            d_2 = d_1;
            d_1 = d_0;
            tmp /= 10;
        }

        if (sum < 10) {
            ++ret;
            std::cout << n << "  ";
        }
    }

    std::cout << std::endl;

    return ret;
}

using MatrixRow = std::vector<uint64_t>;
using Matrix = std::vector<MatrixRow>;

Matrix get_adjacency_matrix() {
    Matrix mat;

    mat.resize(100);
    for (auto& row : mat)
        row.resize(100);

    for (size_t row = 0; row < 100; ++row) {
        for (size_t col = 0; col < 100; ++col) {
            if ((row % 10) == (col / 10)) {
                if ((row / 10) + (row % 10) + (col % 10) < 10)
                    mat[row][col] = 1;
            }
        }
    }

    return mat;
}

Matrix get_start_adjacency_matrix() {
    Matrix mat;

    mat.resize(100);
    for (auto& row : mat)
        row.resize(100);

    for (size_t row = 10; row < 100; ++row) {
        for (size_t col = 0; col < 100; ++col) {
            if ((row % 10) == (col / 10)) {
                if ((row / 10) + (row % 10) + (col % 10) < 10)
                    mat[row][col] = 1;
            }
        }
    }

    return mat;
}


Matrix matrix_product(const Matrix& lhs, const Matrix& rhs) {
    std::size_t out_height = lhs.size();
    std::size_t out_width = rhs[0].size();
    std::size_t common = rhs.size();

    Matrix prod;
    prod.reserve(out_height);
    for (std::size_t row = 0; row < out_height; ++row) {
        MatrixRow mat_row;
        mat_row.reserve(out_width);
        for (std::size_t col = 0; col < out_width; ++col) {
            uint64_t sum{ 0 };
            for (std::size_t ind = 0; ind < common; ++ind) {
                sum += lhs[row][ind] * rhs[ind][col];
            }
            mat_row.push_back(sum);
        }
        prod.push_back(mat_row);
    }

    return prod;
}

Matrix matrix_power(const Matrix& matrix, uint16_t power) {
    std::size_t out_size = matrix.size();

    Matrix mat(matrix);

    // Initialize output as identity matrix.
    Matrix mat_power;
    mat_power.reserve(out_size);
    for (std::size_t row = 0; row < out_size; ++row) {
        MatrixRow mat_row(out_size, 0);
        mat_row[row] = 1;
        mat_power.push_back(mat_row);
    }

    while (power >= 1) {
        if (power % 2) {
            // If odd power, then multiply by the squares.
            mat_power = matrix_product(mat_power, mat);
        }

        // Square the matrix
        mat = matrix_product(mat, mat);
        // Next iteration
        power /= 2;
    }

    return mat_power;
}

void print_matrix(const Matrix& matrix) {
    for (const auto& row : matrix) {
        for (const auto& cell : row) {
            std::cout << cell << "  ";
        }
        std::cout << std::endl;
    }
}

uint64_t sum_matrix(const Matrix& matrix) {
    uint64_t sum{ 0 };
    for (const auto& row : matrix) {
        sum += std::accumulate(row.cbegin(), row.cend(), 0ULL);
    }

    return sum;
}

int main()
{
    std::cout << "Hello World!\n";

    {
        std::cout << brute_force(4) << std::endl;
    }

    {
        Matrix mat = get_start_adjacency_matrix();
        std::cout << sum_matrix(mat) << std::endl;
    }

    {
        Matrix start = get_start_adjacency_matrix();
        Matrix mat = get_adjacency_matrix();
        Matrix square = matrix_product(start, mat);
        std::cout << sum_matrix(square) << std::endl;
//        print_matrix(matrix_power(mat, 3));
    }

    {
        Matrix start = get_start_adjacency_matrix();
        Matrix mat = get_adjacency_matrix();
        Matrix cube = matrix_product(start, matrix_power(mat, 2));
        std::cout << sum_matrix(cube) << std::endl;
    }

    {
        Matrix start = get_start_adjacency_matrix();
        Matrix mat = get_adjacency_matrix();
        Matrix prod = matrix_product(start, matrix_power(mat, 17));
        std::cout << sum_matrix(prod) << std::endl;
    }
}
