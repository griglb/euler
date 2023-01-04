// The following iterative sequence is defined for the set of positive integers:
//          n -> n/2 (n is even)
//          n -> 3n + 1 (n is odd)
// Using the rule above and starting with 13, we generate the following sequence:
//              13 -> 40 -> 20 -> 10 -> 5 -> 16 -> 8 -> 4 -> 2 -> 1
// It can be seen that this sequence (starting at 13 and finishing at 1) contains 10 terms.
// Although it has not been proved yet (Collatz Problem), it is thought that all starting numbers finish at 1.
// Which starting number, under one million, produces the longest chain?
// NOTE: Once the chain starts the terms are allowed to go above one million.


#include <algorithm>
#include <iostream>
#include <vector>


std::vector<int64_t> get_collatz_sequence(int64_t num) {
    std::vector<int64_t> ret{ num };

    while (num > 1) {
        if (num % 2) {
            num = 3 * num + 1;
        }
        else {
            num /= 2;
        }
        ret.push_back(num);
    }

    return ret;
}


std::vector<int32_t> get_collatz_sequence_bad(int32_t num) {
    std::vector<int32_t> ret{ num };

    while (num > 1) {
        if (num % 2) {
            num = 3 * num + 1;
        }
        else {
            num /= 2;
        }
        ret.push_back(num);
    }

    return ret;
}


std::vector<size_t> get_collatz_sequence_lengths(int64_t max_start) {
    std::vector<size_t> ret{ 0 };

    for (int64_t i = 1; i < max_start; ++i) {
        auto seq = get_collatz_sequence(i);
        ret.push_back(seq.size());
    }

    return ret;
}


std::vector<int64_t> get_longest_collatz_sequence(int64_t max_start) {
    std::vector<int64_t> ret;

    for (int64_t i = 1; i < max_start; ++i) {
        auto seq = get_collatz_sequence(i);
        if (seq.size() > ret.size()) {
            ret = seq;
        }
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";
    std::vector<int64_t> sequence = get_collatz_sequence(13);
    for (const auto& n : sequence) {
        std::cout << n << " ";
    }
    std::cout << std::endl;

    sequence = get_longest_collatz_sequence(1'000'000);
    for (const auto& n : sequence) {
        std::cout << n << " ";
    }
    std::cout << std::endl;

    //for (int i = 1; i < 1'000'000; ++i) {
    //    auto good_seq = get_collatz_sequence(i);
    //    auto bad_seq = get_collatz_sequence_bad(i);
    //    if (good_seq.size() != bad_seq.size()) {
    //        std::cout << i << "\t" << good_seq.size() << "\t" << bad_seq.size() << std::endl;
    //    }

    //}

    //auto lengths = get_collatz_sequence_lengths(1'000'000);
    //auto index = std::max_element(lengths.begin(), lengths.end());
    //std::cout << index - lengths.begin() << " = " << *index << std::endl;
    //int i = 0;
    //for (const auto& l : lengths) {
    //    if (l >= 400) {
    //        std::cout << "\t" << i << " = " << l << std::endl;
    //    }
    //    ++i;
    //}
    
}
