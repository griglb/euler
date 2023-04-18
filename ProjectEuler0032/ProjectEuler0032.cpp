// We shall say that an n-digit number is pandigital if it makes use
// of all the digits 1 to n exactly once;
// for example, the 5-digit number, 15234, is 1 through 5 pandigital.
// The product 7254 is unusual, as the identity, 39 × 186 = 7254,
// containing multiplicand, multiplier, and product is 1 through 9 pandigital.
// Find the sum of all products whose multiplicand/multiplier/product identity can be written as a 1 through 9 pandigital.
// HINT: Some products can be obtained in more than one way so be sure to only include it once in your sum.


#include <algorithm>
#include <iostream>
#include <numeric>
#include <set>
#include <vector>


bool is_pandigital_product(int32_t a, int32_t b) {
    static const std::vector<int8_t> PanDigits{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::vector<int8_t> digits;
    int32_t c = a * b;

    auto add_digits = [&digits](int32_t num) {
        while (num > 0) {
            digits.push_back(num % 10);
            num /= 10;
        }
    };

    add_digits(a);
    add_digits(b);
    add_digits(c);

    std::sort(digits.begin(), digits.end());
    return digits == PanDigits;
}


std::vector<std::pair<int32_t, int32_t>> get_pandigital_products() {
    // 5000 is a 4 digit number, and any integer>1 * 5000 is a 5+ digit number, so that's the limit
    const int32_t max_val = 5000;
    std::vector<std::pair<int32_t, int32_t>> ret;

    for (int32_t a = 1; a < max_val; ++a) {
        for (int32_t b = a+1; b < max_val; ++b) {
            if (is_pandigital_product(a, b)) {
                ret.push_back({ a, b });
            }
        }
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    std::cout << is_pandigital_product(39, 186) << std::endl;
    std::cout << is_pandigital_product(186, 39) << std::endl;
    std::cout << is_pandigital_product(38, 186) << std::endl;

    auto pairs = get_pandigital_products();
    std::set<int32_t> products;
    for (const auto& numbers : pairs) {
        std::cout << numbers.first << " * " << numbers.second << " = " << numbers.first * numbers.second << std::endl;
        products.insert(numbers.first * numbers.second);
    }
    std::cout << "sum = " << std::accumulate(products.begin(), products.end(), 0) << std::endl;
}
