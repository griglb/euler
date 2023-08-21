// As we all know the equation x^2 = -1 has no solutions for real x.
// If we however introduce the imaginary number i this equation has two solutions:
//      x = i and x = -i.
// If we go a step further the equation (x - 3)^2 = -4 has two complex solutions:
//      x = 3 + 2i and x = 3 - 2i.
// x = 3 + 2i and x = 3 - 2i and are called each others' complex conjugate.
// Numbers of the form a + bi are called complex numbers.
// In general a + bi and a - bi are each other's complex conjugate.
//
// A Gaussian Integer is a complex number a + bi such that both a and b are integers.
// The regular integers are also Gaussian integers (with b = 0).
// To distinguish them from Gaussian integers with b != 0 we call such integers
// "rational integers."
// A Gaussian integer a + bi is called a divisor of a rational integer n if the result
// n / (a + bi) is also a Gaussian integer.
// If for example we divide 5 by 1 + 2i we can simplify 5/(1 + 2i) in the following manner:
// Multiply numerator and denominator by the complex conjugate of 1 + 2i: 1 - 2i.
// The result is
//        5        5    1 - 2i   5(1 - 2i)    5(1 - 2i)   5(1 - 2i)
//      ------ = ------ ------ = ---------- = --------- = --------- = 1 - 2i
//      1 + 2i   1 + 2i 1 - 2i   1 - (2i)^2    1 - (-4)       5
//
// So 1 + 2i is a divisor of 5.
// Note that 1 + i is not a divisor of because
//        5     5   5
//      ----- = - - -i
//      1 + i   2   2
//
// Note also that if the Gaussian Integer (a + bi) is a divisor of a rational integer,
// then its complex conjugate (a - bi) is also a divisor of n.
//
// In fact, 5 has six divisors such that the real part is positive :
//      1, 1 + 2i, 1 - 2i, 2 + i, 2 - i, 5
//
// The following is a table of all of the divisors for the first five positive rational integers:
//      Gaussian integer divisors       Sum s(n) of
//  n   with positive real part	        these divisors
//  1   1                               1
//  2   1, 1+i, 1-i, 2                  5
//  3   1, 3                            4
//  4   1, 1+i, 1-i, 2, 2+2i, 2-2i, 4   13
//  5   1, 1+2i, 1-2i, 2+i, 2-i, 5      12
//
// For divisors with positive real parts, then, we have:
//       5
//      sum s(n) = 35
//      n=1
//
//     10^5
//      sum s(n) = 17924657155
//      n=1
//
// What is
//     10^8
//      sum s(n) ?
//      n=1


#include <cmath>
#include <cstdint>
#include <iostream>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "chunked_prime_helper.h"


// we can see that the division always follows this pattern:
//        n        n    a - bi    n(a - bi)      n(a - bi)     n(a - bi)
//      ------ = ------ ------ = ------------ = ------------ = --------- = c - di
//      a + bi   a + bi a - bi   a^2 - (bi)^2   a^2 - (-b^2)   a^2 + b^2
//
// In order for the quotient to be a Gaussian Integer, two things must be true:
//      a^2 + b^2 is a divisor of n*a
//      a^2 + b^2 is a divisor of n*b
//
// We know (a^2 + b^2) > a,b so is it possible for the sum to not be a divisor of n?
// Let's say there are common divisors:
//      gcd(a^2 + b^2, a) = k
//      gcd(a^2 + b^2, b) = l
// The we can rewrite as
//      a = k*c
//      b = l*d
//      a^2 + b^2 = k^2*c^2 + l^2*d^2
// In order for the common divisors to exist, we then know:
//      k is a divisor of l^2*d^2
//      l is a divisor of k^2*c^2
// The simple solution to this is that k == l, but that means:
//      k^2 is a divisor of l^2*d^2
//      l^2 is a divisor of k^2*c^2
// which implies
//      gcd(a^2 + b^2, a) = k^2
//      gcd(a^2 + b^2, b) = l^2
// The only way k = k^2, l = l^2 is if k = l = 1.
// if k != l, then let's say k < l.  This 
// Which in general means
//      a^2 + b^2 is a divisor of n
//
// k = 6, l = 2, d = 3, c = ?
// a = 6c
// b = 6
// a^2 + b^2 = 36c^2 + 36
// gcd(36c^2 + 36, 6c) = 6 -> c = 1, a = 6
// a^2 + b^2 = 72
// n(6 - 6i)/72 = c + di
// n = 12*x
// 
//        12       12   6 - 6i   12(6 - 6i)      12(6 - 6i)    12(6 - 6i)
//      ------ = ------ ------ = ------------ = ------------ = ---------- = 1 - i
//      6 + 6i   6 + 6i 6 - 6i   6^2 - (6i)^2   6^2 - (-6^2)    36 + 36
// 
// k = 12, l = 32, d = 3, c = 2x
// a = 24x
// b = 96
// a^2 + b^2 = 576x^2 + 9216
// gcd(576x^2 + 9216, 24x) = 12 -> 576x^2 + 9216 = 48x*y -> y = 12x + 192/x
//   x      y
//   1    204
//   2    120
//   3    100
//   4     96
//   6    104
//   8    120
//  12    160
//  24    296
//  48    580
//  96   1154
// 192   2305
// 
// n(288 - 96i)/92160 = c + di
// n = 960*z
// 
//         960         960    288 - 96i   960(288 - 96i)    960(288 - 96i)    960(288 - 96i)   276480 - 92160i
//      --------- = --------- --------- = --------------- = --------------- = -------------- = --------------- = 3 - i
//      288 + 96i   288 + 96i 288 - 96i   288^2 - (96i)^2   288^2 - (-96^2)    82944 + 9216         92160
//

// Let's look at the Gaussian Integers for the first few values of n:
//  1 : 1
//  2 : 1, 1 + i, 1 - i, 2
//  3 : 1, 3
//  4 : 1, 1 + i, 1 - i, 2, 2 + 2i, 2 - 2i, 4
//  5 : 1, 1 + 2i, 1 - 2i, 2 + i, 2 - i, 5
//  6 : 1, 1 + i, 1 - i, 2, 3, 3 + 3i, 3 - 3i, 6
//  7 : 1, 7
//  8 : 1, 1 + i, 1 - i, 2, 2 + 2i, 2 - 2i, 4, 4 + 4i, 4 - 4i, 8
//  9 : 1, 3, 9
// 10 : 1, 1 + i, 1 - i, 1 + 2i, 1 - 2i, 1 + 3i, 1 - 3i, 2, 2 + i, 2 - i, 2 + 4i, 2 - 4i, 3 + i, 3 - i, 4 + 2i, 4 - 2i, 5, 5 + 5i, 5 - 5i, 10
// 11 : 1, 11
// 12 : 1, 1 + i, 1 - i, 2, 2 + 2i, 2 - 2i, 3, 3 + 3i, 3 - 3i, 4, 6, 6 + 6i, 6 - 6i, 12
// 13 : 1, 2 + 3i, 2 - 3i, 3 + 2i, 3 - 2i, 13
// 14 : 1, 1 + i, 1 - i, 2, 7, 7 + 7i, 7 - 7i, 14
// 15 : 1, 1 + 2i, 1 - 2i, 2 + i, 2 - i, 3, 3 + 6i, 3 - 6i, 5, 6 + 3i, 6 - 3i, 15
// 16 : 1, 1 + i, 1 - i, 2, 2 + 2i, 2 - 2i, 4, 4 + 4i, 4 - 4i, 8, 8 + 8i, 8 - 8i, 16
// 17 : 1, 1 + 4i, 1 - 4i, 4 + i, 4 - i, 17
// 18 : 1, 1 + i, 1 - i, 2, 3, 3 + 3i, 3 - 3i, 6, 9, 9 + 9i, 9 - 9i, 18
// 19 : 1, 19
// 20 : 1, 1 + i, 1 - i, 1 + 2i, 1 - 2i, 1 + 3i, 1 - 3i, 2, 2 + i, 2 - i, 2 + 2i, 2 - 2i, 2 + 4i, 2 - 4i, 2 + 6i, 2 - 6i, 3 + i, 3 - i, 4, 4 + 2i, 4 - 2i, 4 + 8i, 4 - 8i, 5, 5 + 5i, 5 - 5i, 6 + 2i, 6 - 2i, 8 + 4i, 8 - 4i, 10, 10 + 10i, 10 - 10i, 20
// 21 : 1, 3, 7, 21
// 22 : 1, 1 + i, 1 - i, 2, 11, 11 + 11i, 11 - 11i, 22
// 23 : 1, 23
// 24 : 1, 1 + i, 1 - i, 2, 2 + 2i, 2 - 2i, 3, 3 + 3i, 3 - 3i, 4, 4 + 4i, 4 - 4i, 6, 6 + 6i, 6 - 6i, 8, 12, 12 + 12i, 12 - 12i, 24
// 25 : 1, 1 + 2i, 1 - 2i, 2 + i, 2 - i, 3 + 4i, 3 - 4i, 4 + 3i, 4 - 3i, 5, 5 + 10i, 5 - 10i, 10 + 5i, 10 - 5i, 25
// 26 : 1, 1 + i, 1 - i, 1 + 5i, 1 - 5i, 2, 2 + 3i, 2 - 3i, 3 + 2i, 3 - 2i, 4 + 6i, 4 - 6i, 5 + i, 5 - i, 6 + 4i, 6 - 4i, 13, 13 + 13i, 13 - 13i, 26
// 27 : 1, 3, 9, 27
// 28 : 1, 1 + i, 1 - i, 2, 2 + 2i, 2 - 2i, 4, 7, 7 + 7i, 7 - 7i, 14, 14 + 14i, 14 - 14i, 28
// 29 : 1, 2 + 5i, 2 - 5i, 5 + 2i, 5 - 2i, 29
// 30 : 1, 1 + i, 1 - i, 1 + 2i, 1 - 2i, 1 + 3i, 1 - 3i, 2, 2 + i, 2 - i, 2 + 4i, 2 - 4i, 3, 3 + i, 3 - i, 3 + 3i, 3 - 3i, 3 + 6i, 3 - 6i, 3 + 9i, 3 - 9i, 4 + 2i, 4 - 2i, 5, 5 + 5i, 5 - 5i, 6, 6 + 3i, 6 - 3i, 6 + 12i, 6 - 12i, 9 + 3i, 9 - 3i, 10, 12 + 6i, 12 - 6i, 15, 15 + 15i, 15 - 15i, 30
// 31 : 1, 31
// 32 : 1, 1 + i, 1 - i, 2, 2 + 2i, 2 - 2i, 4, 4 + 4i, 4 - 4i, 8, 8 + 8i, 8 - 8i, 16, 16 + 16i, 16 - 16i, 32
// 33 : 1, 3, 11, 33
// 34 : 1, 1 + i, 1 - i, 1 + 4i, 1 - 4i, 2, 2 + 8i, 2 - 8i, 3 + 5i, 3 - 5i, 4 + i, 4 - i, 5 + 3i, 5 - 3i, 8 + 2i, 8 - 2i, 17, 17 + 17i, 17 - 17i, 34
// 35 : 1, 1 + 2i, 1 - 2i, 2 + i, 2 - i, 5, 7, 7 + 14i, 7 - 14i, 14 + 7i, 14 - 7i, 35
// 36 : 1, 1 + i, 1 - i, 2, 2 + 2i, 2 - 2i, 3, 3 + 3i, 3 - 3i, 4, 6, 6 + 6i, 6 - 6i, 9, 9 + 9i, 9 - 9i, 12, 18, 18 + 18i, 18 - 18i, 36
// 37 : 1, 1 + 6i, 1 - 6i, 6 + i, 6 - i, 37
// 38 : 1, 1 + i, 1 - i, 2, 19, 19 + 19i, 19 - 19i, 38
// 39 : 1, 2 + 3i, 2 - 3i, 3, 3 + 2i, 3 - 2i, 6 + 9i, 6 - 9i, 9 + 6i, 9 - 6i, 13, 39
// 40 : 1, 1 + i, 1 - i, 1 + 2i, 1 - 2i, 1 + 3i, 1 - 3i, 2, 2 + i, 2 - i, 2 + 2i, 2 - 2i, 2 + 4i, 2 - 4i, 2 + 6i, 2 - 6i, 3 + i, 3 - i, 4, 4 + 2i, 4 - 2i, 4 + 4i, 4 - 4i, 4 + 8i, 4 - 8i, 4 + 12i, 4 - 12i, 5, 5 + 5i, 5 - 5i, 6 + 2i, 6 - 2i, 8, 8 + 4i, 8 - 4i, 8 + 16i, 8 - 16i, 10, 10 + 10i, 10 - 10i, 12 + 4i, 12 - 4i, 16 + 8i, 16 - 8i, 20, 20 + 20i, 20 - 20i, 40
// 41 : 1, 4 + 5i, 4 - 5i, 5 + 4i, 5 - 4i, 41
// 42 : 1, 1 + i, 1 - i, 2, 3, 3 + 3i, 3 - 3i, 6, 7, 7 + 7i, 7 - 7i, 14, 21, 21 + 21i, 21 - 21i, 42
// 43 : 1, 43
// 44 : 1, 1 + i, 1 - i, 2, 2 + 2i, 2 - 2i, 4, 11, 11 + 11i, 11 - 11i, 22, 22 + 22i, 22 - 22i, 44
// 45 : 1, 1 + 2i, 1 - 2i, 2 + i, 2 - i, 3, 3 + 6i, 3 - 6i, 5, 6 + 3i, 6 - 3i, 9, 9 + 18i, 9 - 18i, 15, 18 + 9i, 18 - 9i, 45
// 46 : 1, 1 + i, 1 - i, 2, 23, 23 + 23i, 23 - 23i, 46
// 47 : 1, 47
// 48 : 1, 1 + i, 1 - i, 2, 2 + 2i, 2 - 2i, 3, 3 + 3i, 3 - 3i, 4, 4 + 4i, 4 - 4i, 6, 6 + 6i, 6 - 6i, 8, 8 + 8i, 8 - 8i, 12, 12 + 12i, 12 - 12i, 16, 24, 24 + 24i, 24 - 24i, 48
// 49 : 1, 7, 49
// 50 : 1, 1 + i, 1 - i, 1 + 2i, 1 - 2i, 1 + 3i, 1 - 3i, 1 + 7i, 1 - 7i, 2, 2 + i, 2 - i, 2 + 4i, 2 - 4i, 3 + i, 3 - i, 3 + 4i, 3 - 4i, 4 + 2i, 4 - 2i, 4 + 3i, 4 - 3i, 5, 5 + 5i, 5 - 5i, 5 + 10i, 5 - 10i, 5 + 15i, 5 - 15i, 6 + 8i, 6 - 8i, 7 + i, 7 - i, 8 + 6i, 8 - 6i, 10, 10 + 5i, 10 - 5i, 10 + 20i, 10 - 20i, 15 + 5i, 15 - 5i, 20 + 10i, 20 - 10i, 25, 25 + 25i, 25 - 25i, 50
// 51 : 1, 1 + 4i, 1 - 4i, 3, 3 + 12i, 3 - 12i, 4 + i, 4 - i, 12 + 3i, 12 - 3i, 17, 51
// 52 : 1, 1 + i, 1 - i, 1 + 5i, 1 - 5i, 2, 2 + 2i, 2 - 2i, 2 + 3i, 2 - 3i, 2 + 10i, 2 - 10i, 3 + 2i, 3 - 2i, 4, 4 + 6i, 4 - 6i, 5 + i, 5 - i, 6 + 4i, 6 - 4i, 8 + 12i, 8 - 12i, 10 + 2i, 10 - 2i, 12 + 8i, 12 - 8i, 13, 13 + 13i, 13 - 13i, 26, 26 + 26i, 26 - 26i, 52
// 53 : 1, 2 + 7i, 2 - 7i, 7 + 2i, 7 - 2i, 53
// 54 : 1, 1 + i, 1 - i, 2, 3, 3 + 3i, 3 - 3i, 6, 9, 9 + 9i, 9 - 9i, 18, 27, 27 + 27i, 27 - 27i, 54
// 55 : 1, 1 + 2i, 1 - 2i, 2 + i, 2 - i, 5, 11, 11 + 22i, 11 - 22i, 22 + 11i, 22 - 11i, 55
// 56 : 1, 1 + i, 1 - i, 2, 2 + 2i, 2 - 2i, 4, 4 + 4i, 4 - 4i, 7, 7 + 7i, 7 - 7i, 8, 14, 14 + 14i, 14 - 14i, 28, 28 + 28i, 28 - 28i, 56
// 57 : 1, 3, 19, 57
// 58 : 1, 1 + i, 1 - i, 2, 2 + 5i, 2 - 5i, 3 + 7i, 3 - 7i, 4 + 10i, 4 - 10i, 5 + 2i, 5 - 2i, 7 + 3i, 7 - 3i, 10 + 4i, 10 - 4i, 29, 29 + 29i, 29 - 29i, 58
// 59 : 1, 59
// 60 : 1, 1 + i, 1 - i, 1 + 2i, 1 - 2i, 1 + 3i, 1 - 3i, 2, 2 + i, 2 - i, 2 + 2i, 2 - 2i, 2 + 4i, 2 - 4i, 2 + 6i, 2 - 6i, 3, 3 + i, 3 - i, 3 + 3i, 3 - 3i, 3 + 6i, 3 - 6i, 3 + 9i, 3 - 9i, 4, 4 + 2i, 4 - 2i, 4 + 8i, 4 - 8i, 5, 5 + 5i, 5 - 5i, 6, 6 + 2i, 6 - 2i, 6 + 3i, 6 - 3i, 6 + 6i, 6 - 6i, 6 + 12i, 6 - 12i, 6 + 18i, 6 - 18i, 8 + 4i, 8 - 4i, 9 + 3i, 9 - 3i, 10, 10 + 10i, 10 - 10i, 12, 12 + 6i, 12 - 6i, 12 + 24i, 12 - 24i, 15, 15 + 15i, 15 - 15i, 18 + 6i, 18 - 6i, 20, 24 + 12i, 24 - 12i, 30, 30 + 30i, 30 - 30i, 60
// 61 : 1, 5 + 6i, 5 - 6i, 6 + 5i, 6 - 5i, 61
// 62 : 1, 1 + i, 1 - i, 2, 31, 31 + 31i, 31 - 31i, 62
// 63 : 1, 3, 7, 9, 21, 63
// 64 : 1, 1 + i, 1 - i, 2, 2 + 2i, 2 - 2i, 4, 4 + 4i, 4 - 4i, 8, 8 + 8i, 8 - 8i, 16, 16 + 16i, 16 - 16i, 32, 32 + 32i, 32 - 32i, 64
// 65 : 1, 1 + 2i, 1 - 2i, 1 + 8i, 1 - 8i, 2 + i, 2 - i, 2 + 3i, 2 - 3i, 3 + 2i, 3 - 2i, 4 + 7i, 4 - 7i, 5, 7 + 4i, 7 - 4i, 8 + i, 8 - i, 10 + 15i, 10 - 15i, 13, 13 + 26i, 13 - 26i, 15 + 10i, 15 - 10i, 26 + 13i, 26 - 13i, 65
// 66 : 1, 1 + i, 1 - i, 2, 3, 3 + 3i, 3 - 3i, 6, 11, 11 + 11i, 11 - 11i, 22, 33, 33 + 33i, 33 - 33i, 66
// 67 : 1, 67
// 68 : 1, 1 + i, 1 - i, 1 + 4i, 1 - 4i, 2, 2 + 2i, 2 - 2i, 2 + 8i, 2 - 8i, 3 + 5i, 3 - 5i, 4, 4 + i, 4 - i, 4 + 16i, 4 - 16i, 5 + 3i, 5 - 3i, 6 + 10i, 6 - 10i, 8 + 2i, 8 - 2i, 10 + 6i, 10 - 6i, 16 + 4i, 16 - 4i, 17, 17 + 17i, 17 - 17i, 34, 34 + 34i, 34 - 34i, 68
// 69 : 1, 3, 23, 69
// 70 : 1, 1 + i, 1 - i, 1 + 2i, 1 - 2i, 1 + 3i, 1 - 3i, 2, 2 + i, 2 - i, 2 + 4i, 2 - 4i, 3 + i, 3 - i, 4 + 2i, 4 - 2i, 5, 5 + 5i, 5 - 5i, 7, 7 + 7i, 7 - 7i, 7 + 14i, 7 - 14i, 7 + 21i, 7 - 21i, 10, 14, 14 + 7i, 14 - 7i, 14 + 28i, 14 - 28i, 21 + 7i, 21 - 7i, 28 + 14i, 28 - 14i, 35, 35 + 35i, 35 - 35i, 70
// 71 : 1, 71
// 72 : 1, 1 + i, 1 - i, 2, 2 + 2i, 2 - 2i, 3, 3 + 3i, 3 - 3i, 4, 4 + 4i, 4 - 4i, 6, 6 + 6i, 6 - 6i, 8, 9, 9 + 9i, 9 - 9i, 12, 12 + 12i, 12 - 12i, 18, 18 + 18i, 18 - 18i, 24, 36, 36 + 36i, 36 - 36i, 72
// 73 : 1, 3 + 8i, 3 - 8i, 8 + 3i, 8 - 3i, 73
// 74 : 1, 1 + i, 1 - i, 1 + 6i, 1 - 6i, 2, 2 + 12i, 2 - 12i, 5 + 7i, 5 - 7i, 6 + i, 6 - i, 7 + 5i, 7 - 5i, 12 + 2i, 12 - 2i, 37, 37 + 37i, 37 - 37i, 74
// 75 : 1, 1 + 2i, 1 - 2i, 2 + i, 2 - i, 3, 3 + 4i, 3 - 4i, 3 + 6i, 3 - 6i, 4 + 3i, 4 - 3i, 5, 5 + 10i, 5 - 10i, 6 + 3i, 6 - 3i, 9 + 12i, 9 - 12i, 10 + 5i, 10 - 5i, 12 + 9i, 12 - 9i, 15, 15 + 30i, 15 - 30i, 25, 30 + 15i, 30 - 15i, 75
//
// Look at a composite number, like 15 and its prime factors:
//      3 : 1, 3
//      5 : 1, 1 + 2i, 1 - 2i, 2 + i, 2 - i, 5
//     15 : 1, 1 + 2i, 1 - 2i, 2 + i, 2 - i, 3, 3 + 6i, 3 - 6i, 5, 6 + 3i, 6 - 3i, 15
//
// Regroup the Gaussian Integers for 15:
//     15 : {1, 1 + 2i, 1 - 2i, 2 + i, 2 - i, 5},   {3, 3 + 6i, 3 - 6i, 6 + 3i, 6 - 3i, 15}
// The first set is the same set as for n=5, and the second set is the same values scaled by 3.
//
// We can see this again with 18:
//      2 : 1, 1 + i, 1 - i, 2
//      9 : 1, 3, 9
//     18 : {1, 1 + i, 1 - i, 2},   {3, 3 + 3i, 3 - 3i, 6},   {9, 9 + 9i, 9 - 9i, 18}
// The Gaussian Integers for 18 are those for 2 scaled by the factors of 9.
//
// It makes sense that if you take all the Gaussian Integers for n=a and n=b and cross multiply
// them, you get the Gaussian Integers for n=a*b.  Conside n=10:
//      2 : 1, 1 + i, 1 - i, 2
//      5 : 1, 1 + 2i, 1 - 2i, 2 + i, 2 - i, 5
//     10 : {1, 1 + 2i, 1 - 2i, 2 + i, 2 - i,  5},
//          {1 + i, -1 + 3i, 3 - i, 1 + 3i, 3 + i, 5 + 5i},
//          {1 - i, 3 + i, -1 - 3i, 3 - i, 1 - 3i, 5 - 5i},
//          {2, 2 + 4i, 2 - 4i, 4 + 2i, 4 - 2i, 10 }
// I show all the cross products, which includes some duplicates and two with negative real components.
//
// One last example, for a power of a prime, n=25:
//      5 : 1, 1 + 2i, 1 - 2i, 2 + i, 2 - i, 5
//     25 : 1, 1 + 2i, 1 - 2i, 2 + i, 2 - i, 3 + 4i, 3 - 4i, 4 + 3i, 4 - 3i, 5, 5 + 10i, 5 - 10i, 10 + 5i, 10 - 5i, 25
// The cross products of 5 and 5 are:
//     25 : {1, 1 + 2i, 1 - 2i, 2 + i, 2 - i, 5},
//          {1 + 2i, -3 + 4i, 5, 0 + 5i, 4 + 3i, 5 + 10i},
//          {1 - 2i, 5, -3 - 4i, 4 - 3i, 0 - 5i, 5 - 10i},
//          {2 + i, 0 + 5i, 4 - 3i, 3 + 4i, 5, 10 + 5i},
//          {2 - i, 4 + 3i, 0 - 5i, 5, 3 - 4i, 10 - 5i},
//          {5, 5 + 10i, 5 - 10i, 10 + 5i, 10 - 5i, 25}
// This matches the Gaussian Integers for n=25, which you remove any with nonpositive real part.


using Value = int64_t;
struct Complex {
    Complex(Value real, Value imag) : re(real), im(imag) {}

    bool operator<(const Complex& rhs) const {
        return (re == rhs.re) ? (im < rhs.im) : (re < rhs.re);
    };

    Value re;
    Value im;
};

using GaussianIntSet = std::set<Complex>;
using NormLookup = std::map<Value, GaussianIntSet>;
using IntForPrimes = std::map<Value, GaussianIntSet>;
using IntSumsForN = std::vector<Value>;


// This function is used to find the Gaussian Integers that result from dividing
// each one that sums to a divisor of n.  There are times where the L2 norm of
// valid Gaussian Integers are greater than n, due to the product of n*a, n*b
// being divisible by the norm:
//
//        4        4   1 - i    4(1 - i)       4(1 - i)     4(1 - i)
//      ------ = ----- ----- = ----------- = ------------ = --------- = 2 - 2i
//      1 + 1i   1 + i 1 - i   1^2 - (i)^2   1^2 - (-1^2)   1^2 + 1^2
//
//      |2 - 2i| = 2^2 + 2^2 = 8 > 4
Complex operator/(Value n, const Complex& ab) {
    Value den{ ab.re * ab.re + ab.im * ab.im };
    return { n * ab.re / den, n * ab.im / den };
}


Complex operator*(const Complex& a, const Complex& b) {
    return { a.re * b.re - a.im * b.im, a.re * b.im + a.im * b.re };
}


std::ostream& operator<<(std::ostream& str, const Complex& ab) {
    str << "(" << ab.re << ", " << ab.im << ")";
    if (ab.im > 0)
        str << ", (" << ab.re << ", " << -ab.im << ")";
    return str;
}


std::ostream& operator<<(std::ostream& str, const GaussianIntSet& ints) {
    str << "{ ";
    for (const auto& i : ints)
        str << i << ", ";
    str << " }";
    return str;
}


Value get_set_sum(const GaussianIntSet& ints) {
    Value ret{ 0 };

    for (const auto& ab : ints) {
        ret += ab.re;
    }

    return ret;
}


class GaussianIntegers {
public:
    GaussianIntegers() {
        prime_ints_[1] = { {1, 0} };    // 1 has one factor, 1
        lut_[1] = { {1, 0} };           // 1 has one factor, 1
        sums_for_n_.push_back(0);       // 0 has no terms
        sums_for_n_.push_back(1);       // 1 has 1 divisor, 1
    }


    NormLookup get_gauss_int_norms(Value max_norm) {
        populate_lut(max_norm);

        return lut_;
    }


    GaussianIntSet get_gaussian_ints(Value n) {
        //if ((gaussian_ints_.size() <= n) || (gaussian_ints_[n].empty())) {
        //    CalculateGaussianInts(n);
        //}

        return prime_ints_[n];
    }


    Value s_n(Value n) {
        //if ((gaussian_ints_.size() <= n) || (gaussian_ints_[n].empty())) {
        //    CalculateGaussianInts(n);
        //}
        return get_set_sum(prime_ints_[n]);
    }


    Value solve(Value max_n) {
        auto primes = helper_.get_primes(max_n);
        std::cout << "Calculated " << primes.size() << " needed primes" << std::endl;
    
        populate_lut(max_n);
        std::cout << "Calculated " << lut_ .size() << " quadratic sums" << std::endl;

        if (max_n > sums_for_n_.size())
            sums_for_n_.resize(max_n + 1);

        for (Value n = 2; n <= max_n; ++n) {
            if ((n % 10'000) == 0)
                std::cout << n << std::endl;
            CalculateGaussianInts(n);
        }

        return std::accumulate(sums_for_n_.cbegin(), sums_for_n_.cend(), Value{ 0 });
    }


private:
    void populate_lut(Value max_n) {
        const Value max_root = std::sqrt(max_n);

        for (Value a = 1; a <= max_root; ++a) {
            for (Value b = a; b <= max_root; ++b) {
                Value sum{ a * a + b * b };
                if (lut_.find(sum) == lut_.cend())
                    lut_[sum] = {};
                lut_[sum].insert({ a, b });
                lut_[sum].insert({ a, -b });
                if (a != b) {
                    lut_[sum].insert({ b, a });
                    lut_[sum].insert({ b, -a });
                }
            }
        }
    }


    void CalculateGaussianIntsPrime(Value n) {
        GaussianIntSet ints_for_n;

        // If we already have the Gaussian Integers for n, nothing to do.
        if (prime_ints_.find(n) == prime_ints_.cend()) {
            if (lut_.rbegin()->first < n)
                populate_lut(n);

            // First add rational integer divisors of n.
            ints_for_n.insert({ 1, 0 });
            ints_for_n.insert({ n, 0 });

            // Then add Gaussian Integers whose L2-norm is n.
            // Are there any Gaussian Integers where a^2 + b^2 = n?
            if (lut_.find(n) != lut_.cend()) {
                // Iterate over Gaussian Integers with this norm.
                for (const auto& gi : lut_.at(n)) {
                    // Add this Gaussian Integer.
                    ints_for_n.insert(gi);
                    // Add the multiplicative inverse of this Gaussian Integer, which may have norm > n.
                    ints_for_n.insert(n / gi);

                    if ((gi.re != gi.im) && (gi.im > 0)) {
                        // The LUT only contains Gaussian Integers where b >= a,
                        // so if b > a then add b + ai and its inverse as solutions.
                        ints_for_n.insert({ gi.im, gi.re });
                        ints_for_n.insert(n / Complex{ gi.im, gi.re });
                    }
                }
            }
        }

        prime_ints_[n] = ints_for_n;
    }


    GaussianIntSet CalculateGaussianIntsComposite(Value n) {
        // For composite n, get prime factorization and make sure each prime's
        // Gaussian Integers have been calculated.
        auto factors = helper_.get_factorization_fast(n);

        // Start with 1 in the set of Gaussian Integers.
        GaussianIntSet ints_for_n;
        ints_for_n.insert({ 1, 0 });

        for (const auto& [prime, exp] : factors) {
            const auto& ints_for_prime = prime_ints_[prime];

            for (uint64_t e = 0; e < exp; ++e) {
                auto tmp = ints_for_n;
                for (const auto gi1 : tmp) {
                    for (const auto gi2 : ints_for_prime) {
                        // We only want to consider complex numbers with a positive real component.
                        auto prod = gi1 * gi2;
                        if (prod.re > 0)
                            ints_for_n.insert(prod);
                    }
                }
            }
        }

        return ints_for_n;
    }


    void CalculateGaussianInts(Value n) {
        if (helper_.is_prime(n)) {
            CalculateGaussianIntsPrime(n);
            sums_for_n_[n] = get_set_sum(prime_ints_.at(n));
//            std::cout << n << "\t: " << prime_ints_.at(n) << std::endl;
        }
        else {
            auto ints_for_n = CalculateGaussianIntsComposite(n);
            sums_for_n_[n] = get_set_sum(ints_for_n);
//            std::cout << n << "\t: " << ints_for_n << std::endl;
        }
    }

    ChunkedPrimeHelper<1'000'000> helper_;
    NormLookup lut_;
    IntForPrimes prime_ints_;
    IntSumsForN sums_for_n_;
};


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    GaussianIntegers gi;
    //    auto lut = gi.get_gauss_int_norms(5);
    //    for (const auto& [sum, ints] : lut) {
    //        std::cout << sum << "\t: ";
    //        for (const auto& ab : ints) {
    //            std::cout << ab << ", ";
    //        }
    //        std::cout << std::endl;
    //    }
    //}

    //{
    //    ChunkedPrimeHelper<10> helper;
    //    auto primes = helper.get_primes(101);
    //    for (const auto& p : primes)
    //        std::cout << p << " ";
    //    std::cout << std::endl;

    //    for (uint64_t n = 2; n <= 101; ++n) {
    //        std::cout << n << "\t" << helper.is_prime(n) << std::endl;
    //    }
    //}

    //{
    //    GaussianIntegers gi;
    //    for (Value n = 1; n <= 10; ++n) {
    //        Value s_n{ 0 };
    //        std::cout << n << "\t: ";
    //        auto ints = gi.get_gaussian_ints(n);
    //        for (const auto& ab : ints) {
    //            std::cout << ab << ", ";
    //            s_n += ab.re;
    //        }
    //        std::cout << ", s(n) = " << s_n << std::endl;
    //    }
    //}

    {
        GaussianIntegers gi;
//        auto sum = gi.solve(50);
//        auto sum = gi.solve(100'000);
        auto sum = gi.solve(100'000'000);
        std::cout << sum << std::endl;
    }
    return 0;

    //{
    //    std::cout << s_n(5) << std::endl;
    //    std::cout << s_n(4194304) << std::endl;
    //}
    //return 0;

//    {
//        Value sum_s_n{ 0 };
////        auto sums = solve(5);
//        auto sums = solve(75);
////        auto sums = solve(100'000);
////        auto sums = solve(100'000'000);
//        size_t n{ 1 };
//        for (auto iter = sums.cbegin() + 1; iter != sums.cend(); ++iter, ++n) {
//            Value s_n{ 0 };
//            std::cout << n << "\t: ";
//            for (const auto& [a, b] : *iter) {
//                s_n += a + (b > 0) * a;
//                std::cout << "(" << a << ", " << b << "), ";
//                if (b > 0)
//                    std::cout << "(" << a << ", " << -b << "), ";
//            }
//
//            std::cout << "sum = " << s_n << std::endl;
//            sum_s_n += s_n;
//        }
//        std::cout << "Sum s_n = " << sum_s_n << std::endl;
//    }
}


//(2, 4)
//(2, -4)
//(3, 6)
//(3, -6)
//(4, 2)
//(4, -2)
//(6, 3)
//(6, -3)

