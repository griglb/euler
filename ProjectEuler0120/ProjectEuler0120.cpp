// Let r be the remainder when (a−1)^n + (a+1)^n is divided by a^2.

// For example, if a = 7 and n = 3, then r = 42: 6^3 + 8^3 = 728 == 42 mod 49.
// And as n varies, so too will r, but for a = 7 it turns out that r_max = 42.

// For 3 <= a <= 1000, find sum(r_max).


#include <cstdint>
#include <iostream>

#include "big_int.h"


BigInt get_r_max(uint64_t a) {
    uint64_t minus{a - 1};
    uint64_t plus{a + 1};
    uint64_t sq{a * a};

    BigInt t1{minus};
    BigInt t2{plus};
    BigInt r_max{(t1 + t2) % sq};
// std::cout << 1 << " " << t1 << " " << t2 << " " << r_max << std::endl;
    for (BigInt n{ 2 }; n <= 2*a; ++n) {
        t1 *= minus;
        t2 *= plus;
        BigInt r = (t1 + t2) % sq;
// std::cout << n << " " << t1 << " " << t2 << " " << r << std::endl;
        if (r > r_max)
            r_max = r;
    }
    return r_max;
}


int main()
{
    std::cout << "Hello World!\n";

    {
        auto r_max = get_r_max(13);
        std::cout << r_max << std::endl;
    }

    {
        BigInt sum{0};
        for (uint64_t a = 3; a <= 1000; ++a) {
            auto r_max = get_r_max(a);
            sum += r_max;
            std::cout << a << "\t" << r_max << std::endl;
        }
        std::cout << "sum = " << sum << std::endl;
    }
}

