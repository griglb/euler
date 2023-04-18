// The 5-digit number, 16807=7^5, is also a fifth power. Similarly, the 9-digit number, 134217728=8^9, is a ninth power.
// How many n-digit positive integers exist which are also an nth power?

#include <iostream>

#include "big_int.h"


void get_powers() {
    int16_t count{0};
    for (int16_t base = 2; base < 10; ++base) {
        BigInt num{1};
        for (uint16_t exp = 1; exp < 50; ++exp) {
            num *= base;
            if (num.get_digits().size() == exp)
                std::cout << ++count << "\t" << base << " ^ " << exp << " = " << num << std::endl;
        }
    }
}


int main()
{
    std::cout << "Hello World!\n";

    get_powers();
}

