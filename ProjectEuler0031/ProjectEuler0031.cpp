// ProjectEuler0031.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <map>
#include <set>


using Coins = std::multiset<int16_t>;
using CoinPermutations = std::set<Coins>;


CoinPermutations get_permutations(const Coins& coins, int16_t total) {
    static std::map<int16_t, CoinPermutations> memos;
    if (memos.find(total) != memos.end())
        return memos[total];

    CoinPermutations ret;

    if (0 >= total)
        return ret;

    for (const auto &c : coins) {
        if (c > total)
            continue;
        if (c == total)
            ret.insert({c});
        else {
            CoinPermutations subset = get_permutations(coins, total - c);
            for (auto perm : subset) {
                perm.insert(c);
                ret.insert(perm);
            }
        }
    }

    memos[total] = ret;
    std::cout << total << "\t" << ret.size() << std::endl;

    return ret;
}

int main()
{
    std::cout << "Hello World!\n";

    CoinPermutations perms = get_permutations({ 1, 2, 5, 10, 20, 50, 100, 200 }, 200);
    //for (const auto& perm : perms) {
    //    std::cout << "{ ";
    //    for (const auto& coin : perm)
    //        std::cout << coin << ", ";
    //    std::cout << " }" << std::endl;
    //}
    std::cout << "count = " << perms.size() << std::endl;
}
