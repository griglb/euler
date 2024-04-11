// 70 coloured balls are placed in an urn, 10 for each of the seven rainbow colours.
//
// What is the expected number of distinct colours in 20 randomly picked balls?
//
// Give your answer with nine digits after the decimal point(a.bcdefghij).


#include <iostream>
#include <set>
#include <vector>


// Start small, with 7 balls, one of each color.  Then choosing 2 balls randomly
// will always result in 2 colors.

// Then have 2 balls of each color, 14 total, and pick 4.
// The 14 balls have 14! unique orders, but because of duplicate colors there are
// many less discernable orders.
// Let's start with only 3 colors to demonstrate.
// Call the balls R1, R2, G1, G2, B1, B2.
// There are 6! = 720 permutatipns, but we can show how these sequences are all
// visibly the same:
//      R1, R2, G1, G2, B1, B2
//      R2, R1, G1, G2, B1, B2
//      R1, R2, G2, G1, B1, B2
//      R1, R2, G1, G2, B2, B1
//      R2, R1, G2, G1, B1, B2
//      R2, R1, G1, G2, B2, B1
//      R1, R2, G2, G1, B2, B1
//      R2, R1, G2, G1, B2, B1
// Each color can independently be flipped, so you can think of it as 3 boolean
// decisions - do I flip color X?
// So in this case there are 2^8 permutations that look the same, and the total
// number of visibly unique sequences is 6! / 2^3 = 720 / 8 = 90.
// The 2^3 is actually (2!)^3, but 2! = 2 so its hard to tell.  Let's extend this
// to 3 balls of each color.  In each color, the 3 balls can be in any order,
// which is 3! = 6 permutations.  So in the case of 3 balls each of 3 colors,
// the total number of permutations is:
//                    9 * 8 * 7 * 6 * 5 * 4 * 3 * 2   
//      9! / (3!)^3 = ----------------------------- = 7 * 6 * 5 * 4 * 2 = 1680
//                        3 * 2 * 3 * 2 * 3 * 2
//
// Returning to the 14 balls, we can see that it is
//                     14 * 13 * 12 * 11 * 10 * 9 * 8 * 7 * 6 * 5 * 4 * 3 * 2
//      14! / (2!)^7 = ------------------------------------------------------ = 7 * 13 * 6 * 11 * 5 * 9 * 4 * 7 * 3 * 5 * 2 * 3 = 681'080'400
//                                  2 * 2 * 2 * 2 * 2 * 2 * 2
//
// We can see that this quickly gets out of control, we don't want to enumerate all
// 6.8E8 sequences and calculate the average number of unique colors in the first 4.
// Instead we will try a forward probability approach.  We don't care what color
// the first ball is, just if the next is the same or different.
// Let's return to the 2 balls of 3 colors, now called X, X', Y, Y', Z, Z'.  In this
// case X is the first color pulled, Y the second, and Z the third (if at all).  The
// first ball of a given color has no prime, and the second has the prime.
// So for a sequence of 4 balls pulled out, the first will always be X.  The question is
// then what are the odds X' or Y is second?  We can see easily here that there are 5
// remaining balls, so the odds of X' are 1/4, and the odds of Y are 4/5.  So we have
// the two sequences and probabilities:
//      X X' = 1/5
//      X Y  = 4/5
// Now look at the two sequences separately.  In the first case, we have run out of X
// colored balls, so there a 100% chance of a different color Y being drawn.  Things
// get more complicated for the second sequence, we could pull X', Y', or Z.  So in a
// sequence of 3 we have these odds:
//      X X' Y  = 1/5 * 1 = 1/5
//      X Y  X' = 4/5 * 1/4 = 1/5
//      X Y  Y' = 4/5 * 1/4 = 1/5
//      X Y  Z  = 4/5 * 2/4 = 2/5
// First, we check that the odds sitll sum to 1, and they do.
// Now we add the fourth ball.  In the first two sequences, it could be Y' or Z, in the
// third it can be X' or Z, and in the fourth it can be X', Y', or Z':
//      X X' Y  Y' = 1/5 * 1 * 1/3 = 1/15
//      X X' Y  Z  = 1/5 * 1 * 2/3 = 2/15
//      X Y  X' Y' = 4/5 * 1/4 * 1/3 = 1/15
//      X Y  X' Z  = 4/5 * 1/4 * 2/3 = 2/15
//      X Y  Y' X' = 4/5 * 1/4 * 1/3 = 1/15
//      X Y  Y' Z  = 4/5 * 1/4 * 2/3 = 2/15
//      X Y  Z  X' = 4/5 * 2/4 * 1/3 = 2/15
//      X Y  Z  Y' = 4/5 * 2/4 * 1/3 = 2/15
//      X Y  Z  Z' = 4/5 * 2/4 * 1/3 = 2/15
// Again, verify the odds all sum to 1, and they do.
// The average number of colors can be calculated by adding the odds for the sequences
// that do and don't contain a Z.  Of the 9 sequences, 3 only have X and Y, and the other
// 9 have a Z.  So there is a 1/15 + 1/15 + 1/15 = 1/5 chance of 2 colors and 12/15 = 4/5
// chance of 3 colors.  So the average number of colors = 2 * 1/5 + 3 * 4/5 = 14/5 colors.

// Let's confirm this by enumerating all 90 sequences of 3 colors, to count how many
// have 2 and 3 colors in the first 4 positions:
//      R R G G B B *      G R R G B B *      B R R G G B
//      R R G B G B        G R R B G B        B R R G B G
//      R R G B B G        G R R B B G        B R R B G G *
//      R R B G G B        G R G R B B *      B R G R G B
//      R R B G B G        G R G B R B        B R G R B G
//      R R B B G G *      G R G B B R        B R G G R B
//      R G R G B B *      G R B R G B        B R G G B R
//      R G R B G B        G R B R B G        B R G B R G
//      R G R B B G        G R B G R B        B R G B G R
//      R G G R B B *      G R B G B R        B R B R G G *
//      R G G B R B        G R B B R G        B R B G R G
//      R G G B B R        G R B B G R        B R B G G R
//      R G B R G B        G G R R B B *      B G R R G B
//      R G B R B G        G G R B R B        B G R R B G
//      R G B G R B        G G R B B R        B G R G R B
//      R G B G B R        G G B R R B        B G R G B R
//      R G B B R G        G G B R B R        B G R B R G
//      R G B B G R        G G B B R R *      B G R B G R
//      R B R G G B        G B R R G B        B G G R R B
//      R B R G B G        G B R R B G        B G G R B R
//      R B R B G G *      G B R G R B        B G G B R R *
//      R B G R G B        G B R G B R        B G B R R G
//      R B G R B G        G B R B R G        B G B R G R
//      R B G G R B        G B R B G R        B G B G R R *
//      R B G G B R        G B G R R B        B B R R G G *
//      R B G B R G        G B G R B R        B B R G R G
//      R B G B G R        G B G B R R *      B B R G G R
//      R B B R G G *      G B B R R G        B B G R R G
//      R B B G R G        G B B R G R        B B G R G R
//      R B B G G R        G B B G R R *      B B G G R R *
// The sequences with only 2 colors in the first 4 positions are asterisked, and
// there are 18 out of 90, which matches the 1/5 calculated above. 

// We can calculate the permutations for different numbers of colors in the
// first N positions, instead of taking the probabilistic approach.
// Conside the same 6 balls, and the first 4 positions.
// Since there are only 2 balls of each color, we know there can't be only 1
// color in all 4 positions.  On the other hand, we can do 2 each of 2 colors
// and 3 colors in a 2-1-1 configuration.  We also know we can't do 4 colors,
// since there are only 3 colors in the mix.  Let's consider 2 & 3 colors:
//
// For 2 colors, we know there are 4! / (2!)^2 = 6 visibly different permutations:
//    XXYY, XYXY, XYYX, YXXY, YXYX, YYXX
// To enumerate the color assignment permutations, any of the 3 colors can be X,
// and either of the remaining 2 can be Y.  But due ot the completeness of the
// positional permutations we can swap X & Y, so we need to divide by 2 to avoid
// double counting.  So there are 3 * 2 / 2 = 3 color permutations.
// Lastly, there are the remaining balls.  There are 2 balls left, and we know they
// are both the same color, so there is only 1 permutation for them.  This leads
// to the 6 positional permutations times the 3 color permutations times the 1
// permutation for the extra balls to get the same 18 visibly different color sequences.
//
// For 3 colors, there are 4! / (2! * 1! * 1!) = 12 positional permutations:
//    XXYZ, XXZY, XYXZ, XYZX, XZXY, XZYX, YXXZ, YXZX, YZXX, ZXXY, ZXYX, ZYXX
// For the color assignments, we once again have 3 colors for X, 2 colors for Y, and
// 1 color for Z.  But since Y & Z have the same cardinality we have to divide
// by 2 to avoid double counting.  So there are 3 * 2 * 1 / 2 = 3 color assingments.
// This time, however, the 2 extra balls have different colors.  So we have
// 2! / (1! * 1!) = 2 permutations for them.  There might only be 12 * 3 = 36 unique
// color sequences for the first 4 balls, but when we consider all 6 balls we
// get 12 * 3 * 2 = 72 visibly different sequences, which matches above.

// Return to the 2 balls each of 7 colors, and 4 being selected.
// Once again, we know they can't all be the same color, and that 2 each of 2
// colors and 3 colors in a 2-1-1 configuration are possible.  But this time we can
// also do 4 colors, with 1 ball of each.  But 5 or more colors are not possible
// with only 4 balls.
//
// For 2 colors, we have the same 4! / (2!)^2 = 6 positional permutations.  The
// color assingment is different this time.  We can have any of 7 colors for X
// and 6 colors for Y, with the same division by 2 to account for X/Y swapping.
// So there are 7 * 6 / 2 = 21 color assignment permutations.
// We now have 10 balls, 2 each of 5 colors with 10! / (2!)^5 permutations.
// This means the total number of visibily different permutations with only 2
// colors in the first 4 positions is 
//    6 * 21 * 10! / 32 = 14'288'400
//
// For 3 colors, there are the same 4! / (2! * 1! * 1!) = 12 positional permutations.
// This time the color assignment allows 7 colors for X, 6 for Y, and 5 for Z.
// But again since Y & Z have the same cardinality we divide by 2, yielding
// 7 * 6 * 5 / 2 = 105 permutations.
// There are still 10 extra balls, but this time the color cardinalities are
// 2-2-2-2-1-1.  So this leads to 10! / (2!^4 * 1!^2) permutations.
// This means the total number of visibily different permutations with only 2
// colors in the first 4 positions is 
//    12 * 105 * 10! / 16 = 285'768'000
//
// For 4 colors, we now have 4! / (1!)^4 = 24 positional permutations.
// For the color assignments, we can have 7 colors for X, 6 for Y, 5 for Z, and 4
// for W, but since all 4 colors have the same cardinality we divide by 4! to
// prevent double counting, yielding 7 * 6 * 5 * 4 / (4 * 3 * 2) = 35 color
// assignment permutations.
// There are again 10 extra balls, but now the color cardinalities are
// 2-2-2-1-1-1-1, which leads to 10! / (2!^3 * 1!^4) permutations.
// This means the total number of visibily different permutations with only 2
// colors in the first 4 positions is 
//    24 * 35 * 10! / 8 = 381'024'000
//
// Therefore the total number of permutations is
//    14'288'400 + 285'768'000 + 381'024'000 = 681'080'400
// This matches the total calculated above!

// No how does one use this information to calculate the mean number of colors?
// We use the number of permutations for each color count to calculate a weighted
// average.  In this case it is 
//    (2 * 14'288'400 + 3 * 285'768'000 + 4 * 381'024'000) / 681'080'400 = 
//       (28'576'800 + 857'304'000 + 1'524'096'000) / 681'080'400 = 3.538461538

// For the real problem, we have 10 balls each of 7 colors, and want to draw 20.
// We still can't have all 20 be the same color, but we can have 2, 3, 4, 5, 6, or
// 7 colors in the first 20.  Except for the 10-10 case of 2 colors, there will
// be multiple ways to define the positional permutations in those firt 20:
// 10-10
// 10-9-1
// 10-8-2
// 10-7-3
// 10-6-4
// 10-5-5
//  9-9-2
//  9-8-3
//  9-7-4
//  9-6-5
//  8-8-4
//  8-7-5
//  8-6-6
//  7-7-6
// 10-8-1-1
// 10-7-2-1
// 10-6-3-1
// 10-6-2-2
// 10-5-4-1
// 10-5-3-2
// 10-4-4-2
// 10-4-3-3
//  9-9-1-1
//  9-8-2-1
//  9-7-3-1
//  9-7-2-2
//  9-6-4-1
//  9-6-3-2
//  9-5-5-1
//  9-5-4-2
//  9-5-3-3
//  9-4-4-3
//  8-8-3-1
//  8-8-2-2
//  8-7-4-1
//  8-7-3-2
//  8-6-5-1
//  8-6-4-2
//  8-6-3-3
//  8-5-5-2
//  8-5-4-3
//  8-4-4-4
//  7-7-5-1
//  7-7-4-2
//  7-7-3-3
//  7-6-6-1
//  7-6-5-2
//  7-6-4-3
//  7-5-4-3
//  6-6-6-2
//  6-6-5-3
//  6-6-4-4
//  6-5-5-4


using PositionalCounts = std::multiset<int16_t>;
using AllPositionalCounts = std::vector<PositionalCounts>;

AllPositionalCounts get_counts(int16_t sum, int16_t num_balls, int16_t num_colors) {



}


int main()
{
    std::cout << "Hello World!\n";
}
