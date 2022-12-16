#include "combinatorics.h"

#include <sstream>
#include <vector>

#include "gtest/gtest.h"


TEST(Combinatorics, CombinationN0) {
	for (int n = 1; n < 10; ++n) {
		auto combo_list = get_combinations(n, 0);
		EXPECT_EQ(combo_list.size(), 1);
		EXPECT_EQ(combo_list.begin()->size(), 0);
	}
}

TEST(Combinatorics, CombinationNN) {
	for (int n = 1; n < 10; ++n) {
		auto combo_list = get_combinations(n, n);
		EXPECT_EQ(combo_list.size(), 1);
		EXPECT_EQ(combo_list.begin()->size(), n);
	}
}

TEST(Combinatorics, CombinationN1) {
	for (int n = 1; n < 10; ++n) {
		auto combo_list = get_combinations(n, 1);
		EXPECT_EQ(combo_list.size(), n);
		for (const auto& combo : combo_list) {
			EXPECT_EQ(combo.size(), 1);
		}
	}
}

TEST(Combinatorics, CombinationN2) {
	for (int n = 2; n < 10; ++n) {
		auto combo_list = get_combinations(n, 2);
		EXPECT_EQ(combo_list.size(), n*(n-1)/2);
		for (const auto& combo : combo_list) {
			EXPECT_EQ(combo.size(), 2);
		}
	}
}

TEST(Combinatorics, CombinationN3) {
	for (int n = 3; n < 10; ++n) {
		auto combo_list = get_combinations(n, 3);
		EXPECT_EQ(combo_list.size(), n * (n - 1) * (n - 2) / 6);
		for (const auto& combo : combo_list) {
			EXPECT_EQ(combo.size(), 3);
		}
	}
}

TEST(Combinatorics, PermutationN0) {
	for (int n = 1; n < 10; ++n) {
		auto perm_list = get_permutations(n, 0);
		// For perm(n, 0) we should get a single empty permutation.
		EXPECT_EQ(perm_list.size(), 1);
		EXPECT_EQ(perm_list.front().size(), 0);
	}
}

TEST(Combinatorics, PermutationN1) {
	for (int n = 1; n < 10; ++n) {
		auto perm_list = get_permutations(n, 1);
		// For perm(n, 1) we should get n 1-element permutations, which are each in [0, n) in order.
		EXPECT_EQ(perm_list.size(), n);
		for (int i = 0; i < n; ++i) {
			EXPECT_EQ(perm_list[i].size(), 1);
			EXPECT_EQ(perm_list[i].front(), i);
		}
	}
}

TEST(Combinatorics, PermutationNN) {
	for (int n = 1; n < 10; ++n) {
		auto perm_list = get_permutations(n, n);
		EXPECT_EQ(perm_list.size(), factorial(n));
		for (int i = 0; i < n; ++i) {
			EXPECT_EQ(perm_list[i].size(), n);
		}
		// Verify each oredered permutation is unique by putting them into a set
		std::set<Permutation> unique_perms;
		for (const auto& perm : perm_list)
			unique_perms.insert(perm);
		// The set should have the same number of elements as the vector.
		EXPECT_EQ(unique_perms.size(), perm_list.size());
		// The vector should have the permutations in the same order as the set.
		auto unique_perm_iter = unique_perms.cbegin();
		for (auto perm_iter = perm_list.begin(); perm_iter != perm_list.end(); ++perm_iter, ++unique_perm_iter) {
			EXPECT_EQ(*perm_iter, *unique_perm_iter);
		}
	}
}
