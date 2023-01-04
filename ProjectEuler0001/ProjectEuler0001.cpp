// If we list all the natural numbers below 10 that are multiples of 3 or 5, we get 3, 5, 6 and 9.
// The sum of these multiples is 23.
// Find the sum of all the multiples of 3 or 5 below 1000.


#include <iostream>
#include <vector>


std::vector<int32_t> get_multiples(const std::vector<int32_t> factors, int32_t max_val) {
    std::vector<int32_t> multiples;
    for (int32_t i = 1; i < max_val; ++i) {
        for (const auto fac : factors) {
            if (i % fac == 0) {
                multiples.push_back(i);
                break;
            }
        }
    }

    return multiples;
}

int32_t main()
{
    std::cout << "Hello World!\n";
    std::vector<int32_t> multiples = get_multiples({ 3, 5 }, 1000);
    int32_t sum = 0;
    for (const auto val : multiples) {
        std::cout << val << std::endl;
        sum += val;
    }
    std::cout << sum << std::endl;
}
