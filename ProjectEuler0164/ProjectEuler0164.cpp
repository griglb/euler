// How many 20 digit numbers (without any leading zero) exist such that no
// three consecutive digits of have a sum greater than 9?

#include <iostream>
#include <vector>

// Let's start by enumerating the 3 digit sequences that have a sum < 10:
//      000, 001, 002, 003, 004, 005, 006, 007, 008, 009
//      010, 011, 012, 013, 014, 015, 016, 017, 018
//      020, 021, 022, 023, 024, 025, 026, 027
//      030, 031, 032, 033, 034, 035, 036
//      040, 041, 042, 043, 044, 045
//      050, 051, 052, 053, 054
//      060, 061, 062, 063
//      070, 071, 072
//      080, 081
//      090
//      100, 101, 102, 103, 104, 105, 106, 107, 108
//      110, 111, 112, 113, 114, 115, 116, 117
//      120, 121, 122, 123, 124, 125, 126
//      130, 131, 132, 133, 134, 135
//      140, 141, 142, 143, 144
//      150, 151, 152, 153
//      160, 161, 162
//      170, 171
//      180
//      200, 201, 202, 203, 204, 205, 206, 207
//      210, 211, 212, 213, 214, 215, 216
//      220, 221, 222, 223, 224, 225
//      230, 231, 232, 233, 234
//      240, 241, 242, 243
//      250, 251, 252
//      260, 261
//      270
//      300, 301, 302, 303, 304, 305, 306
//      310, 311, 312, 313, 314, 315
//      320, 321, 322, 323, 324
//      330, 331, 332, 333
//      340, 341, 342
//      350, 351
//      360
//      400, 401, 402, 403, 404, 405
//      410, 411, 412, 413, 414
//      420, 421, 422, 423
//      430, 431, 432
//      440, 441
//      450
//      500, 501, 502, 503, 504
//      510, 511, 512, 513
//      520, 521, 522
//      530, 531
//      540
//      600, 601, 602, 603
//      610, 611, 612
//      620, 621
//      630
//      700, 701, 702
//      710, 711
//      720
//      800, 801
//      810
//      900
// The sequences were organzied like this to show that the number of sequences that
// start with the same digit is a triangle number T(10-n) = n*(n+1)/2.
// So for 3 digit sequences, we have 55 + 45 + 36 + 28 + 21 + 15 + 10 + 6 + 3 + 1 = 220.

// How to construct longer sequences?  We can look at a 4 digit number ABCD as being
// comprised of two 3 digit sequences - ABC and BCD.  Both of these must be in the set of
// 220 valid 3 digit numbers, but the values of A & D are completely decoupled.

//       0  1  2  3  4  5  6  7  8  9
//    0 10  9  8  7  6  5  4  3  2  1
//    1  9  8  7  6  5  4  3  2  1  0
//    2  8  7  6  5  4  3  2  1  0  0
//    3  7  6  5  4  3  2  1  0  0  0
//    4  6  5  4  3  2  1  0  0  0  0
//    5  5  4  3  2  1  0  0  0  0  0
//    6  4  3  2  1  0  0  0  0  0  0
//    7  3  2  1  0  0  0  0  0  0  0
//    8  2  1  0  0  0  0  0  0  0  0
//    9  1  0  0  0  0  0  0  0  0  0


uint64_t brute_force(int16_t num_digits) {
    uint64_t ret{ 0 };

    int64_t max_val{ 1 };
    for (int16_t n = 0; n < num_digits; ++n)
        max_val *= 10;

    std::cout << max_val << std::endl;

    for (int64_t n = 100; n < max_val; ++n) {
        int64_t sum{ 0 };
        int64_t tmp{ n };
        int64_t d_0{ 0 };
        int64_t d_1{ 0 };
        int64_t d_2{ 0 };
        for (int16_t d = 0; d < num_digits; ++d) {
            d_0 = tmp % 10;

            sum += d_0 - d_2;
            if (sum > 9)
                break;

            d_2 = d_1;
            d_1 = d_0;
            tmp /= 10;
        }

        if (sum < 10) {
            ++ret;
            std::cout << n << "  ";
        }
    }

    std::cout << std::endl;

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    {
        std::cout << brute_force(3) << std::endl;
    }
}
