// The radical of n, rad(n), is the product of the distinct prime factors of n.
// For example, 504 = 2^3 x 3^2 x 7, so rad(504) = 2 x 3 x 7 = 42.
//
// If we calculate rad(n) for 1 <= n <= 10, then sort them on rad(n), and sorting
// on n if the radical values are equal, we get:
//          Unsorted           Sorted
//          n   rad(n)      n 	rad(n) 	k
//          1     1         1     1     1
//          2     2         2     2     2
//          3     3         4     2     3
//          4     2         8     2     4
//          5     5         3     3     5
//          6     6         9     3     6
//          7     7         5     5     7
//          8     2         6     6     8
//          9     3         7     7     9
//         10    10        10    10    10
//
// Let E(k) be the k'th element in the sorted n column;
// for example, E(4) = 8 and E(6) = 9.
//
// If rad(n) is sorted for 1 <= n <= 100,000, find E(10,000).


#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

#include "prime_helper.h"


using n_rad = std::pair<uint64_t, uint64_t>;


std::vector<n_rad> get_radicals(uint64_t max_n) {
    std::vector<n_rad> ret;
    ret.reserve(max_n + 1);

    ret.push_back({ 0, 0 });  // add rad(0)
    ret.push_back({ 1, 1 });  // add rad(1)

    PrimeHelper helper;

    for (uint64_t n = 2; n <= max_n; ++n) {
        auto factors = helper.get_factorization(n);
        uint64_t rad{ 1 };
        for (const auto& [prime, exp] : factors) {
            rad *= prime;
        }
        ret.push_back({ n, rad });
    }

    return ret;
}


void sort_radicals(std::vector<n_rad>& radicals) {
    std::sort(radicals.begin(), radicals.end(), [](const n_rad& lhs, const n_rad& rhs) {
        if (lhs.second != rhs.second)
            return lhs.second < rhs.second;
        return lhs.first < rhs.first;
        });
}


int main()
{
    std::cout << "Hello World!\n";

    {
        auto radicals = get_radicals(10);
        for (const auto &[n, rad] : radicals)
            std::cout << n << "\t" << rad << std::endl;

        sort_radicals(radicals);
        std::cout << "sorted:" << std::endl;
        for (const auto& [n, rad] : radicals)
            std::cout << n << "\t" << rad << std::endl;
    }

    {
        auto radicals = get_radicals(100'000);

        sort_radicals(radicals);

        std::cout << "E(10,000) = " << radicals[10'000].first << "\t" << radicals[10'000].second << std::endl;

    }
}
