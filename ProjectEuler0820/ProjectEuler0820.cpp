// Let d_n(x) be the nth decimal digit of the fractional part of x, or 0 if the
// fractional part has fewer than n digits.
//
// For example:
//      - d_7(1) = d_7(1/2) = d_7(1/4) = d_7(1/5) = 0
//      - d_7(1/3) = 3 since 1/3 = 0.333333_3_333...
//      - d_7(1/6) = 6 since 1/6 = 0.166666_6_666...
//      - d_7(1/7) = 1 since 1/7 = 0.142857_1_428...
//
// Let S(n) = sum{k = 1.n} d_n(1/k).
//
// You are given:
//      - S(7) = 0 + 0 + 3 + 0 + 0 + 6 + 1 = 10
//      - S(100) = 418
//
// Find S(10^7).


#include <iostream>


int main()
{
    std::cout << "Hello World!\n";
}
