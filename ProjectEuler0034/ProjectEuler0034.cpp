// 145 is a curious number, as 1! + 4! + 5! = 1 + 24 + 120 = 145.
// Find the sum of all numbers which are equal to the sum of the factorial of their digits.
// Note: As 1! = 1 and 2! = 2 are not sums they are not included.


#include <iostream>
#include <vector>


uint64_t factorial(int16_t n) {
    uint64_t prod{ 1 };
    for (int16_t i = 2; i <= n; ++i)
        prod *= i;
    return prod;
}


std::vector<uint64_t> get_digit_factorials() {
    std::vector<uint64_t> ret;

    for (uint64_t num = 10; num < 1'000'000; ++num) {
        uint64_t sum{ 0 };
        for (uint64_t n = num; n > 0; n /= 10) {
            sum += factorial(n % 10);
        }
        if (sum == num)
            ret.push_back(num);
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    for (int16_t i = 1; i < 10; ++i) {
        std::cout << i << "! = " << factorial(i) << std::endl;
    }

    auto factorials = get_digit_factorials();
    uint64_t sum{ 0 };
    for (const auto& num : factorials) {
        std::cout << num << std::endl;
        sum += num;
    }
    std::cout << "sum = " << sum << std::endl;
}
