// Let S(A) represent the sum of elements in set A of size n.
// We shall call it a special sum set if for any two non-empty disjoint subsets,
// B and C, the following properties are true:
//      1. S(B) != S(C); that is, sums of subsets cannot be equal.
//      2. If B contains more elements than C then S(B) > S(C).
//
// For this problem we shall assume that a given set contains n strictly increasing
// element sand it already satisfies the second rule.
//
// Surprisingly, out of the 25 possible subset pairs that can be obtained from a set
// for which n=4, only 1 of these pairs need to be tested for equality (first rule).
// Similarly, when n=7, only 70 out of the 966 subset pairs need to be tested.
//
// For n=12, how many of the 261625 subset pairs that can be obtained need to be tested for equality ?
//
// NOTE : This problem is related to Problem 103 and Problem 105.


#include <iostream>
#include <map>
#include <set>

#include "combinatorics.h"


using Subset = std::set<int64_t>;
using SubsetSet = std::set<Subset>;


// Take the example of n=4.  There are 9 subset pairs that invoke rule 1:
//   - {a} {b}
//   - {a} {c}
//   - {a} {d}
//   - {b} {c}
//   - {b} {d}
//   - {c} {d}
//   - {a, b}  {c, d}
//   - {a, c}  {b, d}
//   - {a, d}  {b, c}
// The problem states that we have strictly increasing elements, so the single
// element sets are guaranteed to pass.  That leaves the 3 dual-element set pairs,
// but why is only 1 needed?
// We know a < b < c < d
// Rearrange in pairs:
//   a < c && b < d  =>  a + b < c + d
//   a < b && c < d  =>  a + c < b + d
//   a < b && d > c  =>  a + d ? b + c
// Only the last subset pair is needed, the others are guaranteed to pass!

// Now consider n=7.  There are 966 total subset pairs, but only 196 have the same sizes.
// Of those 196, 21 are pairs of single-element sets, which leaves 175 pairs of 2- and 3-element subsets.
// Rather than brute force it, use the logic from n=4:
//   Iterate over the first subset from largest to smallest
//     For each element, see if there are any elements in second subset that are larger
//       If not, then count this pair as needing to be tested
//       If yes, remove the element from second subset
//   Either we count the pair, or we end up with empty subsets, which means they are guaranteed to pass


std::set<SubsetSet> get_disjoint_subset_pairs(size_t num_elements) {
    static std::map<size_t, std::set<SubsetSet>> memo;
    if (memo.find(num_elements) != memo.end())
        return memo.at(num_elements);

    std::set<SubsetSet> ret;

    Subset all_elems;
    for (size_t i = 0; i < num_elements; ++i)
        all_elems.insert(i);

    // Consider the first subset size to be in [1, num_elements-1].
    for (int sub1_size = 1; sub1_size < num_elements; ++sub1_size) {
        // Get all combinations of this sized subset.
        auto sub1_combos = get_combinations(num_elements, sub1_size);
        for (const auto combo1 : sub1_combos) {
            const Subset sub1{ combo1.begin(), combo1.end() };
            // Construct the set of all elements not in sub1.
            Subset other_elems{ all_elems };
            for (const auto& el : sub1)
                other_elems.erase(el);
            // Consider the second subset size to from 1 to the number of elements not in sub1.
            for (int sub2_size = 1; sub2_size <= other_elems.size(); ++sub2_size) {
                // Get all combinations of this sized subset.
                auto sub2_combos = get_combinations(other_elems.size(), sub2_size);
                for (const auto& combo2 : sub2_combos) {
                    // The values in sub1 are indices into the set of size num_elements, but
                    // the values in sub2 are indices into the other_elems set.
                    Subset sub2;
                    for (const auto& el : combo2) {
                        auto iter = other_elems.cbegin();
                        std::advance(iter, el);
                        sub2.insert(*iter);
                    }
                    SubsetSet pair;
                    pair.insert(sub1);
                    pair.insert(sub2);
                    ret.insert(pair);
                }
            }
        }
    }

    memo[num_elements] = ret;

    return ret;
}


std::set<SubsetSet> get_pairs_needing_testing(size_t num_elements) {
    std::set<SubsetSet> ret;

    auto all_pairs = get_disjoint_subset_pairs(num_elements);

    for (const auto& pair : all_pairs) {
        auto sub1 = *pair.begin();
        auto sub2 = *pair.rbegin();

        if (sub1.size() != sub2.size())
            continue;
        if (sub1.size() == 1)
            continue;

        for (auto iter1 = sub1.crbegin(); iter1 != sub1.crend(); ++iter1) {
            if (*sub2.crbegin() > *iter1) {
                sub2.erase(*sub2.crbegin());
            }
            else {
                SubsetSet new_pair;
                new_pair.insert(sub1);
                new_pair.insert(*pair.rbegin());
                ret.insert(new_pair);
            }
        }
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    auto pairs = get_disjoint_subset_pairs(7);
    //    int64_t count{ 0 };
    //    for (const auto& pair : pairs) {
    //        if (pair.begin()->size() != pair.rbegin()->size())
    //            continue;
    //        auto iter = pair.begin();
    //        std::cout << "(";
    //        for (const auto& el : *iter)
    //            std::cout << el << ", ";
    //        std::cout << ")  (";
    //        ++iter;
    //        for (const auto& el : *iter)
    //            std::cout << el << ", ";
    //        std::cout << ")" << std::endl;
    //        ++count;
    //    }
    //    std::cout << "There are " << pairs.size() << " pairs of subsets, with " << count << " that have same sizes" << std::endl;
    //}

    {
        auto pairs = get_pairs_needing_testing(12);
        int64_t count{ 0 };
        for (const auto& pair : pairs) {
            auto iter = pair.begin();
            std::cout << "(";
            for (const auto& el : *iter)
                std::cout << el << ", ";
            std::cout << ")  (";
            ++iter;
            for (const auto& el : *iter)
                std::cout << el << ", ";
            std::cout << ")" << std::endl;
            ++count;
        }
        std::cout << "There are " << pairs.size() << " pairs of subsets" << std::endl;
    }
}
