// Consider the fraction, n / d, where nand d are positive integers.
// If n < d and HCF(n, d) = 1, it is called a reduced proper fraction.
//
// If we list the set of reduced proper fractions for d ≤ 8 in ascending order of size, we get :
//
//  1/8, 1/7, 1/6, 1/5, 1/4, 2/7, 1/3, 3/8, 2/5, 3/7, 1/2, 4/7, 3/5, 5/8, 2/3, 5/7, 3/4, 4/5, 5/6, 6/7, 7/8
//
// It can be seen that 2/5 is the fraction immediately to the left of 3/7.
//
// By listing the set of reduced proper fractions for d <= 1, 000, 000 in ascending order of size,
// find the numerator of the fraction immediately to the left of 3/7.
//

#include <iostream>
#include <set>
#include <utility>

using Fraction = std::pair<uint64_t, uint64_t>;
struct FractionCmp {
    bool operator()(const Fraction& lhs, const Fraction& rhs) const {
        return lhs.first * rhs.second < lhs.second* rhs.first;
    }
};


Fraction get_largest_fraction(uint64_t max_den, const Fraction& threshold) {
    std::set<Fraction, FractionCmp> fractions;

    for (uint64_t d = 2; d <= max_den; ++d) {
        // Skip the exact threshold
        if (d == threshold.second)
            continue;
        uint64_t n = d * threshold.first / threshold.second;
        if ((n > 0) && (n*threshold.second != d*threshold.first))
            fractions.insert({ n, d });
    }

    return *fractions.rbegin();
}


int main()
{
    std::cout << "Hello World!\n";

    {
        // Demonstrate default sort order of std::pair is sort on first, then second
        std::set<Fraction> fractions{
            {1,8}, {1,7}, {1,6}, {1,5}, {1,4}, {2,7}, {1,3}, {3,8}, {2,5}, {3,7}, {1,2}, {4,7}, {3,5}, {5,8}, {2,3}, {5,7}, {3,4}, {4,5}, {5,6}, {6,7}, {7,8} };
        for (const auto& f : fractions)
            std::cout << f.first << "/" << f.second << "  ";
        std::cout << std::endl;
    }

    {
        // Demonstrate that FractionCmp yields the desired sort order
        std::set<Fraction, FractionCmp> fractions{
            {1,8}, {1,7}, {1,6}, {1,5}, {1,4}, {2,7}, {1,3}, {3,8}, {2,5}, {3,7}, {1,2}, {4,7}, {3,5}, {5,8}, {2,3}, {5,7}, {3,4}, {4,5}, {5,6}, {6,7}, {7,8} };
        for (const auto& f : fractions)
            std::cout << f.first << "/" << f.second << "  ";
        std::cout << std::endl;
    }

    {
        auto frac = get_largest_fraction(8, Fraction{ 3, 7 });
        std::cout << frac.first << " / " << frac.second << std::endl;
    }

    {
        auto frac = get_largest_fraction(1'000'000, Fraction{ 3, 7 });
        std::cout << frac.first << " / " << frac.second << std::endl;
    }
}
