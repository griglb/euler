// In the 5 by 5 matrix below, the minimal path sum from the top left to the bottom right,
// by only moving to the right and down, is indicated with underscores is equal to 2427.
//              /_131_ 673  234  103   18 \
//              |_201_ _96__342_ 965  150 |
//              | 630  803 _746__422_ 111 |
//              | 537  699  497 _121_ 956 |
//              \ 805  732  524  _37__331_/
// Find the minimal path sum from the top left to the bottom right by only moving right
// and down in matrix.txt(right click and "Save Link/Target As..."), a 31K text file containing an 80 by 80 matrix.

#include <string.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>


using Row = std::vector<uint32_t>;
using Matrix = std::vector<Row>;


Matrix get_small_matrix() {
    return { { 131, 673, 234, 103,  18 },
             { 201,  96, 342, 965, 150 },
             { 630, 803, 746, 422, 111 },
             { 537, 699, 497, 121, 956 },
             { 805, 732, 524,  37, 331 } };
}


Matrix get_matrix() {
    // The file has no carriage returns, so read entire file into string.
#ifdef _CONSOLE
    std::ifstream infile("./p081_matrix.txt");
#else
    std::ifstream infile("./ProjectEuler0081/p081_matrix.txt");
#endif

    Matrix ret;

    while (!infile.eof()) {
        constexpr int16_t kLineLen{ 500 };
        char line[kLineLen + 1];
        infile.getline(&line[0], kLineLen);

        if (strlen(line) > 0) {
            std::stringstream str(line);
            Row row;
            while (!str.eof()) {
                int32_t val;
                char comma;
                str >> val >> comma;
                row.push_back(val);
            }
            ret.push_back(row);
        }
    }

    return ret;
}


uint64_t get_shortest_path(const Matrix& matrix) {
    // This is not quite as complicated as Dijkstra's pathfinding algorithm.
    // We can iteratively process this in SW-NE diagonals, which have
    // the same city distance from the upper left corner.
    // For each cell, you can find the min cost by adding its value to
    // the lesser of its 2 neighboring cells.

    Matrix costs{ matrix };

    size_t num_diag = matrix.size() + matrix[0].size() - 1;

    for (size_t diag = 1; diag < num_diag; ++diag) {
        for (size_t row = 0; row <= diag; ++row) {
            if (row >= costs.size())
                break;

            size_t col = diag - row;
            if (col >= costs[0].size())
                continue;

            auto up_val = (row > 0) ? costs[row - 1][col] : 999999;
            auto left_val = (col > 0) ? costs[row][col - 1] : 999999;
            costs[row][col] += std::min(up_val, left_val);
        }
    }

    return *costs.rbegin()->rbegin();
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    Matrix matrix = get_small_matrix();
    //    for (const auto& row : matrix) {
    //        for (const auto& el : row)
    //            std::cout << el << "  ";
    //        std::cout << std::endl;
    //    }
    //}

    //{
    //    Matrix matrix = get_matrix();
    //    for (const auto& row : matrix) {
    //        for (const auto& el : row)
    //            std::cout << el << "  ";
    //        std::cout << std::endl;
    //    }
    //}

    std::cout << "small matrix cost = " << get_shortest_path(get_small_matrix()) << std::endl;
    std::cout << "big matrix cost = " << get_shortest_path(get_matrix()) << std::endl;
}
