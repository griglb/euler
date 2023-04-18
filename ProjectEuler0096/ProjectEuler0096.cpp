// Su Doku (Japanese meaning number place) is the name given to a popular puzzle concept.
// Its origin is unclear, but credit must be attributed to Leonhard Euler who invented a
// similar, and much more difficult, puzzle idea called Latin Squares.  The objective of
// Su Doku puzzles, however, is to replace the blanks(or zeros) in a 9 by 9 grid in such
// that each row, column, and 3 by 3 box contains each of the digits 1 to 9.
// Below is an example of a typical starting puzzle grid and its solution grid.
// 
//      +-----+-----+-----+     +-----+-----+-----+
//      |0 0 3|0 2 0|6 0 0|     |4 8 3|9 2 1|6 5 7|
//      |9 0 0|3 0 5|0 0 1|     |9 6 7|3 4 5|8 2 1|
//      |0 0 1|8 0 6|4 0 0|     |2 5 1|8 7 6|4 9 3|
//      +-----+-----+-----+     +-----+-----+-----+
//      |0 0 8|1 0 2|9 0 0|     |5 4 8|1 3 2|9 7 6|
//      |7 0 0|0 0 0|0 0 8|     |7 2 9|5 6 4|1 3 8|
//      |0 0 6|7 0 8|2 0 0|     |1 3 6|7 9 8|2 4 5| 
//      +-----+-----+-----+     +-----+-----+-----+
//      |0 0 2|6 0 9|5 0 0|     |3 7 2|6 8 9|5 1 4|
//      |8 0 0|2 0 3|0 0 9|     |8 1 4|2 5 3|7 6 9|
//      |0 0 5|0 1 0|3 0 0|     |6 9 5|4 1 7|3 8 2|
//      +-----+-----+-----+     +-----+-----+-----+
//
// A well constructed Su Doku puzzle has a unique solution and can be solved by logic,
// although it may be necessary to employ "guess and test" methods in order to eliminate
// options (there is much contested opinion over this).
// The complexity of the search determines the difficulty of the puzzle; the example above
// is considered easy because it can be solved by straight forward direct deduction.
//
// The 6K text file, sudoku.txt(right click and 'Save Link/Target As...'), contains fifty
// different Su Doku puzzles ranging in difficulty, but all with unique solutions
// (the first puzzle in the file is the example above).
//
// By solving all fifty puzzles find the sum of the 3-digit numbers found in the top left
// corner of each solution grid; for example, 483 is the 3 - digit number found in the top
// left corner of the solution grid above.

#include <string.h>

#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>


using Cell = int16_t;
using Row = std::array<Cell, 9>;
using Grid = std::array<Row, 9>;

using WorkingCell = std::set<Cell>;
using WorkingRow = std::array<WorkingCell, 9>;
using WorkingGrid = std::array<WorkingRow, 9>;


// forward declare for recursion
Grid solve_sudoku(const Grid& input);


std::vector<Grid> get_boards() {
    // The file has no carriage returns, so read entire file into string.
#ifdef _CONSOLE
    std::ifstream infile("./p096_sudoku.txt");
#else
    std::ifstream infile("./ProjectEuler0096/p096_sudoku.txt");
#endif

    std::vector<Grid> ret;

    while (!infile.eof()) {
        constexpr int16_t kLineLen{ 50 };
        char line[kLineLen + 1];
        infile.getline(line, kLineLen);
        if (strlen(line) <= 0) {
            break;
        }

        Grid grid;
        for (int8_t r = 0; r < 9; ++r) {
            std::string line;
            infile >> line;
            for (int8_t c = 0; c < 9; ++c) {
                grid[r][c] = line[c] - '0';
            }
        }
        ret.push_back(grid);

        infile.getline(line, kLineLen);
    }

    return ret;
}


void print_board(const Grid& board) {
    for (const auto& row : board) {
        for (const auto& el : row)
            std::cout << el << "  ";
//            std::cout << (el != 0 ? el : -1) << "  ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


void print_board(const WorkingGrid& board) {
//    return;
    for (const auto& row : board) {
        for (const auto& el : row)
            std::cout << ((el.size() > 1) ? 0 : *el.begin()) << "  ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


void check_subset(const WorkingRow& subset) {
    std::set<Cell> used_digits;
    for (const auto& cell : subset) {
        for (const auto& el : cell)
            used_digits.insert(el);
    }
    if (used_digits.size() != 9)
        throw "What happened?";
}


bool is_solved(const Grid& board) {
    for (const auto& row : board) {
        for (const auto& cell : row) {
            if (cell == 0)
                return false;
        }
    }
    return true;
}


bool is_solved(const WorkingGrid& board) {
    for (const auto& row : board) {
        for (const auto& cell : row) {
            if (cell.size() > 1)
                return false;
        }
    }
    return true;
}


// Look for any singleton entries in the subset, and
// remove them from other cells in the subset.
WorkingRow clean_subset(const WorkingRow& origSubset) {
    WorkingRow subset{ origSubset };

    // Iterate over subset, tracking which cells have a single digit in them.
    std::set<Cell> digits;
    for (int8_t i = 0; i < 9; ++i) {
        if (subset[i].size() == 1)
            digits.insert(*subset[i].begin());
    }

    // If any digits reside in a cell by themselves, remove them from other cells.
    for (const auto& dig : digits) {
        for (auto& cell : subset) {
            if (cell.size() > 1)
                cell.erase(dig);
            if (cell.empty()) throw "why?";
        }
    }

    check_subset(subset);

    return subset;
}


// Look for any digits that only exist in one cell in the subset, and
// make them the only digit in that cell.
WorkingRow check_subset_for_uniques(const WorkingRow& origSubset) {
    WorkingRow subset{ origSubset };

    std::array<WorkingCell, 10> populations;

    // Catalog which digits exist in which cells.
    for (int8_t i = 0; i < 9; ++i) {
        for (const auto& el : subset[i]) {
            populations[el].insert(i);
        }
    }

    // If any digits exist in only 1 cell, make them the unique member of that cell.
    for (int8_t digit = 1; digit <= 9; ++digit) {
        if (populations[digit].size() == 1) {
            subset[*populations[digit].begin()] = { digit };
        }
    }

    check_subset(subset);

    return subset;
}


// Look for any pairs of cells in this subset with same 2 elements,
// or trios of cells with same 3 elements, to remove from other cells.
WorkingRow check_subset_for_cycles(const WorkingRow& origSubset) {
    WorkingRow subset{ origSubset };

    std::map<WorkingCell, WorkingCell> pairs;
    std::map<WorkingCell, WorkingCell> trios;

    for (int8_t i = 0; i < 9; ++i) {
        if (subset[i].size() == 2) {
            if (pairs.find(subset[i]) == pairs.end())
                pairs[subset[i]] = { };
            pairs[subset[i]].insert(i);
        }
        if (subset[i].size() == 3) {
            if (trios.find(subset[i]) == trios.end())
                trios[subset[i]] = { };
            trios[subset[i]].insert(i);
        }
    }

    for (const auto& [pair, ind] : pairs) {
        if (ind.size() == 2) {
            for (int8_t i = 0; i < 9; ++i) {
                if (ind.find(i) != ind.end())
                    continue;
                for (const auto& el : pair)
                    subset[i].erase(el);
                if (subset[i].empty()) throw "why?";
            }
        }
    }

    for (const auto& [trio, ind] : trios) {
        if (ind.size() == 3) {
            for (int8_t i = 0; i < 9; ++i) {
                if (ind.find(i) != ind.end())
                    continue;
                for (const auto& el : trio)
                    subset[i].erase(el);
                if (subset[i].empty()) throw "why?";
            }
        }
    }

    check_subset(subset);

    return subset;
}


bool process_row(WorkingGrid& grid, int16_t row) {
    // Cache the original row values
    const WorkingRow origRow = grid[row];

    WorkingRow tmpRow1 = clean_subset(origRow);
//    print_board(tmpRow1);
    WorkingRow tmpRow2 = check_subset_for_uniques(tmpRow1);
//    print_board(tmpRow2);
    WorkingRow newRow = check_subset_for_cycles(tmpRow2);
//    print_board(newRow);

    if (origRow == newRow) {
        return false;
    }

    grid[row] = newRow;
//    print_board(grid);
    return true;
}


bool process_col(WorkingGrid& grid, int16_t col) {
    // Cache the original col values
    const WorkingRow origCol = { grid[0][col], grid[1][col], grid[2][col], grid[3][col], grid[4][col], grid[5][col], grid[6][col], grid[7][col], grid[8][col] };

    WorkingRow tmpCol1 = clean_subset(origCol);
//    print_board(tmpCol1);
    WorkingRow tmpCol2 = check_subset_for_uniques(tmpCol1);
//    print_board(tmpCol2);
    WorkingRow newCol = check_subset_for_cycles(tmpCol2);
//    print_board(newCol);

    if (origCol == newCol) {
        return false;
    }

    for (int8_t r = 0; r < 9; ++r)
        grid[r][col] = newCol[r];
//    print_board(grid);
    return true;
}


bool process_square(WorkingGrid& grid, int16_t index) {
    int16_t row = 3 * (index / 3);
    int16_t col = 3 * (index % 3);

    // Cache the original col values
    const WorkingRow origSquare = { grid[row][col],     grid[row][col + 1],     grid[row][col + 2],
                                    grid[row + 1][col], grid[row + 1][col + 1], grid[row + 1][col + 2],
                                    grid[row + 2][col], grid[row + 2][col + 1], grid[row + 2][col + 2] };

    WorkingRow tmpSquare1 = clean_subset(origSquare);
//    print_board(tmpSquare1);
    WorkingRow tmpSquare2 = check_subset_for_uniques(tmpSquare1);
//    print_board(tmpSquare2);
    WorkingRow newSquare = check_subset_for_cycles(tmpSquare2);
//    print_board(newSquare);

    if (origSquare == newSquare) {
        return false;
    }

    for (int8_t r = 0; r < 3; ++r) {
        for (int8_t c = 0; c < 3; ++c)
            grid[row + r][col + c] = newSquare[3 * r + c];
    }
//    print_board(grid);
    return true;
}


// Look for any row or column that must have a digit in a multi-digit cell,
// to remove from other cells in the same square not in that row/column.
// And vice versa.
bool check_for_required_cells(WorkingGrid& grid) {
    const WorkingGrid origGrid{ grid };

    // Check rows.
    for (int8_t row = 0; row < 9; ++row) {
        // Catalog which digits exist in which cells in the current row.
        std::array<WorkingCell, 10> populations;
        for (int8_t i = 0; i < 9; ++i) {
            for (const auto& el : grid[row][i]) {
                populations[el].insert(i);
            }
        }

        // Iterate over populations to see if any digit must exist in an overlapping square.
        for (int8_t digit = 1; digit <= 9; ++digit) {
            // Skip digits that have already been locked into a cell, or that
            // can go into more than 3 cells, it overlaps multiple squares.
            if ((populations[digit].size() < 2) || (populations[digit].size() > 3))
                continue;

            const auto third = *populations[digit].cbegin() / 3;

            // Check if the cell indices are in same third of the row.
            bool unique{ true };
            for (const auto& index : populations[digit]) {
                unique &= index / 3 == third;
            }
            if (!unique)
                continue;

            // Remove this digit from the other rows in the overlapping square where it belongs.
            uint8_t square_row = 3 * (row / 3);
            uint8_t square_col = 3 * third;
            for (uint8_t r = 0; r < 3; ++r) {
                if (square_row + r == row)
                    continue;
                for (uint8_t c = 0; c < 3; ++c) {
                    grid[square_row + r][square_col + c].erase(digit);
                    if (grid[square_row + r][square_col + c].empty()) throw "why?";
                }
            }
        }
    }

    // Check columns.
    for (int8_t col = 0; col < 9; ++col) {
        // Catalog which digits exist in which cells in the current column.
        std::array<WorkingCell, 10> populations;
        for (int8_t i = 0; i < 9; ++i) {
            for (const auto& el : grid[i][col]) {
                populations[el].insert(i);
            }
        }

        // Iterate over populations to see if any digit must exist in an overlapping square.
        for (int8_t digit = 1; digit <= 9; ++digit) {
            // Skip digits that have already been locked into a cell, or that
            // can go into more than 3 cells, it overlaps multiple squares.
            if ((populations[digit].size() < 2) || (populations[digit].size() > 3))
                continue;

            const auto third = *populations[digit].cbegin() / 3;

            // Check if the cell indices are in same third of the column.
            bool unique{ true };
            for (const auto& index : populations[digit]) {
                unique &= index / 3 == third;
            }
            if (!unique)
                continue;

            // Remove this digit from the other columns in the overlapping square where it belongs.
            uint8_t square_row = 3 * third;
            uint8_t square_col = 3 * (col / 3);
            for (uint8_t c = 0; c < 3; ++c) {
                if (square_col + c == col)
                    continue;
                for (uint8_t r = 0; r < 3; ++r) {
                    grid[square_row + r][square_col + c].erase(digit);
                    if (grid[square_row + r][square_col + c].empty()) throw "why?";
                }
            }
        }
    }

    // Check squares
    for (int8_t ind = 0; ind < 9; ++ind) {
        // Catalog which digits exist in which cells in the current square.
        std::array<WorkingCell, 10> populations;
        for (int8_t i = 0; i < 9; ++i) {
            auto r = 3 * (ind / 3) + i / 3;
            auto c = 3 * (ind % 3) + i % 3;
            for (const auto& el : grid[r][c]) {
                populations[el].insert(i);
            }
        }

        // Iterate over populations to see if any digit must exist in an overlapping row.
        for (int8_t digit = 1; digit <= 9; ++digit) {
            // Skip digits that have already been locked into a cell, or that
            // can go into more than 3 cells, it overlaps multiple rows.
            if ((populations[digit].size() < 2) || (populations[digit].size() > 3))
                continue;

            const auto third = *populations[digit].cbegin() / 3;

            // Check if the cell indices are in same row of the square.
            bool unique{ true };
            for (const auto& i : populations[digit]) {
                unique &= i / 3 == third;
            }
            if (!unique)
                continue;

            // Remove this digit from the other columns in the overlapping square where it belongs.
            uint8_t row = 3 * (ind / 3) + third;
            uint8_t square_col = ind % 3;
            for (uint8_t c = 0; c < 9; ++c) {
                if (c / 3 == square_col)
                    continue;
                if (grid[row][c].size() > 1)
                    grid[row][c].erase(digit);
                if (grid[row][c].empty()) throw "why?";
            }
        }

        // Iterate over populations to see if any digit must exist in an overlapping column.
        for (int8_t digit = 1; digit <= 9; ++digit) {
            // Skip digits that have already been locked into a cell, or that
            // can go into more than 3 cells, it overlaps multiple columns.
            if ((populations[digit].size() < 2) || (populations[digit].size() > 3))
                continue;

            const auto third = *populations[digit].cbegin() % 3;

            // Check if the cell indices are in same column of the square.
            bool unique{ true };
            for (const auto& i : populations[digit]) {
                unique &= i % 3 == third;
            }
            if (!unique)
                continue;

            // Remove this digit from the other rows in the overlapping square where it belongs.
            uint8_t square_row = ind / 3;
            uint8_t col = 3 * (ind % 3) + third;
            for (uint8_t r = 0; r < 9; ++r) {
                if (r / 3 == square_row)
                    continue;
                if (grid[r][col].size() > 1)
                    grid[r][col].erase(digit);
                if (grid[r][col].empty()) throw "why?";
            }
        }
    }

    return origGrid != grid;
}


WorkingGrid start_guessing(const WorkingGrid& unsolved) {
    WorkingGrid working{ unsolved };

    for (uint16_t row = 0; row < 9; ++row) {
        for (uint16_t col = 0; col < 9; ++col) {
            // Skip solved cells.
            if (working[row][col].size() == 1)
                continue;

            WorkingCell options = working[row][col];
            for (const auto& value : options) {
                Grid trial_grid;
                for (int8_t r = 0; r < 9; ++r) {
                    for (int8_t c = 0; c < 9; ++c) {
                        trial_grid[r][c] = (working[r][c].size() > 1) ? 0 : *working[r][c].begin();
                    }
                }
                trial_grid[row][col] = value;
                std::cout << "Guessing that " << value << " goes in (" << row << ", " << col << ")" << std::endl;

                try {
                    Grid output = solve_sudoku(trial_grid);
                    if (is_solved(output)) {
                        for (int8_t r = 0; r < 9; ++r) {
                            for (int8_t c = 0; c < 9; ++c) {
                                working[r][c] = { output[r][c] };
                            }
                        }
                        return working;
                    }
                }
                catch (...) {
                    std::cout << "Guess failed, retrying..." << std::endl;
                }
            }
        }
    }

    return unsolved;
}


Grid solve_sudoku(const Grid& input) {
    // Populate a working grid with the input grid values.
    WorkingGrid working;
    WorkingCell cell{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    for (int8_t row = 0; row < 9; ++row) {
        for (int8_t col = 0; col < 9; ++col) {
            working[row][col] = (0 == input[row][col]) ? cell : WorkingCell{ input[row][col] };
        }
    }

//    std::cout << "original" << std::endl;
    print_board(working);

    while (true) {
        bool changed{ false };

        for (int8_t row = 0; row < 9; ++row)
            changed |= process_row(working, row);
        std::cout << "rows changed = " << changed << std::endl;
        print_board(working);

        for (int8_t col = 0; col < 9; ++col)
            changed |= process_col(working, col);
        std::cout << "cols changed = " << changed << std::endl;
        print_board(working);

        for (int8_t ind = 0; ind < 9; ++ind)
            changed |= process_square(working, ind);
        std::cout << "squares changed = " << changed << std::endl;
        print_board(working);

        changed |= check_for_required_cells(working);
        std::cout << "required changed = " << changed << std::endl;
        print_board(working);

        if (!changed)
            break;
    }

    if (!is_solved(working))
        working = start_guessing(working);

    Grid output;
    for (int8_t row = 0; row < 9; ++row) {
        for (int8_t col = 0; col < 9; ++col) {
            output[row][col] = (working[row][col].size() > 1) ? -1 : *working[row][col].begin();
        }
    }
    return output;
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    auto boards = get_boards();
    //    for (const auto& grid : boards) {
    //        print_board(grid);
    //    }
    //}

    //{
    //    auto boards = get_boards();
    //    auto solved = solve_sudoku(boards[49]);
    //    print_board(solved);
    //}

    {
        uint64_t sum{ 0 };
        auto boards = get_boards();
        for (const auto& grid : boards) {
            auto solved = solve_sudoku(grid);
            print_board(solved);
            sum += 100 * solved[0][0] + 10 * solved[0][1] + solved[0][2];
        }
        std::cout << "sum = " << sum << std::endl;
    }
}
