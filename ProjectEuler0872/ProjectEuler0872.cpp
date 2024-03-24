// A sequence of rooted trees T_N is constructed such that has nodes numbered 1 to n.
//
// The sequence starts at T_1, a tree with a single node as a root with the number 1.
//
// For n>1, T_n is constructed from T_n_1 using the following procedure:
//      1. Trace a path from the root of T_n-1 to a leaf by following the largest-
//         numbered child at each node.
//      2. Remove all edges along the traced path, disconnecting all nodes along it
//         from their parents.
//      3. Connect all orphaned nodes directly to a new node numbered n, which becomes
//         the root of T_n.
//
// For example, the following figure shows T_6 and T_7.
// The path traced through T_6 during the construction of T_7 is coloured red.
//             6                     7
//            /|\                   /|\
//           5 4 2        =>       6 5 3
//          / \                   /| |
//         3   1                 4 2 1
//
// Let f(n, k) be the sum of the node numbers along the path connecting the root of T_n
// to the node k, including the root and the node k.
// For example,
//      f(6, 1) = 6 + 5 + 1 = 12
//      f(10, 3) = 29
//
// Find f(10^17, 9^17).


#include <iostream>


// First, 10^17 can fit in an int64_t, so no need for BigInt :)

// Let's construct the first few trees, look for a pattern:
//      1
//
//      2
//      |
//      1
//
//      3       4
//     / \     / \
//    2   1   3   2
//            |
//            1
//
//      5         6         7         8
//     /|\       /|\       /|\       /|\
//    4 3 1     5 4 2     6 5 3     7 6 4
//    |        / \       /| |      /| |
//    2       3   1     4 2 1     5 3 2
//                                |
//                                1
//
//        __9__         __10__          __11__          __12__          __13__          __14__          ___15__           ___16___
//       / / \ \       /  | \ \        /  | \ \        /  | \ \        /  | \ \        /  | \ \        /   | \ \         /   | \  \
//      8 7   5 1     9   8  6 2      10  9 7  3      11 10  8 4      12  11 9 5      13 12 10 6      14  13 11 7       15   14 12 8
//     /| |          /|\  |          /|\  |\         /|\  |\         /|\  |\  \      /|\  |\  \      / |\  |\  \       / |\  | \  \
//    6 4 3         7 5 1 4         8 6 2 5 1       9 7 3 6 2      10 8 4 7 3  1   11 9 5 8 4  2   12 10 6 9 5  3    13 11 7 10 6  4
//    |             |               |              /|              /|              /| |            /|  |   |         /|  |   |
//    2             3               4             5 1             6 2             7 3 1           8 4  2   1        9 5  3   2
//                                                                                                                  |
//                                                                                                                  1
//
//              ____17_____               __10001___        _17_____          __10001___
//             /   |  |  \ \             /  |   |   \       |  |  \ \        |  |    \  \
//            16   15 13  9 1         1111 1101 1001 1      2  4   8 16      10 100 1000 10000
//           / |\  | \  \              |  \    \            |\  \            | \   \
//         14 12 8 11 7  5            1011 111  101         6 10 12        110 1010 1100
//         /|  |   |                   |                    |                |
//       10 6  4   3                   11                   14             1110
//        |
//        2
// The trees are groups by powers of 2.  From this layout we can see a number of patterns:
//  - For a T_n with n in (2^k, 2^k+1], the root n will have k children.
//  - If n is even, then the subtree rooted by n-1 will contain only odd numbers,
//    and the rest of T_n will contain only even numbers
//  - If n is odd, then the subtree rooted by n-1 will contain only even numbers,
//    and the rest of T_n will contain only odd numbers
//  - The tree for T_2^k+1 can be constructed from T_2^k:
//    - First take T_2^k and double every number in it
//    - The copy that tree and subtract 1 from each value
//    - Stick the copy as a child of the root with the largest value
//    - For example, note that in T_16 the subtree under 15 is the same a T_8 doubled minus 1
//



// To calculate f(n, k), we will decimate the tree one level at a time.
// If k == n, then f(n, k) = n.
// If n + k is odd, then we know k is in the left-most subtree, aka T_n-1.  f(n, k) = n + f(n - 1, k).  (Rule 1)
// If n + k is even, then we know k is in one of the right subtrees.
//   If n is even then f(n, k) = 2 * f(n/2, k/2)                                                        (Rule 2)
//   If n is odd, then f(n, k) = 2 * f((n + 1)/2, (k + 1)/2) - path_len(n, k)                           (Rule 3)
// The last case is the tricky one, as it involves the length of the path from n to k.
// To understand, lets look at f(17, 7) and f(17, 3) as examples.
// In both cases, both n & k are odd, so we go into the last case.
// Compare the values of the full path and the recursion call:
//      f(17, 7) = 17 + 15 + 7 = 39         f(9, 4) = 9 + 8 + 4 = 21
//      f(17, 3) = 17 + 15 + 11 + 3 = 46    f(9, 2) = 9 + 8 + 6 + 2 = 25
// We can see how the path length comes into play:
//      39 = 2 * 21 - 3
//      46 = 2 * 25 - 4

// How to measure path length?  Let's look at some of the T_n for odd n with odd k.
// Instead of looking at the values, let's replace each k by n-k:
//       9              9               _9__
//      /|\            /|\             / |  \
//     7 5 1    =>    2 4 8   ==   0010 0100 1000
//     |              |              |
//     3              6            0110
//
//       11_           11_              _11__
//      / | \         / | \            / |   \
//     9  7  3  =>   2  4  8  ==   0010 0100 1000
//    / \           / \            /  \
//   5   1         6  10         0110 1010
//
//      13_            13_              _13__
//     / \ \          / | \            / |   \
//    11  9 5   =>   2  4  8  ==   0010 0100 1000
//    /\  |         / \  \         /  \    \
//   7  3 1        6  10  12     0110 1010 1100
//
//      15_            15_              _15__
//     / \ \          / | \            / |   \
//    13 11 7   =>   2  4  8  ==   0010 0100 1000
//    /\  \         / \  \         /  \    \
//   9  5  3       6  10 12      0110 1010 1100
//   |             |               |
//   1            14             1110
//
//   _17____          __17___              ____17______
//  /  |  \ \        /  |  \ \            /  |    \    \
//  15 13  9 1  =>   2  4   8 16  ==    0010 0100 1000 10000
//  | \  \           |\  \             /  \   \
//  11 7  5          6 10 12         0110 1010 1100
//  |                |                 |
//  3                14              1110
//
// The path length from n to k is the number of 1 bits in n - k.


int64_t num1Bits(int64_t value) {
    int64_t count{ 0 };
    while (value) {
        count += value & 1;
        value >>= 1;
    }
    return count;
}


int64_t ff(int64_t n, int64_t k, int64_t& path_len) {
    std::cout << "in f(" << n << ", " << k << ")";

    if (k == n) {
        path_len = 1;
        return n;
    }

    if (n < 5) {
        switch (n) {
        case 2:
            path_len = 2;
            return 3;

        case 3:
            path_len = 2;
            return k == 1 ? 4 : 5;

        case 4: {
            switch (k) {
            case 1:
                path_len = 3;
                return 8;

            case 2:
                path_len = 2;
                return 6;

            case 3:
                path_len = 2
                    ;
                return 7;
            }  // switch k
        }  // case 4
        }  // switch n
    }

    if ((n + k) % 2 == 1) {
        // n and k are not both even or odd, use the left subtree.
        std::cout << ", Rule 1" << std::endl;
        int64_t subpath = ff(n - 1, k, path_len) + n;
        ++path_len;
        return subpath;
    }

    if (n % 2 == 0) {
        // n and k are both even, use right subtree and double it
        std::cout << ", Rule 2" << std::endl;
        int64_t subpath = ff(n / 2, k / 2, path_len) * int64_t{ 2 };
        // path_len stays the same
        return subpath;
    }

    // n and k are both odd, use right subtree with path length
    std::cout << ", Rule 3, path_len = " << num1Bits(n - k) + 1 << std::endl;
    int64_t subpath_len{ 0 };
    int64_t subpath = ff((n + 1) / 2, (k + 1) / 2, subpath_len) * int64_t{ 2 };
    ++path_len;
    return subpath - num1Bits(n - k) - int64_t{ 1 };
}


int64_t f(int64_t n, int64_t k) {
    int64_t len{ 0 };
    int64_t ret = ff(n, k, len);
    std::cout << "Path length = " << len << std::endl;
    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    {
        std::cout << f(3, 1) << std::endl;
        std::cout << f(5, 1) << std::endl;
        std::cout << f(6, 1) << std::endl;
        std::cout << f(10, 3) << std::endl;
    }

    {
        int64_t n = 1;
        int64_t k = 1;
        for (int i = 0; i < 17; ++i) {
            n *= 10;
            k *= 9;
        }
        std::cout << f(n, k) << std::endl;
    }
}


// We want  f(100'000'000'000'000'000, 16'677'181'699'666'569).
// Level  1 - 100'000'000'000'000'000, 16'677'181'699'666'569 - n + k is odd, Rule 1, f = n + f(n-1, k)
// Level  2 -  99'999'999'999'999'999, 16'677'181'699'666'569 - n & k both odd, Rule 3, f = 2 * f((n+1)/2, (k+1)/2) - 29
// Level  3 -  50'000'000'000'000'000,  8'338'590'849'833'285 - n + l is odd, Rule 1, f = n + f(n-1, k)
// Level  4 -  49'999'999'999'999'999,  8'338'590'849'833'285 - n & k both odd, Rule 3, f = 2 * f((n+1)/2, (k+1)/2) - 28
// Level  5 -  25'000'000'000'000'000,  4'169'295'424'916'643 - n + k is odd, Rule 1, f = n + f(n-1, k)
// Level  6 -  24'999'999'999'999'999,  4'169'295'424'916'643 - n & k both odd, Rule 3, f = 2 * f((n+1)/2, (k+1)/2) - 27
// Level  7 -  12'500'000'000'000'000,  2'084'647'712'458'322 - n & k both even, Rule 2, f = 2 * f(n/2, k/2)
// Level  8 -   6'250'000'000'000'000,  1'042'323'856'229'161 - n + k is odd, Rule 1, f = n + f(n-1, k)
// Level  9 -   6'249'999'999'999'999,  1'042'323'856'229'161 - n & k both odd, Rule 3, f = 2 * f((n+1)/2, (k+1)/2) - 26
// Level 10 -   3'125'000'000'000'000,    521'161'928'114'581 - n + k is odd, Rule 1, f = n + f(n-1, k)
// Level 11 -   3'124'999'999'999'999,    521'161'928'114'581 - n & k both odd, Rule 3, f = 2 * f((n+1)/2, (k+1)/2) - 25
// Level 12 -   1'562'500'000'000'000,    260'580'964'057'291 - n + k is odd, Rule 1, f = n + f(n-1, k)
// Level 13 -   1'562'499'999'999'999,    260'580'964'057'291 - n & k both odd, Rule 3, f = 2 * f((n+1)/2, (k+1)/2) - 24
// Level 14 -     781'250'000'000'000,    130'290'482'028'646 - n & k both even, Rule 2, f = 2 * f(n/2, k/2)
// Level 15 -     390'625'000'000'000,     65'145'241'014'323 - n + k is odd, Rule 1, f = n + f(n-1, k)
// Level 16 -     390'624'999'999'999,     65'145'241'014'323 - n & k both odd, Rule 3, f = 2 * f((n+1)/2, (k+1)/2) - 23
// Level 17 -     195'312'500'000'000,     32'572'620'507'162 - n & k both even, Rule 2, f = 2 * f(n/2, k/2)
// Level 18 -      97'656'250'000'000,     16'286'310'253'581 - n + k is odd, Rule 1, f = n + f(n-1, k)
// Level 19 -      97'656'249'999'999,     16'286'310'253'581 - n & k both odd, Rule 3, f = 2 * f((n+1)/2, (k+1)/2) - 22
// Level 20 -      48'828'125'000'000,      8'143'155'126'791 - n + k is odd, Rule 1, f = n + f(n-1, k)
// Level 21 -      48'828'124'999'999,      8'143'155'126'791 - n & k both odd, Rule 3, f = 2 * f((n+1)/2, (k+1)/2) - 21
// Level 22 -      24'414'062'500'000,      4'071'577'563'396 - n & k both even, Rule 2, f = 2 * f(n/2, k/2)
// Level 23 -      12'207'031'250'000,      2'035'788'781'698 - n & k both even, Rule 2, f = 2 * f(n/2, k/2)
// Level 24 -       6'103'515'625'000,      1'017'894'390'849 - n + k is odd, Rule 1, f = n + f(n-1, k)
// Level 25 -       6'103'515'624'999,      1'017'894'390'849 - n & k both odd, Rule 3, f = 2 * f((n+1)/2, (k+1)/2) - 20
// Level 26 -       3'051'757'812'500,        508'947'195'425 - n + k is odd, Rule 1, f = n + f(n-1, k)
// Level 27 -       3'051'757'812'499,        508'947'195'425 - n & k both odd, Rule 3, f = 2 * f((n+1)/2, (k+1)/2) - 19
// Level 28 -       1'525'878'906'250,        254'473'597'713 - n + k is odd, Rule 1, f = n + f(n-1, k)
// Level 29 -       1'525'878'906'249,        254'473'597'713 - n & k both odd, Rule 3, f = 2 * f((n+1)/2, (k+1)/2) - 18
// Level 30 -         762'939'453'125,        127'236'798'857 - n & k both odd, Rule 3, f = 2 * f((n+1)/2, (k+1)/2) - 18
// Level 31 -         381'469'726'563,         63'618'399'429 - n & k both odd, Rule 3, f = 2 * f((n+1)/2, (k+1)/2) - 18
// Level 32 -         190'734'863'282,         31'809'199'715 - n + k is odd, Rule 1, f = n + f(n-1, k)
// Level 33 -         190'734'863'281,         31'809'199'715 - n & k both odd, Rule 3, f = 2 * f((n+1)/2, (k+1)/2) - 17
// Level 34 -          95'367'431'641,         15'904'599'858 - n + k is odd, Rule 1, f = n + f(n-1, k)
// Level 35 -          95'367'431'640,         15'904'599'858 - n & k both even, Rule 2, f = 2 * f(n/2, k/2)
// Level 36 -          47'683'715'820,          7'952'299'929 - n + k is odd, Rule 1, f = n + f(n-1, k)
// Level 37 -          47'683'715'819,          7'952'299'929 - n & k both odd, Rule 3, f = 2 * f((n+1)/2, (k+1)/2) - 15
// Level 38 -          23'841'857'910,          3'976'149'965 - n + k is odd, Rule 1, f = n + f(n-1, k)
// Level 39 -          23'841'857'909,          3'976'149'965 - n & k both odd, Rule 3, f = 2 * f((n+1)/2, (k+1)/2) - 14
// Level 40 -          11'920'928'955,          1'988'074'983 - n & k both odd, Rule 3, f = 2 * f((n+1)/2, (k+1)/2) - 14
// Level 41 -           5'960'464'478,            994'037'492 - n & k both even, Rule 2, f = 2 * f(n/2, k/2)
// Level 42 -           2'980'232'239,            497'018'746 - n + k is odd, Rule 1, f = n + f(n-1, k)
// Level 43 -           2'980'232'238,            497'018'746 - n & k both even, Rule 2, f = 2 * f(n/2, k/2)
// Level 44 -           1'490'116'119,            248'509'373 - n & k both odd, Rule 3, f = 2 * f((n+1)/2, (k+1)/2) - 13
// Level 45 -             745'058'060,            124'254'687 - n + k is odd, Rule 1, f = n + f(n-1, k)
// Level 46 -             745'058'059,            124'254'687 - n & k both odd, Rule 3, f = 2 * f((n+1)/2, (k+1)/2) - 12
// Level 47 -             372'529'030,             62'127'344 - n & k both even, Rule 2, f = 2 * f(n/2, k/2)
// Level 48 -             186'264'515,             31'063'672 - n + k is odd, Rule 1, f = n + f(n-1, k)
// Level 49 -             186'264'514,             31'063'672 - n & k both even, Rule 2, f = 2 * f(n/2, k/2)
// Level 50 -              93'132'257,             15'531'836 - n + k is odd, Rule 1, f = n + f(n-1, k)
// Level 51 -              93'132'256,             15'531'836 - n & k both even, Rule 2, f = 2 * f(n/2, k/2)
// Level 52 -              46'566'128,              7'765'918 - n & k both even, Rule 2, f = 2 * f(n/2, k/2)
// Level 53 -              23'283'064,              3'882'959 - n + k is odd, Rule 1, f = n + f(n-1, k)
// Level 54 -              23'283'063,              3'882'959 - n & k both odd, Rule 3, f = 2 * f((n+1)/2, (k+1)/2) - 9
// Level 55 -              11'641'532,              1'941'480 - n & k both even, Rule 2, f = 2 * f(n/2, k/2)
// Level 56 -               5'820'766,                970'740 - n & k both even, Rule 2, f = 2 * f(n/2, k/2)
// Level 57 -               2'910'383,                485'370 - n + k is odd, Rule 1, f = n + f(n-1, k)
// Level 58 -               2'910'382,                485'370 - n & k both even, Rule 2, f = 2 * f(n/2, k/2)
// Level 59 -               1'455'191,                242'685 - n & k both odd, Rule 3, f = 2 * f((n+1)/2, (k+1)/2) - 8
// Level 60 -                 727'596,                121'343 - n + k is odd, Rule 1, f = n + f(n-1, k)
// Level 61 -                 727'595,                121'343 - n & k both odd, Rule 3, f = 2 * f((n+1)/2, (k+1)/2) - 7
// Level 62 -                 363'798,                 60'672 - n & k both even, Rule 2, f = 2 * f(n/2, k/2)
// Level 63 -                 181'899,                 30'336 - n + k is odd, Rule 1, f = n + f(n-1, k)
// Level 64 -                 181'898,                 30'336 - n & k both even, Rule 2, f = 2 * f(n/2, k/2)
// Level 65 -                  90'949,                 15'168 - n + k is odd, Rule 1, f = n + f(n-1, k)
// Level 66 -                  90'948,                 15'168 - n & k both even, Rule 2, f = 2 * f(n/2, k/2)
// Level 67 -                  45'474,                  7'584 - n & k both even, Rule 2, f = 2 * f(n/2, k/2)
// Level 68 -                  22'737,                  3'792 - n + k is odd, Rule 1, f = n + f(n-1, k)
// Level 69 -                  22'736,                  3'792 - n & k both even, Rule 2, f = 2 * f(n/2, k/2)
// Level 70 -                  11'368,                  1'896 - n & k both even, Rule 2, f = 2 * f(n/2, k/2)
// Level 71 -                   5'684,                    948 - n & k both even, Rule 2, f = 2 * f(n/2, k/2)
// Level 72 -                   2'842,                    474 - n & k both even, Rule 2, f = 2 * f(n/2, k/2)
// Level 73 -                   1'421,                    237 - n & k both odd, Rule 3, f = 2 * f((n+1)/2, (k+1)/2) - 4
// Level 74 -                     711,                    119 - n & k both odd, Rule 3, f = 2 * f((n+1)/2, (k+1)/2) - 4
// Level 75 -                     356,                     60 - n & k both even, Rule 2, f = 2 * f(n/2, k/2)
// Level 76 -                     178,                     30 - n & k both even, Rule 2, f = 2 * f(n/2, k/2)
// Level 77 -                      89,                     15 - n & k both odd, Rule 3, f = 2 * f((n+1)/2, (k+1)/2) - 4
// Level 78 -                      45,                      8 - n + k is odd, Rule 1, f = n + f(n-1, k)
// Level 79 -                      44,                      8 - n & k both even, Rule 2, f = 2 * f(n/2, k/2)
// Level 80 -                      22,                      4 - n & k both even, Rule 2, f = 2 * f(n/2, k/2)
// Level 81 -                      11,                      2 - n + k is odd, Rule 1, f = n + f(n-1, k)
// Level 82 -                      10,                      2 - n & k both even, Rule 2, f = 2 * f(n/2, k/2)
// Level 83 -                       5,                      1 - n & k both odd, Rule 3, f = 2 * f((n+1)/2, (k+1)/2) - 2
// Level 84 -                       3,                      1 - n & k both odd, Rule 3, f = 2 * f((n+1)/2, (k+1)/2) - 2
// Level 85 -                       2,                      1 - n + k is odd, Rule 1, f = n + f(n-1, k)
// Level 86 -                       1,                      1 - n == k, f = 1
//
// Unroll the stack:
// Level 86 - f(1, 1) = 1
// Level 85 - f(2, 1) = 2 + 1 = 3
// Level 84 - f(3, 1) = 2 * 3 - 2 = 4
// Level 83 - f(5, 1) = 2 * 4 - 2 = 6
// Level 82 - f(10, 2) = 2 * 6 = 12
// Level 81 - f(11, 2) = 11 + 12 = 23
// Level 80 - f(22, 4) = 2 * 23 = 46
// Level 79 - f(44, 8) = 2 * 46 = 92
// Level 78 - f(45, 8) = 45 + 92 = 137
// Level 77 - f(89, 15) = 2 * 137 - 4 = 270
// Level 76 - f(178, 30) = 2 * 270 = 540
// Level 75 - f(356, 60) = 2 * 540 = 1'080
// Level 74 - f(711, 119) = 2 * 1'080 - 4 = 2'156
// Level 73 - f(1'421, 237) = 2 * 2'156 - 4 = 4'308
// Level 72 - f(2'842, 474) = 2 * 4'308 = 8'616
// Level 71 - f(5'684, 948) = 2 * 8'616 = 17'232
// Level 70 - f(11'368, 1'896) = 2 * 17'232 = 34'464
// Level 69 - f(22'736, 3'792) = 2 * 34'464 = 68'928
// Level 68 - f(22'737, 3'792) = 22'737 + 68'928 = 91'665
// Level 67 - f(45'474, 7'584) = 2 * 91'665 = 183'330
// Level 66 - f(90'948, 15'168) = 2 * 183'330 = 366'660
// Level 65 - f(90'949, 15'168) = 90'949 + 366'660 = 457'609
// Level 64 - f(181'898, 30'336) = 2 * 457'609 = 915'218
// Level 63 - f(181'899, 30'336) = 181'899 + 915'218 = 1'097'117
// Level 62 - f(363'798, 60'672) = 2 * 1'097'117 = 2'194'234
// Level 61 - f(727'595, 121'343) = 2 * 2'194'234 - 7 = 4'388'461
// Level 60 - f(727'596, 121'343) = 727'596 + 4'388'461 = 5'116'057
// Level 59 - f(1'455'191, 242'685) = 2 * 5'116'057 - 8 = 10'232'106
// Level 58 - f(2'910'382, 485'370) = 2 * 10'232'106 = 20'464'212
// Level 57 - f(2'910'383, 485'370) = 2'910'383 + 20'464'212 = 23'374'595
// Level 56 - f(5'820'766, 970'740) = 2 * 23'374'595 = 46'749'190
// Level 55 - f(11'641'532, 1'941'480) = 2 * 46'749'190 = 93'498'380
// Level 54 - f(23'283'063, 3'882'959) = 2 * 93'498'380 - 9 = 186'996'751
// Level 53 - f(23'283'064, 3'882'959) = 23'283'064 + 186'996'751 = 210'279'815
// Level 52 - f(46'566'128, 7'765'918) = 2 * 210'279'815 = 420'559'630
// Level 51 - f(93'132'256, 15'531'836) = 2 * 420'559'630 = 841,119,260
// Level 50 - f(93'132'257, 15'531'836) = 93'132'257 + 841,119,260 = 934'251'517
// Level 49 - f(186'264'514, 31'063'672) = 2 * f934'251'517 = 1'868'503'034
// Level 48 - f(186'264'515, 31'063'672) = 186'264'515 + 1'868'503'034 = 2'054'767'549
// Level 47 - f(372'529'030, 62'127'344) = 2 * 2'054'767'549 = 4'109'535'098
// Level 46 - f(745'058'059, 124'254'687) = 2 * 4'109'535'098 - 12 = 8'219'070'184
// Level 45 - f(745'058'060, 124'254'687) = 745'058'060 + 8'219'070'184 = 8'964'128'244
// Level 44 - f(1'490'116'119, 248'509'373) = 2 * 8'964'128'244 - 13 = 17'928'256'475
// Level 43 - f(2'980'232'238, 497'018'746) = 2 * 17'928'256'475 = 35'856'512'950
// Level 42 - f(2'980'232'239, 497'018'746) = 2'980'232'239 + 35'856'512'950 = 38'836'745'189
// Level 41 - f(5'960'464'478, 994'037'492) = 2 * 38'836'745'189 = 77'673'490'378
// Level 40 - f(11'920'928'955, 1'988'074'983) = 2 * 77'673'490'378 - 14 = 155'346'980'742
// Level 39 - f(23'841'857'909, 3'976'149'965) = 2 * 155'346'980'742 - 14 = 310'693'961'470
// Level 38 - f(23'841'857'910, 3'976'149'965) = 23'841'857'910 + 310'693'961'470 = 334'535'819'380
// Level 37 - f(47'683'715'819, 7'952'299'929) = 2 * 334'535'819'380 - 15 = 669'071'638'745
// Level 36 - f(47'683'715'820, 7'952'299'929) = 47'683'715'820 + 669'071'638'745 = 716'755'354'565
// Level 35 - f(95'367'431'640, 15'904'599'858) = 2 * 716'755'354'565 = 1'433'510'709'130
// Level 34 - f(95'367'431'641, 15'904'599'858) = 95'367'431'641 + 1'433'510'709'130 = 1'528'878'140'771
// Level 33 - f(190'734'863'281, 31'809'199'715) = 2 * 1'528'878'140'771 - 17 = 3'057'756'281'525
// Level 32 - f(190'734'863'282, 31'809'199'715) = 190'734'863'282 + 3'057'756'281'525 = 3'248'491'144'807
// Level 31 - f(381'469'726'563, 63'618'399'429) = 2 * 3'248'491'144'807 - 18 = 6'496'982'289'596
// Level 30 - f(762'939'453'125, 127'236'798'857) = 2 * f((n+1)/2, 6'496'982'289'596 - 18 = 12'993'964'579'174
// Level 29 - f(1'525'878'906'249, 254'473'597'713) = 2 * 12'993'964'579'174 - 18 = 25'987'929'158'330
// Level 28 - f(1'525'878'906'250, 254'473'597'713) = 1'525'878'906'250 + 25'987'929'158'330 = 27'513'808'064'580
// Level 27 - f(3'051'757'812'499, 508'947'195'425) = 2 * 27'513'808'064'580 - 19 = 55'027'616'129'141
// Level 26 - f(3'051'757'812'500, 508'947'195'425) = 3'051'757'812'500 + 55'027'616'129'141 = 58'079'373'941'641
// Level 25 - f(6'103'515'624'999, 1'017'894'390'849) = 2 * 58'079'373'941'641 - 20 = 116'158'747'883'262
// Level 24 - f(6'103'515'625'000, 1'017'894'390'849) = 6'103'515'625'000 + 116'158'747'883'262 = 122'262'263'508'262
// Level 23 - f(12'207'031'250'000, 2'035'788'781'698) = 2 * 122'262'263'508'262 = 244'524'527'016'524
// Level 22 - f(24'414'062'500'000, 4'071'577'563'396) = 2 * 244'524'527'016'524 = 489'049'054'033'048
// Level 21 - f(48'828'124'999'999, 8'143'155'126'791) = 2 * 489'049'054'033'048 - 21 = 978'098'108'066'075
// Level 20 - f(48'828'125'000'000, 8'143'155'126'791) = 48'828'125'000'000 + 978'098'108'066'075 = 1'026'926'233'066'075
// Level 19 - f(97'656'249'999'999, 16'286'310'253'581) = 2 * 1'026'926'233'066'075 - 22 = 2'053'852'466'132'128
// Level 18 - f(97'656'250'000'000, 16'286'310'253'581) = 97'656'250'000'000 + 2'053'852'466'132'128 = 2'151'508'716'132'128
// Level 17 - f(195'312'500'000'000, 32'572'620'507'162) = 2 * 2'151'508'716'132'128 = 4'303'017'432'264'256
// Level 16 - f(390'624'999'999'999, 65'145'241'014'323) = 2 * 4'303'017'432'264'256 - 23 = 8'606'034'864'528'489
// Level 15 - f(390'625'000'000'000, 65'145'241'014'323) = 390'625'000'000'000 + 8'606'034'864'528'489 = 8'996'659'864'528'489
// Level 14 - f(781'250'000'000'000, 130'290'482'028'646) = 2 * 8'996'659'864'528'489 = 17'993'319'729'056'978
// Level 13 - f(1'562'499'999'999'999, 260'580'964'057'291) = 2 * 17'993'319'729'056'978 - 24 = 35'986'639'458'113'932
// Level 12 - f(1'562'500'000'000'000, 260'580'964'057'291) = 1'562'500'000'000'000 + 35'986'639'458'113'932 = 37'549'139'458'113'932
// Level 11 - f(3'124'999'999'999'999, 521'161'928'114'581) = 2 * 37'549'139'458'113'932 - 25 = 75'098'278'916'227'839
// Level 10 - f(3'125'000'000'000'000, 521'161'928'114'581) = 3'125'000'000'000'000 + 75'098'278'916'227'839 = 78'223'278'916'227'839
// Level  9 - f(6'249'999'999'999'999, 1'042'323'856'229'161) = 2 * 78'223'278'916'227'839 - 26 = 156'446'557'832'455'652
// Level  8 - f(6'250'000'000'000'000, 1'042'323'856'229'161) = 6'250'000'000'000'000 + 156'446'557'832'455'652 = 162'696'557'832'455'652
// Level  7 - f(12'500'000'000'000'000, 2'084'647'712'458'322) = 2 * 162'696'557'832'455'652 = 325'393'115'664'911'304
// Level  6 - f(24'999'999'999'999'999, 4'169'295'424'916'643) = 2 * 325'393'115'664'911'304 - 27 = 650'786'231'329'822'581
// Level  5 - f(25'000'000'000'000'000, 4'169'295'424'916'643) = 25'000'000'000'000'000 + 650'786'231'329'822'581 = 675'786'231'329'822'581
// Level  4 - f(49'999'999'999'999'999, 8'338'590'849'833'285) = 2 * 675'786'231'329'822'581 - 28 = 1'351'572'462'659'645'134
// Level  3 - f(50'000'000'000'000'000, 8'338'590'849'833'285) = 50'000'000'000'000'000 + 1'351'572'462'659'645'134 = 1'401'572'462'659'645'134
// Level  2 - f(99'999'999'999'999'999, 16'677'181'699'666'569) = 2 * 1'401'572'462'659'645'134 - 29 = 2'803'144'925'319'290'239
// Level  1 - f(100'000'000'000'000'000, 16'677'181'699'666'569) = 100'000'000'000'000'000 + 2'803'144'925'319'290'239 = 2'903'144'925'319'290'239
