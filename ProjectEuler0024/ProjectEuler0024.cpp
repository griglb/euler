// ProjectEuler0024.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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
