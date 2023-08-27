// Taking three different letters from the 26 letters of the alphabet, character
// strings of length three can be formed.
// Examples are 'abc', 'hat' and 'zyx'.
// When we study these three examples we see that for 'abc' two characters come
// lexicographically after its neighbour to the left.
// For 'hat' there is exactly one character that comes lexicographically after its
// neighbour to the left.
// For 'zyx' there are zero characters that come lexicographically after its
// neighbour to the left.
// In all there are 10400 strings of length 3 for which exactly one character comes
// lexicographically after its neighbour to the left.
//
// We now consider strings of n <= 26 different characters from the alphabet.
// For every n, p(n) is the number of strings of length n for which exactly one
// character comes lexicographically after its neighbour to the left.
//
// What is the maximum value of p(n)?


#include <array>
#include <cstdint>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <vector>

#include "big_int.h"
#include "combinatorics.h"


// We know that by definition p(1) = 0, because there is no neighbour to the left.

// For p(2), there are 26^2 = 676 possible strings, and exactly 1 left neighbour in each.
// We can look at this in terms of triangle numbers.  For each first letter we know how
// many second letters can follow it (26 - the position of the first).  So 'a' can be
// be followed by 25 letters, 'b' by 24, 'c' by 23, ... down to 'y' followed by 1 and
// 'z' by 0.  This means there are 25 + 24 + 23 + ... + 2 + 1 + 0 strings that qualify,
// which we know as T(25) = 25 * 26 / 2 = 325.  The brute force approach confirms this.

// For p(3), we can brute force it again, but this approach is not sustainable to p(26).
// Instead, let's look at all the 2 letter words, but split them into 2 subsets - those
// with 1 character that is lexicographically after its neighbour to the left and those
// with 0 characters.  Furthermore, manage these 2 subsets by their last letter.
// So 'aa' goes in the 0 bin under 'a', while 'ab' goes in the 1 bin under 'b', ...
// We can then iterate over the 2 letter word bins, and look at how many 2 letter words
// end in each letter in the 0 bin.  We then know that there are (26 - position) 3-letter
// words that can start with that 2-letter prefix.  So we can iterate over all the letters
// that come after the 2nd letter and increment their count in the 3-letter bins for 1.
// After we've processed all the 2-letter bins, we can calculate the 0 bin values for
// the 3 letter words as (26 - 1-bin value).
//

// There are 2 ways we can constuct an n-letter word with one character that comes after
// its left neighbour:
//   - start with an (n-1)-letter word with one such character and append any letter
//     that does not come after the last letter of the prefix
//   - start with an (n-1)-letter word with no such characters and append any letter
//     that comes after the last letter of the prefix
// In order to repeat this process for (n+1)-letter words, we also need to keep track
// of how many n-letter words have 0 such characters.  This can be counted as
//   - start with an (n-1)-letter word with no such characters and append any letter
//     that does not come after the last letter of the prefix
//

// 3 letter words ending in 'a':
//   0 bin: sum of 0-bin 2 letter words in [0, 25] = 351
//   1 bin: sum of 1-bin 2 letter words in [0, 25] = 325
//   2 bin: 0 - the 3rd letter cannot come after 2nd

// 3 letter words ending in 'b':
//   0 bin: sum of 0-bin 2 letter words in [1, 25] = 325
//   1 bin: sum of 0-bin 2 letter words in [0, 0]
//        + sum of 1-bin 2 letter words in [1, 25] = 26 + 325 = 351
//   2 bin: either 3rd can't come after 2nd or 2nd can't come after 1st = 0

// 3 letter words ending in 'c':
//   0 bin: sum of 0-bin 2 letter words in [2, 25] = 300
//   1 bin: sum of 0-bin 2 letter words in [0, 1]
//        + sum of 1-bin 2 letter words in [2, 25] = 51 + 324 = 375
//   2 bin: only solution is 'abc', sum of 1-bin 2 letter words in [0, 1] = 1

// ****************************************************************************
// ***  WHOOPS!  I wasn't answering the right problem!
// I missed the first sentence in the problem statement, and allowed repeated
// letters in the words, which is not allowed.  The word 'aab' should not be
// counted, but I was.
// This explains why I kept getting 11700 for p(3) instead of 10400, not that
// I noticed that until implementing all the code for the wrong problem.

// To begin again...
// If we do not allow letters to be repeated, then this problem is not a
// 26^N problem but a P(26, N) = 26! / N! problem, which is actually much
// smaller as N grows.  This also explains why they ask for the max p(n),
// because I bet it exhibits parabolic behavior, wherease my original
// solution showed exponential growth.
//

// Observe the counts for the different bins, per terminal letter and in toto,
// for n in [2, 6]:

// For 2-letter words:
// a       25       0       25 = P(25, 1)
// b       24       1       25
// c       23       2       25
// d       22       3       25
// e       21       4       25
// f       20       5       25
// g       19       6       25
// h       18       7       25
// i       17       8       25
// j       16       9       25
// k       15      10       25
// l       14      11       25
// m       13      12       25
// n       12      13       25
// o       11      14       25
// p       10      15       25
// q        9      16       25
// r        8      17       25
// s        7      18       25
// t        6      19       25
// u        5      20       25
// v        4      21       25
// w        3      22       25
// x        2      23       25
// y        1      24       25
// z        0      25       25
// Sums = 325     325  == 325 * { 1  1 }
// Total of counts = 650 = 26 * 25 = P(26, 2)

// For 3-letter words:
// a       300      300      0      600 = 25 * 24 = P(25, 2)
// b       276      324      0      600
// c       253      346      1      600
// d       231      366      3      600
// e       210      384      6      600
// f       190      400     10      600
// g       171      414     15      600
// h       153      426     21      600
// i       136      436     28      600
// j       120      444     36      600
// k       105      450     45      600
// l        91      454     55      600
// m        78      456     66      600
// n        66      456     78      600
// o        55      454     91      600
// p        45      450    105      600
// q        36      444    120      600
// r        28      436    136      600
// s        21      426    153      600
// t        15      414    171      600
// u        10      400    190      600
// v         6      384    210      600
// w         3      366    231      600
// x         1      346    253      600
// y         0      324    276      600
// z         0      300    300      600
// Sums = 2600    10400   2600  == 2600 * { 1  4  1 }
// Total of counts = 15600 = 26 * 25 * 24 = P(26, 3)

// For 4-letter words:
// a       2300     9200    2300       0        13800 = 25 * 24 * 23 = P(25, 3)
// b       2024     9200    2576       0        13800
// c       1771     9154    2875       0        13800
// d       1540     9065    3194       1        13800
// e       1330     8936    3530       4        13800
// f       1140     8770    3880      10        13800
// g        969     8570    4241      20        13800
// h        816     8339    4610      35        13800
// i        680     8080    4984      56        13800
// j        560     7796    5360      84        13800
// k        455     7490    5735     120        13800
// l        364     7165    6106     165        13800
// m        286     6824    6470     220        13800
// n        220     6470    6824     286        13800
// o        165     6106    7165     364        13800
// p        120     5735    7490     455        13800
// q         84     5360    7796     560        13800
// r         56     4984    8080     680        13800
// s         35     4610    8339     816        13800
// t         20     4241    8570     969        13800
// u         10     3880    8770    1140        13800
// v          4     3530    8936    1330        13800
// w          1     3194    9065    1540        13800
// x          0     2875    9154    1771        13800
// y          0     2576    9200    2024        13800
// z          0     2300    9200    2300        13800
// Sums = 14950   164450  164450   14950  == 14950 * { 1 11 11 1 }
// Total of counts = 358800 = 26 * 25 * 24 * 23 = P(26, 4)

// For 5-letter words:
// a      12650    139150    139150     12650       0       303600 = 25 * 24 * 23 * 22 = P(25, 4)
// b      10626    133078    145222     14674       0       303600
// c       8855    126753    151041     16951       0       303600
// d       7315    120241    156541     19503       0       303600
// e       5985    113604    161662     22348       1       303600
// f       4845    106900    166350     25500       5       303600
// g       3876    100183    170557     28969      15       303600
// h       3060     93503    174241     32761      35       303600
// i       2380     86906    177366     36878      70       303600
// j       1820     80434    179902     41318     126       303600
// k       1365     74125    181825     46075     210       303600
// l       1001     68013    183117     51139     330       303600
// m        715     62128    183766     56496     495       303600
// n        495     56496    183766     62128     715       303600
// o        330     51139    183117     68013    1001       303600
// p        210     46075    181825     74125    1365       303600
// q        126     41318    179902     80434    1820       303600
// r         70     36878    177366     86906    2380       303600
// s         35     32761    174241     93503    3060       303600
// t         15     28969    170557    100183    3876       303600
// u          5     25500    166350    106900    4845       303600
// v          1     22348    161662    113604    5985       303600
// w          0     19503    156541    120241    7315       303600
// x          0     16951    151041    126753    8855       303600
// y          0     14674    145222    133078   10626       303600
// z          0     12650    139150    139150   12650       303600
// Sums = 65780   1710280   4341480   1710280   65780  == 65780 * { 1 26 66 26 1 }
// Total of counts = 7893600 = 26 * 25 * 24 * 23 * 22 = P(26, 5)

// A few big patterns emerge:
//   - the total counts for each bin are equal to a multiple of the 0-bin value
//   - the total counts for each bin form a palindromic sequence:
//          p(n; k) = p(n; n-1-k)

// Let's look more closely at the scale factors of the 0-bin value:
//      n = 2 : 1 + 1                                                                                       = 2 = 2!
//      n = 3 : 1 + 4 + 1                                                                                   = 6 = 3!
//      n = 4 : 1 + 11 + 11 + 1                                                                             = 24 = 4!
//      n = 5 : 1 + 26 + 66 + 26 + 1                                                                        = 120 = 5!
//      n = 6 : 1 + 57 + 302 + 302 + 57 + 1                                                                 = 720 = 6!
//      n = 7 : 1 + 120 + 1191 + 2416 + 1191 + 120 + 1                                                      = 5040 = 7!
//      n = 8 : 1 + 247 + 4293 + 15619 + 15619 + 4293 + 247 + 1                                             = 40320 = 8!
//      n = 9 : 1 + 502 + 14608 + 88234 + 156190 + 88234 + 14608 + 502 + 1                                  = 362880 = 9!
//      n = 10: 1 + 1013 + 47840 + 455192 + 1310354 + 1310354 + 455192 + 47840 + 1013 + 1                   = 3628800 = 10!
//      n = 11: 1 + 2036 + 152637 + 2203488 + 9738114 + 15724248 + 9738114 + 2203488 + 152637 + 2036 + 1    = 39916800 = 11!
//
// The scale factors for the word length n sum to n!.  This makes sense, because
// one way to generate all the permutations P(n, k) is to first generate the set
// of unique subsets C(n, k), and then take all possible orderings of each of those
// subsets, which is k! in size.

// Here was aren't worried about all the scale factors, just the second scale factor,
// for p(n; 1).  Let's look at that sequence more closely:
//      1, 4, 11, 26, 57, 120, 247, 502, 1013, 2036
// Look at the differences of adjacent terms:
//         3,  7, 15, 31,  63, 127, 255,  511, 1023
// We notice an interesting pattern here, each term is twice the previous, plus 1.
// If you compare these difference to the previous term, we see:
//      3 =    1 + 2
//      7 =    4 + 3
//     15 =   11 + 4
//     31 =   26 + 5
//     63 =   57 + 6
//    127 =  120 + 7
//    255 =  247 + 8
//    511 =  502 + 9
//   1023 = 1013 + 10
//
// We can calculate the scale factor s(n; 1) as:
//      s(n; 1) = 2 * s(n-1; 1) + n - 1
//      s(2; 1) = 1

// We have the scale factors, now we need the total count for the 0-bin.
// Let's look at those values more closely:
//        n=2   n=3     n=4     n=5
// a       25   300    2300   12650
// b       24   276    2076   10626
// c       23   253    1771    8855
// d       22   231    1540    7315
// e       21   210    1330    5985
// f       20   190    1140    4845
// g       19   171     969    3876
// h       18   153     816    3060
// i       17   136     680    2380
// j       16   120     560    1820
// k       15   105     455    1365
// l       14    91     364    1001
// m       13    78     286     715
// n       12    66     220     495
// o       11    55     165     330
// p       10    45     120     210
// q        9    36      84     126
// r        8    28      56      70
// s        7    21      35      35
// t        6    15      20      15
// u        5    10      10       5
// v        4     6       4       1
// w        3     3       1       0
// x        2     1       0       0
// y        1     0       0       0
// z        0     0       0       0
// Sums = 325  2600   14950   65780
//
// Let us call the values in this table C(n, l) where n >= 2 and l in ['a', 'z'].
//
// If we look carefully, we will notice that each column other than the first
// is cumulative a sum of the previous column, minus 1 row.  The makes sense
// based on what these numbers represent.
// We'll start with the first column, for 2 letter words.  This column holds
// the number of 2-letter words with no repeated letter that end with each
// letter, such that that terminal letter comes before the first letter.
// We can see that for 'a', there are 25 options in ['b', 'z'], for 'b' there
// are 24 in ['c', 'z'], ... for 'y' there is only 1 option 'z', and for
// 'z' there are no options since 'z' must come after the first letter that
// is in ['a', 'y'].  Therefore we have the function:
//      C(2, l) = 'z' - l
//
// The second column holds the counts of all 3-letter words that end in each
// letter and have every letter come before it's left neighbour.  We see this
// one ends in 0s for both 'y' and 'z', because there is no way 3 different
// letters can end in either of those and always be before their neighbours.
// If we look at the differences between values in this column, we see that
// they are equal to the values in the n=2 column, for the next letter.  This
// is because for any letter in this column, the total number of 3-letter words
// that end in it and are a monotonically decreasing function is the sum of
// the counts of all the 2-letter words that end in any subsequent letter.
// For example:
//      C(3, 't') = C(2, 'u') + C(2, 'v') + C(2, 'w') + C(2, 'x') + C(2, 'y') + C(2, 'z')
// There are 2 ways we can define C(3, l):
//      C(3, l) = sum(C(2, m); m = l+1 .. 'z')
//      C(3, l) = C(3, l+1) + C(2, l+1)
//
// The same arguments apply to all the columns for n >= 3..  The construction
// of an n-letter word ending in l is from all (n-1)-letter words ending in
// any letter after l.  So we have the recurrence relation for n >= 3:
//      C(n, l) = C(n, l+1) + C(n-1, l+1)

// Once we know all the C(n, l) values, we can sum each column to get the 0-bin
// value for that word length, and multiple by the scale factor for that value of n.


constexpr size_t kNumLetters{ 26 };

using Letter = char;
// LetterCounts keeps track of the the number of words that have N characters who
// come after their left neighbour.
// For example, LetterCounts[2] is the number of words with 2 such characters.
using LetterCounts = std::array<BigInt, kNumLetters>;
// WordCounts keeps track of the LetterCounts by the word's terminal letter,
// using 'a' for 0-based indexing.
// For example, WordCounts[3][2] is the number of words ending in 'd' with 2 such characters.
using WordCounts = std::array<LetterCounts, kNumLetters>;
// AllWordCounts keeps the WordCounts tables separated by word length.
// For example, AllWordCounts[4][3][2] is the number of 4 character words ending in 'd'
// with 2 such characters.
using AllWordCounts = std::vector<WordCounts>;


// This is the bad version, which allows repeating letters.
std::vector<std::string> get_good_strings_repeat(size_t len) {
    std::vector<std::string> ret;

    if (2 > len)
        return ret;

    if (2 == len) {
        for (char a = 'a'; a <= 'z'; ++a) {
            for (char b = 'a'; b <= 'z'; ++b) {
                if (b > a) {
                    ret.emplace_back(std::string({ a, b }));
                }
            }
        }
    }

    if (3 == len) {
        for (char a = 'a'; a <= 'z'; ++a) {
            for (char b = 'a'; b <= 'z'; ++b) {
                for (char c = 'a'; c <= 'z'; ++c) {
                    if ((b > a) && (c <= b)) {
                        ret.emplace_back(std::string({ a, b }));
                    }
                    if ((b <= a) && (c > b)) {
                        ret.emplace_back(std::string({ a, b }));
                    }
                }
            }
        }
    }

    return ret;
}


// This is the bad version, which allows repeating letters.
AllWordCounts get_word_counts_repeat(size_t max_len) {
    AllWordCounts ret;
    ret.resize(max_len + 1);

    // Skip ret[0] and ret[1], they are empty.

    if (max_len < 2)
        return ret;

    {
        // Get reference to the WordCounts map to update.
        auto& counts = ret[2];
        // For a given second letter L, we know that for the 0 bin
        // it is all the letters that don't come before L, or 27 - L.
        // For the 1 bin, it's all the letters that come before L, which
        // is L's 0-based index.
        // Populate WordCounts with 26 empty LetterCounts, one for each letter.
        for (Letter l = 0; l < kNumLetters; ++l)
            counts[l] = { BigInt{26 - l}, BigInt{l} };
    }

    for (size_t len = 3; len <= max_len; ++len) {
        const auto& prefix_counts = ret[len - 1];
        auto& counts = ret[len];

        // Populate WordCounts with 26 empty LetterCounts, one for each letter.
        for (Letter l = 0; l < kNumLetters; ++l) {
            for (size_t w = 0; w < kNumLetters; ++w)
                counts[w][l] = BigInt{ 0 };
        }

        // Iterate over the prefix bins by terminal letter.
        for (Letter suffix = 0; suffix < kNumLetters; ++suffix) {
            for (Letter prefix = suffix; prefix < kNumLetters; ++prefix) {
                counts[suffix][0] += prefix_counts.at(prefix)[0];
            }
        }
        for (size_t bin = 1; bin < len; ++bin) {
            for (Letter suffix = 0; suffix < kNumLetters; ++suffix) {
                for (Letter prefix = 0; prefix < suffix; ++prefix) {
                    counts[suffix][bin] += prefix_counts.at(prefix)[bin - 1];
                }
                for (Letter prefix = suffix; prefix < kNumLetters; ++prefix) {
                    counts[suffix][bin] += prefix_counts.at(prefix)[bin];
                }
            }
        }

        //for (Letter prefix = 0; prefix < kNumLetters; ++prefix) {
        //    for (Letter suffix = 0; suffix <= prefix; ++suffix) {
        //        for (size_t col = 0; col < len - 1; ++col) {
        //            counts[suffix][col] += prefix_counts.at(prefix)[col];
        //        }
        //    }
        //    for (Letter suffix = prefix + 1; suffix < kNumLetters; ++suffix) {
        //        for (size_t col = len-1; col < len; ++col) {
        //            counts[suffix][col] += prefix_counts.at(prefix)[col - 1];
        //        }
        //    }
        //}
    }

    return ret;
}


// This is the good version, which does not allow repeating letters.
std::vector<std::string> get_good_strings(size_t len) {
    std::vector<std::string> ret;

    if (2 > len)
        return ret;

    const auto all_words = get_permutations(26, len);

    for (const auto& word : all_words) {
        size_t count{ 0 };
        for (size_t i = 1; i < len; ++i) {
            count += word[i] > word[i - 1];
        }
        if (1 == count) {
            std::string w;
            for (const auto& ind : word) {
                w += ('a' + ind);
            }
            ret.push_back(w);
        }
    }

    return ret;
}


AllWordCounts get_word_counts(size_t max_len) {
    AllWordCounts ret;
    ret.resize(max_len + 1);

    // Skip ret[0] and ret[1], they are empty.

    if (max_len < 2)
        return ret;

    for (size_t len = 2; len <= max_len; ++len) {
        const auto all_words = get_permutations(26, len);

        for (const auto& word : all_words) {
            size_t count{ 0 };
            for (size_t i = 1; i < len; ++i) {
                count += word[i] > word[i - 1];
            }

            ++ret[len][word.back()][count];
        }
    }

    return ret;
}


std::vector<size_t> get_order_counts(size_t num) {
    std::vector<size_t> ret;
    ret.resize(num);

    const auto perms = get_permutations(num, num);

    for (const auto& perm : perms) {
        size_t count{ 0 };
        for (size_t i = 1; i < num; ++i) {
            count += perm[i] > perm[i - 1];
        }
        ++ret[count];
    }

    return ret;
}


std::vector<int64_t> get_bin_1_scale_factors(size_t max_len) {
    std::vector<int64_t> ret;
    ret.resize(max_len + 1);

    ret[0] = ret[1] = 0;

    for (size_t n = 2; n <= max_len; ++n)
        ret[n] = 2 * ret[n - 1] + n - 1;

    return ret;
}


std::vector<int64_t> get_bin_0_counts(size_t max_len) {
    std::vector<int64_t> ret;
    ret.resize(max_len + 1);

    ret[0] = ret[1] = 0;

    using Column = std::array<int64_t, kNumLetters>;
    std::array< Column, kNumLetters + 1> letter_counts;

    for (size_t r = 0; r < kNumLetters; ++r)
        letter_counts[1][r] = 1;

    for (int64_t col = 2; col <= max_len; ++col) {
        ret[col] = 0;
        letter_counts[col] = Column{ 0 };
        for (int64_t row = kNumLetters - col; row >= 0; --row) {
            letter_counts[col][row] = letter_counts[col][row + 1] + letter_counts[col - 1][row + 1];
            ret[col] += letter_counts[col][row];
        }
    }

    return ret;
}


std::vector<int64_t> get_bin_1_counts(size_t max_len) {
    auto ret = get_bin_0_counts(max_len);

    const auto scale = get_bin_1_scale_factors(max_len);
    for (size_t i = 0; i <= max_len; ++i)
        ret[i] *= scale[i];

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    auto perms = get_permutations(26, 2);
    //    //for (const auto& p : perms) {
    //    //    for (const auto& el : p)
    //    //        std::cout << el << " ";
    //    //    std::cout << std::endl;
    //    //}
    //    std::cout << "Num words = " << perms.size() << std::endl;
    //    std::cout << "Should be " << get_num_permutations(26, 5) << std::endl;
    //}

    //{
    //    auto str = get_good_strings(3);
    //    for (const auto& s : str)
    //        std::cout << s << std::endl;
    //    std::cout << "There are " << str.size() << " strings that qualify" << std::endl;
    //    return 0;
    //}

    //{
    //    size_t len = 2;
    //    auto all_counts = get_word_counts(len);
    //    std::array<BigInt, kNumLetters> sums;
    //    const auto& word_counts = all_counts[len];
    //    for (size_t l = 0; l < kNumLetters; ++l) {
    //        BigInt letter_count{ 0 };
    //        std::cout << char('a' + l) << "\t";
    //        for (size_t bin = 0; bin < len; ++bin) {
    //            std::cout << word_counts[l][bin] << "\t";
    //            letter_count += word_counts[l][bin];
    //        }
    //        std::cout << letter_count << std::endl;
    //        for (size_t c = 0; c < kNumLetters; ++c)
    //            sums[c] += word_counts[l][c];
    //    }
    //    std::cout << "Sums =\t";
    //    for (size_t c = 0; c < kNumLetters; ++c)
    //        std::cout << sums[c] << "\t";
    //    std::cout << std::endl;
    //    std::cout << "Total of counts = " << std::accumulate(sums.cbegin(), sums.cend(), BigInt{ 0 }) << std::endl;
    //}

    //{
    //    const auto counts = get_order_counts(11);

    //    for (size_t i = 0; i < counts.size(); ++i)
    //        std::cout << i << "\t" << counts[i] << std::endl;
    //    std::cout << "Total = " << std::accumulate(counts.cbegin(), counts.cend(), size_t{ 0 });

    //    return 0;
    //}

    {
        auto counts = get_bin_1_scale_factors(26);
        for (size_t n = 0; n <= 26; ++n)
            std::cout << n << "\t" << counts[n] << std::endl;
    }

    {
        auto counts = get_bin_0_counts(26);
        for (size_t n = 0; n <= 26; ++n)
            std::cout << n << "\t" << counts[n] << std::endl;
    }

    {
        auto counts = get_bin_1_counts(26);
        for (size_t n = 0; n <= 26; ++n)
            std::cout << n << "\t" << counts[n] << std::endl;
    }

    //{
    //    auto all_counts = get_word_counts(26);
    //    for (size_t len = 1; len <= 26; ++len) {
    //        const auto& word_counts = all_counts[len];
    //        BigInt count{ 0 };
    //        for (size_t l = 0; l < 26; ++l)
    //            count += word_counts[l][1];
    //        std::cout << len << "\t" << count << std::endl;
    //    }
    //}
}
