// ProjectEuler0044.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <set>
#include <vector>


std::vector<uint32_t> get_pentagonal_numbers(int32_t n) {
    std::vector<uint32_t> ret{ 1 };

    //p(n) = n(3n-1)/2 = 3/2 n^2 - n/2
    //p(n+1) = (n+1)(3n+2)/2 = 3/2 n^2 + 5/2 n + 1
    //p(n+1) - p(n) = 3 n + 1

    for (int32_t i = 1; i < n; ++i)
        ret.push_back(ret.back() + 3 * i + 1);

    return ret;
}


std::set<std::pair<uint64_t, uint64_t>> get_special_pentagonals(int32_t maxN) {
    std::set<std::pair<uint64_t, uint64_t>> ret;

    auto tmp = get_pentagonal_numbers(maxN);
    std::set<uint64_t> numbers(tmp.begin(), tmp.end());

    for (auto iter1 = numbers.begin(); iter1 != numbers.end(); ++iter1) {
        auto iter2 = iter1;
        for (++iter2; iter2 != numbers.end(); ++iter2) {
            if (numbers.find(*iter2 - *iter1) == numbers.end())
                continue;
            if (numbers.find(*iter2 + *iter1) == numbers.end())
                continue;
            ret.insert(std::make_pair(*iter1, *iter2));
        }
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    auto numbers = get_pentagonal_numbers(10);
    for (const auto& n : numbers) {
        std::cout << n << std::endl;
    }

    auto pairs = get_special_pentagonals(100000);
    for (const auto& p : pairs) {
        std::cout << p.second << " - " << p.first << " = " << p.second - p.first << std::endl;
    }
}
