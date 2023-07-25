// Looking at the table below, it is easy to verify that the maximum possible
// sum of adjacent numbers in any direction(horizontal, vertical, diagonal or
// anti-diagonal) is 16 (= 8 + 7 + 1).
//
//          -2  5  3  2
//           9 -6  5  1
//           3  2  7  3
//          -1  8 -4  8
//
// Now, let us repeat the search, but on a much larger scale:
// First, generate four million pseudo-random numbers using a specific form of
// what is known as a "Lagged Fibonacci Generator":
//      For 1 <= k <= 55,       s_k = [100003 - 200003*k + 300007*k^3] (mod 1000000) - 500000
//      For 56 <= k <= 4000000, s_k = [s_(k-24) + s_(k-55) + 1000000] (mod 1000000) - 500000
//
// Thus,
//      s_10 = -393027
//      s_100 = 86613
//
// The terms of s are then arranged in a 2000 x 2000 table, using the first 2000
// numbers to fill the first row(sequentially), the next 2000 numbers to fill the
// second row, and so on.
//
// Finally, find the greatest sum of(any number of) adjacent entries in any
// direction(horizontal, vertical, diagonal or anti-diagonal).


#include <cstdint>
#include <iostream>
#include <numeric>
#include <vector>


using Row = std::vector<int64_t>;
using Grid = std::vector<Row>;


Grid get_grid() {

    Row values;
    values.resize(4'000'001);

    for (size_t k = 1; k <= 55; ++k)
        values[k] = ((100'003 - 200'003 * k + 300'007 * k * k * k) % 1'000'000) - 500'000;

    size_t lag_24{ 32 };
    size_t lag_55{ 1 };
    for (size_t k = 56; k <= 4'000'000; ++k, ++lag_24, ++lag_55)
        // The +1'000'000 seems unnecessary, but it is to make sure the sum being modded is positive.
        values[k] = ((values[lag_24] + values[lag_55] + 1'000'000) % 1'000'000) - 500'000;

    Grid ret;
    ret.resize(2'000);
    auto first_iter = values.cbegin() + 1;
    auto last_iter = first_iter + 2'000;
    for (auto& row : ret) {
        row = Row{ first_iter, last_iter };
        if (last_iter == values.cend())
            break;
        first_iter = last_iter;
        last_iter += 2'000;
    }
    return ret;
}


Grid get_summed_area_table(const Grid& grid) {
    Grid sat;

    // Set SAT to correct number of rows
    sat.resize(grid.size());

    // Start by processing each row horizontally
    size_t r{ 0 };
    for (const auto& row : grid) {
        // Get reference to current SAT row and resize to correct length.
        Row& sat_row = sat[r++];
        sat_row.resize(row.size());
        // Copy first element over
        sat_row[0] = row[0];

        // Iterator pointing to next input element to add.
        auto in_iter = row.cbegin() + 1;
        // Iterator pointing to last output element to add.
        auto last_iter = sat_row.cbegin();
        // Iterator pointing to output element to assign sum to.
        auto out_iter = sat_row.begin() + 1;
        // Iterate over rest of row, calculating cumulative sums.
        while (in_iter != row.cend()) {
            // Write cumulative sum to next output element.
            *out_iter = *last_iter + *in_iter;
            // Increment all iterators.
            ++in_iter;
            ++last_iter;
            ++out_iter;
        }
    }

    // Next do vertical summation, one row at a time.
    size_t num_rows = grid.size();
    for (size_t ind = 1; ind < num_rows; ++ind) {
        const auto& last_row = sat[ind - 1];
        auto last_iter = last_row.cbegin();
            
        auto& row = sat[ind];
        for (auto iter = row.begin(); iter != row.end(); ++iter, ++last_iter)
            *iter += *last_iter;
    }

    return sat;
}


int64_t get_max_horizontal_sum(const Grid &grid) {
    // 48024480 in 4:45 minutes
    int64_t max_sum{ 0 };

    for (const auto& row : grid) {
        for (auto first_iter = row.cbegin(); first_iter != row.cend(); ++first_iter) {
            for (auto last_iter = first_iter + 1; last_iter != row.cend(); ++last_iter) {
                int64_t sum = std::accumulate(first_iter, last_iter, 0LL);
                if (sum > max_sum)
                    max_sum = sum;
            }
        }
    }

    return max_sum;
}


int64_t get_max_horizontal_sum_sat(const Grid& sat) {
    // 48024480 in less than 2 seconds
    int64_t max_sum{ 0 };

    // Analyze first row by itself.
    auto row = sat.cbegin();
    for (auto left_iter = row->cbegin(); left_iter != row->cend(); ++left_iter) {
        for (auto right_iter = left_iter + 1; right_iter != row->cend(); ++right_iter) {
            int64_t sum = *right_iter - *left_iter;
            if (sum > max_sum)
                max_sum = sum;
        }
    }

    // The rest of the rows need 2 values from previous row too.
    auto prev_row = sat.cbegin();
    for (++row; row != sat.cend(); ++row, ++prev_row) {
        auto last_left_iter = prev_row->cbegin();
        for (auto left_iter = row->cbegin(); left_iter != row->cend(); ++left_iter, ++last_left_iter) {
            auto last_right_iter = last_left_iter + 1;
            for (auto right_iter = left_iter + 1; right_iter != row->cend(); ++right_iter, ++last_right_iter) {
                int64_t sum = *right_iter - *left_iter - *last_right_iter + *last_left_iter;
                if (sum > max_sum)
                    max_sum = sum;
            }
        }
    }

    return max_sum;
}


int64_t get_max_vertical_sum(const Grid& grid) {
    // 48578009 in 22:09 minutes
    int64_t max_sum{ 0 };

    size_t num_rows = grid.size();
    size_t num_cols = grid[0].size();

    for (size_t col = 0; col < num_cols; ++col) {
        for (size_t first_row = 0; first_row < num_rows - 1; ++first_row) {
            for (size_t last_row = first_row; last_row < num_rows; ++last_row) {
                int64_t sum{ 0 };
                for (size_t row = first_row; row <= last_row; ++row) {
                    sum += grid[row][col];
                }
                if (sum > max_sum) {
                    max_sum = sum;
                    std::cout << "New max sum " << max_sum << std::endl;
                }
            }
        }
    }

    return max_sum;
}


int64_t get_max_vertical_sum_sat(const Grid& sat) {
    // 48578009 in 3 seconds.
    int64_t max_sum{ 0 };

    // Analyze first column by itself.
    for (auto top_row = sat.cbegin(); top_row != sat.cend(); ++top_row) {
        for (auto bottom_row = top_row + 1; bottom_row != sat.cend(); ++bottom_row) {
            int64_t sum = bottom_row->front() - top_row->front();
            if (sum > max_sum)
                max_sum = sum;
        }
    }

    // The rest of the rows need 2 values from previous row too.
    for (auto top_row = sat.cbegin(); top_row != sat.cend(); ++top_row) {
        for (auto bottom_row = top_row + 1; bottom_row != sat.cend(); ++bottom_row) {
            auto top_left_iter = top_row->cbegin();
            auto bottom_left_iter = bottom_row->cbegin();

            auto top_right_iter = top_left_iter + 1;
            auto bottom_right_iter = bottom_left_iter + 1;

            for ( ; bottom_right_iter != bottom_row->cend(); ++top_left_iter, ++top_right_iter, ++bottom_left_iter, ++bottom_right_iter) {
                int64_t sum = *bottom_right_iter - *bottom_left_iter - *top_right_iter + *top_left_iter;
                if (sum > max_sum)
                    max_sum = sum;
            }
        }
    }

    return max_sum;
}


int64_t get_max_ne_diagonal_sum_sat(const Grid& grid) {
    // 44246151 in less than 1 second
    int64_t max_sum{ 0 };

    // Start with cell in left column, then bottom row
    size_t num_rows = grid.size();
    size_t num_cols = grid[0].size();

    // First build a diamond-shaped SAT, which only needs horizontal summation
    Grid sat;
    sat.reserve(num_rows + num_cols);

    for (size_t row = 0; row < num_rows; ++row) {
        Row new_row{ grid[row][0] };
        for (size_t col = 1; col <= row; ++col) {
            new_row.push_back(new_row.back() + grid[row - col][col]);
        }
        sat.emplace_back(std::move(new_row));
    }
    for (size_t start_col = 1; start_col < num_cols; ++start_col) {
        Row new_row{ grid[num_rows - 1][start_col] };
        for (size_t col = start_col + 1; col < num_cols; ++col) {
            new_row.push_back(new_row.back() + grid[num_rows - 1 - col][col]);
        }
        sat.emplace_back(std::move(new_row));
    }

    // Analyze first column by itself.
    for (const auto& row : sat) {
        for (auto left_iter = row.cbegin(); left_iter != row.cend(); ++left_iter) {
            for (auto right_iter = left_iter + 1; right_iter != row.cend(); ++right_iter) {
                int64_t sum = *right_iter - *left_iter;
                if (sum > max_sum)
                    max_sum = sum;
            }
        }
    }

    return max_sum;
}


int64_t get_max_se_diagonal_sum_sat(const Grid& grid) {
    // 44246151 in less than 1 second
    int64_t max_sum{ 0 };

    // Start with cell in top row right to left, then left column
    size_t num_rows = grid.size();
    size_t num_cols = grid[0].size();

    // First build a diamond-shaped SAT, which only needs horizontal summation
    Grid sat;
    sat.reserve(num_rows + num_cols);

    for (int64_t start_col = num_cols - 1; start_col >= 0; --start_col) {
        Row new_row{ grid[0][start_col] };
        for (size_t col = start_col + 1; col < num_cols; ++col) {
            new_row.push_back(new_row.back() + grid[col - start_col][col]);
        }
        sat.emplace_back(std::move(new_row));
    }
    for (size_t row = 1; row < num_rows; ++row) {
        Row new_row{ grid[row][0] };
        for (size_t col = 1; col < num_cols - row; ++col) {
            new_row.push_back(new_row.back() + grid[row + col][col]);
        }
        sat.emplace_back(std::move(new_row));
    }

    // Analyze first column by itself.
    for (const auto& row : sat) {
        for (auto left_iter = row.cbegin(); left_iter != row.cend(); ++left_iter) {
            for (auto right_iter = left_iter + 1; right_iter != row.cend(); ++right_iter) {
                int64_t sum = *right_iter - *left_iter;
                if (sum > max_sum)
                    max_sum = sum;
            }
        }
    }

    return max_sum;
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    auto grid = get_grid();
    //    std::cout << "s_10 = " << grid[0][9] << std::endl;
    //    std::cout << "s_100 = " << grid[0][99] << std::endl;
    //    std::cout << "s_4000000 = " << grid[1'999][1'999] << std::endl;
    //}

    {
        auto grid = get_grid();
        //std::cout << "max horizontal = " << get_max_horizontal_sum(grid) << std::endl;
        //std::cout << "max_vertical = " << get_max_vertical_sum(grid) << std::endl;
        auto sat = get_summed_area_table(grid);
        std::cout << "max horizontal = " << get_max_horizontal_sum_sat(sat) << std::endl;
        std::cout << "max_vertical = " << get_max_vertical_sum_sat(sat) << std::endl;
        std::cout << "max NE diagonal = " << get_max_ne_diagonal_sum_sat(grid) << std::endl;
        std::cout << "max SE diagonal = " << get_max_se_diagonal_sum_sat(grid) << std::endl;
    }
}
