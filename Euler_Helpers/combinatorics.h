#pragma once

#include <cstdint>
#include <set>
#include <vector>


uint64_t factorial(uint64_t n);

uint64_t get_num_combinations(uint64_t num, uint64_t choose);

using Combination = std::set<uint64_t>;
using CombinationList = std::set<Combination>;
CombinationList get_combinations(uint64_t num, uint64_t choose);

uint64_t get_num_permutations(uint64_t num, uint64_t choose);

using Permutation = std::vector<uint64_t>;
using PermutationList = std::vector<Permutation>;
PermutationList get_permutations(const Combination& elements);
PermutationList get_permutations(uint64_t num, uint64_t choose);

