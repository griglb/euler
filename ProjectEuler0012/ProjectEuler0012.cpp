#include <iostream>

#include "prime_helper.h"


uint32_t num_factors_triangular_number(uint32_t index, PrimeHelper &helper) {
    // The Nth triangular number has the closed form expression:
    //    N * (N+1) / 2
    // We don't need to calculate all the factors of the triangular number,
    // we can derive it from its prime factorization.  If the number K has the
    // prime factorization of
    //    p_1^e_1 * p_2^e_2 * ... * p_n^e_n
    // then the number of unique factors (including 1 and itself) is equal to
    //    (e_1 + 1) * (e_2 + 1) * ... * (e_n + 1)
    // The prime factorization of the product of 2 numbers is the union of their
    // prime factorizations.  So we can use the prime factorizations of N and (N+1),
    // merge those 2 sets, and subtract one factor of 2 for the division to get the answer.
    // We can also leverage the fact that 2 adjacent integers will never share a common
    // factor, so the two prime factorizations share no common primes and can be more easily
    // processed.

    uint32_t num_factors{ 1 };

    {
        auto fact = helper.get_factorization_fast(index);
        for (auto& [prime, exponent] : fact) {
            if (2 >= prime) {
                num_factors *= exponent;
            }
            else {
                num_factors *= (exponent + 1);
            }
        }
    }

    {
        auto fact = helper.get_factorization_fast(index + 1);
        for (auto& [prime, exponent] : fact) {
            if (2 >= prime) {
                num_factors *= exponent;
            }
            else {
                num_factors *= (exponent + 1);
            }
        }
    }

    return num_factors;
}

int main()
{
    std::cout << "Hello World!\n";
    //PrimeHelper helper;
    //auto prim = helper.get_primes(100);
    //for (auto p : prim) {
    //    std::cout << p << "  ";
    //}
    //std::cout << std::endl;

    for (int i = 1; i < 100; ++i) {
        PrimeHelper helper;
        Factorization factors = helper.get_factorization_fast(i);
        std::cout << i << ": ";
        for (auto f : factors) {
            std::cout << f.first << "^" << f.second << "  ";
        }
        std::cout << std::endl;
    }

    uint32_t max_factors{ 1 };
    uint32_t i = 1;
    PrimeHelper helper;
    while (true) {
        auto num_factors = num_factors_triangular_number(i, helper);
        if (num_factors > max_factors) {
            std::cout << i << " = " << num_factors << std::endl;
            max_factors = num_factors;
        }
        if (num_factors > 500) {
            auto triangular = i * (i + 1) / 2;
            std::cout << "Done!, the " << i << "th triangular number " << triangular << " has " << num_factors << " factors.";
            Factorization fact1 = helper.get_factorization_fast(i);
            Factorization fact2 = helper.get_factorization_fast(i + 1);
            fact1.merge(fact2);
            fact1[2] = fact1.at(2) - 1;

            std::cout << "    ";
            for (auto f : fact1) {
                std::cout << f.first << "^" << f.second << "  ";
            }
            std::cout << std::endl;
            break;
        }
        ++i;
    }
}
