// A natural number, N, that can be written as the sum and product of a given set
// of at least two natural numbers, { a1, a2, ... , ak } is called a product-sum number:
//      N = a1 + a2 + ... + ak = a1 * a2 * ... * ak.
// For example, 6 = 1 + 2 + 3 = 1 * 2 * 3.
//
// For a given set of size, k, we shall call the smallest N with this property a minimal
// product-sum number.  The minimal product-sum numbers for sets of size, k = 2, 3, 4, 5,
// and 6 are as follows.
//
//      k = 2 : 4 = 2 * 2 = 2 + 2
//      k = 3 : 6 = 1 * 2 * 3 = 1 + 2 + 3
//      k = 4 : 8 = 1 * 1 * 2 * 4 = 1 + 1 + 2 + 4
//      k = 5 : 8 = 1 * 1 * 2 * 2 * 2 = 1 + 1 + 2 + 2 + 2
//      k = 6 : 12 = 1 * 1 * 1 * 1 * 2 * 6 = 1 + 1 + 1 + 1 + 2 + 6
//
// Hence for 2 <= k <= 6, the sum of all the minimal product-sum numbers is
//      4 + 6 + 8 + 12 = 30; note that 8 is only counted once in the sum.
// In fact, as the complete set of minimal product-sum numbers for 2 <= k <= 12 is
// { 4, 6, 8, 12, 15, 16 }, the sum is 61.
//
// What is the sum of all the minimal product-sum numbers for 2 <= k <= 12000?


#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <vector>

#include "prime_helper.h"
#include "combinatorics.h"


// To complete the set up to k = 12:
//      k =  7 : 12 = 1 * 1 * 1 * 1 * 1 * 3 * 4 = 1 + 1 + 1 + 1 + 1 + 3 + 4
//      k =  8 : 12 = 1 * 1 * 1 * 1 * 1 * 2 * 2 * 3 = 1 + 1 + 1 + 1 + 1 + 2 + 2 + 3
//      k =  9 : 15 = 1 * 1 * 1 * 1 * 1 * 1 * 1 * 3 * 5 = 1 + 1 + 1 + 1 + 1 + 1 + 1 + 3 + 5
//      k = 10 : 16 = 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 4 * 4 = 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 4 + 4
//      k = 11 : 16 = 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 2 * 2 * 4 = 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 2 + 2 + 4
//      k = 12 : 16 = 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 2 * 2 * 2 * 2 = 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 2 + 2 + 2 + 2

// Call function for minimal product-sum number with k terms MPS(k)
// Posit 1 : MPS(k) is a not strictly monotonically increasing function
// Posit 2 : MPS(k) is never a prime number - the only way to have a prime product
//           is (k-1) 1's and the prime, which is never going to sum to the same number
// Posit 3 : For any even k, the number 2*k is a product-sum number, but rarely the minimal product-sum number.
//           It is {(k-2) 1's, 2, k}, which yields yields 2*k as both a sum and product.
// Posit 4 : For any composite N, every possible combination of factors can be a product-sum with enough 1's,
//           but not always a minimum product-sum number.

// k = 22 : 32 = {20 x 1, 4, 8}
// k = 23 : 32 = {20 x 1, 2, 2, 8}
// k = 24 : 32 = {20 x 1, 2, 2, 2, 4}
// k = 25 : 32 = {22 x 1, 2, 4, 4}
// k = 26 : 32 = {21 x 1, 2, 2, 2, 2, 2}


class ProductSumFinder {
public :
    using Addends = std::multiset<uint16_t>;
    using SameSum = std::set<Addends>;
    using Factors = std::vector<uint64_t>;
    using FactorSet = std::set<Factors>;
    using ProductSum = std::vector<uint64_t>;  // 0-based list of cardinalities - {0, 2, 3} is k=5 for N=8
    using ProductSumMap = std::map<uint64_t, std::map<uint64_t, ProductSum>>;  // map of k to map of N to factor cardinalities

    ProductSumFinder() {
        summations_.push_back({ {} });
        summations_.push_back({ {1} });
    }


    ~ProductSumFinder() = default;


    SameSum get_sum_sets(uint16_t total, bool add_total) {
        // This function works correctly, but does not scale well in time or space.
        // It ran out of memory on a 128GB machine before it got to 100.
        for (uint16_t sum = summations_.size(); sum <= total; ++sum) {
            // Set of all sets of addends to add to 'sum'
            SameSum this_sum;
            // If we aren't processing total, then add current number to
            this_sum.insert({ sum });

            for (uint16_t n = 1; n < sum; ++n) {
                SameSum sub_sums = summations_[sum - n];
                for (auto sub : sub_sums) {
                    sub.insert(n);
                    this_sum.insert(sub);
                }
            }
            summations_.push_back(this_sum);
        }

        SameSum ret{ summations_[total] };
        if (!add_total) {
            ret.erase({total});
        }
        return ret;
    }


    FactorSet merge_factors(const FactorSet &set1, const FactorSet &set2) {
        FactorSet ret;

        for (const auto &fact1 : set1) {
            for (const auto &fact2 : set2) {
                Factors concat{fact1};
                std::copy(fact2.begin(), fact2.end(), std::back_inserter(concat));
                std::sort(concat.begin(), concat.end());
                ret.insert(concat);

                for (size_t ind1 = 0; ind1 < fact1.size(); ++ind1) {
                    for (size_t ind2 = 0; ind2 < fact2.size(); ++ind2) {
                        Factors combo;
                        for (size_t copy_ind = 0; copy_ind < fact1.size(); ++copy_ind) {
                            if (copy_ind == ind1)
                                continue;
                            combo.push_back(fact1[copy_ind]);
                        }
                        combo.push_back(fact1[ind1] * fact2[ind2]);
                        for (size_t copy_ind = 0; copy_ind < fact2.size(); ++copy_ind) {
                            if (copy_ind == ind2)
                                continue;
                            combo.push_back(fact2[copy_ind]);
                        }

                        std::sort(combo.begin(), combo.end());
                        ret.insert(combo);
                    }
                }
            }
        }

        return ret;
    }


    FactorSet get_all_factorizations(uint64_t number) {
        FactorSet ret;

        const auto &prime_factorization = helper_.get_factorization_fast(number);

        // First convert each factorization pn^en into collection of every
        // permutation of multiplications.
        std::list<FactorSet> prime_factors;
        for (const auto &[prime, exp] : prime_factorization) {
            FactorSet prime_perms;
            auto sum_sets = get_sum_sets(exp, prime_factorization.size() > 1);
            for (const auto & set : sum_sets) {
                Factors factors;
                for (const auto &el : set) {
                    uint64_t factor{ 1 };
                    for (uint64_t i = 0; i < el; ++i) {
                        factor *= prime;
                    }
                    factors.push_back(factor);
                }
                prime_perms.insert(factors);
            }
            prime_factors.push_back(prime_perms);
        }

        ret = prime_factors.front();
        prime_factors.pop_front();

        while (!prime_factors.empty()) {
            ret = merge_factors(ret, prime_factors.front());
            prime_factors.pop_front();
        }

        auto last_iter = std::prev(ret.end());
        if (last_iter->size() == 1)
            ret.erase(last_iter);

        return ret;
    }


    ProductSumMap get_product_sums(uint64_t max_N) {
        ProductSumMap ret;

        auto tmp = helper_.get_primes(max_N);
        std::set<uint64_t> primes{tmp.begin(), tmp.end()};

        for (uint64_t N = 2; N <= max_N; ++N) {
            // Skip prime numbers;
            if (primes.find(N) != primes.end())
                continue;

            auto factor_set = get_all_factorizations(N);
            // std::cout << N << "\t{";
            // for (const auto & factors : factor_set) {
            //     std::cout << " {";
            //     for (const auto & f : factors)
            //         std::cout << f << ", ";
            //     std::cout << "}, ";
            // }
            // std::cout << std::endl;
            if (N % 10'000 == 0)
                std::cout << N << std::endl;
            for (const auto & factors : factor_set) {
                uint64_t sum = std::accumulate(factors.begin(), factors.end(), 0);
                uint64_t num_ones = N - sum;
                uint64_t k = num_ones + factors.size();

                if (ret.find(k) == ret.end())
                    ret[k] = { };
                ret[k][N] = { };
            }
        }

        return ret;
    }

private:
    std::vector<SameSum> summations_;
    PrimeHelper helper_;

};


int main()
{
    std::cout << "Hello World!\n";

    // {
    //     ProductSumFinder finder;
    //     std::cout << "get_sum_sets(1)" << std::endl;
    //     auto sum_sets = finder.get_sum_sets(1, true);
    //     for (const auto & set : sum_sets) {
    //         for (const auto & el : set) {
    //             std::cout << el << " ";
    //         }
    //         std::cout << std::endl;
    //     }
    //     std::cout << "get_sum_sets(2)" << std::endl;
    //     auto sum_sets2 = finder.get_sum_sets(2, true);
    //     for (const auto & set : sum_sets2) {
    //         for (const auto & el : set) {
    //             std::cout << el << " ";
    //         }
    //         std::cout << std::endl;
    //     }
    //     std::cout << "get_sum_sets(4)" << std::endl;
    //     auto sum_sets4 = finder.get_sum_sets(4, false);
    //     for (const auto & set : sum_sets4) {
    //         for (const auto & el : set) {
    //             std::cout << el << " ";
    //         }
    //         std::cout << std::endl;
    //     }
    // }

    // {
    //     PrimeHelper helper;
    //     ProductSumFinder finder;
    //     auto all_factors = finder.get_all_factorizations(helper.get_factorization_fast(30));
    //     for (const auto & factors: all_factors) {
    //         for (const auto &f : factors)
    //             std::cout << f << " ";
    //         std::cout << std::endl;
    //     }
    // }
    {
        ProductSumFinder finder;
        auto all_factors = finder.get_all_factorizations(18);
        for (const auto & factors: all_factors) {
            for (const auto &f : factors)
                std::cout << f << " ";
            std::cout << std::endl;
        }
        auto all_factors2 = finder.get_all_factorizations(16);
        for (const auto & factors: all_factors2) {
            for (const auto &f : factors)
                std::cout << f << " ";
            std::cout << std::endl;
        }
    }

    {
        ProductSumFinder finder;
        auto product_sum_map = finder.get_product_sums(2'000'000);
        std::set<uint64_t> unique_Ns;
        for (const auto &[k, product_sums] : product_sum_map) {
            if (k > 12'000)
                continue;
            unique_Ns.insert(product_sums.begin()->first);
//            std::cout << k << ":\t" << product_sums.begin()->first << std::endl;
            // for (const auto &[N, factors] : product_sums) {
            //     std::cout << N << " ";
            // }
            // std::cout << std::endl;
        }
        // for (const auto &N : unique_Ns)
        //     std::cout << N << std::endl;
        std::cout << "sum = " << std::accumulate(unique_Ns.begin(), unique_Ns.end(), 0) << std::endl;
    }
}
