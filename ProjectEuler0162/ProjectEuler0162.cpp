// In the hexadecimal number system numbers are represented using different digits:
//              0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F
//The hexadecimal number AF when written in the decimal number system equals 10 x 16 + 15 = 175.
//
// In the 3-digit hexadecimal numbers 10A, 1A0, A10, and A01 the digits 0, 1, and A
// are all present.
// Like numbers written in base ten we write hexadecimal numbers without leading zeroes.
//
// How many hexadecimal numbers containing at most sixteen hexadecimal digits exist
// with all of the digits 0, 1, and A present at least once?
// Give your answer as a hexadecimal number.
//
// (A, B, C, D, E and F in upper case, without any leading or trailing code that marks
// the number as hexadecimal and without leading zeroes, e.g. 1A3F and not: 1a3f and
// not 0x1a3f and not $1A3F and not #1A3F and not 0000001A3F)

#include <algorithm>
#include <iostream>
#include <list>
#include <numeric>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "big_int.h"
#include "combinatorics.h"
#include "gcd.h"

//  len   # strings
//   3          4
//   4        258
//   5       9928
//   6     299490
//   7    7823824

// Let's start with 4 digit numbers.  We can construct these from the 4 strings of
// length 3:
//   10A - X10A, 1X0A, 10XA, 10AX = 15 + 16 + 16 + 16 = 63
//   1A0 - X1A0, 1XA0, 1AX0, 1A0X = 15 + 16 + 16 + 16 = 63
//   A01 - XA01, AX01, A01X, A01X = 15 + 16 + 16 + 16 = 63
//   A10 - XA10, AX10, A1X0, A10X = 15 + 16 + 16 + 16 = 63
// But there are other new options:
//   01A - X01A = 15
//   0A1 - X0A1 = 15
// This overcounts though, 63 + 63 + 63 + 63 + 15 + 15 = 292, which is 34 too many.

// Construct strings based on how the counts of the different digits:
// Let {0,1,A} == {p,q,r} in any order, then 4 digit strings are of form
//   Xpqr, pXqr, pqXr, pqrX with 13 values for X
// So we get 13*6 + 13*4 + 13*4 + 13*4 = 78 + 52 + 52 + 52 = 234 strings.
// Then there are strings with duplicated 0, 1, or A:
//   ppqr, pqpr, pqrp
//   qppr, qprp
//   qrpp - 6 * 4 (p != 0) = 24
// 234 + 24 = 258
//   110A, 11A0, AA01, AA10
//   101A, 1A10, A0A1, A1A0
//   10A1, 1A01, A01A, A10A
//   100A, 1AA0, A001, A110
//   10A0, 1A0A, A010, A101
//   10AA, 1A00, A011, A100

// Construct 5 digit strings:
// Single occurrences of 0,1,A:
//   XYpqr, XpYqr, XpqYr, XpqrY - 13 * 13 * 4 * 6 = 4056
//   pXYqr, pXqYr, pXqrY, pqXYr, pqXrY, pqrXY - 13 * 13 * 6 * 4 (p != 0) = 4056
// Double occurrences of 0,1,A:
//   Xppqr, Xpqpr, Xpqrp, Xqppr, Xqprp, Xqrpp - 13 * 6 * 6 = 468
//   pXpqr, pXqpr, pXqrp, qXppr, qXprp, qXrpp - 13 * 6 * 4 (p != 0) = 312
//   ppXqr, pqXpr, pqXrp, qpXpr, qpXrp, qrXpp - 13 * 6 * 4 (p != 0) = 312
//   ppqXr, pqpXr, pqrXp, qppXr, qprXp, qrpXp - 13 * 6 * 4 (p != 0) = 312
//   ppqrX, pqprX, pqrpX, qpprX, qprpX, qrppX - 13 * 6 * 4 (p != 0) = 312
// Double doubles:
//   ppqqr, ppqrq, pprqq, prpqq, rppqq
//   pqpqr, pqprq, pqrpq, prqpq, rpqpq
//   pqqpr, pqqrp, pqrqp, prqqp, rpqqp - 3 * 5 * 4 (p != 0) = 60
// Triple occurrences:
//   pppqr, ppqpr, ppqrp, pqppr, pqprp, pqrpp - 6 * 4 = 24
//   qpppr, qpprp, qprpp, qrppp - 4 * 4 = 16
// Total = (4056 * 2) + (468 + 312 * 4) + (48 + 12) + (24 + 16) = 9928

// Construct 6 digit strings:
// Single occurrences of 0,1,A:
//   XYZpqr, XYpZqr, XYpqZr, XYpqrZ, XpYZqr, XpYqZr, XpYqrZ, XpqYZr, XpqYrZ, XpqrYZ - 13 * 13 * 13 * 10 * 6 = 131820
//   pXYZqr, pXYqZr, pXYqrZ, pXqYZr, pXqYrZ, pXqrYZ, pqXYZr, pqXYrZ, pqXrYZ, pqrXYZ - 13 * 13 * 13 * 10 * 4 (p != 0) = 87880
// Double occurrences of 0,1,A:
//   XYppqr, XpYpqr, XppYqr, XppqYr, XppqrY,
//   XYpqpr, XpYqpr, XpqYpr, XpqpYr, XpqprY,
//   XYpqrp, XpYqrp, XpqYrp, XpqrYp, XpqrpY,
//   XYpqqr, XpYqqr, XpqYqr, XpqqYr, XpqqrY,
//   XYpqrq, XpYqrq, XpqYrq, XpqrYq, XpqrqY,
//   XYpqrr, XpYqrr, XpqYrr, XpqrYr, XpqrrY - 13 * 13 * 6 * 5 * 6 = 30420
//   pXYpqr, pXYqpr, pXYqrp, pXYqqr, pXYqrq, pXYqrr - 13 * 13 * 6 * 4 (p != 0) = 4056
//   pXpYqr, pXqYpr, pXqYrp, pXqYqr, pXqYrq, pXqYrr - 13 * 13 * 6 * 4 (p != 0) = 4056
//   pXpqYr, pXqpYr, pXqrYp, pXqqYr, pXqrYq, pXqrYr - 13 * 13 * 6 * 4 (p != 0) = 4056
//   pXpqrY, pXqprY, pXqrpY, pXqqrY, pXqrqY, pXqrrY - 13 * 13 * 6 * 4 (p != 0) = 4056
//   ppXYqr, pqXYpr, pqXYrp, pqXYqr, pqXYrq, pqXYrr - 13 * 13 * 6 * 4 (p != 0) = 4056
//   ppXqYr, pqXpYr, pqXrYp, pqXqYr, pqXrYq, pqXrYr - 13 * 13 * 6 * 4 (p != 0) = 4056
//   ppXqrY, pqXprY, pqXrpY, pqXqrY, pqXrqY, pqXrrY - 13 * 13 * 6 * 4 (p != 0) = 4056
//   ppqXYr, pqpXYr, pqrXYp, pqqXYr, pqrXYq, pqrXYr - 13 * 13 * 6 * 4 (p != 0) = 4056
//   ppqXrY, pqpXrY, pqrXpY, pqqXrY, pqrXqY, pqrXrY - 13 * 13 * 6 * 4 (p != 0) = 4056
//   ppqrXY, pqprXY, pqrpXY, pqqrXY, pqrqXY, pqrrXY - 13 * 13 * 6 * 4 (p != 0) = 4056
// Double doubles:
//   Xppqqr, Xppqrq, Xpprqq, Xpqpqr, Xpqprq, Xprpqq, Xpqqpr, Xpqrpq, Xprqpq, Xpqqrp, Xpqrqp, Xprqqp - 13 * 12 * 6 = 936
//   pXpqqr, pXpqrq, pXprqq, pXqpqr, pXqprq, pXrpqq, pXqqpr, pXqrpq, pXrqpq, pXqqrp, pXqrqp, pXrqqp - 13 * 12 * 4 (p != 0) = 624
//   ppXqqr, ppXqrq, ppXrqq, pqXpqr, pqXprq, prXpqq, pqXqpr, pqXrpq, prXqpq, pqXqrp, pqXrqp, prXqqp - 13 * 12 * 4 (p != 0) = 624
//   ppqXqr, ppqXrq, pprXqq, pqpXqr, pqpXrq, prpXqq, pqqXpr, pqrXpq, prqXpq, pqqXrp, pqrXqp, prqXqp - 13 * 12 * 4 (p != 0) = 624
//   ppqqXr, ppqrXq, pprqXq, pqpqXr, pqprXq, prpqXq, pqqpXr, pqrpXq, prqpXq, pqqrXp, pqrqXp, prqqXp - 13 * 12 * 4 (p != 0) = 624
//   ppqqrX, ppqrqX, pprqqX, pqpqrX, pqprqX, prpqqX, pqqprX, pqrpqX, prqpqX, pqqrpX, pqrqpX, prqqpX - 13 * 12 * 4 (p != 0) = 624
//   Xpqqrr, Xpqrqr, Xpqrrq - 13 * 3 * 6 (p != 0) = 234
//   pXqqrr, pXqrqr, pXqrrq - 13 * 3 * 4 (p != 0) = 156
//   pqXqrr, pqXrqr, pqXrrq - 13 * 3 * 4 (p != 0) = 156
//   pqqXrr, pqrXqr, pqrXrq - 13 * 3 * 4 (p != 0) = 156
//   pqqrXr, pqrqXr, pqrrXq - 13 * 3 * 4 (p != 0) = 156
//   pqqrrX, pqrqrX, pqrrqX - 13 * 3 * 4 (p != 0) = 156
// Triple doubles:
//   ppqqrr, ppqrqr, ppqrrq, pqpqrr, pqprqr, pqprrq, pqqprr, pqqrpr, pqqrrp,
//   pqrpqr, pqrprq, pqrqpr, pqrqrp, pqrrpq, pqrrqp - 15 * 4 (p != 0) = 60
// Triple occurrences:
//   Xpppqr, Xppqpr, Xppqrp, Xpqppr, Xpqprp, Xpqrpp - 13 * 6 * 6 = 468
//   pXppqr, pXpqpr, pXpqrp, pXqppr, pXqprp, pXqrpp - 13 * 6 * 4 (p != 0) = 312
//   ppXpqr, ppXqpr, ppXqrp, pqXppr, pqXprp, pqXrpp - 13 * 6 * 4 (p != 0) = 312
//   pppXqr, ppqXpr, ppqXrp, pqpXpr, pqpXrp, pqrXpp - 13 * 6 * 4 (p != 0) = 312
//   pppqXr, ppqpXr, ppqrXp, pqppXr, pqprXp, pqrpXp - 13 * 6 * 4 (p != 0) = 312
//   pppqrX, ppqprX, ppqrpX, pqpprX, pqprpX, pqrppX - 13 * 6 * 4 (p != 0) = 312
//   Xqpppr, Xqpprp, Xqprpp, Xqrppp - 13 * 4 * 6 = 312
//   qXpppr, qXpprp, qXprpp, qXrppp - 13 * 4 * 4 = 208
//   qpXppr, qpXprp, qpXrpp, qrXppp - 13 * 4 * 4 = 208
//   qppXpr, qppXrp, qprXpp, qrpXpp - 13 * 4 * 4 = 208
//   qpppXr, qpprXp, qprpXp, qrppXp - 13 * 4 * 4 = 208
//   qppprX, qpprpX, qprppX, qrpppX - 13 * 4 * 4 = 208
// Triple and double occurrences:
//   pppqqr, pppqrq, ppprqq, pprpqq, prppqq, rpppqq
//   ppqpqr, ppqprq, ppqrpq, pprqpq, prpqpq, rppqpq
//   ppqqpr, ppqqrp, ppqrqp, pprqqp, prpqqp, rppqqp
//   pqppqr, pqpprq, pqprpq, pqrppq, prqppq, rpqppq
//   pqpqpr, pqpqrp, pqprqp, pqrpqp, prqpqp, rpqpqp
//   pqqppr, pqqprp, pqqrpp, pqrqpp, prqqpp, rpqqpp
//   qpppqr, qppprq, qpprpq, qprppq, qrpppq, rqpppq
//   qppqpr, qppqrp, qpprqp, qprpqp, qrppqp, rqppqp
//   qpqppr, qpqprp, qpqrpp, qprqpp, qrpqpp, rqpqpp
//   qqpppr, qqpprp, qqprpp, qqrppp, qrqppp, rqqppp - 60 * 4 (!= 0) = 240
// Quadruple occurrences:
//   ppppqr
//   pppqpr, pppqrp
//   ppqppr, ppqprp, ppqrpp
//   pqpppr, pqpprp, pqprpp, pqrppp
//   qppppr, qppprp, qpprpp, qprppp, qrpppp - 15 * 4 (!= 0) = 60
// Total = (131820 + 87880) + (30420 + 4056 * 10) + (936 + 624*5 + 234 + 156*5) + (60) + (468 + 312*5 + 312 + 208*5) + 140 + 60
//          263640 + 70980 + 4992 + 60 + 3380 + 240 + 60 = 299490

// Construct all the N-digit strings:
// For the following discussion, use the following set labels:
//   - S = {0, 1, A}, the set of digits that must appear at least once
//   - T = {2, 3, 4, 5, 6, 7, 8, 9, B, C, D, E, F}, the set of other digits that may appear

// Single occurrences of all digits in S:
//   - There are N! / (3! * (N-3)!) = N*(N-1)*(N-2)/6 total permutations 
//   - They fall into 2 categories - those that start with one of S and those that start with one of T
//   - For the S category, there are (N-1)! / (2! * (N-3)!) permutations for S/T interleaves
//   - For the T category, there are (N-1)! / (3! * (N-4)!) = (N-1)*(N-2)*(N-3)/6
//   - The S category has 4 permutations of the members of S, so we don't start with 0
//   - The T category has 6 permutations of the members of S
//   - Both categories have 13^(N-3) permutations of the members of T
//   - So the total number is:
//        13^(N-3) * {(N-1)*(N-2)/2!*4 + (N-1)*(N-2)*(N-3)/3!*6}
//      = 13^(N-3) * (N-1)*(N-2)*(2 + (N-3))
//      = 13^(N-3) * (N-1)^2 * (N-2)
// Tabulated results:
//    N            # strings
//    3        1 *  2^2 *  1 = 4
//    4       13 *  3^2 *  2 = 234
//    5     13^2 *  4^2 *  3 = 8112
//    6     13^3 *  5^2 *  4 = 219700
//    7     13^4 *  6^2 *  5 = 5140980
//   16    13^13 * 15^2 * 14 = 954,056,585,765,596,950 (9.5E17)

// Double occurrences of 1 digit from S (N >= 4):
//   - There are the same 6 patterns for the digits in S: ppqr, pqpr, pqrp, qppr, qprp, qrpp
//     - Can calculate as 4! / (2! * 2) = 6 - the 2! is for the repeated p, the 2 for the q/r symmetry
//   - These 4 digits are interleaved with [0, N-4] digits from T
//   - For the S category, there are (N-1)! / (3! * (N-4)!) permutations for S/T interleaves
//   - For the T category, there are (N-1)! / (4! * (N-5)!) permutations for S/T interleaves
//   - The S category has 4 permutations of the members of S, so we don't start with 0
//   - The T category has 6 permutations of the members of S
//   - Both categories have 13^(N-4) permutations of the members of T
//   - So the total number is:
//        13^(N-4) * {(N-1)*(N-2)*(N-3)/3!*6*4 + (N-1)*(N-2)*(N-3)*(N-4)/4!*6*6}
//      = 13^(N-4) * (N-1)*(N-2)*(N-3) * {4 + (N-4)*3/2}
// Tabulated results:
//    N             # strings
//    4        1 *  3*2*1*(4+0)        = 24
//    5       13 *  4*3*2*(4+3/2)      = 1716
//    6     13^2 *  5*4*3*(4+2*3/2)    = 70980
//    7     13^3 *  6*5*4*(4+3*3/2)    = 2240940
//   16    13^12 * 15*14*13*(4+12*3/2) = 1,399,282,992,456,208,860 (1.4E18) 

// Double occurrences of 2 digits from S (N >= 5):
//   - There are 15 patterns for the digits in S:
//     ppqqr, ppqrq, pprqq, prpqq, rppqq
//     pqpqr, pqprq, pqrpq, prqpq, rpqpq
//     pqqpr, pqqrp, pqrqp, prqqp, rpqqp
//     Calculate as 5! / (2! * 2! * 2) = 15
//     The first 2! is for the repeated p, the second for the repeated q, the 2 is for the p/q symmetry
//   - These 5 digits are interleaved with [0, N-5] digits from T
//   - For the S category, there are (N-1)! / (4! * (N-5)!) permutations for S/T interleaves
//   - For the T category, there are (N-1)! / (5! * (N-6)!) permutations for S/T interleaves
//   - The S category has 4 permutations of the members of S, so we don't start with 0
//   - The T category has 6 permutations of the members of S
//   - Both categories have 13^(N-5) permutations of the members of T
//   - So the total number is:
//        13^(N-5) * {(N-1)*(N-2)*(N-3)*(N-4)/4!*15*4 + (N-1)*(N-2)*(N-3)*(N-4)*(N-5)/5!*15*6}
//      = 13^(N-5) * (N-1)*(N-2)*(N-3)*(N-4) * {5/2 + (N-5)*3/4}
// Tabulated results:
//    N            # strings
//    5        1 *  4*3*2*1*(5/2+0)      = 60
//    6       13 *  5*4*3*2*(5/2+3/4)    = 5070
//    7     13^2 *  6*5*4*3*(5/2+2*3/4)  = 243360
//   16    13^11 * 15*14*13*(5/2+11*3/2) = 92,959,359,638,699,190 (9.3E16) 

// Triple occurences of 1 digit from S (N >= 5):
//   - There are 10 patterns for the digits in S:
//     pppqr, ppqpr, pqppr, qpppr,
//     ppqrp, pqprp, qpprp
//     pqrpp, qprpp,
//     qrppp
//     Calculate as 5! / (3! * 2)
//     The 3! is for the repeated p, the 2 is for the q/r symmetry
//   - These 5 digits are interleaved with [0, N-5] digits from T
//   - For the S category, there are (N-1)! / (4! * (N-5)!) permutations for S/T interleaves
//   - For the T category, there are (N-1)! / (5! * (N-6)!) permutations for S/T interleaves
//   - The S category has 4 permutations of the members of S, so we don't start with 0
//   - The T category has 6 permutations of the members of S
//   - Both categories have 13^(N-5) permutations of the members of T
//   - So the total number is:
//        13^(N-5) * {(N-1)*(N-2)*(N-3)*(N-4)/4!*10*4 + (N-1)*(N-2)*(N-3)*(N-4)*(N-5)/5!*10*6}
//      = 13^(N-5) * (N-1)*(N-2)*(N-3)*(N-4) * {5/3 + (N-5)/2}
// Tabulated results:
//    N    # double occurrence strings
//    5        1 *  4*3*2*1*(5/3+0)       = 40
//    6       13 *  5*4*3*2*(5/3+1/2)     = 3380
//    7     13^2 *  6*5*4*3*(5/3+2/2)     = 162240
//   16    13^11 * 15*14*13*12*(5/3+11/2) = 420,763,417,312,006,860 (4.2E17)

// Double occurrences of all 3 digits from S (N >= 6):
//   - There are 15 patterns for the digits in S:
//     ppqqrr, ppqrqr, ppqrrq
//     pqpqrr, pqprqr, pqprrq
//     pqqprr, pqqrpr, pqqrrp
//     pqrpqr, pqrprq, pqrqpr, pqrqrp, pqrrpq, pqrrqp
//     Calculate as 6! / (2! * 2! * 2! * 3!) = 15
//     The 2! factors are for the repeated p,q,r, the 3! is for the p/q/r symmetry
//   - These 6 digits are interleaved with [0, N-6] digits from T
//   - For the S category, there are (N-1)! / (5! * (N-6)!) permutations for S/T interleaves
//   - For the T category, there are (N-1)! / (6! * (N-7)!) permutations for S/T interleaves
//   - The S category has 4 permutations of the members of S, so we don't start with 0
//   - The T category has 6 permutations of the members of S
//   - Both categories have 13^(N-6) permutations of the members of T
//   - So the total number is:
//        13^(N-6) * {(N-1)*(N-2)*(N-3)*(N-4)*(N-5)/5!*15*4 + (N-1)*(N-2)*(N-3)*(N-4)*(N-5)*(N-6)/6!*15*6}
//      = 13^(N-6) * (N-1)*(N-2)*(N-3)*(N-4)*(N-5) * {1/2 + (N-6)/8}
// Tabulated results:
//    N            # strings
//    6        1 *  5*4*3*2*1*(1/2+0)        = 60
//    7       13 *  6*5*4*3*2*(1/2+1/8)      = 5850
//    8     13^2 *  7*6*5*4*3*(1/2+2/8)      = 319410
//   16    13^10 * 15*14*13*12*11*(1/2+10/8) = 86,937,700,714,734,870 (8.6E16) 

// Triple occurrence of 1 digit and double occurrence of another digit from S (N >= 6):
//   - There are 60 patterns for the digits in S:
//     pppqqr, pppqrq, ppprqq, pprpqq, prppqq, rpppqq
//     ppqpqr, ppqprq, ppqrpq, pprqpq, prpqpq, rppqpq
//     ppqqpr, ppqqrp, ppqrqp, pprqqp, prpqqp, rppqqp
//     pqppqr, pqpprq, pqprpq, pqrppq, prqppq, rpqppq
//     pqpqpr, pqpqrp, pqprqp, pqrpqp, prqpqp, rpqpqp
//     pqqppr, pqqprp, pqqrpp, pqrqpp, prqqpp, rpqqpp
//     qpppqr, qppprq, qpprpq, qprppq, qrpppq, rqpppq
//     qppqpr, qppqrp, qpprqp, qprpqp, qrppqp, rqppqp
//     qpqppr, qpqprp, qpqrpp, qprqpp, qrpqpp, rqpqpp
//     qqpppr, qqpprp, qqprpp, qqrppp, qrqppp, rqqppp
//     Calculate as 6! / (4! * 2)
//     The 4! is for the repeated p, the 2 is for the q/r symmetry.
//   - These 6 digits are interleaved with [0, N-6] digits from T
//   - For the S category, there are (N-1)! / (5! * (N-6)!) permutations for S/T interleaves
//   - For the T category, there are (N-1)! / (6! * (N-7)!) permutations for S/T interleaves
//   - The S category has 4 permutations of the members of S, so we don't start with 0
//   - The T category has 6 permutations of the members of S
//   - Both categories have 13^(N-6) permutations of the members of T
//   - So the total number is:
//        13^(N-6) * {(N-1)*(N-2)*(N-3)*(N-4)*(N-5)/5!*60*4 + (N-1)*(N-2)*(N-3)*(N-4)*(N-5)*(N-6)/6!*15*6}
//      = 13^(N-6) * (N-1)*(N-2)*(N-3)*(N-4)*(N-5) * {2 + (N-6)/2}
// Tabulated results:
//    N    # double occurrence strings
//    6        1 *  5*4*3*2*1*(2+0)        = 240
//    7       13 *  6*5*4*3*2*(2+1/2)      = 23400
//    8     13^2 *  7*6*5*4*3*(2+2/2)      = 1277640
//   16    13^10 * 15*14*13*12*11*(2+10/2) = 347,750,802,858,939,480 (3.5E17) 

// Quadruple occurences of 1 digit from S (N >= 6):
//   - There are  patterns for the digits in S:
//     ppppqr
//     pppqpr, pppqrp
//     ppqppr, ppqprp, ppqrpp
//     pqpppr, pqpprp, pqprpp, pqrppp
//     qppppr, qppprp, qpprpp, qprppp, qrpppp
//     Calculate as 5! / (3! * 2)
//     The 3! is for the repeated p, the 2 is for the q/r symmetry
//   - These 5 digits are interleaved with [0, N-5] digits from T
//   - For the S category, there are (N-1)! / (4! * (N-5)!) permutations for S/T interleaves
//   - For the T category, there are (N-1)! / (5! * (N-6)!) permutations for S/T interleaves
//   - The S category has 4 permutations of the members of S, so we don't start with 0
//   - The T category has 6 permutations of the members of S
//   - Both categories have 13^(N-5) permutations of the members of T
//   - So the total number is:
//        13^(N-6) * {(N-1)*(N-2)*(N-3)*(N-4)*(N-5)/5!*15*4 + (N-1)*(N-2)*(N-3)*(N-4)*(N-5)*(N-6)/6!*15*6}
//      = 13^(N-6) * (N-1)*(N-2)*(N-3)*(N-4)*(N-5) * {1/2 + (N-6)/8}
// Tabulated results:
//    N    # double occurrence strings
//    6        1 *  5*4*3*2*1*(1/2+0)        = 60
//    7       13 *  6*5*4*3*2*(1/2+1/8)      = 5850
//    8     13^2 *  7*6*5*4*3*(1/2+2/8)      = 319410
//   16    13^10 * 15*14*13*12*11*(1/2+10/8) = 86,937,700,714,734,870 (8.6E16) 

//    7     13^4 *  6^2 *  5 = 5140980
//    7     13^3 *  6*5*4*(4+3*3/2)    = 2240940
//    7     13^2 *  6*5*4*3*(5/2+2*3/4)  = 243360
//    7     13^2 *  6*5*4*3*(5/3+2/2)     = 162240
//    7       13 *  6*5*4*3*2*(1/2+1/8)      = 5850
//    7       13 *  6*5*4*3*2*(2+1/2)      = 23400
//    7       13 *  6*5*4*3*2*(1/2+1/8)      = 5850
// pppqqrr = 7! / (3! * 2! * 2! * 2) = 105 * 4 = 420
// pppqqqr = 7! / (3! * 3! * 2) = 70 * 4 = 280
// ppppqqr = 7! / (4! * 2!) = 105 * 4 = 420
// pppppqr = 7! / (5! * 2) = 21 * 4 = 84

std::vector<std::string> validStringsOfLength(size_t len) {
    std::vector<std::string> ret;
    // When a string of length N is interpreted as a hex number, its max value is 16^N.
    // We don't want the string to start with 0, so it's smallest value is 2*16^(N-1).

    size_t min_value = 1;
    for (size_t i = 1; i < len; ++i)
        min_value *= 16;
    size_t max_value = min_value * 16;
std::cout << min_value << " " << max_value << std::endl;
    for (size_t n = min_value; n <= max_value; ++n) {
        std::stringstream stream;
        stream << std::hex << n;
        auto str = stream.str();
//std::cout << str << std::endl;
        bool found0{ false };
        bool found1{ false };
        bool foundA{ false };
        for (const auto &ch: str) {
            if ('0' == ch) {
                found0 = true;
            }
            else if ('1' == ch) {
                found1 = true;
            }
            else if ('a' == ch) {
                foundA = true;
            }
        }
        if (found0 && found1 && foundA) {
            ret.push_back(str);
        }
    }

    return ret;
}


uint64_t numStringsOfLength(size_t len) {
    uint64_t ret { 0 };

    // When a string of length N is interpreted as a hex number, its max value is 16^N.
    // We don't want the string to start with 0, so it's smallest value is 2*16^(N-1).
    size_t min_value = 1;
    for (size_t i = 1; i < len; ++i)
        min_value *= 16;
    size_t max_value = min_value * 16;
std::cout << min_value << " " << max_value << std::endl;
    for (size_t n = min_value; n <= max_value; ++n) {
        std::stringstream stream;
        stream << std::hex << n;
        auto str = stream.str();
//std::cout << str << std::endl;
        bool found0{ false };
        bool found1{ false };
        bool foundA{ false };
        for (const auto &ch: str) {
            if ('0' == ch) {
                found0 = true;
            }
            else if ('1' == ch) {
                found1 = true;
            }
            else if ('a' == ch) {
                foundA = true;
            }
        }
        if (found0 && found1 && foundA) {
            ++ret;
        }
    }

    return ret;
}


using S_Counts = std::tuple<uint16_t, uint16_t, uint16_t>;
using Factors = std::list<uint16_t>;

void cancel_factors(Factors &factors, const uint16_t max_divisor) {
    for (uint16_t i = 2; i <= max_divisor; ++i) {
        auto it = std::find(factors.begin(), factors.end(), i);
        if (it != factors.cend()) {
            factors.erase(it);
        }
        else {
            uint16_t tmp{ i };
            while (tmp > 1) {
                for (Factors::iterator it = factors.begin(); it != factors.end(); ++it) {
                    auto g = gcd(*it, tmp);
                    if (g > 1) {
                        *it /= g;
                        if (1 == *it) {
                            factors.erase(it);
                        }
                        tmp /= g;
                        break;
                    }
                }
            }
        }
    }
}

uint64_t num_s_perms(const S_Counts &counts) {
    const auto &[p, q, r] = counts;
    uint16_t total = p + q + r;

    // The factors of the final product.
    Factors factors;

    // Add all the factors for (p + q + r)!
    for (uint16_t i = 2; i <= total; ++i) {
        factors.push_back(i);
    }

    // Cancel out the factors of p!, q!, r!
    cancel_factors(factors, p);
    cancel_factors(factors, q);
    cancel_factors(factors, r);

    if ((p == q) && (q == r)) {
        cancel_factors(factors, 3);
    }
    else if ((p == q) || (p == r) || (q == r)) {
        cancel_factors(factors, 2);
    }

    return std::accumulate(factors.begin(), factors.end(), 1ULL, std::multiplies<uint64_t>());
}


std::vector<S_Counts> get_s_perms(uint16_t max_digits) {
    std::vector<S_Counts> ret;

    for (uint16_t p = 1; p <= max_digits-2; ++p) {
        for (uint16_t q = 1; q <= p; ++q) {
            if (p + q + 1 > max_digits)
                break;
            for (uint16_t r = 1; r <= q; ++r) {
                if (p + q + r > max_digits)
                    break;
                ret.push_back({p, q, r});
            }
        }
    }

    return ret;
}


BigInt num_strings_for_counts(uint16_t num_digits, const S_Counts &counts) {
    const BigInt s_perms{ num_s_perms(counts) };
    const auto &[p, q, r] = counts;
    uint16_t total = p + q + r;

    //   - There are K (total) digits from S

    //   - For the S category, there are (N-1)! / ((K-1)! * (N-K)!) permutations for S/T interleaves
    Factors s_factors;
    for (uint16_t n = num_digits-1; n > num_digits - total; --n)
        s_factors.push_back(n);
    cancel_factors(s_factors, total-1);
    //   - The S category has 4 permutations of the members of S, so we don't start with 0
    BigInt num_s{ 4 };
    for (auto fac : s_factors) {
        num_s *= fac;
    }

    //   - For the T category, there are (N-1)! / (K! * (N-K-1)!) permutations for S/T interleaves
    Factors t_factors;
    BigInt num_t;
    if (num_digits > total) {
        for (uint16_t n = num_digits-1; n > num_digits - total - 1; --n)
            t_factors.push_back(n);
        cancel_factors(t_factors, total);
        //   - The T category has 6 permutations of the members of S

        num_t = BigInt{ 6 };
        for (auto fac : t_factors) {
            num_t *= fac;
        }
    }

    //   - Both categories have 13^(N-K) permutations of the members of T
    BigInt t_perms{ 1 };
    for (uint16_t n = total; n < num_digits; ++n)
        t_perms *= 13;

    return s_perms * (num_s + num_t) * t_perms;
}


BigInt num_strings(uint16_t num_digits) {
    BigInt ret{ 0 };

    const auto counts = get_s_perms(num_digits);
    for (const auto &[p,q,r] : counts) {
        BigInt num = num_strings_for_counts(num_digits, {p,q,r});
//        std::cout << p << ", " << q << ", " << r << "\t = " << num << std::endl;
        ret += num;
    }

    return ret;
}

int main()
{
    std::cout << "Hello World!\n";

    // {
    //     auto list = validStringsOfLength(4);
    //     for (const auto &val: list)
    //         std::cout << val << std::endl;
    //     std::cout << "There are " << list.size() << " strings of len 3." << std::endl;
    // }

    // {
    //     for (size_t len = 3; len < 8; ++len) {
    //         auto num = numStringsOfLength(len);
    //         std::cout << "There are " << num << "(" << std::hex << num << std::dec << ") strings of len " << len << std::endl;
    //     }
    // }

    // {
    //     auto num = num_s_perms({8,4,4});
    //     std::cout << "num = " << num << std::endl;
    // }

    // {
    //     const auto counts = get_s_perms(7);
    //     for (const auto &[p,q,r] : counts) {
    //         std::cout << p << ", " << q << ", " << r << std::endl;
    //     }
    // }

    {
//    3        1 *  2^2 *  1 = 4
//    4       13 *  3^2 *  2 = 234
//    5     13^2 *  4^2 *  3 = 8112
//    6     13^3 *  5^2 *  4 = 219700
//    7     13^4 *  6^2 *  5 = 5140980
//   16    13^13 * 15^2 * 14 = 954,056,585,765,596,950 (9.5E17)

//    4        1 *  3*2*1*(4+0)        = 24
//    5       13 *  4*3*2*(4+3/2)      = 1716
//    6     13^2 *  5*4*3*(4+2*3/2)    = 70980
//    7     13^3 *  6*5*4*(4+3*3/2)    = 2240940
//   16    13^12 * 15*14*13*(4+12*3/2) = 1,399,282,992,456,208,860 (1.4E18) 

        // BigInt num = num_strings_for_counts(4, {1,1,1});
        // std::cout << "num = " << num << std::endl;
    }
//return 0;
    {
        BigInt total;
        for (uint16_t len = 3; len <= 16; ++len) {
            auto num = num_strings(len);
            std::cout << "num = " << num << std::endl;
            total += num;
        }
        std::cout << "tot = " << total << "\t" << std::uppercase << std::hex << total.to_int() << std::endl;
//        std::cout << "tot = " << total << std::endl;
    }
    BigInt tmp{ 1 };
    for (int i = 0; i < 64; ++i)
        tmp *= 2;
    std::cout << "tmp = " << tmp << std::endl;
}
