// NOTE: This problem is a significantly more challenging version of Problem 81.
//              /_131_ 673 _234__103_ _18_\
//              |_201_ _96__342_ 965 _150_|
//              | 630  803  746 _422__111_|
//              | 537  699  497 _121_ 956 |
//              \ 805  732  524  _37__331_/
// In the 5 by 5 matrix below, the minimal path sum from the top left to the bottom right,
// by moving left, right, up, and down, is indicated in underscores and is equal to 2297.
// Find the minimal path sum from the top left to the bottom right by moving left, right,
// up, and down in matrix.txt(right click and "Save Link/Target As..."), a 31K text file containing an 80 by 80 matrix.


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
    std::ifstream infile("./p083_matrix.txt");
#else
    std::ifstream infile("./ProjectEuler0083/p083_matrix.txt");
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


uint32_t get_shortest_path(const Matrix& matrix) {
    // We can use a full-blown gridded Dijkstra's pathfinding algorithm using rook's neighbors.
    // Data Structures:
    //   - cost matrix - same size as input
    //   - 4 direction based matrices that are recalculated every iteration
    // 
    // Initialize:
    //   - copy matrix[0][0] into cost[0][0], set the rest to Inf
    // 
    // Iterate:
    //   - make backup copy of cost matrix
    //   - calculate values for the 4 direction matrices
    //     - up_vals[i][j] = matrix[i][j] + cost[i+1][j] (when i+1 in range, else Inf)
    //     - down_vals[i][j] = matrix[i][j] + cost[i-1][j] (when i-1 in range, else Inf)
    //     - left_vals[i][j] = matrix[i][j] + cost[i][j+1] (when j+1 in range, else Inf)
    //     - right_vals[i][j] = matrix[i][j] + cost[i][j-1] (when j-1 in range, else Inf)
    //   - figure out new min costs from cost, up_vals, down_vals, left_vals, right_vals
    //   - break if cost matrix is same as backup copy

    Matrix costs{ matrix };
    Matrix up_vals{ matrix };
    Matrix down_vals{ matrix };
    Matrix left_vals{ matrix };
    Matrix right_vals{ matrix };
    size_t num_rows = costs.size();
    size_t num_cols = costs[0].size();

    for (size_t row = 0; row < num_rows; ++row) {
        for (size_t col = 0; col < num_cols; ++col) {
            costs[row][col] = 999999;
        }
    }
    costs[0][0] = matrix[0][0];

    while (true) {
        const Matrix old_costs{ costs };
        for (size_t row = 0; row < num_rows; ++row) {
            for (size_t col = 0; col < num_cols; ++col) {
                costs[row][col] = std::min(costs[row][col], matrix[row][col] + ((row < num_rows - 1) ? costs[row + 1][col] : 999999));
                costs[row][col] = std::min(costs[row][col], matrix[row][col] + ((row > 0) ? costs[row - 1][col] : 999999));
                costs[row][col] = std::min(costs[row][col], matrix[row][col] + ((col < num_cols - 1) ? costs[row][col + 1] : 999999));
                costs[row][col] = std::min(costs[row][col], matrix[row][col] + ((col > 0) ? costs[row][col - 1] : 999999));
            }
        }

        if (old_costs == costs)
            break;
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
