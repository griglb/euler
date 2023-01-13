// It is possible to write five as a sum in exactly six different ways:
//      4 + 1
//      3 + 2
//      3 + 1 + 1
//      2 + 2 + 1
//      2 + 1 + 1 + 1
//      1 + 1 + 1 + 1 + 1
//
// How many different ways can one hundred be written as a sum of at least two positive integers?

#include <iostream>
#include <set>
#include <vector>


using Addends = std::multiset<uint16_t>;
using SameSum = std::set<Addends>;

SameSum get_sum_sets(uint16_t total) {
    std::vector<SameSum> memo;
    memo.push_back({ {} });
    memo.push_back({ {1} });

    for (uint16_t sum = 2; sum <= total; ++sum) {
        SameSum this_sum;
        if (sum < total)
            this_sum.insert({ sum });

        for (uint16_t n = 1; n < sum; ++n) {
            auto sub_sums = memo[sum - n];
            for (auto sub : sub_sums) {
                sub.insert(n);
                this_sum.insert(sub);
            }
        }
        memo.push_back(this_sum);
        std::cout << "\t" << sum << "\t" << this_sum.size() << std::endl;
    }

    return memo[total];
}


int main()
{
    std::cout << "Hello World!\n";

    {
        for (int16_t total = 1; total < 11; ++total) {
            auto sums = get_sum_sets(total);
            std::cout << std::endl << total << "\t" << sums.size() << std::endl << "\t";
            for (const auto& sum : sums) {
                for (const auto& el : sum)
                    std::cout << el << " + ";
                std::cout << std::endl << "\t";
            }
        }
    }
    return 0;
    {
        auto sums = get_sum_sets(100);
        //for (const auto& sum : sums) {
        //    for (const auto& el : sum)
        //        std::cout << el << " + ";
        //    std::cout << std::endl;
        //}
        std::cout << sums.size() << std::endl;
    }
}
