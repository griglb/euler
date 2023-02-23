// In the following equation x, y, and n are positive integers.
//      1   1   1
//      - + - = -
//      x   y   n
//
// For n = 4 there are exactly three distinct solutions :
//      1   1    1
//      - + -- = -
//      5   20   4
// 
//      1   1    1
//      - + -- = -
//      6   12   4
//
//      1   1   1
//      - + - = -
//      8   8   4
//
// What is the least value of n for which the number of distinct solutions exceeds one-thousand?
//
// NOTE : This problem is an easier version of Problem 110; it is strongly advised that you solve this one first.


#include <iostream>
#include <utility>
#include <vector>


// Using positive integers, we know both x,y > n.
// We also know the max value for x is when x = y = 2*n;
// So x is in (n, 2*n], y is in [2*n, inf)

using XY = std::pair<uint64_t, uint64_t>;
std::vector<XY> get_xy(const uint64_t n) {
    std::vector<XY> ret;

    for (uint64_t x = n + 1; x < 2 * n; ++x) {
        uint64_t num = n * x;
        uint64_t den = x - n;
        if (num % den == 0)
            ret.push_back({ x, num / den });
    }
    ret.push_back({ 2 * n, 2 * n });

    return ret;
}


uint64_t get_answer() {
    uint64_t ret{ 1000 };
    while (true) {
        std::cout << ret << std::endl;
        auto xys = get_xy(ret);
        if (xys.size() > 1000)
            return ret;
        ++ret;
    }
}


int main()
{
    std::cout << "Hello World!\n";

    {
        uint64_t n = 4;
        auto xys = get_xy(n);
        for (const auto& [x, y] : xys) {
            std::cout << "1/" << x << " + 1/" << y << " = 1/" << n << std::endl;
        }
    }

    std::cout << "Answer is " << get_answer() << std::endl;
}
