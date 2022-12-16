// ProjectEuler0006.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

int64_t get_sum_square_difference(int n) {
    int64_t sum_squares{ 0 };
    for (int i = 1; i <= n; ++i) {
        sum_squares += (i * i);
    }

    int64_t square_sum = n * (n + 1) / 2;
    square_sum *= square_sum;
    std::cout << sum_squares << ", " << square_sum << std::endl;
    return square_sum - sum_squares;
}

int main()
{
    std::cout << "Hello World!\n";
    std::cout << get_sum_square_difference(10) << std::endl;
    std::cout << get_sum_square_difference(100) << std::endl;
}
