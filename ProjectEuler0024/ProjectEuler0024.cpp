// A permutation is an ordered arrangement of objects.
// For example, 3124 is one possible permutation of the digits 1, 2, 3 and 4.
// If all of the permutations are listed numerically or alphabetically, we call it lexicographic order.
// The lexicographic permutations of 0, 1 and 2 are:
//                      012   021   102   120   201   210
// What is the millionth lexicographic permutation of the digits 0, 1, 2, 3, 4, 5, 6, 7, 8 and 9?


#include <iostream>

#include "combinatorics.h"


Permutation get_nth_permutation(int length, int64_t n) {
    // Create a set of the elements that should be in the permutation.
    Combination elements;
    for (int i = 0; i < length; ++i)
        elements.insert(i);

    Permutation ret;
    while (n > 1) {
        int64_t num_sub_perms = factorial(length - 1 - ret.size());
        auto curr_digit_iter = elements.begin();
        while (n > num_sub_perms) {
            curr_digit_iter++;
            n -= num_sub_perms;
        }
        ret.push_back(*curr_digit_iter);
        elements.erase(curr_digit_iter);
    }
    for (const auto elem : elements)
        ret.push_back(elem);
    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    //PermutationList permutations = get_permutations(10, 10);
    //Permutation desired_perm = permutations[999'999];
    //for (const auto& elem : desired_perm)
    //    std::cout << elem << " ";
    //std::cout << std::endl;

    Permutation desired_perm = get_nth_permutation(10, 1'000'000);
    for (const auto& elem : desired_perm)
        std::cout << elem << " ";
    std::cout << std::endl;
   }
