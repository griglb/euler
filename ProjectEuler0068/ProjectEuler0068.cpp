// Consider the following "magic" 3-gon ring, filled with the numbers 1 to 6, and each line adding to nine.
//                      4
//                       \
//                        3
//                       / \
//                      1---2---6
//                     /
//                    5
// Working clockwise, and starting from the group of three with the numerically lowest external
// node (4,3,2 in this example), each solution can be described uniquely.
// For example, the above solution can be described by the set: 4,3,2; 6,2,1; 5,1,3.
// It is possible to complete the ring with four different totals: 9, 10, 11, and 12. There are eight solutions in total.
// Total	Solution Set
// 9	4,2,3; 5,3,1; 6,1,2
// 9	4,3,2; 6,2,1; 5,1,3
// 10	2,3,5; 4,5,1; 6,1,3
// 10	2,5,3; 6,3,1; 4,1,5
// 11	1,4,6; 3,6,2; 5,2,4
// 11	1,6,4; 5,4,2; 3,2,6
// 12	1,5,6; 2,6,4; 3,4,5
// 12	1,6,5; 3,5,4; 2,4,6
// By concatenating each group it is possible to form 9-digit strings; the maximum string for a 3-gon ring is 432621513.
// Using the numbers 1 to 10, and depending on arrangements, it is possible to form 16- and 17-digit strings.
// What is the maximum 16-digit string for a "magic" 5-gon ring?
//  (pentagonal instead of triangular, still lines of 3 numbers)


#include <iostream>

#include "big_int.h"
#include "combinatorics.h"


std::vector<BigInt> get_5_gon_ring() {
    // If we sort the vertices on x, then the 5 triplets are
    //    (0,1,5), (2,5,7), (8,7,6), (9,6,3), (4,3,1)
    // In order to get a 16 digit number, the 10 has to be in one of the outer leaf nodes,
    // which is only in one triplet not two.
    // Due to rotational symmetry, we will put the 10 in node 9 and test all
    // permutations of 1-9 in the other nodes. 

    std::vector<BigInt> ret;

    const auto perm_list = get_permutations(9, 9);
    for (const auto & perm : perm_list) {
        const uint64_t sum1 = perm[0] + perm[1] + perm[5];
        const uint64_t sum2 = perm[2] + perm[5] + perm[7];
        if (sum2 != sum1)
            continue;
        const uint64_t sum3 = perm[8] + perm[7] + perm[6];
        if (sum3 != sum1)
            continue;
        const uint64_t sum4 = 9 + perm[6] + perm[3];
        if (sum4 != sum1)
            continue;
        const uint64_t sum5 = perm[4] + perm[3] + perm[1];
        if (sum5 != sum1)
            continue;
        // If we get here, we have a valid 5-gon ring.
        // Now need to find smallest of the leaf nodes, to be the start of the 16 digit string
        // Need to add 1 to all values in perm, since it is 0-based not 1-based.
        BigInt::Digits digits;
        if ((perm[0] < perm[2]) && (perm[0] < perm[8]) && (perm[0] < perm[4])) {
            digits = BigInt::Digits{ static_cast<int8_t>(perm[0] + 1), static_cast<int8_t>(perm[1] + 1), static_cast<int8_t>(perm[5] + 1),
                                     static_cast<int8_t>(perm[2] + 1), static_cast<int8_t>(perm[5] + 1), static_cast<int8_t>(perm[7] + 1),
                                     static_cast<int8_t>(perm[8] + 1), static_cast<int8_t>(perm[7] + 1), static_cast<int8_t>(perm[6] + 1),
                                     1, 0, static_cast<int8_t>(perm[6] + 1), static_cast<int8_t>(perm[3] + 1),
                                     static_cast<int8_t>(perm[4] + 1), static_cast<int8_t>(perm[3] + 1), static_cast<int8_t>(perm[1] + 1) };
        }
        else if ((perm[2] < perm[8]) && (perm[2] < perm[4]) && (perm[2] < perm[0])) {
            digits = BigInt::Digits{ static_cast<int8_t>(perm[2] + 1), static_cast<int8_t>(perm[5] + 1), static_cast<int8_t>(perm[7] + 1),
                                     static_cast<int8_t>(perm[8] + 1), static_cast<int8_t>(perm[7] + 1), static_cast<int8_t>(perm[6] + 1),
                                     1, 0, static_cast<int8_t>(perm[6] + 1), static_cast<int8_t>(perm[3] + 1),
                                     static_cast<int8_t>(perm[4] + 1), static_cast<int8_t>(perm[3] + 1), static_cast<int8_t>(perm[1] + 1),
                                     static_cast<int8_t>(perm[0] + 1), static_cast<int8_t>(perm[1] + 1), static_cast<int8_t>(perm[5] + 1) };
        }
        else if ((perm[8] < perm[4]) && (perm[8] < perm[0]) && (perm[8] < perm[2])) {
            digits = BigInt::Digits{ static_cast<int8_t>(perm[8] + 1), static_cast<int8_t>(perm[7] + 1), static_cast<int8_t>(perm[6] + 1),
                                     1, 0, static_cast<int8_t>(perm[6] + 1), static_cast<int8_t>(perm[3] + 1),
                                     static_cast<int8_t>(perm[4] + 1), static_cast<int8_t>(perm[3] + 1), static_cast<int8_t>(perm[1] + 1),
                                     static_cast<int8_t>(perm[0] + 1), static_cast<int8_t>(perm[1] + 1), static_cast<int8_t>(perm[5] + 1),
                                     static_cast<int8_t>(perm[2] + 1), static_cast<int8_t>(perm[5] + 1), static_cast<int8_t>(perm[7] + 1) };
        }
        else if ((perm[4] < perm[0]) && (perm[4] < perm[2]) && (perm[4] < perm[8])) {
            digits = BigInt::Digits{ static_cast<int8_t>(perm[4] + 1), static_cast<int8_t>(perm[3] + 1), static_cast<int8_t>(perm[1] + 1),
                                     static_cast<int8_t>(perm[0] + 1), static_cast<int8_t>(perm[1] + 1), static_cast<int8_t>(perm[5] + 1),
                                     static_cast<int8_t>(perm[2] + 1), static_cast<int8_t>(perm[5] + 1), static_cast<int8_t>(perm[7] + 1),
                                     static_cast<int8_t>(perm[8] + 1), static_cast<int8_t>(perm[7] + 1), static_cast<int8_t>(perm[6] + 1),
                                     1, 0, static_cast<int8_t>(perm[6] + 1), static_cast<int8_t>(perm[3] + 1) };
        }
        else {
            throw "what!?";
        }
        BigInt tmp;
        tmp.set_digits(digits);
        ret.push_back(tmp);
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    const auto rings = get_5_gon_ring();
    BigInt max_ring;
    for (const auto& ring : rings) {
        std::cout << ring << std::endl;
        if (ring > max_ring) {
            std::cout << "\tnew max ring" << std::endl;
            max_ring = ring;
        }
    }
    std::cout << "max ring = " << max_ring << std::endl;
}

