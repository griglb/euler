#include "gcd.h"


int64_t gcd(int64_t a, int64_t b) {
    if ((0 == a) || (0 == b))
        return 1;

    int64_t mag_a = std::abs(a);
    int64_t mag_b = std::abs(b);

    if ((1 == mag_a) || (1 == mag_b))
        return 1;
    if (mag_a == mag_b)
        return mag_a;

    //    std::cout << a << "\t" << b << std::endl;
    int64_t bigger = std::max(mag_a, mag_b);
    int64_t smaller = std::min(mag_a, mag_b);

    while (bigger > smaller) {
        int64_t quot = bigger / smaller;
        bigger -= quot * smaller;
    }

    if (0 == bigger)
        return smaller;
    if (smaller == bigger)
        return smaller;

    return gcd(smaller, bigger);
}


BigInt gcd(const BigInt& a, const BigInt& b) {
    if ((a == 0) || (b == 0))
        return BigInt{ 1 };

    BigInt mag_a = a.abs();
    BigInt mag_b = b.abs();

    if ((mag_a == 1) || (mag_b == 1))
        return BigInt{ 1 };
    if (mag_a == mag_b)
        return mag_a;

    //    std::cout << a << "\t" << b << std::endl;
    BigInt bigger, smaller;
    if (mag_a >= mag_b) {
        bigger = mag_a;
        smaller = mag_b;
    }
    else {
        bigger = mag_b;
        smaller = mag_a;
    }

    while (bigger > smaller) {
        BigInt quot = bigger / smaller;
        bigger -= quot * smaller;
    }

    if (bigger == 0)
        return smaller;
    if (smaller == bigger)
        return smaller;

    return gcd(smaller, bigger);
}


int64_t lcm(int64_t a, int64_t b) {
    auto div = gcd(a, b);
    return (a / div) * b;
}


BigInt lcm(BigInt a, BigInt b) {
    auto div = gcd(a, b);
    return (a / div) * b;
}

