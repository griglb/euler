// Define s(n) to be the smallest number that has a digit sum of n.
// For example s(10) = 19.
//
// Let S(k) = sum{n=1..k} s(n)
//
// You are given S(20) = 1074.
//
// Further let f_i be the Fibonacci sequence defined by
//      f_0 = 0
//      f_1 = 1
//      f_i = f_i-2 + f_i-1     for all i >= 2
//
// Find sum{i=2..90} S(f_i).
// Give your answer modulo 1,000,000,007.


#include <array>
#include <iostream>
#include <vector>


// Let's first look at the Fibonacci numbers to see what the upper bound on k is:
//         i                   f_i              f_i % 9
//         0                     0                  0
//         1                     1                  1
//         2                     1                  1
//         3                     2                  2
//         4                     3                  3
//         5                     5                  5
//         6                     8                  8
//         7                    13                  4
//         8                    21                  3
//         9                    34                  7
//        10                    55                  1
//        11                    89                  8
//        12                   144                  0
//        13                   233                  8
//        14                   377                  8
//        15                   610                  7
//        16                   987                  6
//        17                  1597                  4
//        18                  2584                  1
//        19                  4181                  5
//        20                  6765                  6
//        21                 10946                  2
//        22                 17711                  8
//        23                 28657                  1
//        24                 46368                  0
//        25                 75025                  1
//        26                121393                  1
//        27                196418                  2
//        28                317811                  3
//        29                514229                  5
//        30                832040                  8
//        31               1346269                  4
//        32               2178309                  3
//        33               3524578                  7
//        34               5702887                  1
//        35               9227465                  8
//        36              14930352                  0
//        37              24157817                  8
//        38              39088169                  8
//        39              63245986                  7
//        40             102334155                  6
//        41             165580141                  4
//        42             267914296                  1
//        43             433494437                  5
//        44             701408733                  6
//        45            1134903170                  2
//        46            1836311903                  8
//        47            2971215073                  1
//        48            4807526976                  0
//        49            7778742049                  1
//        50           12586269025                  1
//        51           20365011074                  2
//        52           32951280099                  3
//        53           53316291173                  5
//        54           86267571272                  8
//        55          139583862445                  4
//        56          225851433717                  3
//        57          365435296162                  7
//        58          591286729879                  1
//        59          956722026041                  8
//        60         1548008755920                  0
//        61         2504730781961                  8
//        62         4052739537881                  8
//        63         6557470319842                  7
//        64        10610209857723                  6
//        65        17167680177565                  4
//        66        27777890035288                  1
//        67        44945570212853                  5
//        68        72723460248141                  6
//        69       117669030460994                  2
//        70       190392490709135                  8
//        71       308061521170129                  1
//        72       498454011879264                  0
//        73       806515533049393                  1
//        74      1304969544928657                  1
//        75      2111485077978050                  2
//        76      3416454622906707                  3
//        77      5527939700884757                  5
//        78      8944394323791464                  8
//        79     14472334024676221                  4
//        80     23416728348467685                  3
//        81     37889062373143906                  7
//        82     61305790721611591                  1
//        83     99194853094755497                  8
//        84    160500643816367088                  0
//        85    259695496911122585                  8
//        86    420196140727489673                  8
//        87    679891637638612258                  7
//        88   1100087778366101931                  6
//        89   1779979416004714189                  4
//        90   2880067194370816120                  1
//
// f_90 = 2.8E18, which is way too big for a brute force approach.

// The sum of sums leads to a bit of redundancies.  Just look at the first 3 terms in
// the final sum:
//      S(1) + S(2) + S(3) = [s(1)] + [s(1) + s(2)] + [s(1) + s(2) + s(3)]
//                         = 3 * s(1) + 2 * s(2) + s(3)
// Let's extend this to the first 6 terms:
//      sum{i=2..7} S(f_i) = s(1) +
//                           s(1) + s(2) +
//                           s(1) + s(2) + s(3)
//                           s(1) + s(2) + s(3) + s(4) + s(5)
//                           s(1) + s(2) + s(3) + s(4) + s(5) + s(6) + s(7) + s(8)
//                           s(1) + s(2) + s(3) + s(4) + s(5) + s(6) + s(7) + s(8) + s(9) + s(10) + s(11) + s(12) + s(13)
//                         = 6 * [s(1)] +
//                           5 * [s(2)] +
//                           4 * [s(3)] +
//                           3 * [s(4) + s(5)] +
//                           2 * [s(6) + s(7) + s(8)] +
//                           1 * [s(9) + s(10) + s(11) + s(12) + s(13)]

// Consider the s(n) function.  What does this function look like?
// For n < 10, s(n) == n, because you only need the one digit to get that sum.
// What about n >= 10?  The value s(10) = 19 holds a clue - it ends in 9.
// The smallest way to get a sum is to add bunch of large numbers, which in this case max
// out at 9.  So for n >= 10, we can construct the value of s(n) by subtracting 9's from n,
// which are prepended to the function value:
//      n = 10:
//          start with empty string, then subtract 9 from n and prepend it to the empty string to get 9
//          this leaves n = 1 < 9, so prepend that to string and get 19
//      n = 11:
//          start with empty string, then subtract 9 from n and prepend it to the empty string to get 9
//          this leaves n = 2 < 9, so prepend that to string and get 29
//      n = 12:
//          start with empty string, then subtract 9 from n and prepend it to the empty string to get 9
//          this leaves n = 3 < 9, so prepend that to string and get 39
// This continues until you get to n = 19:
//          start with empty string, then subtract 9 from n and prepend it to the empty string to get 9
//          this leaves n = 10 > 9, so subtract 9 from n and prepend it to the string to get 99
//          this leaves n = 1 < 9, so prepend that to string and get 199
//
// So the value of S(20) can be constructed as:
//      s(1) = 1
//      s(2) = 2
//      s(3) = 3
//      s(4) = 4
//      s(5) = 5
//      s(6) = 6
//      s(7) = 7
//      s(8) = 8
//      s(9) = 9
//      s(10) = 19
//      s(11) = 29
//      s(12) = 39
//      s(13) = 49
//      s(14) = 59
//      s(15) = 69
//      s(16) = 79
//      s(17) = 89
//      s(18) = 99
//      s(19) = 199
//      s(20) = 299
//
//      S(20) = 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 19 + 29 + 39 + 49 + 59 + 69 + 79 + 89 + 99 + 199 + 299
//            = 1074
//
// There is a pattern here too.  Note the values of s(n), they come in groups of 9:
//      s(n) = n                                        if n in [1, 9]
//      s(n) = 10*(n - 9) + 9 = 10*n - 81               if n in [10, 18]
//      s(n) = 100*(n - 18) + 99 = 100*n - 1701         if n in [19, 27]
//      s(n) = 1000*(n - 27) + 999 = 1000*n - 26001     if n in [28, 36]
//      ...
//      s(100) = 199,999,999,999 = 1E11 * (n - 99) + 99,999,999,999 = 1E11 * n - 9800000000001
//
// Generalize this to:
//      num_digits = floor((n - 1) / 9) + 1
//      num_nines = num_digits - 1 = floor((n - 1) / 9)
//      s(n) = 10^num_nines * (n - 9 * num_nines) + 10^num_nines - 1
//      s(n) = 10^num_nines * n - 10^num_nines * (9 * num_nines - 1) - 1
// Examples:
//      n = 20:
//          num_nines = floor(19 / 9) = 2
//          s(20) = 1E2 * n - 1E2 * (18 - 1) - 1 = 100 * n - 1701
// 
//      n = 100:
//          num_nines = floor(99 / 9) = 11
//          s(100) = 1E11 * n - 1E11 * (99 - 1) - 1 = 1E11 * n - 98E11 - 1

// Now on to how to calculate S(k) efficiently.
// As detailed above, the s(n) values follow a mod 9 pattern, and are a form of triangle numbers.
// Let T(n) be the n'th triangle number.  It is defined as:
//      T(n) = n * (n + 1) / 2
//
// We can then define S(k) as:
//      S(k) = T(k) = k * (k + 1) / 2                   if k in [1, 9]
//      S(k) = 10 * T(k - 9) + 9 * (k - 9) + S(9)       if k in [10, 18]
//           = 10 * (k - 9) * (k - 8) / 2 + 9 * k - 81 + 45
//           = 5 * k^2 - 85 * k + 360 + 9 * k - 36
//           = 5 * k^2 - 76 * k + 324
//      S(k) = 100 * T(k - 18) + 99 * (k - 18) + S(18)  if k in [19, 27]
//           = 100 * (k - 18) * (k - 17) / 2 + 99 * k - 1782 + 576
//           = 50 * k^2 - 1750 * k + 15300 + 99 * k - 1206
//           = 50 * k^2 - 1651 * k + 14094
//
//

// S(9*m) will sum all 9 values in each stanza.
//      S(9) = T(9) = 45
//      S(18) = S(9) + 19 + 29 + 39 + 49 + 59 + 69 + 79 + 89 + 99
//            = T(9) + 10 * (2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10) - (1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1)
//            = T(9) + 10 * (T(10) - T(1)) - 9
//            = T(9) + 10 * 54 - 9
//      S(27) = S(18) + 199 + 299 + 399 + 499 + 599 + 699 + 799 + 899 + 999
//            = S(18) + 100 * (2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10) - (1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1)
//            = S(18) + 100 * (T(10) - T(1)) - 9
//            = S(18) + 100 * 54 - 9
// Recurrence:
//      S(9 * m) = S(9 * m - 9) + 10^(m - 1) * 54 - 9
//
//      S(9 * m + 9) = S(9 * m) + 10^m * 54 - 9
//
//      S(9 * m + 9) = S(9 * m - 9) + 10^(m - 1) * 54 - 9 + 10^m * 54 - 9
//                   = S(9 * m - 9) + (10^m + 10^(m-1)) * 54 - 18
//
//      S(9 * (m + k)) = S(9 * m) + 54 * (sum{i = m..m+k-1} 10^i) - 9*k
//      S(9 * (k + 1)) = S(9) + 54 * (sum{i = 1..k} 10^i) - 9*k
//
// Get sneaky:
//      S(9) = T(9) = 45 = 54 - 9
//
// Substitute in:
//      S(9 * (k + 1) = 54 - 9 + 54 * (sum{i = 1..k} 10^i) - 9*k
//      S(9 * (k + 1) = 54 * (sum{i = 0..k} 10^i) - 9 * (k + 1)
//
//      S(9 * k) = 54 * (sum{i = 0..k-1} 10^i) - 9*k
// 
// Examples:
//      k = 0:
//          S(9) = 54 * (sum{i = 0..0} 10^i) - 9
//               = 54 * 1 - 9
//               = 45
//      k = 1:
//          S(18) = 54 * (sum{i = 0..1} 10^i) - 18
//                = 54 * (1 + 10) - 18
//                = 576
//      k = 2:
//          S(27) = 54 * (sum{i = 0..2} 10^i) - 27
//                = 54 * (1 + 10 + 100) - 27
//                = 5967
//      k = 3:
//          S(36) = 54 * (sum{i = 0..3} 10^i) - 36
//                = 54 * (1 + 10 + 100 + 1000) - 36
//                = 59958

// What about S(9 * p + q) for q < 9?
//      S(9 * p + q) = S(9 * p) + sum {i = 1..q} s(9 * p + i)
//
// Recall:
//      num_nines = floor((n - 1) / 9) = p
//      s(n) = 10^num_nines * (n - 9 * num_nines) + 10^num_nines - 1
//      s(9 * p + q) = 10^p * (9 * p + q - 9 * p) + 10^p - 1
//                   = 10^p * q + 10^p - 1
//
//      S(9 * p + q) = S(9 * p) + sum {i = 1..q} (10^p * i + 10^p - 1)
//                   = S(9 * p) + 10^p * sum {i = 1..q} (i) + (10^p - 1) * q
//                   = S(9 * p) + 10^p * T(q) + (10^p - 1) * q
//                   = S(9 * p) + 10^p * q * (q + 1) / 2 + 10^p * q - q
//                   = S(9 * p) + 10^p * q * (q + 3) / 2 - q

// Can S(f_i) be efficiently calculated from S(f_i-1) and S(f_i-2)?
// Define:
//      f_i-2 = 9 * p2 + q2
//      f_i-1 = 9 * p1 + q1
//      f_i   = 9 * p  + q
//
// Looking at the values of p over f_i, it appears that one of these two relations is true:
//      p = p1 + p2
//      p = p1 + p2 + 1
//
// If p = p1 + p2:
//      S(9 * p2) = 54 * (sum{i = 0..p2-1} 10^i) - 9*p2
//      S(9 * p1) = 54 * (sum{i = 0..p1-1} 10^i) - 9*p1
//      S(9 * p) = 54 * (sum{i = 0..p-1} 10^i) - 9*p
// Example:
//      i = 11, f_i = 89, p = 9, p1 = 6, p2 = 3
//      S(9 * p2) = 54 * 111 - 27
//      S(9 * p1) = 54 * 111111 - 54
//      S(9 * p)  = 54 * 111111111 - 81
//      S(9 * p)  = 54 * (111111 * 1000 + 111) - 54 - 27
// We can't just say S(9 - p) = 1000 * S(9 * p1) + S(9 * p2), bacause that turns the
// (- 54) into (- 54000).  But the repunit modulus is the expensize part, maybe this can
// be optimized.
// Define the repunit modulus function RM:
//      RM(k) = (sum{i = 0..k-1} 10^i) mod 1,000,000,007
// Then we can define:
//      RM(a) = (sum{i = 0..a-1} 10^i) mod 1,000,000,007
//      RM(b) = (sum{i = 0..b-1} 10^i) mod 1,000,000,007
//      RM(a + b) = (sum{i = 0..a+b-1} 10^i) mod 1,000,000,007
//      RM(a + b) = (10^b * RM(a) + RM(b)) mod 1,000,000,007
// We can use the sum formula for RM to speed up RM(a + b) by avoiding a digits
// Then:
//      S(9 * p) = 54 * RM(p) - 9*p
// 
// else p = p1 + p2 + 1:
// In this case, a = p1 and b = p2 + 1, so we need one extra step:
//      RM(a + b + 1) = (10^(b+1) * RM(a) + 10 * RM(b) + 1) mod 1,000,000,007
//      RM(a + b + 1) = (10 * (10^b * RM(a) + RM(b)) + 1) mod 1,000,000,007
//
// The higher Fibonacci numbers are large, which create repunits with correspondingly
// large numbers of digits.  We need to reduce multiplication by 10^b to a smaller
// value when b ~ 1E17.
// Modular arithmetic has the identity:
//      (a + m) mod m == a mod m
// Or in our case
//      10^b mod 1,000,000,007 == (10^b - k * 1,000,000,007) mod 1,000,000,007
// 
// So how to calculate k to reduce the difference to less than the modulus?
// Can we use the binary representation of b to reduce 10^b?
//
// Start with mod 107:
//      n       10^n mod 107
//      1             10
//      2            100
//      4             49
//      8             47
//     16             69
// We know 100^2 mod 107 = 49, because 10,000 mod 107 = 49.
// Is 49^2 mod 107 == 47?  Yes!
// Is 47^2 mod 107 == 69?  Yes!
// Is 1E12 mod 107 == 49 * 47 mod 107?  Yes, both == 56
// 
// Do again with mod 1007:
//      n       10^n mod 1007
//      1             10
//      2            100
//      4            937
//      8            872
//     16             99
// Is 937^2 mod 1007 == 872?  Yes!
// Is 872^2 mod 1007 == 99?   Yes!
// Is 1E12 mod 1007 == 937 * 872 mod 107?  Yes, both == 387
// 
// Let's build the table for 10^(2^n):
//      n       2^n     10^(2^n) mod 1,000,000,007
//  0                     1                    10
//  1                     2                   100
//  2                     4                 10000
//  3                     8             100000000
//  4                    16             930000007
//  5                    32             965700007
//  6                    64             991764577
//  7                   128             306810146
//  8                   256              29614061
//  9                   512             602772777
// 10                  1024             148946603
// 11                  2048             389943979
// 12                  4096             693958306
// 13                  8192              95342747
// 14                 16384             341874336
// 15                 32768             797294476
// 16                 65536              10365237
// 17                131072             137314103
// 18                262144             750708475
// 19                524288             491883155
// 20               1048576             479110773
// 21               2097152             197827612
// 22               4194304             795672203
// 23               8388608             195213459
// 24              16777216             307986623
// 25              33554432             282953816
// 26              67108864             428521829
// 27             134217728             644088549
// 28             268435456              48975009
// 29             536870912             489760224
// 30            1073741824             333474651
// 31            2147483648              81134407
// 32            4294967296             953162112
// 33            8589934592             392674509
// 34           17179869184             939038205
// 35           34359738368             277072817
// 36           68719476736             382930074
// 37          137438953472             547197396
// 38          274877906944              93205900
// 39          549755813888             733998627
// 40         1099511627776             666607269
// 41         2199023255552             973081632
// 42         4398046511104             907568439
// 43         8796093022208             703133466
// 44        17592186044416             548396487
// 45        35184372088832             848770241
// 46        70368744177664             964321669
// 47       140737488355328             793531643
// 48       281474976710656              34432201
// 49       562949953421312             457405369
// 50      1125899906842624             125488471
// 51      2251799813685248             243686349
// 52      4503599627370496             273268549
// 53      9007199254740992             349835508
// 54     18014398509481984             800923897
// 55     36028797018963968             295313021
// 56     72057594037927936             761677988
// 57    144115188075855872             342654673
// 58    288230376151711744             106851361
// 59    576460752303423488             267631830
// 60   1152921504606846976             927761335
// 61   2305843009213693952             695794609
// 62   4611686018427387904             524551943
// 63   9223372036854775808             978992083
// 
//


constexpr int64_t kMod = 1'000'000'007;


std::vector<int64_t> repunit_mods(int64_t max_digits) {
    std::vector<int64_t> ret;
    ret.reserve(max_digits);

    ret.push_back(0);

    for (int n = 0; n < max_digits; ++n)
        ret.push_back((10 * ret.back() + 1) % kMod);

    return ret;
}


int64_t repunit_mod(int64_t num_digits) {
    int64_t digits_left = num_digits;

    int64_t ret = 1;

    for (; digits_left > 9; digits_left -= 9) {
        ret *= 1'000'000'000;
        ret +=   111'111'111;
        ret %= kMod;
    }

    for (; digits_left > 1; --digits_left) {
        ret *= 10;
        ++ret;
        ret %= kMod;
    }

    return ret;
}


int64_t S(int64_t n) {
    const int64_t p = n / 9;
    const int64_t q = n % 9;

    if (n < 10) {
        return n * (n + 1) / 2;
    }

    // S(9 * p) = 54 * (sum{i = 0..p-1} 10^i) - 9*p
    int64_t ret = 54 * repunit_mod(p) - 9 * p;
    while (ret < 0)
        ret += kMod;
    ret %= kMod;

    // S(9 * p + q) = S(9 * p) + 10^p * q * (q + 3) / 2 - q
    if (q > 0) {
        int64_t delta = q * (q + 3) / 2;

        int64_t digits_left = p;

        for (; digits_left > 9; digits_left -= 9) {
            delta *= 1'000'000'000;
            delta %= kMod;
        }

        for (; digits_left > 0; --digits_left) {
            delta *= 10;
        }
        delta %= kMod;

        ret += delta - q;
        if (ret < 0)
            ret += kMod;
        ret %= kMod;
    }

    return ret;
}


int64_t solve_it() {
    std::cout << "solve_it" << std::endl;
    constexpr size_t kMax{ 50 };

    std::vector<int64_t> fib{0, 1};
    fib.reserve(kMax + 1);
    for (size_t i = 2; i <= kMax; ++i)
        fib.push_back(*fib.crbegin() + *(fib.crbegin() + 1));

    std::vector<int64_t> S_val{ 0, 0 };
    S_val.reserve(kMax + 1);
    for (size_t i = 2; i <= kMax; ++i) {
        S_val.push_back(S(fib[i]));
        std::cout << i << "\t" << fib[i] << "\t" << S_val.back() << std::endl;
    }

    //for (size_t i = 0; i < fib.size(); ++i)
    //    std::cout << i << "\t" << fib[i] << "\t" << S_val[i] << std::endl;

    int64_t sum{ 0 };
    for (size_t i = 2; i <= kMax; ++i) {
        sum += S_val[i];
        sum %= kMod;
    }

    return sum;
}


// This helper returns an array containing the values of
//      10^(2^n) mod 1,000,000,007
// in the n'th position.
struct BinaryPow10Mod {
    int64_t pow2;
    int64_t pow10Mod;
};
std::vector<BinaryPow10Mod> getPow10Mods() {
    std::vector<BinaryPow10Mod> ret;
    ret.reserve(64);

    ret.push_back({ 1, 10 });  // 10^(2^0) = 10

    for (size_t n = 1; n < 64; ++n) {
        const auto& last = ret.back();
        ret.push_back({ 2 * last.pow2, (last.pow10Mod * last.pow10Mod) % kMod });
    }

    return ret;
}


int64_t solve_it2() {
    std::cout << "solve_it2" << std::endl;
    constexpr size_t kMax{ 90 };

    constexpr std::array<int64_t, 10> pow10{ 1, 10, 100, 1'000, 10'000, 100'000, 1'000'000, 10'000'000, 100'000'000, 1'000'000'000 };
    const auto pow10Mods = getPow10Mods();

    // Need to be more efficient in the repunit calculations to speed this up
    std::vector<int64_t> fib{ 0, 1 };
    fib.reserve(kMax + 1);
    for (size_t i = 2; i <= kMax; ++i)
        fib.push_back(*fib.crbegin() + *(fib.crbegin() + 1));

    struct rep_mod {
        int64_t p;
        int64_t rep_mod_val;
    };

    std::vector<rep_mod> rep_mods;
    rep_mods.reserve(kMax + 1);
    rep_mods.push_back({ 0, 0 });  //  f_0 = 0
    rep_mods.push_back({ 0, 1 });  //  f_1 = 1

    for (size_t i = 2; i <= kMax; ++i) {
        if (fib[i] < 9) {
            rep_mods.push_back({ 0, 0 });
            continue;
        }

        const auto& mods_minus_2 = rep_mods[i - 2];
        const auto& mods_minus_1 = rep_mods[i - 1];
        const int64_t fib_minus_2 = fib[i - 2];
        const int64_t fib_minus_1 = fib[i - 1];
        const int64_t curr_fib = fib[i];

        const int64_t p = curr_fib / 9;
        const int64_t q = curr_fib % 9;

        const int64_t a = mods_minus_1.p;
        const int64_t b = mods_minus_2.p;

        int64_t rep_mod_val = mods_minus_1.rep_mod_val;

        // Multiply RM(a) * 10^b
        // Use the bits of b to know which powers of 10 to multiply by.
        //int64_t digits_left = b;
        //for (; digits_left > 9; digits_left -= 9) {
        //    rep_mod_val *= pow10[9];
        //    rep_mod_val %= kMod;
        //}
        //rep_mod_val *= pow10[digits_left];
        //rep_mod_val %= kMod;
        for (size_t i = 0; i < 63; ++i) {
            if (pow10Mods[i].pow2 & b) {
                rep_mod_val *= pow10Mods[i].pow10Mod;
                rep_mod_val %= kMod;
            }
        }

        // Add RM(b)
        rep_mod_val += mods_minus_2.rep_mod_val;
        rep_mod_val %= kMod;

        if (p > a + b) {
            rep_mod_val *= 10;
            rep_mod_val += 1;
            rep_mod_val %= kMod;
        }

        rep_mods.push_back({ p, rep_mod_val });
        std::cout << i << "  ";
    }
    std::cout << std::endl;

    std::vector<int64_t> S_val{ 0, 0 };
    S_val.reserve(kMax + 1);
    for (size_t i = 2; i <= kMax; ++i) {
        if (fib[i] < 10) {
            S_val.push_back(fib[i] * (fib[i] + 1) / 2);
        }
        else {
            int64_t value = 54 * rep_mods[i].rep_mod_val - 9 * rep_mods[i].p;
            while (value < 0)
                value += kMod;
            value %= kMod;

            // If f_i is not a multiple of 9, add the extra terms
            const int64_t q = fib[i] % 9;
            // S(9 * p + q) = S(9 * p) + 10^p * q * (q + 3) / 2 - q
            if (q > 0) {
                int64_t delta = q * (q + 3) / 2;

                int64_t digits_left = fib[i] / 9;

                //for (; digits_left > 9; digits_left -= 9) {
                //    delta *= pow10[9];
                //    delta %= kMod;
                //}
                //delta *= pow10[digits_left];
                //delta %= kMod;
                for (size_t i = 0; i < 63; ++i) {
                    if (pow10Mods[i].pow2 & digits_left) {
                        delta *= pow10Mods[i].pow10Mod;
                        delta %= kMod;
                    }
                }

                value += delta - q;
                if (value < 0)
                    value += kMod;
                value %= kMod;
            }

            S_val.push_back(value);
        }

        std::cout << i << "\t" << fib[i] << "\t" << S_val.back() << std::endl;
    }

    //for (size_t i = 0; i < fib.size(); ++i)
    //    std::cout << i << "\t" << fib[i] << "\t" << S_val[i] << std::endl;

    int64_t sum{ 0 };
    for (size_t i = 2; i <= kMax; ++i) {
        sum += S_val[i];
        sum %= kMod;
    }

    return sum;
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    auto mods = repunit_mods(100);
    //    for (int i = 0; i < mods.size(); ++i)
    //        std::cout << i << "\t" << mods[i] << "\t" << repunit_mod(i) << std::endl;
    //}

    //{
    //    for (int n = 1; n < 25; ++n)
    //        std::cout << n << "\t" << repunit_mod(n) << std::endl;
    //}

    //{
    //    for (auto& n : { 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987,
    //                     1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025 }) {
    //        std::cout << n << "\t" << repunit_mod(n) << std::endl;
    //    }
    //}

    //{
    //    for (int n = 1; n < 37; ++n)
    //        std::cout << n << "\t" << S(n) << std::endl;
    //}

    //{
    //    auto mods = getPow10Mods();
    //    for (int i = 0; i < mods.size(); ++i)
    //        std::cout << i << "\t" << mods[i].pow2 << "\t" << mods[i].pow10Mod << std::endl;
    //}


    {
//        std::cout << solve_it() << std::endl;
        std::cout << solve_it2() << std::endl;
    }
}


//  i                f_i          S(f_i) mod 1,000,000,007
//  2                        1              1
//  3                        2              3
//  4                        3              6
//  5                        5             15
//  6                        8             36
//  7                       13            181
//  8                       21           1473
//  9                       34          40960
// 10                       55        7999939
// 11                       89      999999562
// 12                      144      579999857
// 13                      233      499999593
// 14                      377        4999533
// 15                      610      347371750
// 16                      987      624743368
// 17                     1597      454779723
// 18                     2584      171845680
// 19                     4181       77574818
// 20                     6765      337686523
// 21                    10946      712848938
// 22                    17711      698257996
// 23                    28657      650799321
// 24                    46368      307966277
// 25                    75025      215522108
// 26                   121393      810971768
// 27                   196418      476041083
// 28                   317811      129154418
// 29                   514229      173746594
// 30                   832040      876815299
// 31                  1346269      820003290
// 32                  2178309      746796906
// 33                  3524578      593059951
// 34                  5702887      962624661
// 35                  9227465      363783616
// 36                 14930352      529251569
// 37                 24157817      258362748
// 38                 39088169      738865531
// 39                 63245986      470270000
// 40                102334155      928315937
// 41                165580141      954935615
// 42                267914296      620792004
// 43                433494437      428814067
// 44                701408733      987403791
// 45               1134903170      739415870
// 46               1836311903      921929447
// 47               2971215073      530878979
// 48               4807526976      956490830
// 49               7778742049      272663731
// 50              12586269025      294520721
// 51              20365011074      618951506
// 52              32951280099      719770178
// 53              53316291173      521531032
// 54              86267571272      522582276
// 55             139583862445      568756978
// 56             225851433717      122998063
// 57             365435296162      652181637
// 58             591286729879      675498212
// 59             956722026041      17649379
// 60            1548008755920      991591591
// 61            2504730781961      474170157
// 62            4052739537881      285046153
// 63            6557470319842      165147115
// 64           10610209857723      985639248
// 65           17167680177565      210485532
// 66           27777890035288      765913784
// 67           44945570212853      135199856
// 68           72723460248141      276522427
// 69          117669030460994       83884372
// 70          190392490709135      862742608
// 71          308061521170129      241296933
// 72          498454011879264      772152761
// 73          806515533049393       95631801
// 74         1304969544928657      203824574
// 75         2111485077978050      900666528
// 76         3416454622906707      482355345
// 77         5527939700884757      409310639
// 78         8944394323791464      469465780
// 79        14472334024676221      346974231
// 80        23416728348467685       44671182
// 81        37889062373143906      243142581
// 82        61305790721611591      419364614
// 83        99194853094755497      545936777
// 84       160500643816367088      590242119
// 85       259695496911122585      153955037
// 86       420196140727489673      281378872
// 87       679891637638612258       81617620
// 88      1100087778366101931      205015327
// 89      1779979416004714189      258546150
// 90      2880067194370816120      241901414
