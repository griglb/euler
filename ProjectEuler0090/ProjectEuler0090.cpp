// Each of the six faces on a cube has a different digit (0 to 9) written on it;
// the same is done to a second cube.  By placing the two cubes side-by-side in
// different positions we can form a variety of 2-digit numbers.
// For example, the square number 64 could be formed :
//                                 -- --
//                                / // /|
//                                |6||4|/
//                                +-++-+
// In fact, by carefully choosing the digits on both cubes it is possible to display
// all of the square numbers below one - hundred : 01, 04, 09, 16, 25, 36, 49, 64, and 81.
//
// For example, one way this can be achieved is by placing { 0, 5, 6, 7, 8, 9 } on one cube
// and { 1, 2, 3, 4, 8, 9 } on the other cube.
//
// However, for this problem we shall allow the 6 or 9 to be turned upside-down so that
// an arrangement like { 0, 5, 6, 7, 8, 9 } and { 1, 2, 3, 4, 6, 7 } allows for all nine
// square numbers to be displayed; otherwise it would be impossible to obtain 09.
//
// In determining a distinct arrangement we are interested in the digits on each cube, not the order.
//      { 1, 2, 3, 4, 5, 6 } is equivalent to { 3, 6, 4, 1, 2, 5 }
//      { 1, 2, 3, 4, 5, 6 } is distinct from { 1, 2, 3, 4, 5, 9 }
//
// But because we are allowing 6 and 9 to be reversed, the two distinct sets in the last example
// both represent the extended set { 1, 2, 3, 4, 5, 6, 9 } for the purpose of forming 2-digit numbers.
//
// How many distinct arrangements of the two cubes allow for all of the square numbers to be displayed?


#include <iostream>
#include <set>

#include "combinatorics.h"


// For each cube there are C(10, 6) = 10! / (6! * 4!) = 10 * 9 * 8 * 7 / (4 * 3 * 2) = 210 possible combinations.
// So the brute force approach of 210^2 pairs of cubes is not too big a problem space to search.
uint64_t num_cube_pairs() {
    uint64_t count{ 0 };

    const std::set<uint64_t> squares_ref { 1, 4, 9, 16, 25, 36, 49, 64, 81 };

    auto combos = get_combinations(10, 6);
    for (const auto &block1 : combos) {
        for (const auto &block2 : combos) {
            auto squares = squares_ref;

            for (const auto &el1 : block1) {
                for (const auto &el2 : block2) {
                    squares.erase(10 * el1 + el2);
                    squares.erase(10 * el2 + el1);
                    if (6 == el1) {
                        squares.erase(10 * 9 + el2);
                        squares.erase(10 * el2 + 9);
                    }
                    if (9 == el1) {
                        squares.erase(10 * 6 + el2);
                        squares.erase(10 * el2 + 6);
                    }
                    if (6 == el2) {
                        squares.erase(10 * el1 + 9);
                        squares.erase(10 * 9 + el1);
                    }
                    if (9 == el2) {
                        squares.erase(10 * el1 + 6);
                        squares.erase(10 * 6 + el1);
                    }
                }
            }

            if (squares.empty())
                ++count;
        }
    }

    // Need to halve the count, as we double count each combination by switching block1 & block2
    return count / 2;
}


int main()
{
    std::cout << "Hello World!\n";

    std::cout << num_cube_pairs() << std::endl;
}
