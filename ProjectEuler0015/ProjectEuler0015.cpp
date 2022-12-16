// ProjectEuler0015.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <list>
#include <numeric>

#include "combinatorics.h"


int64_t get_number_routes(int w, int h) {
    // The number of paths to get to vertex (i, j) is the same as the number in a rotated
    // Pascal's Triangle, for the binomial expansion.  This is a combinatoric calculation,
    // so the answer is (w+h)Cw == (w+h)! / w! h!

    return get_num_combinations(w + h, h);
}


int main()
{
    std::cout << "Hello World!\n";
    std::cout << get_number_routes(2, 2) << std::endl;
    std::cout << get_number_routes(20, 20) << std::endl;
}
