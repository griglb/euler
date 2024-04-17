// On Sunday 5 April 2020 the Project Euler membership first exceeded one million members.
// We would like to present this problem to celebrate that milestone.
// Thank you to everyone for being a part of Project Euler.
//
// The number 6 can be written as a palindromic sum in exactly eight different ways:
//      (1, 1, 1, 1, 1, 1), (1, 1, 2, 1, 1), (1, 2, 2, 1), (1, 4, 1), (2, 1, 1, 2), (2, 2, 2), (3, 3), (6)
//
// We shall define a twopal to be a palindromic tuple having at least one element with a value of 2.
// It should also be noted that elements are not restricted to single digits.
// For example, (3, 2, 13, 6, 13, 2, 3) is a valid twopal.
//
// If we let t(n) be the number of twopals whose elements sum to n, then it can be seen that t(6) = 4:
//              (1, 1, 2, 1, 1), (1, 2, 2, 1), (2, 1, 1, 2), (2, 2, 2)
//
// Similarly, t(20) = 824.
//
// In searching for the answer to the ultimate question of life, the universe, and everything, it can be
// verified that t(42) = 1999923, which happens to be the first value of t(n) that exceeds one million.
//
// However, your challenge to the "ultimatest" question of life, the universe, and everything is to find
// the least value of n > 42 such that is divisible by one million.


#include <iostream>


// The first question is whether n is odd or even.  If n is odd, then all the palindromic sets must have
// an odd number of elements, with the middle number being odd.  So for odd n, the twopals all have an odd
// central number and matching pairs of 2s in the two symmetric halves.
// When n is even, on the other hand, there will be a set of twopals that have 2 as the central number and
// then all the various sets of numbers that sum to n/2 - 1.  This is in addition to all the even sized
// sets, where each half sums to n/2..

// Let's start by enumerating the sets that contribute to t(n) to look for patterns:
//      t(1) = 0
//      t(2) = 1  -  {2}
//      t(3) = 0
//      t(4) = 2  -  {2, 2}, {1, 2, 1}
//      t(5) = 1  -  {2, 1, 2}
//      t(6) = 4  -  {1, 1, 2, 1, 1}, {1, 2, 2, 1}, {2, 1, 1, 2}, {2, 2, 2}
//      t(7) = 2  -  {1, 2, 1, 2, 1}, {2, 1, 1, 1, 2}
//      t(8) = 9  -  {1, 1, 1, 2, 1, 1, 1}, {1, 1, 2, 2, 1, 1}, {1, 2, 1, 1, 2, 1}, {2, 1, 1, 1, 1 ,2},
//                   {1, 2, 2, 2, 1}, {2, 1, 2, 1, 2}, {2, 2, 2, 2}, {3, 2, 3}, {2, 4, 2}
//
// We can see a few recursion patterns to generate the sets for t(n) from t(n-2):
//      - insert matching 1's in any position of the halves
//      - add 1 to symmetric elements in each half
//
// To demonstrate, here's how to generate t(6) from t(4):
//      {2, 2} -> {1, 2, 2, 1}, {2, 1, 1, 2}
//      {1, 2, 1} -> {1, 1, 2, 1, 1}, {1, 1, 2, 1, 1}, {2, 2, 2}
//
// Next, generate t(8) from t(6):
//      {1, 1, 2, 1, 1} -> {1, 1, 1, 2, 1, 1, 1}, {1, 1, 1, 2, 1, 1, 1}, {1, 1, 1, 2, 1, 1, 1}, {2, 1, 2, 1, 2}, {1, 2, 2, 2, 1}
//      {1, 2, 2, 1} -> {1, 1, 2, 2, 1, 1}, {1, 1, 2, 2, 1, 1}, {1, 2, 1, 1, 2, 1}, {2, 2, 2, 2}
//      {2, 1, 1, 2} -> {1, 2, 1, 1, 2, 1}, {2, 1, 1, 1, 1 ,2}, {2, 2, 2, 2}
//      {2, 2, 2} -> {1, 2, 2, 2, 1}, {2, 1, 2, 1, 2}, {3, 2, 3}, {2, 4, 2}
//
// This works, but it doesn't scale well and requires a lot of bookkeeping.  For example, you have to know
// that when {2, 1, 1, 2} has the 2s augmented that it is no longer a twopal as {3, 1, 1, 3}.
// A more closed form expression is needed.

// Let's go back to the rules for the even and odd values of n.  When n is odd, the twopals have an
// odd cardinality and the middle value in the set k must be odd.  When n is even, the twopals can
// have an even number of elements.  In both of these cases, we know the two symmetric halves must
// contain at least one 2, so all we need to do is count these sets.
// Define S(x) the number of sets of natural numbers that sum to x and have at least one 2.
// Define P(x) the number of sets of natural numbers that sum to x.
// Then we know that:
//      t(n) = S((n-1)/2) + S((n-3)/2 + S((n-5)/2) + ... + S(2)             for odd n
//      t(n) = S(n/2) + P((n-2)/2) + S((n-4)/2) + S((n-6)/2) + ... + S(2)   for even n
//
// Let's examine S(n) and P(n):
//      S(1) = 0
//      S(2) = 1  -  {2}
//      S(3) = 2  -  {1, 2}, {2, 1}
//      S(4) = 4  -  {1, 1, 2}, {1, 2, 1}, {2, 1, 1},
//                   {2, 2}
//      S(5) = 9  -  {1, 1, 1, 2}, {1, 1, 2, 1}, {1, 2, 1, 1}, {2, 1, 1, 1},
//                   {1, 2, 2}, {2, 1, 2}, {2, 2, 1},
//                   {2, 3}, {3, 2}
//      S(6) = 20 -  {1, 1, 1, 1, 2}, {1, 1, 1, 2, 1}, {1, 1, 2, 1, 1}, {1, 2, 1, 1, 1}, {2, 1, 1, 1, 1},
//                   {1, 1, 2, 2}, {1, 2, 1, 2}, {1, 2, 2, 1}, {2, 1, 1, 2}, {2, 1, 2, 1}, {2, 2, 1, 1},
//                   {1, 2, 3}, {1, 3, 2}, {2, 1, 3}, {2, 3, 1}, {3, 1, 2}, {3, 2, 1},
//                   {2, 2, 2},
//                   {2, 4}, {4, 2}
//
//      P(1) = 1  -  {1}
//      P(2) = 2  -  {1, 1}, {2}
//      P(3) = 4  -  {1, 1, 1}, {1, 2}, {2, 1}, {3}
//      P(4) = 8  -  {1, 1, 1, 1},
//                   {1, 1, 2}, {1, 2, 1}, {2, 1, 1},
//                   {1, 3}, {3, 1},
//                   {2, 2},
//                   {4}
//      P(5) = 16 -  {1, 1, 1, 1, 1},
//                   {1, 1, 1, 2}, {1, 1, 2, 1}, {1, 2, 1, 1}, {2, 1, 1, 1},
//                   {1, 2, 2}, {2, 1, 2}, {2, 2, 1},
//                   {1, 1, 3}, {1, 3, 1}, {3, 1, 1},
//                   {1, 4}, {4, 1},
//                   {2, 3}, {3, 2},
//                   {5}
//      P(6) = 32 -  {1, 1, 1, 1, 1, 1},
//                   {1, 1, 1, 1, 2}, {1, 1, 1, 2, 1}, {1, 1, 2, 1, 1}, {1, 2, 1, 1, 1}, {2, 1, 1, 1, 1},
//                   {1, 1, 2, 2}, {1, 2, 1, 2}, {1, 2, 2, 1}, {2, 1, 1, 2}, {2, 1, 2, 1}, {2, 2, 1, 1},
//                   {1, 1, 1, 3}, {1, 1, 3, 1}, {1, 3, 1, 1}, {3, 1, 1, 1},
//                   {1, 2, 3}, {1, 3, 2}, {2, 1, 3}, {2, 3, 1}, {3, 1, 2}, {3, 2, 1},
//                   {1, 1, 4}, {1, 4, 1}, {4, 1, 1},
//                   {2, 2, 2},
//                   {1, 5}, {5, 1},
//                   {2, 4}, {4, 2},
//                   {3, 3},
//                   {6}
//
// The P(n) function looks like it evaluates to P(n) = 2^(n-1).
// We can prove this by construction. For each set in P(n-1), we can construct 2 unique sets in P(n):
//      - add a 1 to the set at the end
//      - increment the last element of the set by 1
// This shows that P(n) = 2 * P(n-1), QED
//
// The S(n) function is more complicated.  We can see how the set of sets is a subset of the corresponding
// set of sets for P(n), but we have to filter out those that don't contain any 2s.

// Going back to the expression for t(n), let's see if we can show t(6) = 4.
// 6 is even so we use that form:
//      t(n) = S(n/2) + P((n-2)/2) + S((n-4)/2) + S((n-6)/2) + ... + S(2)   for even n
//      t(6) = S(3) + P(2)
//      t(6) = 2 + 2 = 4
// We can see this from the sets too:
// For S(3) we have {1, 2}, {2, 1}, which create the even sized palindromes {1, 2, 2, 1}, {2, 1, 1, 2}.
// For P(2) we have {1, 1}, {2}, which create the odd sized palindromes {1, 1, 2, 1, 1}, {2, 2, 2}.
// The union of those sets of sets is identical to the full set for t(6).

// Next let's see if we can show t(20) = 824.
// 20 is even so we use that form:
//      t(n) = S(n/2) + P((n-2)/2) + S((n-4)/2) + S((n-6)/2) + ... + S(2)   for even n
//      t(20) = S(10) + P(9) + S(8) + S(7) + S(6) + S(5) + S(4) + S(3) + S(2)
// We've already calculated S(n) up to 6, and know P(9) = 2^8 = 256, so the missing pieces are S(10),
// S(8), and S(7). We know S(10) counts a subset of the sets that P(10) does, and P(10) = 2^9 = 512.
// Let's brute force is and enumerate the sets for S(7), S(8), and S(10):
//      S(7) = 43 -  {1, 1, 1, 1, 1, 2}, {1, 1, 1, 1, 2, 1}, {1, 1, 1, 2, 1, 1}, {1, 1, 2, 1, 1, 1}, {1, 2, 1, 1, 1, 1}, {2, 1, 1, 1, 1, 1},
//                   {1, 1, 1, 2, 2}, {1, 1, 2, 1, 2}, {1, 1, 2, 2, 1}, {1, 2, 1, 1, 2}, {1, 2, 1, 2, 1},
//                   {1, 2, 2, 1, 1}, {2, 1, 1, 1, 2}, {2, 1, 1, 2, 1}, {2, 1, 2, 1, 1}, {2, 2, 1, 1, 1},
//                   {1, 2, 2, 2}, {2, 1, 2, 2}, {2, 2, 1, 2}, {2, 2, 2, 1},
//                   {1, 1, 2, 3}, {1, 1, 3, 2}, {1, 2, 1, 3}, {1, 2, 3, 1}, {1, 3, 1, 1}, {1, 3, 2, 1},
//                   {2, 1, 1, 3}, {2, 1, 3, 1}, {2, 3, 1, 1}, {3, 1, 1, 2}, {3, 1, 2, 1}, {3, 2, 1, 1},
//                   {1, 2, 4}, {1, 4, 2}, {2, 1, 4}, {2, 4, 1}, {4, 1, 2}, {4, 2, 1},
//                   {2, 2, 3}, {2, 3, 2}, {3, 2, 2},
//                   {2, 5}, {5, 2}
//                   6!/5! + 5!/3!/2! + 4!/3! + 4!/2! + 3! + 3!/2! + 2!
//                   6 + 10 + 4 + 12 + 6 + 3 + 2
//
//      S(8) = 91 -  {1, 1, 1, 1, 1, 1, 2}  = 7!/6!     =   7
//                   {1, 1, 1, 1, 2, 2}     = 6!/4!/2!  =   15
//                   {1, 1, 2, 2, 2}        = 5!/3!/2!  =   10
//                   {2, 2, 2, 2}           = 4!/4!     =   1
//                   {1, 1, 1, 2, 3}        = 5!/3!     =   20
//                   {1, 2, 2, 3}           = 4!/2!     =   12
//                   {1, 1, 2, 4}           = 4!/2!     =   12
//                   {2, 2, 4}              = 3!/2!     =   3
//                   {1, 2, 5}              = 3!        =   6
//                   {2, 3, 3}              = 3!/2!     =   3
//                   {2, 6}                 = 2!        =   2
//
//      S(10) = 398 -  {1, 1, 1, 1, 1, 1, 1, 1, 2}  = 9!/8!     =   9
//                     {1, 1, 1, 1, 1, 1, 2, 2}     = 8!/6!/2!  =   28
//                     {1, 1, 1, 1, 2, 2, 2}        = 7!/4!/3!  =   35
//                     {1, 1, 2, 2, 2, 2}           = 6!/2!/4!  =   15
//                     {2, 2, 2, 2, 2}              = 5!/5!     =   1
//                     {1, 1, 1, 1, 1, 2, 3}        = 7!/5!     =   42
//                     {1, 1, 1, 2, 2, 3}           = 6!/3!/2!  =   60
//                     {1, 2, 2, 2, 3}              = 5!/3!     =   20
//                     {1, 1, 1, 1, 2, 4}           = 6!/4!     =   30
//                     {1, 1, 2, 2, 4}              = 5!/2!/2!  =   30
//                     {2, 2, 2, 4}                 = 4!/3!     =   4
//                     {1, 1, 1, 2, 5}              = 5!/3!     =   20
//                     {1, 2, 2, 5}                 = 4!/2!     =   12
//                     {1, 1, 2, 6}                 = 4!/2!     =   12
//                     {2, 2, 6}                    = 3!/2!     =   3
//                     {1, 2, 7}                    = 3!        =   6
//                     {2, 8}                       = 2!        =   2
//                     {1, 1, 2, 3, 3}              = 5!/2!/2!  =   30
//                     {2, 2, 3, 3}                 = 4!/2!/2!  =   6
//                     {1, 2, 3, 4}                 = 4!        =   24
//                     {2, 3, 5}                    = 3!        =   6
//                     {2, 4, 4}                    = 3!/2!     =   3
//
//      t(20) = S(10) + P(9) + S(8) + S(7) + S(6) + S(5) + S(4) + S(3) + S(2)
//      t(20) = 398 + 256 + 91 + 43 + 20 + 9 + 4 + 2 + 1 = 824 QED
//

int main()
{
    std::cout << "Hello World!\n";
}
