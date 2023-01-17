// Let p(n) represent the number of different ways in which n coins can be separated into piles.
// For example, five coins can be separated into piles in exactly seven different ways, so p(5) = 7.
//      OOOOO
//      OOOO   O
//      OOO   OO
//      OOO   O   O
//      OO   OO   O
//      OO   O   O   O
//      O   O   O   O   O
//
// Find the least value of n for which p(n) is divisible by one million.

#include <iostream>
#include <numeric>
#include <set>
#include <thread>
#include <vector>


uint64_t get_num_sums_with_n_terms(uint64_t total, uint64_t num_terms) {
    if (1 == num_terms)
        return 1;
    if (2 == num_terms) {
        // As the comments above detail, there are separate equations for even and odd n,
        // but that is for pure math.  With integer math on the computer, we can combine
        // the two expressions into one.  In this case the even expression works for all
        // values, as odd values of n will have the 1/2 truncated away.
        return total / 2;
    }

    uint64_t ret{ 0 };
    // The expressions above vary the last 2 terms, for fixed sets of the first (k-2) terms.
    std::vector<uint16_t> smallest_terms(num_terms - 2, 1);
    uint64_t smallest_sum{ num_terms };
    // For the different sets of fixed smallest terms, we start by incrementing the last term
    // and move forward.
    while (true) {
        // 1. Add new sums to ret
        // 2. Increment odometer
        //   a. Loop index from back to front
        //     i. If incrementing current index stays under total, then break out of for loop for next while loop
        //    ii. If not last index, then reset all subsequent values to new value
        //   b. If run out of indices to increment, then break out of while loop

        ret += (total - smallest_sum + 2) / 2;

        // Increment varying_index to next value.
        for (int64_t varying_index = smallest_terms.size() - 1; varying_index >= 0; --varying_index) {
            ++smallest_terms[varying_index];
            for (uint64_t later_index = varying_index + 1; later_index < smallest_terms.size(); ++later_index)
                smallest_terms[later_index] = smallest_terms[varying_index];
            // Get the sum of all the terms, adding the smallest values for the last 2,
            // which is the last element of smallest_terms.
            smallest_sum = std::accumulate(smallest_terms.cbegin(), smallest_terms.cend(), 2 * (*smallest_terms.rbegin()));
            // 
            if (smallest_sum <= total)
                break;
        }

        if (smallest_sum > total)
            break;
    }

    return ret;
}


uint64_t get_num_sums(uint64_t total) {
    // Handle special cases
    if (1 > total)
        return 0;

    uint64_t ret{ 0 };
    for (uint64_t num_terms = 1; num_terms <= total; ++num_terms)
        ret += get_num_sums_with_n_terms(total, num_terms);
    return ret;
}


void thread_func(uint64_t offset, uint64_t max_val, uint64_t stride) {
    for (uint64_t total = 1 + offset; total <= max_val; total += stride) {
        auto num = get_num_sums(total);
        std::cout << total << " = \t" << num << std::endl;
        if (num % 1'000'000 == 0)
            exit(0);
    }
}


std::vector<uint64_t> num_sums_dp(uint64_t n) {
    // Initialize a list
    std::vector<uint64_t> dp(n + 1, 0);

    // Update dp[0] to 1
    dp[0] = 1;

    // Iterate over the range [1, K + 1]
    for (int64_t row = 1; row < n + 1; ++row) {
        // Iterate over the range [row, N + 1]
        for (int64_t col = row; col < n + 1; ++col) {
            // Update current dp[col] state
            dp[col] += dp[col - row];
            dp[col] %= 1'000'000'000'000'000;
        }
    }

    // Return the total number of ways
    return dp;
}


int main()
{
    std::cout << "Hello World!\n";

    {
        for (uint16_t total = 1; total < 21; ++total) {
            std::cout << total << " = \t" << get_num_sums(total) << " - \t";
            for (uint16_t n = 1; n <= total; ++n)
                std::cout << n << ": " << get_num_sums_with_n_terms(total, n) << "\t";
            std::cout << std::endl;
        }
    }

    {
        constexpr uint64_t MaxValue{ 60'000 };
        auto sums = num_sums_dp(MaxValue);
        for (uint16_t total = 1; total <= MaxValue; ++total) {
//            std::cout << total << " = \t" << sums[total] << std::endl;
            if (sums[total] % 1'000'000 == 0) {
                std::cout << "FOUND IT!!!" << total << std::endl;
                break;
            }
        }
    }

    return 0;

    //{
    //    for (uint16_t total = 1; total < 1'000; ++total) {
    //        auto num = get_num_sums(total);
    //        std::cout << total << " = \t" << num << std::endl;
    //        if (num % 1'000'000 == 0)
    //            break;
    //    }
    //}

    //{
    //    constexpr uint64_t max_val{ 1'000 };
    //    constexpr uint64_t num_thread{ 16 };
    //    std::vector<std::thread> threads;

    //    for (uint64_t offset = 0; offset < num_thread; ++offset) {
    //        threads.emplace_back(thread_func, offset, max_val, num_thread);
    //    }

    //    for (auto& th : threads)
    //        th.join();
    //}

    return -1;
}
