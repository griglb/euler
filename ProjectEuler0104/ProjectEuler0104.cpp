// The Fibonacci sequence is defined by the recurrence relation :
//      F_n = F_n−1 + F_n−2, where F_1 = 1 and F_2 = 1.
//
// It turns out that F_541, which contains 113 digits, is the first Fibonacci number
// for which the last nine digits are 1-9 pandigital (contain all the digits 1 to 9,
// but not necessarily in order).  And F_2749, which contains 575 digits, is the first
// Fibonacci number for which the first nine digits are 1-9 pandigital.
//
// Given that F_k is the first Fibonacci number for which the first nine digits AND
// the last nine digits are 1-9 pandigital, find k.


#include <iostream>
#include <set>

#include "big_int.h"


int64_t get_pandigital_fibonacci() {
    const std::set<int8_t> Digits{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    BigInt n_2{ 1 };
    BigInt n_1{ 1 };

    int64_t ind{ 2 };
    while (true) {
        ++ind;
        BigInt tmp{ n_1 };
        tmp += n_2;

        std::cout << ind << std::endl;

        auto fib_digits = tmp.get_digits();
        if (fib_digits.size() >= 18) {

            // test last 9 digits
            std::set<int8_t> last_digits;
            auto riter = fib_digits.rbegin();
            for (int8_t i = 0; i < 9; ++i)
                last_digits.insert(*riter++);

            // test first 9 digits
            std::set<int8_t> first_digits;
            auto iter = fib_digits.begin();
            for (int8_t i = 0; i < 9; ++i)
                first_digits.insert(*iter++);

            if ((Digits == first_digits) && (Digits == last_digits)) {
                std::cout << tmp << std::endl << fib_digits.size() << std::endl;
                return ind;
            }
        }

        n_2 = n_1;
        n_1 = tmp;
    }

}


int main()
{
    std::cout << "Hello World!\n";

    std::cout << get_pandigital_fibonacci() << std::endl;
}
