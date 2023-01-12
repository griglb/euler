// The number 145 is well known for the property that the sum of the factorial of its digits is equal to 145:
//      1! + 4! + 5! = 1 + 24 + 120 = 145
//
// Perhaps less well known is 169, in that it produces the longest chain of numbers that link back to 169;
// it turns out that there are only three such loops that exist :
//
//      169 -> 363601 -> 1454 -> 169
//      871 -> 45361 -> 871
//      872 -> 45362 -> 872
//
// It is not difficult to prove that EVERY starting number will eventually get stuck in a loop.For example,
//      69 -> 363600 -> 1454 -> 169 -> 363601 (-> 1454)
//      78 -> 45360 -> 871 -> 45361 (-> 871)
//      540 -> 145 (-> 145)
//
// Starting with 69 produces a chain of five non-repeating terms, but the longest non-repeating
// chain with a starting number below one million is sixty terms.
//
// How many chains, with a starting number below one million, contain exactly sixty non-repeating terms?


#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "big_int.h"


constexpr uint64_t Factorials[] = { 1,
                                    1,
                                    2,
                                    3 * 2,
                                    4 * 3 * 2,
                                    5 * 4 * 3 * 2,
                                    6 * 5 * 4 * 3 * 2,
                                    7 * 6 * 5 * 4 * 3 * 2,
                                    8 * 7 * 6 * 5 * 4 * 3 * 2,
                                    9 * 8 * 7 * 6 * 5 * 4 * 3 * 2,
};

using Chain = std::vector<BigInt>;
std::map<BigInt, Chain> digit_factorial_chains;

Chain get_digit_factorial_chains(uint64_t number) {
    BigInt curr_num{ number };

    if (digit_factorial_chains.find(curr_num) != digit_factorial_chains.end())
        return digit_factorial_chains.at(curr_num);

    Chain chain{ curr_num };
    while (true) {
        // Calculate the next number in the chain from this number's digits.
        auto digits = curr_num.get_digits();
        BigInt next_num{ 0 };
        for (const auto& dig : digits) {
            next_num += Factorials[dig];
        }
        // Check if we have completed a cycle.
        if (std::find(chain.cbegin(), chain.cend(), next_num) != chain.cend()) {
            chain.push_back(next_num);
            break;
        }
        // Check if we have reached the beginning of an known chain.
        if (digit_factorial_chains.find(next_num) != digit_factorial_chains.end()) {
            const auto& tmp = digit_factorial_chains.at(next_num);
            std::copy(tmp.begin(), tmp.end(), std::back_inserter(chain));
            break;
        }
        // Add number to chain and update for next iteration.
        chain.push_back(next_num);
        curr_num = next_num;
    }
    digit_factorial_chains[BigInt{ number }] = chain;
    return chain;
}


int main()
{
    std::cout << "Hello World!\n";

    {
        for (const auto& num : { 145, 169, 871, 872, 69, 78, 540 }) {
            auto chain = get_digit_factorial_chains(num);
            for (const auto& el : chain)
                std::cout << el << " -> ";
            std::cout << std::endl;
        }
    }

    std::vector<uint64_t> chains_60;
    for (uint64_t i = 0; i < 1'000'000; ++i) {
        if (i % 1'000 == 0) std::cout << i << std::endl;
        auto chain = get_digit_factorial_chains(i);
        //for (const auto& el : chain)
        //    std::cout << el << " -> ";
        //std::cout << std::endl;
        if (chain.size() >= 60) {
            std::set<BigInt> tmp{ chain.begin(), chain.end() };
            if (tmp.size() == 60)
                chains_60.push_back(i);
        }
    }
    std::cout << std::endl << std::endl << "Chains with 60 numbers:" << std::endl;
    for (const auto& num : chains_60)
        std::cout << "\t" << num << std::endl;
    std::cout << chains_60.size() << std::endl;
}
