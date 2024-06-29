// If we calculate a^2 mod 6 for 0 <= a <= 5 we get : 0, 1, 4, 3, 4, 1.
//
// The largest value of a such that is a^2 == a mod 6 is 4.
// Let's call M(n) the largest value of a < n such that a^2 == a (mod n).
// So M(6) = 4.
//
// Find sum M(n) for 1 <= n <= 10^7.


#define _CRT_SECURE_NO_WARNINGS 1
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iostream>
#include <numeric>
#include <unordered_set>

#include "combinatorics.h"
#include "prime_helper.h"

// The answer is not 39782849136422, after 24:10 hours.
// The answer is     39782849136421, after 17:14 hours
// The answer is     39782849136421, after  6:10 hours

// We know that all modular systems have cyclical behavior when you look
// at the series n, n^2, n^3, ... n^k.  The period of this cycle will vary
// for different starting n values for each modulus M. The period of that
// cycle will reach its maximum when the modulus is prime, with a length
// of M-1 for prime M for all starting n>1.
// We therefore know that M(n) = 1 for prime n, because the only way the
// cycle can have a period of M-1 is if no numbers >1 are idempotent.
//

// Let's start with values of n that are the product of 2 distinct primes p < q.

// Look at cases with p = 2:
//       M(2*3) =  M(6) =  4 =  3+1
//       M(2*5) = M(10) =  6 =  5+1
//       M(2*7) = M(14) =  8 =  7+1
//      M(2*11) = M(22) = 12 = 11+1
//      M(2*13) = M(26) = 14 = 13+1
//      M(2*17) = M(34) = 18 = 17+1
//      M(2*19) = M(38) = 20 = 19+1
//      M(2*23) = M(46) = 24 = 23+1
//      M(2*29) = M(58) = 30 = 29+1
//      M(2*31) = M(62) = 32 = 31+1
//      M(2*37) = M(74) = 38 = 37+1
//      M(2*41) = M(82) = 42 = 41+1
//      M(2*43) = M(86) = 44 = 43+1
//      M(2*47) = M(94) = 48 = 47+1
// The pattern looks to be:
//      M(2*q) = q+1
// Proof:
//      M^2 = (q+1)^2 = q^2 + 2q + 1
//      q^2 + 2q + 1 =?= q + 1 (mod 2q)
// The 2q is the modulus, so it zeroes out.  Can also remove the +1, reducing to
//      q^2 == -q (mod 2q)
//      q == -q (mod 2q)
//      q^2 == q (mod 2q)
// This is the definition of M(n), QED

// Look at cases with p = 3:
//       M(3*2) =  M(6) =  4 = 2*2
//       M(3*5) = M(15) = 10 = 2*5
//       M(3*7) = M(21) = 15 = 2*7+1
//      M(3*11) = M(33) = 22 = 2*11
//      M(3*13) = M(39) = 27 = 2*13+1
//      M(3*17) = M(51) = 34 = 2*17
//      M(3*19) = M(57) = 39 = 2*19+1
//      M(3*23) = M(69) = 46 = 2*23
//      M(3*29) = M(87) = 58 = 2*29
//      M(3*31) = M(93) = 63 = 2*31+1
// The pattern looks to be:
//      M(3*q) = 2*q+1      if q == 1 (mod 3)
//               2*q        if q == 2 (mod 3)
// Proof 1: q == 1 (mod 3)
//      M^2 = (2q+1)^2
//      4q^2 + 4q + 1 =?= 2q + 1 (mod 3q)
//      4q^2 + 2q =?= 0 (mod 3q)
//    let q = 3k+1
//      4*(3k+1)^2 + 2(3k+1) =?= 0 (mod 9k+3)
//      36k^2 + 24k + 4 + 6k + 2 =?= 0 (mod 9k+3)
//      36k^2 + 30k + 6 =?= 0 (mod 9k+3)
//      (9k+3)(4k+2) == 0 (mod 9k+3)  QED
// Proof 2: q == 2 (mod 3)
//      M^2 = 4q^2
//      4q^2 =?= 2q (mod 3q)
//    let q = 3k+2
//      4*(3k+2)^2 =?= 6k + 4 (mod 9k+6)
//      36k^2 + 48k + 16 =?= 6k + 4 (mod 9k+6)
//      36k^2 + 42k + 12 =?= 0 (mod 9k+6)
//      (9k+6)(4k+2) == 0 (mod 9k+6)  QED

// Look at cases with p = 5:
//       M(5*2) =  M(10) =   6 = 3*2
//       M(5*3) =  M(15) =  10 = 3*3+1
//       M(5*7) =  M(35) =  21 = 3*7
//      M(5*11) =  M(55) =  45 = 4*11+1
//      M(5*13) =  M(65) =  40 = 3*13+1
//      M(5*17) =  M(85) =  51 = 3*17
//      M(5*19) =  M(95) =  76 = 4*19
//      M(5*23) = M(115) =  70 = 3*23+1
//      M(5*29) = M(145) = 116 = 4*29
//      M(5*31) = M(155) = 125 = 4*31+1
//      M(5*37) = M(185) = 111 = 3*37
// The pattern looks to be:
//      M(5*q) = 4*q+1      if q == 1 (mod 5)
//               3*q        if q == 2 (mod 5)
//               3*q+1      if q == 3 (mod 5)
//               4*q        if q == 4 (mod 5)
// Proof 1: q == 1 (mod 5)
//      M^2 = (4q+1)^2 
//      16q^2 + 8q + 1 =?= 4q + 1 (mod 5q)
//      16q^2 + 4q =?= 0 (mod 5q)
//    let q = 5k+1
//      16*(5k+1)^2 + 4(5k+1) =?= 0 (mod 25k+5)
//      400k^2 + 160k + 16 + 20k + 4 =?= 0 (mod 25k+5)
//      400k^2 + 180k + 20 =?= 0 (mod 25k+5)
//      (25k+5)(16k+4) == 0 (mod 25k+5)  QED
// Proof 2: q == 2 (mod 5)
//      M^2 = 9q^2
//      9q^2 =?= 3q (mod 5q)
//    let q = 5k+2
//      9*(5k+2)^2 =?= 3(5k+2) (mod 25k+10)
//      225k^2 + 180k + 36 =?= 15k + 6 (mod 25k+10)
//      225k^2 + 165k + 30 =?= 0 (mod 25k+10)
//      (25k+10)(9k+3) == 0 (mod 25k+10)  QED
// Proof 3: q == 3 (mod 5)
//      M^2 = (3q+1)^2
//      9q^2 + 6q + 1 =?= 3q + 1 (mod 5q)
//      9q^2 + 3q =?= 0 (mod 5q)
//    let q = 5k+3
//      9*(5k+3)^2 + 3(5k+3) =?= 0 (mod 25k+15)
//      225k^2 + 270k + 81 + 15k + 9 =?= 0 (mod 25k+15)
//      225k^2 + 285k + 90 =?= 0 (mod 25k+15)
//      (25k+15)(9k+6) == 0 (mod 25k+15)  QED
// Proof 4: q == 4 (mod 5)
//      M^2 = 16q^2
//      16q^2 =?= 4q (mod 5q)
//    let q = 5k+4
//      16*(5k+4)^2 =?= 4(5k+4) (mod 25k+20)
//      400k^2 + 640k + 256 =?= 20k + 16 (mod 25k+20)
//      400k^2 + 620k + 240 =?= 0 (mod 25k+20)
//      (25k+20)(16k+12) == 0 (mod 25k+20)  QED

// Proof A: q == x (mod 5)
//      M(5*q) = aq
//      M^2 = a^2q^2
//      a^2q^2 =?= aq (mod 5q)
//    let q = 5k+x
//      a^2*(5k+x)^2 =?= a(5k+x) (mod 25k+5x)
//      25a^2k^2 + 10a^2kx + a^2x^2 =?= 5ak + ax (mod 25k+5x)
//      25a^2k^2 + (10a^2x-5a)k + a^2x^2-ax =?= 0 (mod 25k+5x)
//      (25k+5x)(a^2k+(a^2x-a)/5) =?= 0 (mod 25k+5x)
//      (a^2x-a)/5 is an integer
//      x = 1 -> a(a - 1)  = 5i  -> a in {5, 1}
//      x = 2 -> a(2a - 1) = 5i  -> a in {5, 3}**
//      x = 3 -> a(3a - 1) = 5i  -> a in {5, 2}
//      x = 4 -> a(4a - 1) = 5i  -> a in {5, 4}**
// Proof B: q == x (mod 5)
//      M(5*q) = aq + 1
//      M^2 =?= (aq+1)^2 (mod 5q)
//      a^2q^2 + 2aq + 1 =?= aq + 1 (mod 5q)
//      a^2q^2 + aq =?= 0 (mod 5q)
//    let q = 5k+x
//      a^2(5k+x)^2 + a(5k+x) =?= 0 (mod 25k+5x)
//      25a^2k^2 + 10a^2kx + a^2x^2 + 5ak + ax =?= 0 (mod 25k+5x)
//      25a^2k^2 + (10a^2x + 5a)k + (a^2x^2 + ax) =?= 0 (mod 25k+5x)
//      (25k+5x)(a^2k+(a^2x+a)/5) =?= 0 (mod 25k+5x)
//      (a^2+a)/5 is an integer
//      x = 1 -> a(a + 1)  = 5i  -> a in {5, 4}**
//      x = 2 -> a(2a + 1) = 5i  -> a in {5, 2}
//      x = 3 -> a(3a + 1) = 5i  -> a in {5, 3}**
//      x = 4 -> a(4a + 1) = 5i  -> a in {5, 1}

// Look at cases with p = 7:
//       M(7*2) =  M(14) =   8 = 4*2
//       M(7*3) =  M(21) =  15 = 5*3
//       M(7*5) =  M(35) =  21 = 4*5+1
//      M(7*11) =  M(77) =  56 = 5*11+1
//      M(7*13) =  M(91) =  78 = 6*13
//      M(7*17) = M(119) =  85 = 5*17
//      M(7*19) = M(133) =  77 = 4*19+1
//      M(7*23) = M(161) =  92 = 4*23
//      M(7*29) = M(203) = 175 = 6*29+1
//      M(7*31) = M(217) = 155 = 5*31
//      M(7*37) = M(259) = 148 = 4*37
// The pattern looks to be:
//      M(7*q) = 6*q+1      if q == 1 (mod 7)
//               4*q        if q == 2 (mod 7)
//               5*q        if q == 3 (mod 7)
//               5*q+1      if q == 4 (mod 7)
//               4*q+1      if q == 5 (mod 7)
//               6*q        if q == 6 (mod 7)
//      M(7*24) = M(168) = 120 = 5*24

// Look at cases with p = 11:
//       M(11*2) =  M(22) =  12 = 6*2
//       M(11*3) =  M(33) =  22 = 7*3+1
//       M(11*5) =  M(55) =  45 = 9*5
//       M(11*7) =  M(77) =  56 = 8*7
//      M(11*13) = M(143) =  78 = 6*13
//      M(11*17) = M(187) = 154 = 9*17+1
//      M(11*19) = M(209) = 133 = 7*19
//      M(11*23) = M(253) = 231 = 10*23+1
//      M(11*29) = M(319) = 232 = 8*29
//      M(11*31) = M(341) = 187 = 6*31+1
//      M(11*37) = M(407) = 297 = 8*37+1
// The pattern looks to be:
//      M(11*q) = 10*q+1     if q == 1 (mod 11)
//                6*q        if q == 2 (mod 11)
//                7*q+1      if q == 3 (mod 11)
//                8*q+1      if q == 4 (mod 11)
//                9*q        if q == 5 (mod 11)
//                9*q+1      if q == 6 (mod 11)
//                8*q        if q == 7 (mod 11)
//                7*q        if q == 8 (mod 11)
//                6*q+1      if q == 9 (mod 11)
//                10*q       if q == 10 (mod 11)*

// Proof A: q == x (mod 11)
//      M(11*q) = aq
//      M^2 = a^2q^2
//      a^2q^2 =?= aq (mod 11q)
//    let q = 11k+x
//      a^2*(11k+x)^2 =?= a(11k+x) (mod 121k+11x)
//      121a^2k^2 + 22a^2kx + a^2x^2 =?= 11ak + ax (mod 121k+11x)
//      121a^2k^2 + (22a^2x-11a)k + a^2x^2-ax =?= 0 (mod 121k+11x)
//      (121k+11x)(a^2k+(a^2x-a)/11) =?= 0 (mod 121k+11x)
//      (a^2x-a)/11 is an integer
//      x = 1 -> a(a - 1)  = 11i   -> a in {11, 1}
//      x = 2 -> a(2a - 1) = 11i   -> a in {11, 6}**
//      x = 3 -> a(3a - 1) = 11i   -> a in {11, 4}
//      x = 4 -> a(4a - 1) = 11i   -> a in {11, 3}
//      x = 5 -> a(5a - 1) = 11i   -> a in {11, 9}**
//      x = 6 -> a(6a - 1) = 11i   -> a in {11, 2}
//      x = 7 -> a(7a - 1) = 11i   -> a in {11, 8}**
//      x = 8 -> a(8a - 1) = 11i   -> a in {11, 7}**
//      x = 9 -> a(9a - 1) = 11i   -> a in {11, 5}
//      x = 10 -> a(10a - 1) = 11i -> a in {11, 10}**
// Proof B: q == x (mod 11)
//      M(11*q) = aq + 1
//      M^2 =?= (aq+1)^2 (mod 11q)
//      a^2q^2 + 2aq + 1 =?= aq + 1 (mod 11q)
//      a^2q^2 + aq =?= 0 (mod 5q)
//    let q = 11k+x
//      a^2(11k+x)^2 + a(11k+x) =?= 0 (mod 121k+11x)
//      121a^2k^2 + 22a^2kx + a^2x^2 + 11ak + ax =?= 0 (mod 121k+11x)
//      121a^2k^2 + (22a^2x + 11a)k + (a^2x^2 + ax) =?= 0 (mod 121k+11x)
//      (121k+11x)(a^2k+(a^2x+a)/1) =?= 0 (mod 121k+11x)
//      (a^2+a)/11 is an integer
//      x = 1 -> a(a + 1)  = 11i   -> a in {11, 10}**
//      x = 2 -> a(2a + 1) = 11i   -> a in {11, 5}
//      x = 3 -> a(3a + 1) = 11i   -> a in {11, 7}**
//      x = 4 -> a(4a + 1) = 11i   -> a in {11, 8}**
//      x = 5 -> a(5a + 1) = 11i   -> a in {11, 2}
//      x = 6 -> a(6a + 1) = 11i   -> a in {11, 9}**
//      x = 7 -> a(7a + 1) = 11i   -> a in {11, 3}
//      x = 8 -> a(8a + 1) = 11i   -> a in {11, 4}
//      x = 9 -> a(9a + 1) = 11i   -> a in {11, 6}**
//      x = 10 -> a(10a + 1) = 11i -> a in {11, 1}

// Look at cases with p = 11:
//       M(13*2) =  M(26) =  14 = 7*2
//       M(13*3) =  M(39) =  27 = 9*3
//       M(13*5) =  M(65) =  40 = 8*5
//       M(13*7) =  M(91) =  78 = 11*7+1
//      M(13*11) = M(143) =  78 = 7*11+1
//      M(13*17) = M(221) = 170 = 10*17
//      M(13*19) = M(247) = 209 = 11*19
//      M(13*23) = M(299) = 208 = 9*23+1
//      M(13*29) = M(377) = 261 = 9*29
//      M(13*31) = M(403) = 248 = 8*31
//      M(13*37) = M(481) = 260 = 7*37+1
// The pattern looks to be:
//      M(13*q) = 12*q+1     if q == 1 (mod 13)*
//                7*q        if q == 2 (mod 13)
//                9*q        if q == 3 (mod 13)
//                10*q       if q == 4 (mod 13)
//                8*q        if q == 5 (mod 13)
//                11*q       if q == 6 (mod 13)
//                11*q+1     if q == 7 (mod 13)
//                8*q+1      if q == 8 (mod 13)*
//                10*q+1     if q == 9 (mod 13)*
//                9*q+1      if q == 10 (mod 13)
//                7*q+1      if q == 11 (mod 13)
//                12*q       if q == 12 (mod 13)*

// Proof 3: q == 3 (mod 13)
//      M^2 = (8q+1)^2
//      64q^2 + 16q + 1 =?= 8q + 1 (mod 13q)
//      64q^2 + 8q =?= 0 (mod 13q)
//    let q = 8k+3
//      64*(8k+3)^2 + 8(8k+3) =?= 0 (mod 169k+39)
//      4096k^2 + 3072k + 576 + 64k + 24 =?= 0 (mod 169k+39)
//      4096k^2 + 3008k + 552 =?= 0 (mod 169k+39)
//      (169k+39)(9k+6) == 0 (mod 169k+39)  QED


//       M(3*4) = M(12) =  9 = 2*4+1
//       M(3*6) = M(18) = 10 = 1*6+4    == M(2*9) = 9+1
//       M(3*8) = M(24) = 16 = 2*8
//      M(3*10) = M(30) = 25 = 2*10+5   == M(5*6) = 4*6+1
//      M(3*12) = M(36) = 28 = 2*12+4   == M(4*9) = 3*9+1 ???
//      M(3*14) = M(42) = 36 = 2*14+8   == M(7*6) = 6*6
//      M(3*16) = M(48) = 33 = 2*16+1
//      M(3*18) = M(54) = 28 = 1*18+10  == M(2*27) = 27+1
//      M(3*20) = M(60) = 45 = 2*20+5   == M(4*15) = 3*15

//       M(4*3) = M(12) =  9 = 3*3
//       M(4*5) = M(20) = 16 = 3*5+1
//       M(4*6) = M(24) = 16 = 2*6+4    == M(8*3) = 5*3+1
//       M(4*7) = M(28) = 21 = 3*7
//       M(4*9) = M(36) = 28 = 3*9+1
//      M(4*10) = M(40) = 25 = 2*10+5   == M(8*5) = 5*5
//      M(4*11) = M(44) = 33 = 3*11
//      M(4*12) = M(48) = 33 = 2*12+9   == M(16*3) = 11*3
//      M(4*13) = M(52) = 40 = 3*13+1
//      M(4*14) = M(56) = 49 = 3*14+7   == M(8*7) = 7*7
//      M(4*15) = M(60) = 45 = 3*15

//       M(8*3) =  M(24) =  16 = 5*3+1
//       M(8*5) =  M(40) =  25 = 5*5
//       M(8*7) =  M(56) =  49 = 7*7
//       M(8*9) =  M(72) =  64 = 7*9+1
//      M(8*11) =  M(88) =  56 = 5*11+1
//      M(8*13) = M(104) =  65 = 5*13
//      M(8*15) = M(120) = 105 = 7*15
//      M(8*17) = M(136) = 120 = 7*17+1
//      M(8*19) = M(152) =  96 = 5*19+1
//      M(8*21) = M(168) = 120 = 5*21+15 ???
//      M(8*23) = M(184) = 161 = 7*23
//      M(8*25) = M(200) = 176 = 7*25+1
//      M(8*27) = M(216) = 136 = 5*27+1
//      M(8*29) = M(232) = 145 = 5*29
//      M(8*31) = M(248) = 217 = 7*31
//      M(8*33) = M(264) = 232 = 7*33+1
// The pattern looks to be:
//      M(8*q) = 7*q+1      if q == 1 (mod 8)
//               6*q        if q == 2 (mod 8)*
//               5*q+1      if q == 3 (mod 8)
//               4*q        if q == 4 (mod 8)*
//               5*q        if q == 5 (mod 8)
//               6*q+1      if q == 6 (mod 8)*
//               7*q        if q == 7 (mod 8)

//      M(14*2) =  M(28) =  21 = 10*3+1
//      M(14*3) =  M(42) =  36 = 12*3
//      M(14*5) =  M(70) =  56 = 11*5+1
//      M(14*7) =  M(98) =  50 = 7*7+1
//      M(14*9) = M(126) =  99 = 11*9
//     M(14*11) = M(154) = 133 = 12*11+1
//     M(14*13) = M(182) = 169 = 13*13
//     M(14*15) = M(210) = 196 = 13*15+1
//     M(14*17) = M(238) = 204 = 12*17
//     M(14*19) = M(266) = 210 = 11*19+1
//     M(14*21) = M(294) = 246 = 11*21+15 ???
//     M(14*23) = M(322) = 253 = 11*23
//     M(14*25) = M(350) = 301 = 12*25+1
//     M(14*27) = M(378) = 351 = 13*27
//     M(14*29) = M(406) = 378 = 13*29+1
//     M(14*31) = M(434) = 372 = 12*31
//     M(14*33) = M(462) = 441 = 13*33+12 ???
// The pattern looks to be:
//     M(14*q) = 13*q+1     if q == 1 (mod 14)
//               10*q+1     if q == 2 (mod 14)
//               12*q       if q == 3 (mod 14)
//               4*q        if q == 4 (mod 14)*
//               11*q+1     if q == 5 (mod 14)
//               6*q+1      if q == 6 (mod 14)*
//               7*q+1      if q == 7 (mod 14)
//               7*q        if q == 8 (mod 14)*
//               11*q       if q == 9 (mod 14)
//               7*q        if q == 10 (mod 14)*
//               12*q+1     if q == 11 (mod 14)
//               10*q       if q == 12 (mod 14)*
//               13*q       if q == 13 (mod 14)

//       M(9*2) =  M(18) = 10 = 5*2
//       M(9*3) =  M(27) =  1 =     3^3
//       M(9*4) =  M(36) = 28 = 7*4
//       M(9*5) =  M(45) = 36 = 7*5+1
//       M(9*6) =  M(54) = 28 = 4*6+4   == M(27*2) = 14*2
//       M(9*7) =  M(63) = 36 = 5*7+1
//       M(9*8) =  M(72) = 64 = 8*8
//      M(9*10) =  M(90) = 81 = 8*10+1
//      M(9*11) =  M(99) = 55 = 5*11
//      M(9*12) = M(108) = 81 = 6*12+9  == M(27*4)
//      M(9*13) = M(117) = 91 = 7*13
//      M(9*14) = M(126) = 99 = 7*14+1
//      M(9*15) = M(135) = 81 = 5*15+6  == M(27*5)
// The pattern looks to be:
//      M(5*q) = 8*q+1      if q == 1 (mod 9)
//               5*q        if q == 2 (mod 9)
//               6*q        if q == 3 (mod 9)*
//               7*q        if q == 4 (mod 9)
//               7*q+1      if q == 5 (mod 9)
//               6*q+1      if q == 6 (mod 9)*
//               5*q+1      if q == 7 (mod 9)
//               8*q        if q == 8 (mod 9)

// The basic pattern is this:
//      - if n is prime, or the power of a prime, then M(n) = 1
//      - get the prime factorization of n = p1^e1 * p2^e2 * p3^e3 * ... * pn^en
//        - for each prime factor, we can calculate 1 potential value for M(n)
//        - let k = p_i^e_i, q =  n / k
//        - then there are k-1 possible remainders for the q (mod k) operation
//        - each of these is associated with an expression for the idempotent number (mod n)
//        - these expressions are in the set { (k-1)*q, (k-1)*q+1, (k-2)*q, (k-2)*q+1, ... (k/2)*q, (k/2)*q+1 }
//        - test these k expressions, one will yield an idempotent number (mod n)
//        - the value for M(n) is the largest of these

// Return to generic case where n = p*q
// Proof A: q == x (mod p)
//      M(pq) = aq
//      M^2 = a^2q^2
//      a^2q^2 =?= aq (mod pq)
//    let q = pk+x
//      a^2*(pk+x)^2 =?= a(pk+x) (mod p^2k+px)
//      a^2p^2k^2 + 2a^2pxk + a^2x^2 =?= apk + ax (mod p^2k+px)
//      a^2p^2k^2 + (2a^2px-ap)k + a^2x^2-ax =?= 0 (mod p^2k+px)
//      (p^2k+px)(a^2k+(a^2x-a)/p) =?= 0 (mod p^2k+px)
//      (a^2x-a)/p is an integer
//      a(xa - 1) = pj  for some j
//    If p is prime:
//      one solution is a = p, but a must be < p, so this is not allowed
//      so (xa - 1) = pj for some j
//      this is the same as xa == 1 (mod p)
//      a is the multiplicative inverse of x, mod p
//      All values of x will yield a value for a, but only
//      those values such that a >= p/2 are actual solutions here.
//    If p is composite:
//
// Proof B: q == x (mod p)
//      M(p*q) = aq + 1
//      M^2 =?= (aq+1)^2 (mod pq)
//      a^2q^2 + 2aq + 1 =?= aq + 1 (mod pq)
//      a^2q^2 + aq =?= 0 (mod 5q)
//    let q = pk+x
//      a^2(pk+x)^2 + a(pk+x) =?= 0 (mod p^2k+px)
//      a^2p^2k^2 + 2a^2pkx + a^2x^2 + apk + ax =?= 0 (mod p^2k+px)
//      a^2p^2k^2 + (2a^2px + ap)k + (a^2x^2 + ax) =?= 0 (mod p^2k+px)
//      (p^2k+px)(a^2k+(a^2x+a)/p) =?= 0 (mod p^2k+px)
//      (a^2x+a)/p is an integer
//      a(xa + 1) = pj  for some j
//    If p is prime:
//      one solution is a = p, but a must be < p, so this is not allowed
//      so (xa + 1) = pj for some j
//      this is the same as xa == -1 (mod p)
//      a is the additive inverse of the multiplicative inverse of x, mod p
//      Another way to think about it is p minus 1/x mod p.
//      All values of x will yield a value for a, but only
//      those values such that a >= p/2 are actual solutions here
//    If p is composite:
//


int64_t M_brute(int64_t n) {
    int64_t ret{ 1 };

    for (int64_t i = 2; i < n; ++i) {
        if ((i * i % n) == i)
            ret = i;
    }

    return ret;
}


int64_t sum_M_brute(int64_t max_n) {
    int64_t ret{ 0 };

    for (int64_t n = 2; n <= max_n; ++n)
        ret += M_brute(n);

    return ret;
}


int64_t M_factors(int64_t n, const Factorization &factors) {
    int64_t ret{ 1 };

    for (const auto& [prime, exp] : factors) {
        int64_t base = prime;
        for (uint64_t i = 2; i <= exp; ++i)
            base *= prime;

        for (int64_t i = base; i < n; i += base) {
            if ((i * i % n) == i)
                ret = std::max(ret, i);
        }
    }

    return ret;
}


int64_t sum_M_primes(int64_t max_n) {
    int64_t ret{ 0 };

    {
        auto now = std::chrono::system_clock::now();
        auto now_t = std::chrono::system_clock::to_time_t(now);
        std::cout << "0  " << std::ctime(&now_t) << std::endl;
    }

    PrimeHelper helper;
    auto tmp = helper.get_primes(max_n);
    std::unordered_set<int64_t> primes{ tmp.cbegin(), tmp.cend() };

    for (int64_t n = 2; n <= max_n; ++n) {
        if (n % 100'000 == 0) {
            auto now = std::chrono::system_clock::now();
            auto now_t = std::chrono::system_clock::to_time_t(now);
            std::cout << n << "  " << std::ctime(&now_t) << std::endl;
        }
        // Skip the prime numbers
        if (primes.find(n) != primes.cend()) {
            ret += 1;
            continue;
        }

        Factorization factors = helper.get_factorization_fast(n);
        // Skip the powers of a single prime
        if (factors.size() == 1) {
            ret += 1;
            continue;
        }

        ret += M_factors(n, factors);
    }

    return ret;
}


int64_t get_idempotent(int64_t n, int64_t p, int64_t e) {
    int64_t k = 1;
    for (int64_t i = 0; i < e; ++i)
        k *= p;

    const int64_t q = n / k;

    std::vector<int64_t> idempotents{ 1 };

    for (int64_t p = k - 1; p >= k / 2; --p) {
        const int64_t m1 = p * q;
        if (m1 * m1 % n == m1)
            idempotents.push_back(m1);

        const int64_t m2 = m1 + 1;
        if (m2 * m2 % n == m2)
            idempotents.push_back(m2);
    }

    std::sort(idempotents.begin(), idempotents.end());
    //for (const auto& val : idempotents)
    //    std::cout << val << "\t";

    return idempotents.back();
}


int64_t get_idempotent(int64_t n, int64_t k) {
    const int64_t q = n / k;

    if (q < k)
        return 1;

    std::vector<int64_t> idempotents{ 1 };

    for (int64_t p = k - 1; p >= k / 2; --p) {
        const int64_t m1 = p * q;
        if (m1 * m1 % n == m1)
            idempotents.push_back(m1);

        const int64_t m2 = m1 + 1;
        if (m2 * m2 % n == m2)
            idempotents.push_back(m2);
    }

    std::sort(idempotents.begin(), idempotents.end());
    //for (const auto& val : idempotents)
    //    std::cout << val << "\t";

    return idempotents.back();
}


int64_t sum_M_factors(int64_t max_n) {
    int64_t ret{ 0 };

    {
        auto now = std::chrono::system_clock::now();
        auto now_t = std::chrono::system_clock::to_time_t(now);
        std::cout << "0  " << std::ctime(&now_t) << std::endl;
    }

    PrimeHelper helper;
    auto tmp = helper.get_primes(max_n);
    std::unordered_set<int64_t> primes{ tmp.cbegin(), tmp.cend() };

    std::vector<PowerSet> power_sets;
    power_sets.push_back({});
    power_sets.push_back({ {0} });
    power_sets.push_back(get_power_set({ 0, 1 }));
    power_sets.push_back(get_power_set({ 0, 1, 2 }));
    power_sets.push_back(get_power_set({ 0, 1, 2, 3 }));
    power_sets.push_back(get_power_set({ 0, 1, 2, 3, 4 }));
    power_sets.push_back(get_power_set({ 0, 1, 2, 3, 4, 5 }));
    power_sets.push_back(get_power_set({ 0, 1, 2, 3, 4, 5, 6 }));
    power_sets.push_back(get_power_set({ 0, 1, 2, 3, 4, 5, 6, 7 }));
    power_sets.push_back(get_power_set({ 0, 1, 2, 3, 4, 5, 6, 7, 8 }));

    for (int64_t n = 2; n <= max_n; ++n) {
        if (n % 100'000 == 0) {
            auto now = std::chrono::system_clock::now();
            auto now_t = std::chrono::system_clock::to_time_t(now);
            std::cout << n << "  " << std::ctime(&now_t) << std::endl;
        }
        // Skip the prime numbers
        if (primes.find(n) != primes.cend()) {
            ret += 1;
            continue;
        }

        Factorization factors = helper.get_factorization_fast(n);
        // Skip the powers of a single prime
        if (factors.size() == 1) {
            ret += 1;
            continue;
        }

        if (factors.size() < 4) {
            int64_t max_idem{ 0 };
            for (const auto& [prime, exp] : factors) {
                int64_t candidate = get_idempotent(n, prime, exp);
                max_idem = std::max(max_idem, candidate);
            }
            ret += max_idem;
        }
        else {
            // More than 4 unique prime factors, we need to worry about
            // products of 2 or more of those factors.
            int64_t max_idem{ 0 };

            // Create a set of the primes to their powers, so we can then
            // get the power set of all combinations of those factors.
            std::vector<int64_t> raised_factors;
            for (const auto& [prime, exp] : factors) {
                int64_t val{ 1 };
                for (uint64_t e = 0; e < exp; ++e)
                    val *= prime;
                raised_factors.push_back(val);
            }

            const PowerSet& power_set = power_sets[factors.size()];

            for (const auto& indices: power_set) {
                int64_t k{ 1 };
                for (const auto& ind : indices)
                    k *= raised_factors[ind];
                int64_t candidate = get_idempotent(n, k);
                max_idem = std::max(max_idem, candidate);
            }

            ret += max_idem;
        }
    }

    return ret;
}


int64_t get_egcd(int64_t a, int64_t b) {
    int64_t x{ 0 };
    int64_t y{ 1 };
    int64_t u{ 1 };
    int64_t v{ 0 };

    while (a != 0) {
        int64_t q = b / a;
        int64_t r = b % a;

        int64_t m = x - u * q;
        int64_t n = y - v * q;

        b = a;
        a = r;
        x = u;
        y = v;
        u = m;
        v = n;
    }

    return x;
}


int64_t get_inverse(int64_t value, int64_t modulus) {
    using Inverses = std::unordered_map<int64_t, int64_t>;
    using AllInverses = std::unordered_map<int64_t, Inverses>;
    static AllInverses inverses_by_mod{};

    if (inverses_by_mod.find(modulus) == inverses_by_mod.cend())
        inverses_by_mod[modulus] = {};

    Inverses& inverses_for_mod = inverses_by_mod.at(modulus);
    if (inverses_for_mod.find(value) != inverses_for_mod.cend())
        return inverses_for_mod.at(value);

    int64_t x = get_egcd(value, modulus);

    int64_t ret{ x % modulus };
    if (ret < 0)
        ret += modulus;

    inverses_for_mod[value] = inverses_for_mod[modulus - value] = ret;

    return ret;
}


int64_t get_idempotent_fast(int64_t n, int64_t p, PrimeHelper& helper) {
    const int64_t q = n / p;

    if (1 == q)
        return 1;

    //if (q < p)
    //    return 1;

    int64_t x = q % p;

    // Special case for M(2*q) = q+1
    // This is only true when q % 2 = 1, which we know is true
    // by the way the factors are calculated to call this function.
    if (2 == p)
        return q + 1;

    // Due to the symmetries, we can replace any large remainder
    // with its modular arithmetic inverse.
//    if (x > p / 2)
//        x = p - x;

    if (1 == x) {
        // The answer should be either (k-1)*q or (k-1)*q+1, test both
        int64_t candidate = (p - 1) * q;
        if (candidate * candidate % n == candidate)
            return candidate;
        ++candidate;
        if (candidate * candidate % n == candidate)
            return candidate;
        throw "Damn, wtf?";
    }

    if (helper.is_prime(p)) {
        // k is prime, so we only need to test 2 possible candidates:
        //      M(n) = a*q or a*q+1
        // where a = 1/mod (mod p)

        int64_t a = get_inverse(x, p);
        if (a < p / 2)
            a = p - a;

        // The +1 may or may not have deterministic symmetry, it's
        // easiest to just check both.
        int64_t candidate = a * q;
        if (candidate * candidate % n == candidate)
            return candidate;
        ++candidate;
        if (candidate * candidate % n == candidate)
            return candidate;
        throw "Damn, wtf?";
    }
    else {
        // k is composite, need to test its factors.
        // We know one of these two expressions is true:
        //      a(xa - 1) == 0 (mod p)
        //      a(xa + 1) == 0 (mod p)
        // We also know a >= p/2
        // Test all a in [p/2, p).
        int64_t max_idem{ 1 };
        for (int64_t a = (p + 1) / 2; a < p; ++a) {
            if (a * (a * x - 1) % p == 0) {
                // We found a for the expression M(n) = aq
                int64_t candidate = a * q;
                if (candidate * candidate % n == candidate)
                    max_idem = std::max(max_idem, candidate);
                //++candidate;
                //if (candidate * candidate % n == candidate)
                //    return candidate;
                else
                    std::cout << "why did n = " << n << ", p = " << p << ", q = " << q << ", x = " << x << ", a = " << a << " fail?" << std::endl;
            }
            if (a * (a * x + 1) % p == 0) {
                // We found a for the expression M(n) = aq + 1
                int64_t candidate = a * q + 1;
                if (candidate * candidate % n == candidate)
                    max_idem = std::max(max_idem, candidate);
                //--candidate;
                //if (candidate * candidate % n == candidate)
                //    return candidate;
                else
                    std::cout << "why did n = " << n << ", p = " << p << ", q = " << q << ", x = " << x << ", a = " << a << " also fail?" << std::endl;
            }
        }
        return max_idem;
    }

    return 1;
}


int64_t sum_M_factors_fast(int64_t max_n) {
    int64_t ret{ 0 };

    {
        auto now = std::chrono::system_clock::now();
        auto now_t = std::chrono::system_clock::to_time_t(now);
        std::cout << "0  " << std::ctime(&now_t) << std::endl;
    }

    PrimeHelper helper;
    auto tmp = helper.get_primes(max_n);
    const std::unordered_set<int64_t> primes{ tmp.cbegin(), tmp.cend() };

    std::vector<PowerSet> power_sets;
    power_sets.push_back({});
    power_sets.push_back({ {0} });
    power_sets.push_back(get_power_set({ 0, 1 }));
    power_sets.push_back(get_power_set({ 0, 1, 2 }));
    power_sets.push_back(get_power_set({ 0, 1, 2, 3 }));
    power_sets.push_back(get_power_set({ 0, 1, 2, 3, 4 }));
    power_sets.push_back(get_power_set({ 0, 1, 2, 3, 4, 5 }));
    power_sets.push_back(get_power_set({ 0, 1, 2, 3, 4, 5, 6 }));
    power_sets.push_back(get_power_set({ 0, 1, 2, 3, 4, 5, 6, 7 }));
    power_sets.push_back(get_power_set({ 0, 1, 2, 3, 4, 5, 6, 7, 8 }));

    for (int64_t n = 2; n <= max_n; ++n) {
        if (n % 100'000 == 0) {
            auto now = std::chrono::system_clock::now();
            auto now_t = std::chrono::system_clock::to_time_t(now);
            std::cout << n << "  " << std::ctime(&now_t) << std::endl;
        }
        // Skip the prime numbers
        if (primes.find(n) != primes.cend()) {
//            std::cout << "M(" << n << ") = 1" << std::endl;
            ret += 1;
            continue;
        }

        Factorization factors = helper.get_factorization_fast(n);
        // Skip the powers of a single prime
        if (factors.size() == 1) {
//            std::cout << "M(" << n << ") = 1" << std::endl;
            ret += 1;
            continue;
        }

        if (factors.size() < 4) {
            // Only 2 or 3 unique prime factors, no need to worry about products of them,
            // we can test only a single prime at a time and cover all bases.
            int64_t max_idem{ 0 };
            for (const auto& [prime, exp] : factors) {
                int64_t k{ 1 };
                for (uint64_t i = 0; i < exp; ++i)
                    k *= prime;
                int64_t candidate = get_idempotent_fast(n, k, helper);
                max_idem = std::max(max_idem, candidate);
            }
//            std::cout << "M(" << n << ") = " << max_idem << std::endl;
            ret += max_idem;
        }
        else {
            // More than 4 unique prime factors, we need to worry about
            // products of 2 or more of those factors.
            int64_t max_idem{ 0 };

            // Create a set of the primes to their powers, so we can then
            // get the power set of all combinations of those factors.
            std::vector<int64_t> raised_factors;
            for (const auto& [prime, exp] : factors) {
                int64_t val{ 1 };
                for (uint64_t e = 0; e < exp; ++e)
                    val *= prime;
                raised_factors.push_back(val);
            }

            const PowerSet& power_set = power_sets[factors.size()];

            for (const auto& indices : power_set) {
                int64_t k{ 1 };
                for (const auto& ind : indices)
                    k *= raised_factors[ind];
                int64_t candidate = get_idempotent_fast(n, k, helper);
                max_idem = std::max(max_idem, candidate);
            }

//            std::cout << "M(" << n << ") = " << max_idem << std::endl;
            ret += max_idem;
        }
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    for (int64_t n = 2; n <= 1000; ++n)
    //        std::cout << "M(" << n << ") = " << M_brute(n) << std::endl;
    //}
//    return 0;
    //{
    //    PrimeHelper helper;
    //    helper.get_primes(100);
    //
    //    for (int64_t n = 2; n <= 100; ++n) {
    //        auto factors = helper.get_factorization_fast(n);
    //        std::cout << "M(" << n << ") = " << M_brute(n) << "\t" << M_factors(n, factors) << std::endl;
    //    }
    //}

    //{
    //    std::cout << "sum M to 100 = " << sum_M_brute(100) << "\t" << sum_M_primes(100) << std::endl;
    //    std::cout << "sum M to 1000 = " << sum_M_brute(1000) << "\t" << sum_M_primes(1000) << std::endl;
    //    std::cout << "sum M to 10000 = " << sum_M_brute(10000) << "\t" << sum_M_primes(10000) << std::endl;
    //    //std::cout << "sum M to 100000 = " << sum_M_brute(100000) << "\t" << sum_M_primes(100000) << std::endl;
    //}

    //{
    //    auto ans = sum_M_primes(10'000'000);
    //    std::cout << "The answer is " << ans << std::endl;
    //}

    //{
    //    std::cout << "105 = " << get_idempotent(105, 3, 1) << "\t" << get_idempotent(105, 3) << std::endl;
    //    std::cout << "105 = " << get_idempotent(105, 5, 1) << "\t" << get_idempotent(105, 5) << std::endl;
    //    std::cout << "105 = " << get_idempotent(105, 7, 1) << "\t" << get_idempotent(105, 7) << std::endl;

    //    std::cout << "24 = " << get_idempotent(24, 2, 3) << "\t" << get_idempotent(24, 8) << std::endl;
    //    std::cout << "24 = " << get_idempotent(24, 3, 1) << "\t" << get_idempotent(24, 3) << std::endl;

    //    std::cout << "36 = " << get_idempotent(36, 2, 2) << "\t" << get_idempotent(36, 4) << std::endl;
    //    std::cout << "36 = " << get_idempotent(36, 3, 2) << "\t" << get_idempotent(36, 9) << std::endl;

    //    std::cout << "315 = " << get_idempotent(315, 3, 2) << "\t" << get_idempotent(315, 9) << std::endl;
    //    std::cout << "315 = " << get_idempotent(315, 5, 1) << "\t" << get_idempotent(315, 5) << std::endl;
    //    std::cout << "315 = " << get_idempotent(315, 7, 1) << "\t" << get_idempotent(315, 7) << std::endl;

    //    std::cout << "165 = " << get_idempotent(165, 3, 1) << "\t" << get_idempotent(165, 3) << std::endl;
    //    std::cout << "165 = " << get_idempotent(165, 5, 1) << "\t" << get_idempotent(165, 5) << std::endl;
    //    std::cout << "165 = " << get_idempotent(165, 11, 1) << "\t" << get_idempotent(165, 11) << std::endl;

    //    std::cout << "330 = " << get_idempotent(330, 2, 1) << "\t" << get_idempotent(330, 2) << std::endl;
    //    std::cout << "330 = " << get_idempotent(330, 3, 1) << "\t" << get_idempotent(330, 3) << std::endl;
    //    std::cout << "330 = " << get_idempotent(330, 5, 1) << "\t" << get_idempotent(330, 5) << std::endl;
    //    std::cout << "330 = " << get_idempotent(330, 11, 1) << "\t" << get_idempotent(330, 11) << std::endl;
    //    std::cout << "330 = " << get_idempotent(330, 6, 1) << "\t" << get_idempotent(330, 6) << std::endl;
    //    std::cout << "330 = " << get_idempotent(330, 10, 1) << "\t" << get_idempotent(330, 10) << std::endl;
    //    std::cout << "330 = " << get_idempotent(330, 22, 1) << "\t" << get_idempotent(330, 22) << std::endl;
    //    std::cout << "330 = " << get_idempotent(330, 15, 1) << "\t" << get_idempotent(330, 15) << std::endl;
    //    std::cout << "330 = " << get_idempotent(330, 33, 1) << "\t" << get_idempotent(330, 33) << std::endl;
    //    std::cout << "330 = " << get_idempotent(330, 55, 1) << "\t" << get_idempotent(330, 55) << std::endl;

    //    std::cout << "96 = " << get_idempotent(96, 2, 1) << "\t" << get_idempotent(96, 2) << std::endl;
    //    std::cout << "96 = " << get_idempotent(96, 2, 2) << "\t" << get_idempotent(96, 4) << std::endl;
    //    std::cout << "96 = " << get_idempotent(96, 2, 3) << "\t" << get_idempotent(96, 8) << std::endl;
    //    std::cout << "96 = " << get_idempotent(96, 2, 4) << "\t" << get_idempotent(96, 16) << std::endl;
    //    std::cout << "96 = " << get_idempotent(96, 2, 5) << "\t" << get_idempotent(96, 32) << std::endl;
    //    std::cout << "96 = " << get_idempotent(96, 3, 1) << "\t" << get_idempotent(96, 3) << std::endl;
    //}

    //{
    //    auto power = get_power_set({ 1,2,3,4,5,6,7,8 });
    //    for (const auto& set : power) {
    //        for (const auto& val : set)
    //            std::cout << val << ", ";
    //        std::cout << std::endl;
    //    }
    //    std::cout << "power set size = " << power.size() << std::endl;
    //}

    //{
    //    std::cout << "sum M to 100 = " << sum_M_brute(100) << "\t" << sum_M_factors(100) << std::endl;
    //    std::cout << "sum M to 1000 = " << sum_M_brute(1000) << "\t" << sum_M_factors(1000) << std::endl;
    //    std::cout << "sum M to 10000 = " << sum_M_brute(10000) << "\t" << sum_M_factors(10000) << std::endl;
    //    std::cout << "sum M to 100000 = " << sum_M_brute(100000) << "\t" << sum_M_factors(100000) << std::endl;
    //}

    //std::cout << "sum M to 10000000 = " << sum_M_factors(10'000'000) << std::endl;

    {
        std::cout << "sum M to 100 = " << sum_M_brute(100) << "\t" << sum_M_factors_fast(100) << std::endl;
        std::cout << "sum M to 1000 = " << sum_M_brute(1000) << "\t" << sum_M_factors_fast(1000) << std::endl;
        std::cout << "sum M to 10000 = " << sum_M_brute(10000) << "\t" << sum_M_factors_fast(10000) << std::endl;
        std::cout << "sum M to 100000 = " << sum_M_brute(100000) << "\t" << sum_M_factors_fast(100000) << std::endl;
    }

    std::cout << "sum M to 10000000 = " << sum_M_factors_fast(10'000'000) << std::endl;
}
