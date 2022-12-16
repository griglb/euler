// ProjectEuler2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

int64_t get_fibonnaci_sum(int32_t max_val) {
    int32_t a{ 1 };
    int32_t b{ 1 };
    int32_t c = a + b;
    int64_t sum{ 0 };

    while (c < max_val) {
        if (c % 2 == 0) {
            std::cout << c << std::endl;
            sum += c;
        }
        a = b;
        b = c;
        c = a + b;
    }

    return sum;
}

int main()
{
    std::cout << "Hello World!\n";
    std::cout << get_fibonnaci_sum(4e6);
}
