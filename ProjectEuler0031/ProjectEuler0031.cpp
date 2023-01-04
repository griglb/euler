// In the United Kingdom the currency is made up of pound (£) and pence (p).
// There are eight coins in general circulation:
//     1p, 2p, 5p, 10p, 20p, 50p, £1 (100p), and £2 (200p).
// It is possible to make £2 in the following way:
//     1×£1 + 1×50p + 2×20p + 1×5p + 1×2p + 3×1p
// How many different ways can £2 be made using any number of coins?


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
