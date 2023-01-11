// Consider the fraction, n / d, where nand d are positive integers.
// If n < d and HCF(n, d) = 1, it is called a reduced proper fraction.
//
//  If we list the set of reduced proper fractions for d ≤ 8 in ascending order of size, we get :
//
//  1/8, 1/7, 1/6, 1/5, 1/4, 2/7, 1/3, 3/8, 2/5, 3/7, 1/2, 4/7, 3/5, 5/8, 2/3, 5/7, 3/4, 4/5, 5/6, 6/7, 7/8
//
//  It can be seen that there are 21 elements in this set.
//
//  How many elements would be contained in the set of reduced proper fractions for d <= 1, 000, 000 ?


#include <iostream>

#include "big_int.h"
#include "totient.h"

// 1/8, 3/8, 5/8, 7/8
// 1/7, 2/7, 3/7, 4/7, 5/7, 6/7
// 1/6, 5/6
// 1/5, 2/5, 3/5, 4/5
// 1/4, 3/4
// 1/3, 2/3
// 1/2


BigInt num_unique_fractions(uint64_t max_den) {
    Phi phi;
    phi(max_den);


    BigInt count{ 0 };
    for (uint64_t d = 2; d <= max_den; ++d) {
        count += phi(d);
//        std::cout << "phi(" << d << ") = " << phi(d) << std::endl;
    }
    return count;
}


int main()
{
    std::cout << "Hello World!\n";

    std::cout << "There are " << num_unique_fractions(8) << " unique fractions with d <= 8" << std::endl;
    std::cout << "There are " << num_unique_fractions(1'000'000) << " unique fractions with d <= 1'000'000" << std::endl;

}
