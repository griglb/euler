// The sum of the squares of the first ten natural numbers is,
//            1^2 + 2^2 + ... + 10^2 = 385
// The square of the sum of the first ten natural numbers is,
//            (1 + 2 + ... + 10)^2 = 55@ = 3025
// Hence the difference between the sum of the squares of the first ten natural numbers
// and the square of the sum is 3025 - 385 = 2640.
// Find the difference between the sum of the squares of the first
// one hundred natural numbers and the square of the sum.


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
