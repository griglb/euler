// Starting in the top left corner of a 2×2 grid, and only being able to move to the right and down,
// there are exactly 6 routes to the bottom right corner.
//                    +-+-+        +-+ +        +-+ +
//                        |          |            |
//                    + + +        + +-+        + + +
//                        |            |          |
//                    + + +        + + +        + +-+
//
//                    + + +        +-+ +        +-+ +
//                    |            |            |
//                    +-+-+        +-+ +        + + +
//                        |          |          |
//                    + + +        + +-+        +-+-+
//
// How many such routes are there through a 20×20 grid?


#include <iostream>
#include <list>
#include <numeric>

#include "combinatorics.h"


int64_t get_number_routes(int w, int h) {
    // The number of paths to get to vertex (i, j) is the same as the number in a rotated
    // Pascal's Triangle, for the binomial expansion.  This is a combinatoric calculation,
    // so the answer is C(w+h, w) == (w+h)! / w! h!

    return get_num_combinations(w + h, h);
}


int main()
{
    std::cout << "Hello World!\n";
    std::cout << get_number_routes(2, 2) << std::endl;
    std::cout << get_number_routes(20, 20) << std::endl;
}
