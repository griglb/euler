// Find the unique positive integer whose square has the form 1_2_3_4_5_6_7_8_9_0,
// where each '_' is a single digit.


#include <iostream>

#include "big_int.h"


BigInt solver() {
    BigInt current{ 1000000000 };

    while (true) {
        BigInt sq{ current };
        sq *= current;

        auto digits = sq.get_digits();
        if (digits.size() < 19)
            continue;
        if (digits.size() > 19)
            throw "Didn't find it";

        if ((1 == digits[0]) && (2 == digits[2]) && (3 == digits[4]) &&
            (4 == digits[6]) && (5 == digits[8]) && (6 == digits[10]) &&
            (7 == digits[12]) && (8 == digits[14]) && (9 == digits[16]) &&
            (0 == digits[18]))
            return current;

        current += 10;
    }

    return BigInt{ 0 };
}


int main()
{
    std::cout << "Hello World!\n";

    std::cout << solver() << std::endl;
}
