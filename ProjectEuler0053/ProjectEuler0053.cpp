// ProjectEuler0053.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>


int64_t get_num_terms() {
    // C(n, r) = n! / r! / (n-r)!
    // C(n, r+1) = n! / (r+1)! / (n-r-1)!
    // C(n, r+1) / C(n, r) = r! * (n-r)! / (r+1)! / (n-r-1)! = (n-r) / (r+1)

    int64_t count{0};

    for (int16_t n = 1; n < 101; ++n) {
        std::cout << n << ": ";
        int64_t term {1};
        for (int16_t r = 0; r < n/2; ++r) {
            term *= (n - r);
            term /= (r + 1);
            std::cout << term << " ";
            if (term > 1'000'000) {
                std::cout << std::endl;
                std::cout << n << "\t" << r+1 << std::endl;
                count += n - 2 * (r  + 1) + 1;
                break;
            }
        }
        std::cout << std::endl;
    }

    return count;
}


int main()
{
    std::cout << "Hello World!\n";

    std::cout << get_num_terms() << std::endl;
}
