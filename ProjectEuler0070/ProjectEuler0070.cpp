// Euler's Totient function, φ(n) [sometimes called the phi function], is used to determine the
// number of positive numbers less than or equal to n which are relatively prime to n.
// For example, as 1, 2, 4, 5, 7, and 8, are all less than nine and relatively prime to nine, φ(9)=6.
// The number 1 is considered to be relatively prime to every positive number, so φ(1) = 1.
//
// Interestingly, φ(87109) = 79180, and it can be seen that 87109 is a permutation of 79180.
//
// Find the value of n, 1 < n < 107, for which φ(n) is a permutation of n
// and the ratio n / φ(n) produces a minimum.


#include <iostream>
#include <set>

#include "big_int.h"
#include "totient.h"


std::set<uint64_t> get_totient_permutations(Phi &phi, uint64_t max_val) {
    std::set<uint64_t> ret;

    for (uint64_t n = 2; n < max_val; ++n) {
        if (n % 10000 == 0) std::cout << n << std::endl;
        auto n_digits = BigInt{ n }.get_digits();
        auto phi_digits = BigInt{ phi(n) }.get_digits();

        std::multiset<int8_t> n_digit_set{ n_digits.begin(), n_digits.end() };
        std::multiset<int8_t> phi_digit_set{ phi_digits.begin(), phi_digits.end() };
        if (n_digit_set == phi_digit_set)
            ret.insert(n);
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    {
        Phi phi;
        phi(100);
        auto perms = get_totient_permutations(phi, 100);
        for (const auto& p : perms)
            std::cout << p << "\t" << phi(p) << std::endl;
    }

    {
        constexpr uint64_t max_val{ static_cast<uint64_t>(1e7) };
        Phi phi;
        phi(max_val);
        auto perms = get_totient_permutations(phi, max_val);
        double min_ratio{ 1E5 };
        uint64_t min_n{ 0 };
        for (const auto& n : perms) {
            double ratio = static_cast<double>(n) / phi(n);
            std::cout << n << "\t" << phi(n) << "\t" << ratio << std::endl;
            if (ratio < min_ratio) {
                std::cout << "\tnew min ratio at n = " << n << std::endl;
                min_ratio = ratio;
                min_n = n;
            }
        }
        std::cout << "min ratio = " << min_ratio << " for n = " << min_n << std::endl;
    }
}
