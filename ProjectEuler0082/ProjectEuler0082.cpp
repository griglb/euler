// NOTE: This problem is a more challenging version of Problem 81.
//
// The minimal path sum in the 5 by 5 matrix below, by starting in any cell in the
// left column and finishing in any cell in the right column, and only moving up,
// down, and right, is indicated in redand bold; the sum is equal to 994.
//              / 131  673 _234__103_ _18_\
//              |_201_ _96__342_ 965  150 |
//              | 630  803  746  422  111 |
//              | 537  699  497  121  956 |
//              \ 805  732  524   37  331 /
// Find the minimal path sum from the left column to the right column in
// matrix.txt(right click and "Save Link/Target As..."), a 31K text file containing an 80 by 80 matrix.


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
    std::ifstream infile("./p082_matrix.txt");
#else
    std::ifstream infile("./ProjectEuler0082/p082_matrix.txt");
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
    // This is still not quite as complicated as Dijkstra's pathfinding algorithm.
    // We can iteratively process this in columns, starting at the second.
    // For each column, we calculate 3 columns of new potential values,
    // based on moving into each cell from left, up, or down.  We then
    // update the cells with the min of those 3 options.
    // Due to up/down motion, we need to repeat each column until it doesn't
    // change.
    // The last column only needs to test left motion, up/down will only
    // add to the minimum cost.

    Matrix costs{ matrix };
    size_t num_rows = costs.size();
    size_t last_col = costs[0].size() - 1;

    for (size_t col = 1; col < last_col; ++col) {
        Row curr_col;
        for (size_t row = 0; row < num_rows; ++row)
            curr_col.push_back(matrix[row][col] + costs[row][col - 1]);

        while (true) {
            const Row prev_col = curr_col;
            Row up_vals, down_vals; // cost to move up or down into current row
            up_vals.resize(curr_col.size());
            down_vals.resize(curr_col.size());
            for (size_t row = 0; row < num_rows; ++row) {
                up_vals[row] = matrix[row][col] + ((row < last_col - 1) ? curr_col[row + 1] : 999999);
                down_vals[row] = matrix[row][col] + ((row > 0) ? curr_col[row - 1] : 999999);
            }
            for (size_t row = 0; row < num_rows; ++row) {
                curr_col[row] = std::min(curr_col[row], std::min(up_vals[row], down_vals[row]));
            }
            if (prev_col == curr_col)
                break;
        }

        for (size_t row = 0; row < num_rows; ++row)
            costs[row][col] = curr_col[row];
    }

    // Process last column as simple move from left.
    for (size_t row = 0; row < num_rows; ++row)
        costs[row][last_col] += costs[row][last_col - 1];

    // Find the smallest value in last column.
    uint32_t min_cost{ 999999 };
    for (size_t row = 0; row < num_rows; ++row)
        min_cost = std::min(min_cost, *costs[row].rbegin());
    return min_cost;
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
