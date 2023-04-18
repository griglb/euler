// In the following equation x, y, and n are positive integers.
//      1   1   1
//      - + - = -
//      x   y   n
//
// It can be verified that when n=1260 there are 113 distinct solutions and this is the
// least value of n for which the total number of distinct solutions exceeds one hundred.
//
// What is the least value of n for which the number of distinct solutions exceeds four million?
//
// NOTE : This problem is a much more difficult version of Problem 108 and as it is well beyond
// the limitations of a brute force approach it requires a clever implementation.


#include <algorithm>
#include <cmath>
#include <iostream>
#include <list>
#include <numeric>
#include <utility>
#include <vector>

#include "combinatorics.h"
#include "prime_helper.h"


// Explore some examples:
// n = 4
//      x = 5, 6, 8
//      x-n = 1, 2, 4
//
// n = 10
//      x = 11, 12, 14, 15, 20
//      x-n = 1, 2, 4, 5, 10
//
// n = 24
//      x = 25, 26, 27, 28, 30, 32, 33, 36, 40, 32, 48
//      x-n = 1, 2, 3, 4, 6, 8, 9, 12, 16, 18, 24
//
// n = 25
//      x = 26, 30, 50
//      x-n = 1, 5, 25
//
// n = 30
//      x = 31, 32, 33, 34, 35, 36, 39, 40, 42, 45, 48, 50, 55, 60
//      x-n = 1, 2, 3, 4, 5, 6, 9, 10, 12, 15, 18, 20, 25, 30
//
// n = 35
//      x = 36, 40, 42, 60, 70
//      x-n = 1, 5, 7, 25, 35
// 
// n = 36
//      x = 37, 38, 39, 40, 42, 44, 45, 48, 52, 54, 60, 63, 72
//      x-n = 1, 2, 3, 4, 6, 8, 9, 12, 16, 18, 24, 27, 36
//
// n = 42
//      x = 43, 44, 45, 46, 48, 49, 51, 54, 56, 60, 63, 70, 78, 84
//      x-n = 1, 2, 3, 4, 6, 7, 9, 12, 14, 18, 21, 28, 36, 42
//
// n = 248
//      x = 249, 250, 252, 256, 264, 279, 280, 310, 312, 372, 496
//      x-n = 1, 2, 4, 8, 16, 31, 32, 62, 64, 124, 248

// Using positive integers, we know both x,y > n.
// We also know the max value for x is when x = y = 2*n;
// So x is in (n, 2*n], y is in [2*n, inf)
// x-n is 1 or has prime factors that are a subset of n's
// Let n = p1^e1 * p2*e2 * ... * pn^en
// Then x-n has the form p1^e1' * p2^e2' * ... * pn^en'
//    where ek' in [0, 2*ek] for all k in [1, n]
// Taking all permutations like this where x-n < n will give all
// the solutions to the equation for a given n.

// 1260 = 2^2 * 3^2 * 5 * 7

using XY = std::pair<uint64_t, uint64_t>;
std::vector<XY> get_xy(const uint64_t n) {
    std::vector<XY> ret;

    for (uint64_t x = n + 1; x < 2 * n; ++x) {
        uint64_t num = n * x;
        uint64_t den = x - n;
        if (num % den == 0)
            ret.push_back({ x, num / den });
    }
    ret.push_back({ 2 * n, 2 * n });

    return ret;
}


using Terms = std::vector<uint64_t>;
Terms cross_product(const Terms& set1, const Terms& set2, uint64_t n) {
    Terms ret;

    for (const auto& t1 : set1) {
        for (const auto& t2 : set2) {
            uint64_t prod = t1 * t2;
            if (prod > n)
                break;
            ret.push_back(prod);
        }
    }

    std::sort(ret.begin(), ret.end());

    return ret;
}


std::vector<XY> get_xy2(const uint64_t n) {
    PrimeHelper helper;
    auto factors = helper.get_factorization_fast(n);

    std::list<Terms> prod;
    for (const auto& [prime, exp] : factors) {
        prod.push_back({ 1 });
        uint64_t p{ prime };
        for (uint64_t e = 0; e < 2 * exp; ++e, p *= prime) {
            if (p > n)
                break;
            prod.back().push_back(p);
        }
    }

    while (prod.size() > 1) {
        const auto set1 = prod.front();
        prod.pop_front();
        const auto set2 = prod.front();
        prod.pop_front();
        prod.push_back(cross_product(set1, set2, n));
    }

    Terms terms = prod.front();
    std::sort(terms.begin(), terms.end());

    std::vector<XY> ret;
    for (const auto& t : terms) {
        uint64_t x = n + t;
        uint64_t y = n * x / t;
        ret.push_back({ x, y });
    }
    return ret;
}


uint64_t get_num_terms(const Factorization& factors, const uint64_t n) {
    std::list<Terms> prod;
    for (const auto& [prime, exp] : factors) {
        prod.push_back({ 1 });
        uint64_t p{ prime };
        for (uint64_t e = 0; e < 2 * exp; ++e, p *= prime) {
            if (p > n)
                break;
            prod.back().push_back(p);
        }
    }

    while (prod.size() > 1) {
        const auto set1 = prod.front();
        prod.pop_front();
        const auto set2 = prod.front();
        prod.pop_front();
        prod.push_back(cross_product(set1, set2, n));
    }

    return prod.front().size();
}


uint64_t get_product(const Factorization& factors) {
    uint64_t ret{ 1 };
    for (const auto& [prime, exp] : factors) {
        for (uint64_t e = 0; e < exp; ++e)
            ret *= prime;
    }
    return ret;
}


uint64_t get_num_potential_terms(const Factorization& factors) {
    uint64_t ret{ 1 };
    for (const auto& [prime, exp] : factors) {
        ret *= 2 * exp + 1;
    }
    return ret;
}


using ExpList = std::vector<uint64_t>;
std::pair<uint64_t, uint64_t> permute_factors(const Factorization &factors, uint64_t min_terms, uint64_t min_n) {
    ExpList exponents;
    for (const auto& [prime, exp] : factors)
        exponents.push_back(exp);

    std::set<ExpList> all_exponents;
    auto perms = get_permutations(exponents.size(), exponents.size());
    for (const auto& perm : perms) {
        ExpList these_exp;
        these_exp.reserve(exponents.size());
        for (const auto& el : perm)
            these_exp.push_back(exponents[el]);
        all_exponents.insert(these_exp);
    }

    uint64_t best_n{ 0xffffffffffffffff };
    uint64_t best_num_terms{ 0 };
    for (const auto& these_exp : all_exponents) {
        auto n = get_product(factors);
        if (n > min_n)
            continue;
        if (n > best_n)
            continue;
        auto num_terms = get_num_terms(factors, n);
        if (num_terms > min_terms) {
            best_n = n;
            best_num_terms = num_terms;
        }
    }

    return { best_n, best_num_terms };
}


std::pair<uint64_t, uint64_t> get_smallest_number(uint64_t min_terms, uint64_t num_primes, uint64_t min_n) {
    PrimeHelper helper;
    auto primes = helper.get_primes(100);

    Factorization factors;

    auto iter = primes.begin();
    for (uint64_t i = 0; i < num_primes; ++i, ++iter)
        factors[*iter] = 1;

    // Index of which prime to increment exponent on next.
    uint64_t index{ 0 };
    while (true) {
        if (get_num_potential_terms(factors) > min_terms) {
            const auto& [n, num_terms] = permute_factors(factors, min_terms, min_n);
            //n = get_product(factors);
            //num_terms = get_num_terms(factors, n);
            if (num_terms > min_terms)
                break;
        }
        auto iter = factors.begin();
        std::advance(iter, index);
        iter->second++;
        // Increment index with wraparound.
        ++index;
        index %= num_primes;
    }

    return { };// n, num_terms };
}


std::set<Factorization> get_exp_sets(uint64_t min_solutions) {
    std::set<Factorization> ret;

    uint64_t max_num_primes = static_cast<uint64_t>(std::ceil(std::log(min_solutions) / std::log(3.0)));

    // max_num_primes is calculated to be the smallest power of 3 that exceeds min_solutions
    ret.insert({ {3, max_num_primes} });

    for (uint64_t num_primes = max_num_primes - 1; num_primes > 1; --num_primes) {
        uint64_t num_3s = num_primes - 1;
        double remainder = min_solutions;
        for (uint64_t e = 0; e < num_3s; ++e)
            remainder /= 3.0;
        uint64_t other = static_cast<uint64_t>(std::ceil(remainder));
        if (other % 2 == 0)
            ++other;
        ret.insert({ {3, num_3s}, {other, 1} });
    }

    return ret;
}


uint64_t get_answer(uint64_t min_terms) {
    uint64_t max_num_primes = static_cast<uint64_t>(std::ceil(std::log(min_terms) / std::log(3.0)));

    uint64_t min_n{ 0xffffffffffffffff };
    for (uint64_t num_primes = max_num_primes; num_primes > 1; --num_primes) {
        const auto& [n, num_terms] = get_smallest_number(min_terms, num_primes, min_n);
        std::cout << "number " << n << " with " << num_primes << " prime factors has " << num_terms << " solutions" << std::endl;
        if (n < min_n) {
            min_n = n;
        }
    }

    return min_n;
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    std::cout << "brute force" << std::endl;
    //    int64_t n = 248;
    //    auto xys = get_xy(n);
    //    for (const auto& [x, y] : xys) {
    //        std::cout << "1/" << x << " + 1/" << y << " = 1/" << n << std::endl;
    //    }

    //    for (const auto& [x, y] : xys) {
    //        std::cout << x << "\t" << x - n << std::endl;
    //    }
    //}

    //{
    //    std::cout << "factors" << std::endl;
    //    int64_t n = 248;
    //    auto xys = get_xy2(n);
    //    for (const auto& [x, y] : xys) {
    //        std::cout << "1/" << x << " + 1/" << y << " = 1/" << n << std::endl;
    //    }

    //    for (const auto& [x, y] : xys) {
    //        std::cout << x << "\t" << x - n << std::endl;
    //    }
    //}

    {
        auto sets = get_exp_sets(4'000'000);
        for (const auto& factors : sets) {
            uint64_t possible{ 1 };
            for (const auto& [num, exp] : factors) {
                for (uint64_t e = 0; e < exp; ++e) {
                    std::cout << num << " ";
                    possible *= num;
                }
            }
            std::cout << " = " << possible << std::endl;
        }
    }

    {
        Factorization factors{ {2, 1}, {3, 1}, {5, 1}, {7, 1}, {11, 1}, {13, 1}, {17, 1}, {19, 1}, {23, 1}, {29, 1}, {31, 2}, {37, 2}, {41, 3} };
        uint64_t n = get_product(factors);
        uint64_t pot = get_num_potential_terms(factors);
        std::cout << n << "\t" << pot << "\t" << get_num_terms(factors, n) << std::endl;
    }

    {
//        std::cout << "answer for 100 terms " << std::endl << get_answer(100) << std::endl;
//        std::cout << "answer for 100 terms " << std::endl << get_answer(4'000'000) << std::endl;
    }
}

//63892555340714100
//4868004216435360 5 N
//9736008432870720 6 N
//19472016865741440 7 N
//38944033731482880 8 Y
//58416050597224320 7 2 Y
//29208025298612160 6 2 Y
//14604012649306080 5 2 Y
//7302006324653040 4 2 N
//21906018973959120 4 3 Y
//36510031623265200 4 2 2 Y
//18255015811632600 3 2 2 Y
//9127507905816300 2 2 2 N
