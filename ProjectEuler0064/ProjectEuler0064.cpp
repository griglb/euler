// All square roots are periodic when written as continued fractions and can be written in the form:
//   sqrt(N) = a0 + 1 / (a1 + 1 / (a2 + 1 / (a3 + ...)))
// For example, let us consider
//   sqrt(23) = 4 + sqrt(23) - 4 = 4 + 1 / (1 / (sqrt(23) - 4)) = 4 + 1 / (1 + (sqrt23) - 3)/ 7)
// If we continue we would get the following expansion:
//   sqrt(23) = 4 + 1 / (1 + 1 / (3 + 1 / (1 + 1 / (8 + ...))))
// The process can be summarised as follows:
//   a0 = 4, 1 / (sqrt(23) - 4) =  (sqrt(23) + 4) / 7  = 1 + (sqrt(23) - 3)) / 7
//   a1 = 1, 7 / (sqrt(23) - 3) = 7(sqrt(23) + 3) / 14 = 3 + (sqrt(23) - 3)) / 2
//   a2 = 3, 2 / (sqrt(23) - 3) = 2(sqrt(23) + 3) / 14 = 1 + (sqrt(23) - 4)) / 7
//   a3 = 1, 7 / (sqrt(23) - 4) = 7(sqrt(23) + 4) / 7  = 8 +  sqrt(23) - 4
//   a4 = 8, 1 / (sqrt(23) - 4) =  (sqrt(23) + 4) / 7  = 1 + (sqrt(23) - 3)) / 7
//   a5 = 1, 7 / (sqrt(23) - 3) = 7(sqrt(23) + 3) / 14 = 3 + (sqrt(23) - 3)) / 2
//   a6 = 3, 2 / (sqrt(23) - 3) = 2(sqrt(23) + 3) / 14 = 1 + (sqrt(23) - 4)) / 7
//   a7 = 1, 7 / (sqrt(23) - 4) = 7(sqrt(23) + 4) / 7  = 8 +  sqrt(23) - 4
//
// It can be seen that the sequence is repeating. For conciseness, we use the notation
// [4; (1, 3, 1, 8)], to indicate that the block (1,3,1,8) repeats indefinitely.
//
// The first ten continued fraction representations of (irrational) square roots are:
// sqrt(2)  = [1; (2)], period=1
// sqrt(3)  = [1; (1, 2)], period=2
// sqrt(5)  = [2; (4)], period=1
// sqrt(6)  = [2; (2, 4)], period=2
// sqrt(7)  = [2; (1, 1, 1, 4)], period=4
// sqrt(8)  = [2; (1, 4)], period=2
// sqrt(10) = [3; (6)], period=1
// sqrt(11) = [3; (3, 6)], period=2
// sqrt(12) = [3; (2, 6)], period=2
// sqrt(13) = [3; (1, 1, 1, 1, 6)], period=5
// Exactly four continued fractions, for N <= 13, have an odd period.
//
// How many continued fractions for N <= 10,000 have an odd period?

#include <iostream>
#include <set>
#include <utility>
#include <vector>

#include "continued_fractions.h"


std::vector<Fraction> get_fractions(uint64_t max_val) {
    
    std::vector<Fraction> ret;
    for (uint64_t n = 1; n <= max_val; ++n)
        ret.push_back(get_continued_fraction(n));

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    {
        auto fractions = get_fractions(13);
        for (const auto& frac : fractions) {
            std::cout << "[" << frac.first << "; (";
            if (!frac.second.empty()) {
                auto iter = frac.second.cbegin();
                std::cout << *iter;
                for (++iter; iter != frac.second.cend(); ++iter) {
                    std::cout << ", " << *iter;
                }
            }
            std::cout << ")]" << std::endl;
        }
    }

    auto fractions = get_fractions(10'000);
    uint64_t count{ 0 };
    for (const auto& frac : fractions) {
        count += frac.second.size() % 2;
    }
    std::cout << count << std::endl;
}

