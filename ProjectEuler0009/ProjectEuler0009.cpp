// A Pythagorean triplet is a set of three natural numbers, a < b < c, for which,
// a^2 + b^2 = c^2
// For example, 3^2 + 4^2 = 9 + 16 = 25 = 5^2.
// There exists exactly one Pythagorean triplet for which a + b + c = 1000.
// Find the product abc.


#include <math.h>

#include <iostream>
#include <tuple>
#include <vector>


using Triplet = std::tuple<int32_t, int32_t, int32_t>;


std::vector<Triplet> get_triplets(int m, int n, int max_k) {
    int32_t a = m * m - n * n;
    int32_t b = 2 * m * n;
    int32_t c = m * m + n * n;

    std::vector<Triplet> ret;
    for (int k = 1; k <= max_k; ++k) {
        ret.push_back({ k * a, k * b, k * c });
    }

    return ret;
}


Triplet find_triplet_with_sum(int sum_target) {
    for (int m = 2; m < sqrt(sum_target); ++m) {
        for (int n = 1; n < m; ++n) {
            int max_k = sum_target / (m * m);
            std::vector<Triplet> triplets = get_triplets(m, n, max_k);
            for (const auto trip : triplets) {
                auto [a, b, c] = trip;
                if (a + b + c == sum_target) {
                    return trip;
                }
            }
        }
    }

    return { 0, 0, 0 };
}


int main()
{
    std::cout << "Hello World!\n";
    std::vector<Triplet> triplets = get_triplets(2, 1, 5);
    for (const auto &trip : triplets) {
        auto [a, b, c] = trip;
        std::cout << a << " " << b << " " << c << std::endl;
    }

    Triplet triplet = find_triplet_with_sum(1000);
    auto [a, b, c] = triplet;
    std::cout << a << " " << b << " " << c << std::endl;
    std::cout << "product = " << a * b * c << std::endl;
}
