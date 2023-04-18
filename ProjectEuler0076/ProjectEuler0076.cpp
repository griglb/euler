// It is possible to write five as a sum in exactly six different ways:
//      4 + 1
//      3 + 2
//      3 + 1 + 1
//      2 + 2 + 1
//      2 + 1 + 1 + 1
//      1 + 1 + 1 + 1 + 1
//
// How many different ways can one hundred be written as a sum of at least two positive integers?

#include <iostream>
#include <numeric>
#include <set>
#include <vector>


using Addends = std::multiset<uint16_t>;
using SameSum = std::set<Addends>;
SameSum get_sum_sets(uint16_t total) {
    // This function works correctly, but does not scale well in time or space.
    // It ran out of memory on a 128GB machine before it got to 100.
    std::vector<SameSum> memo;
    memo.push_back({ {} });
    memo.push_back({ {1} });

    for (uint16_t sum = 2; sum <= total; ++sum) {
        SameSum this_sum;
        if (sum < total)
            this_sum.insert({ sum });

        for (uint16_t n = 1; n < sum; ++n) {
            auto sub_sums = memo[sum - n];
            for (auto sub : sub_sums) {
                sub.insert(n);
                this_sum.insert(sub);
            }
        }
        memo.push_back(this_sum);
//        std::cout << "\t" << sum << "\t" << this_sum.size() << std::endl;
    }

    return memo[total];
}


// How many ways to get a given sum (from get_sum_sets), broken out into number of addends
//  2 = 1   - 2: 1
//  3 = 2   - 2: 1   3: 1
//  4 = 4   - 2: 2   3: 1   4: 1
//  5 = 6   - 2: 2   3: 2   4: 1   5: 1
//  6 = 10  - 2: 3   3: 3   4: 2   5: 1   6: 1
//  7 = 14  - 2: 3   3: 4   4: 3   5: 2   6: 1   7: 1
//  8 = 21  - 2: 4   3: 5   4: 5   5: 3   6: 2   7: 1   8: 1
//  9 = 29  - 2: 4   3: 7   4: 6   5: 5   6: 3   7: 2   8: 1   9: 1
// 10 = 41  - 2: 5   3: 8   4: 9   5: 7   6: 5   7: 3   8: 2   9: 1   10: 1
// 11 = 55  - 2: 5   3: 10  4: 11  5: 10  6: 7   7: 5   8: 3   9: 2   10: 1   11: 1
// 12 = 76  - 2: 6   3: 12  4: 15  5: 13  6: 11  7: 7   8: 5   9: 3   10: 2   11: 1   12: 1
// 13 = 100 - 2: 6   3: 14  4: 18  5: 18  6: 14  7: 11  8: 7   9: 5   10: 3   11: 2   12: 1   13: 1
// 14 = 134 - 2: 7   3: 16  4: 23  5: 23  6: 20  7: 15  8: 11  9: 7   10: 5   11: 3   12: 2   13: 1   14: 1
// 15 = 175 - 2: 7   3: 19  4: 27  5: 30  6: 26  7: 21  8: 15  9: 11  10: 7   11: 5   12: 3   13: 2   14: 1   15: 1
// 16 = 230 - 2: 8   3: 21  4: 34  5: 37  6: 35  7: 28  8: 22  9: 15  10: 11  11: 7   12: 5   13: 3   14: 2   15: 1  16: 1
// 17 = 296 - 2: 8   3: 24  4: 39  5: 47  6: 44  7: 38  8: 29  9: 22  10: 15  11: 11  12: 7   13: 5   14: 3   15: 2  16: 1  17: 1
// 18 = 384 - 2: 9   3: 27  4: 47  5: 57  6: 58  7: 49  8: 40  9: 30  10: 22  11: 15  12: 11  13: 7   14: 5   15: 3  16: 2  17: 1  18: 1
// 19 = 489 - 2: 9   3: 30  4: 54  5: 70  6: 71  7: 65  8: 52  9: 41  10: 30  11: 22  12: 15  13: 11  14: 7   15: 5  16: 3  17: 2  18: 1  19: 1
// 20 = 626 - 2: 10  3: 33  4: 64  5: 84  6: 90  7: 82  8: 70  9: 54  10: 42  11: 30  12: 22  13: 15  14: 11  15: 7  16: 5  17: 3  18: 2  19: 1  20: 1

// Look for patterns in the sums of different numbers of terms, sorted as monotonically increasing function.
// Groups of 2 summing to n:
//   n = 2  (1,1)
//   n = 3  (1,2)
//   n = 4  (1,3)  (2,2)                  # sums = n/2        if n even
//   n = 5  (1,4)  (2,3)                          (n-1)/2     if n odd
//   n = 6  (1,5)  (2,4)  (3,3)
//   n = 7  (1,6)  (2,5)  (3,4)

// Groups of 3 summing to n:
//   n = 2   N/A
//   n = 3  (1,1,1)
//   n = 4  (1,1,2)
//   n = 5  (1,1,3)  (1,2,2)
//   n = 6  (1,1,4)  (1,2,3)                             |  (2,2,2)
//   n = 7  (1,1,5)  (1,2,4)  (1,3,3)                    |  (2,2,3)
//   n = 8  (1,1,6)  (1,2,5)  (1,3,4)                    |  (2,2,4)  (2,3,3)
//   n = 9  (1,1,7)  (1,2,6)  (1,3,5)  (1,4,4)           |  (2,2,5)  (2,3,4)           |  (3,3,3)
//   n = 10 (1,1,8)  (1,2,7)  (1,3,6)  (1,4,5)           |  (2,2,6)  (2,3,5)  (2,4,4)  |  (3,3,4)
//   n = 11 (1,1,9)  (1,2,8)  (1,3,7)  (1,4,6)  (1,5,5)  |  (2,2,7)  (2,3,6)  (2,4,5)  |  (3,3,5)  (3,4,4)
// (pipes split into groups by first addend)
//   Split by the smallest number:
//      n   2 3 4 5 6 7 8 9 10 11
//      1 - 0 1 1 2 2 3 3 4  4  5 ...       # sums = (n-1)/2  if n odd;      (n-2)/2  if n even
//      2 - 0 0 0 0 1 1 2 2  3  3 ...       # sums = (n-4)/2  if n odd > 5;  (n-5)/2  if n even > 5
//      3 - 0 0 0 0 0 0 0 1  1  2 ...       # sums = (n-7)/2  if n odd > 8;  (n-8)/2  if n even > 8

// Groups of 4 summing to n:
//   n = 2   N/A
//   n = 3   N/A
//   n = 4  (1,1,1,1)
//   n = 5  (1,1,1,2)
//   n = 6  (1,1,1,3)  (1,1,2,2)
//   n = 7  (1,1,1,4)  (1,1,2,3)                        |  (1,2,2,2)
//   n = 8  (1,1,1,5)  (1,1,2,4)  (1,1,3,3)             |  (1,2,2,3)                        |             |  (2,2,2,2)
//   n = 9  (1,1,1,6)  (1,1,2,5)  (1,1,3,4)             |  (1,2,2,4)  (1,2,3,3)             |             |  (2,2,2,3)
//   n = 10 (1,1,1,7)  (1,1,2,6)  (1,1,3,5)  (1,1,4,4)  |  (1,2,2,5)  (1,2,3,4)             |  (1,3,3,3)  |  (2,2,2,4)  (2,2,3,3)
//   n = 11 (1,1,1,8)  (1,1,2,7)  (1,1,3,6)  (1,1,4,5)  |  (1,2,2,6)  (1,2,3,5)  (1,2,4,4)  |  (1,3,3,4)  |  (2,2,2,5)  (2,2,3,4)  |  (2,3,3,3)
// (pipes split into groups by first 2 addends)
//   Split by smallest pair of numbers:
//       n    2 3 4 5 6 7 8 9 10 11
//      1,1 - 0 0 1 1 2 2 3 3  4  4 ...     # sums = (n-3)/2  if n odd > 3;  (n-2)/2  if n even > 3
//      1,2 - 0 0 0 0 0 1 1 2  2  3 ...     # sums = (n-5)/2  if n odd > 6;  (n-6)/2  if n even > 6
//      1,3 - 0 0 0 0 0 0 0 0  1  1 ...     # sums = (n-9)/2  if n odd > 9;  (n-9)/2  if n even > 9
//      2,2 - 0 0 0 0 0 0 1 1  2  2 ...     # sums = (n-7)/2  if n odd > 7;  (n-6)/2  if n even > 7
//      2,3 - 0 0 0 0 0 0 0 0  0  1 ...     # sums = (n-9)/2  if n odd > 10; (n-10)/2  if n even > 10

// Groups of 5 summing to n:
//   n = 2   N/A
//   n = 3   N/A
//   n = 4   N/A
//   n = 5  (1,1,1,1,1)
//   n = 6  (1,1,1,1,2)
//   n = 7  (1,1,1,1,3)  (1,1,1,2,2)
//   n = 8  (1,1,1,1,4)  (1,1,1,2,3)                            |  (1,1,2,2,2)
//   n = 9  (1,1,1,1,5)  (1,1,1,2,4)  (1,1,1,3,3)               |  (1,1,2,2,3)               |               |  (1,2,2,2,2)
//   n = 10 (1,1,1,1,6)  (1,1,1,2,5)  (1,1,1,3,4)               |  (1,1,2,2,4)  (1,1,2,3,3)  |               |  (1,2,2,2,3)               |  (2,2,2,2,2)
//   n = 11 (1,1,1,1,7)  (1,1,1,2,6)  (1,1,1,3,5)  (1,1,1,4,4)  |  (1,1,2,2,5)  (1,1,2,3,4)  |  (1,1,3,3,3)  |  (1,2,2,2,4)  (1,2,2,3,3)  |  (2,2,2,2,3)
// (pipes split into groups by first 3 addends)
//   Split by smallest triplet of numbers:
//       n     2 3 4 5 6 7 8 9 10 11 12 13
//     1,1,1 - 0 0 0 1 1 2 2 3  3  4  4  5 ...     # sums = (n-3)/2   if n odd > 4;   (n-4)/2   if n even > 4
//     1,1,2 - 0 0 0 0 0 0 1 1  2  2  3  3 ...     # sums = (n-7)/2   if n odd > 7;   (n-6)/2   if n even > 7
//     1,1,3 - 0 0 0 0 0 0 0 0  0  1  1  2 ...     # sums = (n-9)/2   if n odd > 10;  (n-10)/2  if n even > 10
//     1,2,2 - 0 0 0 0 0 0 0 1  1  2  2  3 ...     # sums = (n-7)/2   if n odd > 8;   (n-8)/2   if n even > 8
//     1,2,3 - 0 0 0 0 0 0 0 0  0  0  1  1 ...     # sums = (n-11)/2  if n odd > 11;  (n-10)/2  if n even > 11
//     1,3,3 - 0 0 0 0 0 0 0 0  0  0  0  1 ...     # sums = (n-11)/2  if n odd > 12;  (n-12)/2  if n even > 12
//     2,2,2 - 0 0 0 0 0 0 0 0  1  1  2  2 ...     # sums = (n-9)/2   if n odd > 9;   (n-8)/2   if n even > 9
//     2,2,3 - 0 0 0 0 0 0 0 0  0  0  0  1 ...     # sums = (n-11)/2  if n odd > 12;  (n-12)/2  if n even > 12


uint64_t get_num_sums_with_n_terms(uint16_t total, uint16_t num_terms) {
    if (2 == num_terms) {
        // As the comments above detail, there are separate equations for even and odd n,
        // but that is for pure math.  With integer math on the computer, we can combine
        // the two expressions into one.  In this case the even expression works for all
        // values, as odd values of n will have the 1/2 truncated away.
        return total / 2;
    }

    uint64_t ret{ 0 };
    // The expressions above vary the last 2 terms, for fixed sets of the first (k-2) terms.
    std::vector<uint16_t> smallest_terms(num_terms - 2, 1);
    uint16_t smallest_sum{ num_terms };
    // For the different sets of fixed smallest terms, we start by incrementing the last term
    // and move forward.
    while (true) {
        // 1. Add new sums to ret
        // 2. Increment odometer
        //   a. Loop index from back to front
        //     i. If incrementing current index stays under total, then break out of for loop for next while loop
        //    ii. If not last index, then reset all subsequent values to new value
        //   b. If run out of indices to increment, then break out of while loop

        ret += (total - smallest_sum + 2) / 2;

        // Increment varying_index to next value.
        for (int16_t varying_index = smallest_terms.size() - 1; varying_index >= 0; --varying_index) {
            ++smallest_terms[varying_index];
            for (size_t later_index = varying_index + 1; later_index < smallest_terms.size(); ++later_index)
                smallest_terms[later_index] = smallest_terms[varying_index];
            // Get the sum of all the terms, adding the smallest values for the last 2,
            // which is the last element of smallest_terms.
            smallest_sum = std::accumulate(smallest_terms.cbegin(), smallest_terms.cend(), 2 * (*smallest_terms.rbegin()));
            // 
            if (smallest_sum <= total)
                break;
        }

        if (smallest_sum > total)
            break;
    }

    return ret;
}


uint64_t get_num_sums(uint16_t total) {
    // Handle special cases
    if (2 > total)
        return 0;

    uint64_t ret{ 0 };
    for (uint16_t num_terms = 2; num_terms <= total; ++num_terms)
        ret += get_num_sums_with_n_terms(total, num_terms);
    return ret;
}



int main()
{
    std::cout << "Hello World!\n";

    {
        for (int16_t total = 2; total < 21; ++total) {
            auto sums = get_sum_sets(total);
            std::cout << total << " =\t" << sums.size() << " :\t";
            std::vector<int16_t> counts(total+1, 0);
            for (const auto& sum : sums) {
                counts[sum.size()]++;
            }
            for (uint16_t ct = 2; ct <= total; ++ct)
                std::cout << ct << ":" << counts[ct] << "\t";
            std::cout << std::endl;
//            for (const auto& sum : sums) {
//                for (const auto& el : sum)
//                    std::cout << el << " + ";
//                std::cout << std::endl << "\t";
//            }
        }
    }

    //{
    //    for (uint16_t total = 2; total < 21; ++total) {
    //        std::cout << total << " - 2:" << get_num_sums_with_n_terms(total, 2) << std::endl;
    //    }
    //}

    //{
    //    for (uint16_t total = 3; total < 21; ++total) {
    //        std::cout << total << " - 3:" << get_num_sums_with_n_terms(total, 3) << std::endl;
    //    }
    //}

    //{
    //    for (uint16_t total = 4; total < 21; ++total) {
    //        std::cout << total << " - 4:" << get_num_sums_with_n_terms(total, 4) << std::endl;
    //    }
    //}

    {
        for (uint16_t total = 2; total < 21; ++total) {
            std::cout << total << " = \t" << get_num_sums(total) << " - \t";
            for (uint16_t n = 2; n <= total; ++n)
                std::cout << n << ": " << get_num_sums_with_n_terms(total, n) << "\t";
            std::cout << std::endl;
        }
    }

    {
        for (uint16_t total = 2; total < 101; ++total) {
            std::cout << total << " = \t" << get_num_sums(total) << std::endl;
        }
    }
}
