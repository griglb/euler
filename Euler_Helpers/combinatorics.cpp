#include "combinatorics.h"

#include <algorithm>
#include <iostream>
#include <numeric>

#include "prime_helper.h"

uint64_t factorial(uint64_t n) {
    if (n <= 1)
        return 1;
    return n * factorial(n - 1);
}


uint64_t get_num_combinations(uint64_t num, uint64_t choose) {
    if (0 == num) {
        throw "Number of objects must be positive.";
    }
    if (num < choose) {
        throw "Number of objects must be at least as big as number of choices.";
    }

    if ((num == choose) || (0 == choose)) {
        return 1;
    }

    PrimeHelper helper;

    Factorization numerator;
    for (uint64_t i = num; i > num - choose; --i) {
        Factorization factors = helper.get_factorization(i);
        for (const auto [prime, exponent] : factors) {
            if (numerator.find(prime) == numerator.end()) {
                numerator[prime] = exponent;
            }
            else {
                numerator[prime] += exponent;
            }
        }
    }

    Factorization denominator;
    for (uint64_t i = choose; i > 1; --i) {
        Factorization factors = helper.get_factorization(i);
        for (const auto [prime, exponent] : factors) {
            if (denominator.find(prime) == denominator.end()) {
                denominator[prime] = exponent;
            }
            else {
                denominator[prime] += exponent;
            }
        }
    }

    std::cout << "numerator: ";
    for (const auto [prime, exponent] : numerator) {
        std::cout << prime << "^" << exponent << " ";
    }
    std::cout << std::endl;
    std::cout << "denominator: ";
    for (const auto [prime, exponent] : denominator) {
        std::cout << prime << "^" << exponent << " ";
    }
    std::cout << std::endl;

    // Iterate over denominator, crossing off common factors in numerator.
    for (const auto [prime, exponent] : denominator) {
        if (numerator.find(prime) == numerator.end()) {
            std::cout << "uh oh, can't find multiple of " << prime << std::endl;
        }
        numerator[prime] -= exponent;
    }

    for (const auto [prime, exponent] : numerator) {
        std::cout << prime << "^" << exponent << " ";
    }
    std::cout << std::endl;

    // Calculate product of remaining numerator terms.
    uint64_t product{ 1 };
    for (const auto [prime, exponent] : numerator) {
        for (uint64_t i = 0; i < exponent; ++i) {
            product *= prime;
        }
    }
    return product;
}


CombinationList get_combinations(uint64_t num, uint64_t choose) {
    if (0 == num) {
        throw "Number of objects must be positive.";
    }
    if (num < choose) {
        throw "Number of objects must be at least as big as number of choices.";
    }

    // Special case of C(n, 0) - return empty vector.
    if (0 == choose) {
        return { {} };
    }
    // Special case of C(n, n) - return vector with all values in [0, n).
    if (num == choose) {
        Combination combo;
        for (uint64_t i = 0; i < num; ++i)
            combo.insert(i);
        return { combo };
    }

    CombinationList ret;

    // Recursion termination C(n, 1) - vector of 1-element vectors.
    if (1 == choose) {
        for (uint64_t i = 0; i < num; ++i) {
            ret.insert({ i });
        }
    }
    else {
        Combination indices;
        for (uint64_t i = 0; i < num; ++i) {
            indices.insert(i);
        }
        CombinationList  subset_list = get_combinations(num, choose - 1);
        for (const auto& subset : subset_list) {
            Combination curr_indices = indices;
            for (const auto& elem : subset) {
                curr_indices.erase(elem);
            }
            for (const auto& left : curr_indices) {
                Combination this_combo = subset;
                this_combo.insert(left);
                ret.insert(this_combo);
            }
        }
    }


    return ret;
}


uint64_t get_num_permutations(uint64_t num, uint64_t choose) {
    if (0 == num) {
        throw "Number of objects must be positive.";
    }
    if (num < choose) {
        throw "Number of objects must be at least as big as number of choices.";
    }

    if ((num == choose) || (0 == choose)) {
        return 1;
    }

    PrimeHelper helper;

    Factorization numerator;
    for (uint64_t i = num; i > 1; --i) {
        Factorization factors = helper.get_factorization(i);
        for (const auto [prime, exponent] : factors) {
            if (numerator.find(prime) == numerator.end()) {
                numerator[prime] = exponent;
            }
            else {
                numerator[prime] += exponent;
            }
        }
    }

    Factorization denominator;
    for (uint64_t i = choose; i > 1; --i) {
        Factorization factors = helper.get_factorization(i);
        for (const auto [prime, exponent] : factors) {
            if (denominator.find(prime) == denominator.end()) {
                denominator[prime] = exponent;
            }
            else {
                denominator[prime] += exponent;
            }
        }
    }

    std::cout << "numerator: ";
    for (const auto [prime, exponent] : numerator) {
        std::cout << prime << "^" << exponent << " ";
    }
    std::cout << std::endl;
    std::cout << "denominator: ";
    for (const auto [prime, exponent] : denominator) {
        std::cout << prime << "^" << exponent << " ";
    }
    std::cout << std::endl;

    // Iterate over denominator, crossing off common factors in numerator.
    for (const auto [prime, exponent] : denominator) {
        if (numerator.find(prime) == numerator.end()) {
            std::cout << "uh oh, can't find multiple of " << prime << std::endl;
        }
        numerator[prime] -= exponent;
    }

    for (const auto [prime, exponent] : numerator) {
        std::cout << prime << "^" << exponent << " ";
    }
    std::cout << std::endl;

    // Calculate product of remaining numerator terms.
    uint64_t product{ 1 };
    for (const auto [prime, exponent] : numerator) {
        for (uint64_t i = 0; i < exponent; ++i) {
            product *= prime;
        }
    }
    return product;
}


PermutationList get_permutations(const Combination& elements) {
    if (elements.empty())
        return { { } };
    if (1 == elements.size())
        return { { *elements.begin() } };

    PermutationList ret;
    for (const auto& elem : elements) {
        Combination subset = elements;
        subset.erase(elem);
        PermutationList subsets = get_permutations(subset);
        for (const auto& sub_perm : subsets) {
            // Put the removed element first in the permutation, to keep them in lexicographic order.
            Permutation perm{ elem };
            perm.insert(perm.end(), sub_perm.begin(), sub_perm.end());
            ret.push_back(perm);
        }
    }
    return ret;
}


PermutationList get_permutations(uint64_t num, uint64_t choose) {
    if (0 == num) {
        throw "Number of objects must be positive.";
    }
    if (num < choose) {
        throw "Number of objects must be at least as big as number of choices.";
    }

    // Special case of C(n, 0) - return empty vector.
    if (0 == choose) {
        return { {} };
    }

    PermutationList ret;

    // Recursion termination C(n, 1) - vector of 1-element vectors.
    if (1 == choose) {
        for (uint64_t i = 0; i < num; ++i) {
            ret.push_back({ i });
        }
    }
    else if (choose == num) {
        Combination elements;
        for (uint64_t i = 0; i < num; ++i)
            elements.insert(i);
        return get_permutations(elements);
    }
    else {
        // TODO: this is not completely correct yet
        for (uint64_t first = 0; first <= num - choose; ++first) {
            std::cout << "first = " << first << ", recursing at (" << num - first - 1 << ", " << choose - 1 << ")" << std::endl;
            PermutationList subsets = get_permutations(num - first - 1, choose - 1);
            std::cout << "got " << subsets.size() << " subset permutations" << std::endl;
            for (const auto& sub_perm : subsets) {
                // Put the removed element first in the permutation, to keep them in lexicographic order.
                Permutation perm{ first };
                for (const auto & subp : sub_perm)
                    perm.push_back(subp + first + 1);
                ret.push_back(perm);
            }
        }
    }

    return ret;
}
