// Let S(A) represent the sum of elements in set A of size n.We shall call it a special
// sum set if for any two non - empty disjoint subsets, B and C, the following properties are true:
//
//   1. S(B) != S(C); that is, sums of subsets cannot be equal.
//   2. If B contains more elements than C then S(B) > S(C).
//
// For example, { 81, 88, 75, 42, 87, 84, 86, 65 } is not a special sum set
// because 65 + 87 + 88 = 75 + 81 + 84, whereas { 157, 150, 164, 119, 79, 159, 161, 139, 158 }
// satisfies both rules for all possible subset pair combinationsand S(A) = 1286.
//
// Using sets.txt(right click and "Save Link/Target As..."), a 4K text file with one-hundred
// sets containing seven to twelve elements(the two examples given above are the first two sets
// in the file), identify all the special sum sets, A1, A2, ..., Ak,
// and find the value of S(A1) + S(A2) + ... + S(Ak).
//
// NOTE: This problem is related to Problem 103 and Problem 106.


#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>

#include "combinatorics.h"


using Subset = std::set<int64_t>;
using SubsetSet = std::set<Subset>;

std::vector<Subset> get_sets() {
#ifdef _CONSOLE
    std::ifstream infile("./p105_sets.txt");
#else
    std::ifstream infile("./ProjectEuler0105/p105_sets.txt");
#endif

    std::vector<Subset> ret;

    while (!infile.eof()) {
        std::string line;
        infile >> line;
        std::stringstream str{ line };

        Subset this_set;

        while (!str.eof()) {
            int64_t value;
            char comma;
            str >> value >> comma;
            this_set.insert(value);
        }
        ret.push_back(this_set);
    }

    return ret;
}


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


std::vector<Subset> get_special_sets() {
    std::vector<Subset> ret;

    auto all_sets = get_sets();
    for (const auto& set : all_sets) {
        for (const auto& el : set)
            std::cout << el << " ";
        std::cout << std::endl;

        if (test_set(set)) {
            ret.push_back(set);
            std::cout << "\tGOOD!" << std::endl;
        }
        else
            std::cout << "\tbad" << std::endl;
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    auto sets = get_sets();
    //    for (const auto& set : sets) {
    //        for (const auto& el : set)
    //            std::cout << el << " ";
    //        std::cout << std::endl;
    //    }
    //    std::cout << sets.size() << std::endl;
    //}

    {
        int64_t sum{ 0 };
        auto sets = get_special_sets();
        for (const auto& set : sets) {
            for (const auto& el : set) {
                std::cout << el << " ";
                sum += el;
            }
            std::cout << std::endl;
        }
        std::cout << sets.size() << std::endl;
        std::cout << "total sum = " << sum << std::endl;
    }
}
