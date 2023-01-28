// A number chain is created by continuously adding the square of the digits in a number to form
// a new number until it has been seen before.
// For example,
// 44 -> 32 -> 13 -> 10 -> 1 -> 1
// 85 -> 89 -> 145 -> 42 -> 20 -> 4 -> 16 -> 37 -> 58 -> 89
// Therefore any chain that arrives at 1 or 89 will become stuck in an endless loop. What is most
// amazing is that EVERY starting number will eventually arrive at 1 or 89.
// How many starting numbers below ten million will arrive at 89?


#include <iostream>
#include <numeric>
#include <vector>


uint64_t num_89_chains(uint64_t max_val) {
    // We need to examine every integer in [1, max_val] have a chain with 89.
    // We do not need to test the whole chain, we can memoize each number as
    // we increment, and then when processing N any time we a number < N we
    // can check the memo to know the answer.
    std::vector<uint8_t> memo;
    memo.reserve(max_val+1);
    memo.push_back(0);  // need for 0-based indexing
    memo.push_back(0);  // 1 obviously is part of a number chain with 89 in it

    for (uint64_t i = 2; i <= max_val; ++i) {
        std::cout << i;
        uint64_t num{ i };
        while (true) {
            uint64_t tmp{ 0 };
            while (num > 0) {
                tmp += (num % 10) * (num % 10);
                num /= 10;
            }
            std::cout << "\t" << tmp;
            if (tmp < i) {
                memo.push_back(memo[tmp]);
                std::cout << "\t" << (memo[tmp] ? "true" : "false") << std::endl;
                break;
            }
            if (1 == tmp) {
                memo.push_back(0);
                std::cout << "\tfalse" << std::endl;
                break;
            }
            if (89 == tmp) {
                memo.push_back(1);
                std::cout << "\ttrue" << std::endl;
                break;
            }
            num = tmp;
        }
    }

    return std::accumulate(memo.begin(), memo.end(), 0);
}


int main()
{
    std::cout << "Hello World!\n";

    std::cout << num_89_chains(100) << std::endl;
    std::cout << num_89_chains(10'000'000) << std::endl;
}
