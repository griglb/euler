// It turns out that 12 cm is the smallest length of wire that can be bent to form an
// integer sided right angle triangle in exactly one way, but there are many more examples.
//
//      12 cm : (3, 4, 5)
//      24 cm : (6, 8, 10)
//      30 cm : (5, 12, 13)
//      36 cm : (9, 12, 15)
//      40 cm : (8, 15, 17)
//      48 cm : (12, 16, 20)
//
// In contrast, some lengths of wire, like 20 cm, cannot be bent to form an integer sided
// right angle triangle, and other lengths allow more than one solution to be found;
// for example, using 120 cm it is possible to form exactly three different integer sided right angle triangles.
//      120 cm: (30, 40, 50), (20, 48, 52), (24, 45, 51)
//
// Given that L is the length of the wire, for how many values of L <= 1,500,000 can exactly one integer
// sided right angle triangle be formed?

#include <math.h>

#include <iostream>
#include <map>
#include <set>
#include <tuple>


using Triplet = std::tuple<int32_t, int32_t, int32_t>;


Triplet get_triplet(int32_t m, int32_t n) {
    int32_t a = m * m - n * n;
    int32_t b = 2 * m * n;
    int32_t c = m * m + n * n;

    return { a, b, c };
}


std::map<uint32_t, std::set<Triplet>> get_triplets(uint32_t max_perim) {
    std::map<uint32_t, std::set<Triplet>> ret;

    for (int32_t m = 2; m < sqrt(max_perim); ++m) {
        for (int32_t n = 1; n < m; ++n) {
            auto [a, b, c] = get_triplet(m, n);
            //if (a > b)
            //    continue;
            if (a > b)
                std::swap(a, b);
            auto perim = a + b + c;
            uint32_t max_k = max_perim / perim;
            for (uint32_t k = 1; k <= max_k; ++k) {
                auto tmp = perim * k;
                if (ret.find(tmp) == ret.end())
                    ret[tmp] = {};
                ret[tmp].insert({ k * a, k * b, k * c });
            }
        }
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    {
        auto trip_map = get_triplets(1'000);
        for (const auto& [perim, triplets] : trip_map) {
            std::cout << perim << " : ";
            for (const auto& [a, b, c] : triplets) {
                std::cout << "(" << a << ", " << b << ", " << c << ")  ";
            }
            std::cout << std::endl;
        }
    }

    {
        auto trip_map = get_triplets(1'500'000);
        std::map<uint32_t, std::set<Triplet>> singles;
        for (const auto& [perim, triplets] : trip_map) {
            if (triplets.size() == 1)
                singles[perim] = triplets;
        }

        for (const auto& [perim, triplets] : singles) {
            std::cout << perim << " : ";
            for (const auto& [a, b, c] : triplets) {
                std::cout << "(" << a << ", " << b << ", " << c << ")  ";
            }
            std::cout << std::endl;
        }
        std::cout << "count = " << singles.size() << std::endl;
    }
}
