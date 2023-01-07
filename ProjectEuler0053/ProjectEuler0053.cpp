// There are exactly ten ways of selecting three from five, 12345:
//          123, 124, 125, 134, 135, 145, 234, 235, 245, and 345
// In combinatorics, we use the notation, C(5, 3) = 10.
// In general, C(n, r) = n! / r! (n-r)!,
// where r <= n, n! = n x (n-1) x ... x 3 x 2 x 1, and 0! = 1.
// It is not until n = 23, that a value exceeds one-million:  C(23, 10) = 1144066.
// How many, not necessarily distinct, values of C(n, r) for 1 <= n <= 100,
// are greater than one-million?


#include <iostream>


int64_t get_num_terms() {
    // C(n, r) = n! / (r! * (n-r)!)
    // C(n, r+1) = n! / ((r+1)! * (n-r-1)!)

    // C(n, r+1) / C(n, r) = [n! / ((r+1)! * (n-r-1)!)] / [n! / (r! * (n-r)!)]
    // C(n, r+1) / C(n, r) = (n-r) / (r+1)

    // C(n, r+1) = C(n, r) * (n-r) / (r+1)

    // take row 23:
    // C(23,  0) = 1
    // C(23,  1) =        1 * 23 / 1 = 23
    // C(23,  2) =       23 * 22 / 2 = 253
    // C(23,  3) =      253 * 21 / 3 = 253 * 7 = 1771
    // C(23,  4) =     1771 * 20 / 4 = 1771 * 5 = 8855
    // C(23,  5) =     8855 * 19 / 5 = (8855 / 5) * 19 = 33649
    // C(23,  6) =    33649 * 18 / 6 = 33649 * 3 = 100947
    // C(23,  7) =   100947 * 17 / 7 = (100947/7) * 17 = 245157
    // C(23,  8) =   245157 * 16 / 8 = 245157 * 2 = 490314
    // C(23,  9) =   490314 * 15 / 9 = (490314 / 3) * 5 = 817190
    // C(23, 10) =   817190 * 14 / 10 = (817190 / 5) * 7 = 1144066
    int64_t count{0};

    for (int16_t n = 1; n < 101; ++n) {
        std::cout << n << ": ";
        int64_t term {1};
        for (int16_t r = 0; r < n/2; ++r) {
            term *= (n - r);
            term /= (r + 1);
            std::cout << term << " ";
            if (term > 1'000'000) {
                std::cout << std::endl;
                std::cout << n << "\t" << r+1 << std::endl;
                count += n - 2 * (r  + 1) + 1;
                break;
            }
        }
        std::cout << std::endl;
    }

    return count;
}


int main()
{
    std::cout << "Hello World!\n";

    std::cout << get_num_terms() << std::endl;
}
