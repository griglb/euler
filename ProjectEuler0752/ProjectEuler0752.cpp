// When (1 + sqrt(7)) is raised to an integral power, n, we always get a number of the form (a + b*sqrt(7)).
// We write (1 + sqrt(7))^2 = alpha(n) + beta(n)*sqrt(7).
// For a given number x we define g(x) to be the smallest positive integer n such that:
//              alpha(n) == 1 (mod x)    and
//              beta(n) == 0 (mod x)
// and g(x) = 0 if there is no such value of n. For example, g(3) = 0, g(5) = 12.
// Further define
//              G(N) = sum{x=2..N}(g(x))
// You are given and G(10^2) = 28891 and G(10^3) = 13131583.
// Find G(10^6).

#include <iostream>
#include <set>
#include <unordered_map>
#include <utility>

#include "prime_helper.h"

// Calculate alpha(n) and beta(n) the hard way:
// (1 + sqrt(7))^0 = 1
// (1 + sqrt(7))^1 = 1 + sqrt(7)
// (1 + sqrt(7))^2 = 1 + 2*sqrt(7) + 7 = 8 + 2*sqrt(7)
// (1 + sqrt(7))^3 = 1 + 3*sqrt(7) + 3*7 + 7*sqrt(7) = 22 + 10*sqrt(7)
// (1 + sqrt(7))^4 = 1 + 4*sqrt(7) + 6*7 + 4*7*sqrt(7) + 49 = 92 + 32*sqrt(7)
// This leads to these values of alpha & beta
//   n     alpha(n)     beta(n)
//   0        1            0
//   1        1            1
//   2        8            2
//   3       22           10
//   4       92           32
// If you have valid values for alpha(n) & beta(n), you can calculate alpha(n+1) & beta(n+1):
//    (1 + sqrt(7))*(alpha(n) + beta(n)*sqrt(7)) = alpha(n) + alpha(n)*sqrt(7) + beta(n)*sqrt(7) + beta(n)*7
// ->  alpha(n+1) = alpha(n) + 7*beta(n)
//     beta(n+1)  = alpha(n) + beta(n)
// These recurrence relationships can be used in the modular space to solve for g(x):


class calculator {
public:
    calculator() = default;
    ~calculator() = default;

    uint64_t g(uint64_t x) {
        std::cout << "in g(" << x << ")" << std::endl;
        if (little_g_values_.find(x) != little_g_values_.end()) {
            return little_g_values_.at(x);
        }

        auto factors = helper_.get_factorization_fast(x);
        std::set<uint64_t> powers;
        for (const auto &[prime, exponent] : factors) {
            if (little_g_values_.find(prime) == little_g_values_.end()) {
                little_g_values_[prime] = real_g(prime);
                // std::cout << "calculated g(" << prime << ") = " << little_g_values_.at(prime) << std::endl;
            }
            uint64_t tmp = little_g_values_.at(prime);
            for (uint64_t exp = 1; exp < exponent; ++exp)
                tmp *= prime;
            powers.insert(tmp);
        }

        return gcd(powers);
    }

    uint64_t G(uint64_t N) {
        // Prime (pun intended) the pump by making sure the helper knows all the primes up to N.
        helper_.get_primes(N);
        uint64_t sum{0};
        for (uint64_t x = 2; x <= N; ++x) {
            // We know that g(2) = g(3) = 0, as do all multiples of 2 or 3.
            if ((x % 2 == 0) || (x % 3 == 0))
                continue;
            sum += g(x);
        }
        return sum;
    }

private:
    PrimeHelper helper_;
    std::unordered_map<uint64_t, uint64_t> little_g_values_;

    uint64_t real_g(uint64_t x) {
        uint64_t alpha {1};
        uint64_t beta {0};

        std::set<std::pair<uint64_t, uint64_t>> coeffs;
        coeffs.insert({1, 0});
        for (uint64_t i = 1; i < x*x; ++i) {
            const uint64_t alpha_n1 = (alpha + 7*beta) % x;
            const uint64_t beta_n1 = (alpha + beta) % x;
    //std::cout << "x = " << x << ", i = " << i << ", alpha = " << alpha_n1 << ", beta = " << beta_n1 << std::endl;
            if ((1 == alpha_n1) && (0 == beta_n1))
                return i;
            if (coeffs.find(std::make_pair(alpha_n1, beta_n1)) != coeffs.end())
                return 0;
            coeffs.insert({alpha_n1, beta_n1});
            alpha = alpha_n1;
            beta = beta_n1;
        }

        return 0;
    }

    uint64_t gcd(std::set<uint64_t> numbers) {
        if (numbers.size() == 1)
            return *numbers.cbegin();

        if (numbers.size() == 2) {
            auto iter = numbers.cbegin();
            uint64_t num1 = *iter++;
            uint64_t num2 = *iter;
            if (num2 % num1 == 0)
                return num2;
        }

        Factorization all_factors;
        for (const auto & num : numbers) {
            auto factors = helper_.get_factorization_fast(num);
            for (const auto &[prime, exp] : factors) {
                if (all_factors.find(prime) == all_factors.end()) {
                    all_factors[prime] = exp;
                }
                else {
                    all_factors[prime] = std::max(all_factors.at(prime), exp);
                }
            }
        }
        uint64_t ret{1};
        for (const auto &[prime, exp] : all_factors) {
            for (uint i = 0; i < exp; ++i)
                ret *= prime;
        }

        return ret;
    }

};


int32_t main()
{
    std::cout << "Hello World!\n";
    calculator calc;
    // uint64_t sum{0};
    // for (int64_t x = 2; x < 101; ++x) {
    //     auto tmp = calc.g(x);
    //     std::cout << "g(" << x << ") = " << tmp << std::endl;
    //     sum += tmp;
    // }
    // std::cout << sum << std::endl;

    // std::cout << "G(100) = " << calc.G(100) << std::endl;
    // std::cout << "G(200) = " << calc.G(200) << std::endl;
    // std::cout << "G(400) = " << calc.G(400) << std::endl;
    // std::cout << "G(800) = " << calc.G(800) << std::endl;
    // std::cout << "G(1,000) = " << calc.G(1'000) << std::endl;
    // std::cout << "G(2,000) = " << calc.G(2'000) << std::endl;
    // std::cout << "G(10,000) = " << calc.G(10'000) << std::endl;
    std::cout << "G(1,000,000) = " << calc.G(1'000'000) << std::endl;

    // std::cout << "G(100) = " << calc_bigG_memoize(100) << std::endl;
    // std::cout << "G(1000) = " << calc_bigG_memoize(1000) << std::endl;
}
