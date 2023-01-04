// It can be seen that the number, 125874, and its double, 251748,
// contain exactly the same digits, but in a different order.
// Find the smallest positive integer, x, such that 2x, 3x, 4x, 5x, and 6x, contain the same digits.


#include <iostream>
#include <set>

#include "big_int.h"


std::set<int64_t> get_permuted_multiples() {
    for (BigInt curr_val{1}; true ; curr_val += 1) {
        auto tmp = curr_val.get_digits();
        std::multiset<int8_t> digits{tmp.begin(), tmp.end()};

        BigInt val2{curr_val};
        val2 *= 2;
        tmp = val2.get_digits();
        std::multiset<int8_t> digits2{tmp.begin(), tmp.end()};

        if (digits != digits2)
            continue;

        BigInt val3{curr_val};
        val3 *= 3;
        tmp = val3.get_digits();
        std::multiset<int8_t> digits3{tmp.begin(), tmp.end()};

        if (digits != digits3)
            continue;

        BigInt val4{curr_val};
        val4 *= 4;
        tmp = val4.get_digits();
        std::multiset<int8_t> digits4{tmp.begin(), tmp.end()};

        if (digits != digits4)
            continue;

        BigInt val5{curr_val};
        val5 *= 5;
        tmp = val5.get_digits();
        std::multiset<int8_t> digits5{tmp.begin(), tmp.end()};

        if (digits != digits5)
            continue;

        BigInt val6{curr_val};
        val6 *= 6;
        tmp = val6.get_digits();
        std::multiset<int8_t> digits6{tmp.begin(), tmp.end()};


        if (digits != digits6)
            continue;

        return { curr_val.to_int(), val2.to_int(), val3.to_int(), val4.to_int(), val5.to_int(), val6.to_int()};
    }

    return {};
}


int main()
{
    std::cout << "Hello World!\n";

    auto multiples = get_permuted_multiples();
    for (const auto &mult : multiples)
        std::cout << mult << " ";
    std::cout << std::endl;
}
