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


using Repeat = std::vector<uint64_t>;
using Fraction = std::pair<uint64_t, Repeat>;


void reduce_fraction(uint64_t &num, uint64_t &den) {
    if ((den % num) == 0) {
        den /= num;
        num = 1;
        return;
    }
    throw "uh-oh";
}


Fraction get_continued_fraction(uint64_t number) {
    double root = sqrt(number);
    // a0 = floor(sqrt(number))
    uint64_t a0 = static_cast<uint64_t>(root);
    // Make sure we don't have a perfect square.
    if (a0 * a0 == number) {
        return { a0, { } };
    }

    // After a0, we get a1 by (let root = sqrt(number):
    //   number = a0 + root - a0
    //   number = a0 + 1 / (1 / (root - a0))
    //   number = a0 + 1 / ((root + a0) / (number - a0^2))
    //   number = a0 + 1 / ((root + addend) / den)
    //   a1 = floor((root + addend) / den)
    //   number = a0 + 1 / (a1 + 1 / ((root - addend)/den))
    // The denominator is the difference between number and the largest perfect square less than it.
    // The numerator is a sum, which can be made a difference after extracting a1.

    uint64_t addend{ a0 };
    uint64_t den{ number - a0 * a0 };
    uint64_t num{ 1 };

    using Terms = std::pair<uint64_t, uint64_t>;
    std::set<Terms> term_history{ {addend, den} };

    Repeat a1n;
    while (true) {
        uint64_t an = static_cast<uint64_t>(num * (root + addend) / den);
        addend = an * den - addend;
        num = den;
        den = number - addend * addend;
        reduce_fraction(num, den);

        a1n.push_back(an);

        Terms terms{ addend, den };
        if (term_history.find(terms) != term_history.end())
            break;
        term_history.insert(terms);
    }

    return { a0, a1n };
}


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

