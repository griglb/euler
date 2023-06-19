// Given the positive integers, x, y, and z, are consecutive terms of an arithmetic progression,
// the least value of the positive integer, n, for which the equation,
//      x^2 - y^2 - z^2 = n
// has exactly two solutions is n=27:
//      34^2 - 27^2 - 20^2 = 12^2 - 9^2 - 6^2 = 27
//
// It turns out that n=1155 is the least value which has exactly ten solutions.
//
// How many values of n less than one million have exactly ten distinct solutions?


#include <cmath>
#include <iostream>
#include <unordered_map>
#include <vector>

// The first question is how to bound the search space.
// The terms x, y, z form a decreasing arithmetic progression, since n > 0.
// This means x - y == y - z == k.
// Let's call x = y + k & z = y - k, the the expression for n becomes:
//      (y + k)^2 - y^2 - (y - k)^2 = n
//      y^2 + 2ky + k^2 - y^2 - (y^2 - 2ky + k^2) = n
//      4ky - y^2 = n
// Check this with the examples:
//      34^2 - 27^2 - 20^2 = 27  ->  y = 27, k = 7 -> 4*7*27 - 27^7 = 28*27 - 27*27 = 27
//      12^2 - 9^2 - 6^2 = 27    ->  y = 9,  k = 3 -> 4*3*9 - 9^2 = 12*9 - 9*9 = 3*9 = 27

// Can the quadratic equation be applied here?
//      4*k*y - y^2 = 27
//      y^2 - 4*k*y + 27 = 0
//      y = (4k +- sqrt(16*k^2 - 4*1*27)) / 2
//      y = 2k +- sqrt(4*k^2 - 27)
//      k = 7 -> y = 14 +- sqrt(4*49 - 27) = 14 +- sqrt(196 - 27) = 14 +- sqrt(169) = 14 +- 13 = { 27, 1 }
//      k = 3 -> y = 6 +- sqrt(36 - 27) = 6 +- sqrt(9) = 6 +- 3 = { 9, 3 }
// In both cases, the sum yields the correct value of y, while the difference results in z <= 0.


class Solver {
public :
    Solver(int64_t max_n) {
        squares_.reserve(max_n + 1);
        for (int64_t n = 1; n <= max_n; ++n)
            squares_.push_back(n * n);
    }

    std::vector<uint64_t> get_counts(uint64_t max_n) {
        std::vector<uint64_t> ret(max_n + 1, 0);

        for (int64_t k = 1; k < max_n; ++k) {
            if (k % 1000 == 0)
                std::cout << k << std::endl;
            for (int64_t y = k + 1; y < max_n; ++y) {
                const int64_t n = 4 * k * y - y * y;
                if ((n >= 0) && (n < max_n))
                    ++ret[n];
            }
        }

        return ret;
    }

private:
    std::vector<int64_t> squares_;

};


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    Solver solver(1200);
    //    auto counts = solver.get_counts(1200);
    //    for (size_t n = 0; n < counts.size(); ++n)
    //        std::cout << n << "\t" << counts[n] << std::endl;
    //}

    {
        Solver solver(1'000'000);
        auto counts = solver.get_counts(1'000'000);
        size_t num{ 0 };
        for (size_t n = 0; n < counts.size(); ++n) {
            if (counts[n] == 10) {
                std::cout << n << "\t" << counts[n] << std::endl;
                ++num;
            }
        }
        std::cout << "There are " << num << " values of n with exactly 10 sequences." << std::endl;
    }
}
