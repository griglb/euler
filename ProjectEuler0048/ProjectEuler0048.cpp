// The series, 1^1 + 2^2 + 3^3 + ... + 10^10 = 10405071317.
// Find the last ten digits of the series, 1^1 + 2^2 + 3^3 + ... + 1000^1000.


#include <iostream>


int64_t get_power_sum(int32_t max_term, int32_t digits) {
    int64_t sum{ 0 };

    int64_t mod{ 1 };
    for (int32_t i = 0; i < digits; ++i)
        mod *= 10;

    for (int32_t n = 1; n <= max_term; ++n) {
        int64_t term{ 1 };
        for (int64_t p = 0; p < n; ++p) {
            term *= n;
            term %= mod;
        }
        sum += term;
        sum %= mod;
    }

    return sum;
}


int main()
{
    std::cout << "Hello World!\n";

    std::cout << get_power_sum(10, 10) << std::endl;
    std::cout << get_power_sum(100, 10) << std::endl;
    std::cout << get_power_sum(1000, 10) << std::endl;
}
