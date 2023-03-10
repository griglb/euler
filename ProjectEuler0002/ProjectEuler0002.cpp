// Each new term in the Fibonacci sequence is generated by adding the previous two terms.
// By starting with 1 and 2, the first 10 terms will be:
//                1, 2, 3, 5, 8, 13, 21, 34, 55, 89, ...
// By considering the terms in the Fibonacci sequence whose values do not exceed four million,
// find the sum of the even-valued terms.


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
    std::cout << get_fibonnaci_sum(4e6) << std::endl;
}
