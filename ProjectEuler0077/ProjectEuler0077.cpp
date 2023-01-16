// It is possible to write ten as the sum of primes in exactly five different ways :
//      7 + 3
//      5 + 5
//      5 + 3 + 2
//      3 + 3 + 2 + 2
//      2 + 2 + 2 + 2 + 2
//
// What is the first value which can be written as the sum of primes in over five thousand different ways?


#include <algorithm>
#include <iostream>
#include <numeric>
#include <unordered_map>
#include <vector>

#include "prime_helper.h"


class sum_tracker {
public:
    sum_tracker() = default;
    ~sum_tracker() = default;

    void calc_num_sums(uint64_t max_value) {
        for (uint32_t num_terms = 2; num_terms <= max_value / 2; ++num_terms) {
            calc_sums_with_n_terms(max_value, num_terms);
        }
    }

    void print_sum_counts() const {
        std::vector<uint64_t> numbers;
        for (const auto& [number, count] : sum_counts_)
            numbers.push_back(number);
        std::sort(numbers.begin(), numbers.end());
        for (const auto& num : numbers)
            std::cout << num << "\t" << sum_counts_.at(num) << std::endl;
    }

private:
    PrimeHelper helper_;
//    std::unordered_set<uint64_t> primes_;
    std::unordered_map<uint64_t, uint64_t> sum_counts_;

    void calc_sums_with_n_terms(uint64_t max_value, uint32_t num_terms) {
        auto primes = helper_.get_primes(max_value);

        std::vector<ULongLongVec::const_iterator> smallest_terms;
        for (uint32_t n = 0; n < num_terms; ++n)
            smallest_terms.push_back(primes.cbegin());
        uint64_t sum{ 0 };
        for (auto iter = smallest_terms.cbegin(); iter != smallest_terms.cend(); ++iter)
            sum += **iter;

        while (true) {
            // 1. Add new sums to ret
            // 2. Increment odometer
            //   a. Loop index from back to front
            //     i. If incrementing current index stays under total, then break out of for loop for next while loop
            //    ii. If not last index, then reset all subsequent values to new value
            //   b. If run out of indices to increment, then break out of while loop

            if (sum_counts_.find(sum) == sum_counts_.end())
                sum_counts_[sum] = 0;
            ++sum_counts_[sum];

            // Increment varying_index to next value.
            for (int64_t varying_index = smallest_terms.size() - 1; varying_index >= 0; --varying_index) {
                ++smallest_terms[varying_index];
                if (smallest_terms[varying_index] == primes.cend())
                    continue;
                for (size_t later_index = varying_index + 1; later_index < smallest_terms.size(); ++later_index)
                    smallest_terms[later_index] = smallest_terms[varying_index];
                // Get the sum of all the terms, adding the smallest values for the last 2,
                // which is the last element of smallest_terms.
                sum = 0;
                for (auto iter = smallest_terms.cbegin(); iter != smallest_terms.cend(); ++iter)
                    sum += **iter;
                // 
                if (sum <= max_value)
                    break;
            }

            if (sum > max_value)
                break;
        }
    }
};


int main()
{
    std::cout << "Hello World!\n";

    {
        sum_tracker tracker;
        tracker.calc_num_sums(10);
        tracker.print_sum_counts();
    }

    {
        sum_tracker tracker;
        tracker.calc_num_sums(100);
        tracker.print_sum_counts();
    }
}
