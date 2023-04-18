// Let S(A) represent the sum of elements in set A of size n.
// We shall call it a special sum set if for any two non-empty disjoint subsets,
// B and C, the following properties are true:
//      1. S(B) != S(C); that is, sums of subsets cannot be equal.
//      2. If B contains more elements than C then S(B) > S(C).
//
// If S(A) is minimised for a given n, we shall call it an optimum special
// sum set.  The first five optimum special sum sets are given below.
//      n = 1 : {1}
//      n = 2 : {1, 2}
//      n = 3 : {2, 3, 4}
//      n = 4 : {3, 5, 6, 7}
//      n = 5 : {6, 9, 11, 12, 13}
//
// It seems that for a given optimum set, A = { a1, a2, ... , an },
// the next optimum set is of the form B = { b, a1 + b, a2 + b, ... ,an + b },
// where b is the "middle" element on the previous row.
//
// By applying this "rule" we would expect the optimum set for n = 6 to be
// A = { 11, 17, 20, 22, 23, 24 }, with S(A) = 117.
// However, this is not the optimum set, as we have merely applied an algorithm
// to provide a near optimum set.
// The optimum set for n = 6 is A = { 11, 18, 19, 20, 22, 25 }, with S(A) = 115
// and corresponding set string: 111819202225.
//
// Given that A is an optimum special sum set for n = 7, find its set string.
//
// NOTE : This problem is related to Problem 105 and Problem 106.


#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <utility>
#include <vector>

#include "combinatorics.h"


// Why is the smallest element the value it is?
//   1: sum of 2 smallest must be > than largest
//   2: if n odd, then sum of (n+1)/2 smallest must be > than sum of (n-1)/2 largest
//   3: if n even, then sum of n/2 smallest must be > than sum of n/2-1 largest
//   4: smallest must be > sum of deltas between n/2-1 next smallest and n/2-1 largest
//
// Play with n=5 set, call the values {a, b, c, d, e} or {a, a+b, a+c, a+d, a+e}.
// From 1: a + a+b > a+e  =>  a + b > e
// From 2: a + a+b + a+c > a+d + a+e  => a + b + c > d + e
// From 3: N/A
// From 4: a > (a+e + a+d) - (a+b + a+c)  =>  a > d + e - b - c
// 
// Why not {6, 9, 10, 11, 13}?  6+13 == 9+10


using Subset = std::set<uint64_t>;
using SubsetSet = std::set<Subset>;
std::set<SubsetSet> get_disjoint_subset_pairs(size_t num_elements) {
    static std::map<size_t, std::set<SubsetSet>> memo;
    if (memo.find(num_elements) != memo.end())
        return memo.at(num_elements);

    std::set<SubsetSet> ret;

    Subset all_elems;
    for (size_t i = 0; i < num_elements; ++i)
        all_elems.insert(i);

    // Consider the first subset size to be in [1, num_elements-1].
    for (size_t sub1_size = 1; sub1_size < num_elements; ++sub1_size) {
        // Get all combinations of this sized subset.
        auto sub1_combos = get_combinations(num_elements, sub1_size);
        for (const auto &combo1 : sub1_combos) {
            const Subset sub1{ combo1.begin(), combo1.end() };
            // Construct the set of all elements not in sub1.
            Subset other_elems{ all_elems };
            for (const auto& el : sub1)
                other_elems.erase(el);
            // Consider the second subset size to from 1 to the number of elements not in sub1.
            for (size_t sub2_size = 1; sub2_size <= other_elems.size(); ++sub2_size) {
                // Get all combinations of this sized subset.
                auto sub2_combos = get_combinations(other_elems.size(), sub2_size);
                for (const auto &combo2 : sub2_combos) {
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


bool test_set(const Subset& set) {
    if (set.size() <= 1)
        return true;

    std::vector<int64_t> numbers{ set.begin(), set.end() };

    auto subset_pairs = get_disjoint_subset_pairs(numbers.size());

    for (const auto& pair : subset_pairs) {
        auto sub1 = *pair.begin();
        int64_t sum1{ 0 };
        for (const auto& ind : sub1) {
            sum1 += numbers[ind];
        }

        auto sub2 = *pair.rbegin();
        int64_t sum2{ 0 };
        for (const auto& ind : sub2) {
            sum2 += numbers[ind];
        }

        if (sum1 == sum2) {
            //std::cout << "Rule 1 violated by these subsets:" << std::endl;
            //std::cout << "\t{ ";
            //for (const auto& el : sub1)
            //    std::cout << el << ", ";
            //std::cout << "}" << std::endl;
            //std::cout << "\t{ ";
            //for (const auto& el : sub2)
            //    std::cout << el << ", ";
            //std::cout << "}" << std::endl;
            return false;
        }
        if ((sub1.size() > sub2.size()) && (sum1 < sum2)) {
            //std::cout << "Rule 2 violated by these subsets:" << std::endl;
            //std::cout << "\t{ ";
            //for (const auto& el : sub1)
            //    std::cout << el << ", ";
            //std::cout << "}" << std::endl;
            //std::cout << "\t{ ";
            //for (const auto& el : sub2)
            //    std::cout << el << ", ";
            //std::cout << "}" << std::endl;
            return false;
        }
        else if ((sub1.size() < sub2.size()) && (sum1 > sum2)) {
            //std::cout << "Rule 2 violated by these subsets:" << std::endl;
            //std::cout << "\t{ ";
            //for (const auto& el : sub1)
            //    std::cout << el << ", ";
            //std::cout << "}" << std::endl;
            //std::cout << "\t{ ";
            //for (const auto& el : sub2)
            //    std::cout << el << ", ";
            //std::cout << "}" << std::endl;
            return false;
        }
    }

    return true;
}


std::vector<Subset> get_special_sets(uint64_t num_elements) {
    std::vector<Subset> ret;

    const auto pairs = get_disjoint_subset_pairs(num_elements);
    uint64_t smallest_sum{ 1ULL << 62 };
    Subset smallest_set;

    constexpr uint64_t ElemMax{ 50 };

    for (uint64_t first = 1; first < ElemMax; ++first) {
        if (smallest_sum < (first * num_elements + num_elements*(num_elements-1)/2))
            break;
        for (uint64_t second = first + 1; second < 2*first; ++second) {
            uint64_t last = first + second - 1;
            std::cout << "\t" << first << "\t" << second << "\t" << last << std::endl;
            if (smallest_sum < (first + 4*second + 6))
                break;
            if (last <= second)
                continue;
            std::vector<uint64_t> middle;
            for (uint64_t mid = second + 1; mid <= last; ++mid)
                middle.push_back(mid);
            if (middle.size() < num_elements - 2)
                continue;
            const auto combos = get_combinations(middle.size(), num_elements - 2);

            Subset curr_set{ first, second };
            for (const auto& combo : combos) {
                Subset tmp_set{ curr_set };
                for (const auto& ind : combo) {
                    tmp_set.insert(middle[ind]);
                }
                if (test_set(tmp_set)) {
                    ret.push_back(tmp_set);
                    auto sum = std::accumulate(tmp_set.begin(), tmp_set.end(), 0ULL);
                    if (sum < smallest_sum) {
                        smallest_sum = sum;
                        smallest_set = tmp_set;
                        std::cout << "smallest sum = " << smallest_sum << ", set = {";
                        for (const auto& el : smallest_set) {
                            std::cout << el << ", ";
                        }
                        std::cout << " }" << std::endl;
                    }
                }
            }
        }
    }

    std::cout << "smallest sum set = {";
    for (const auto& el : smallest_set) {
        std::cout << el << ", ";
    }
    std::cout << " }" << std::endl;

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    auto pairs = get_disjoint_subset_pairs(5);
    //    for (const auto& pair : pairs) {
    //        auto iter = pair.begin();
    //        std::cout << "(";
    //        for (const auto& el : *iter)
    //            std::cout << el << ", ";
    //        std::cout << ")  (";
    //        ++iter;
    //        for (const auto& el : *iter)
    //            std::cout << el << ", ";
    //        std::cout << ")" << std::endl;
    //    }
    //    std::cout << "There are " << pairs.size() << " pairs of subsets" << std::endl;
    //}

    {
        std::cout << test_set({ 6, 9, 10, 11, 13 });
    }

    {
        auto subsets = get_special_sets(7);
        //for (const auto& sub : subsets) {
        //    std::cout << "{ ";
        //    for (const auto& el : sub) {
        //        std::cout << el << ", ";
        //    }
        //    std::cout << " }" << std::endl;
        //}
    }
}
