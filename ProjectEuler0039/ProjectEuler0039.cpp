// If p is the perimeter of a right angle triangle with integral length sides,
// {a,b,c}, there are exactly three solutions for p = 120.
// {20,48,52}, {24,45,51}, {30,40,50}
// For which value of p ≤ 1000, is the number of solutions maximised?

#include <math.h>

#include <iostream>
#include <set>
#include <tuple>
#include <vector>


using Triplet = std::tuple<int32_t, int32_t, int32_t>;


std::vector<Triplet> get_triplets(int32_t m, int32_t n, int32_t max_k) {
    int32_t a = m * m - n * n;
    int32_t b = 2 * m * n;
    int32_t c = m * m + n * n;

    std::vector<Triplet> ret;
    for (int32_t k = 1; k <= max_k; ++k) {
        if (a < b)
            ret.push_back({ k * a, k * b, k * c });
        else
            ret.push_back({ k * b, k * a, k * c });
    }

    return ret;
}


std::set<Triplet> find_triplets_with_sum(int32_t sum_target) {
    std::set<Triplet> ret;
    for (int32_t m = 2; m < sqrt(sum_target); ++m) {
        for (int32_t n = 1; n < m; ++n) {
            int32_t max_k = sum_target / (m * m);
            std::vector<Triplet> triplets = get_triplets(m, n, max_k);
            for (const auto &trip : triplets) {
                auto [a, b, c] = trip;
                if (a + b + c == sum_target) {
                    ret.insert(trip);
                }
            }
        }
    }

    return ret;
}


std::vector<size_t> get_triangle_count(int32_t max_perimeter) {
    std::vector<size_t> ret;

    for (int32_t p = 0; p < max_perimeter; ++p) {
        auto triplets = find_triplets_with_sum(p);
        ret.push_back(triplets.size());
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    auto triplets = find_triplets_with_sum(24);
    for (const auto &[a, b, c] : triplets) {
        std::cout << "( " << a << ", " << b << ", " << c << " )" << std::endl;
    }

    auto counts = get_triangle_count(1000);
    int32_t perim{ 0 };
    size_t max_count{ 0 };
    int32_t argmax{ -1 };
    int32_t index{ 0 };
    for (const auto & c : counts) {
        std::cout << perim++ << " : " << c << std::endl;
        if (c > max_count) {
            max_count = c;
            argmax = index;
        }
        ++index;
    }
    std::cout << argmax << " : " << max_count << std::endl;
    triplets = find_triplets_with_sum(argmax);
    for (const auto &[a, b, c] : triplets) {
        std::cout << "( " << a << ", " << b << ", " << c << " )" << std::endl;
    }
}
