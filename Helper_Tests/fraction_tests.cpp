#include "fraction.h"

#include "gtest/gtest.h"


TEST(GCD, GCD) {
	std::vector<int64_t> test_vals{ 0, 1, -1, 2, 3, -4, 12, -24, 1234560, -987650 };

	for (const auto& val1 : test_vals) {
		for (const auto& val2 : test_vals) {
			auto div = gcd(val1, val2);

			EXPECT_EQ(val1 % div, 0);
			EXPECT_EQ(val2 % div, 0);
		}
	}

	EXPECT_EQ(gcd(85, 119), 17);
	EXPECT_EQ(gcd(84, 120), 12);
}

TEST(Fraction, DefaultCtor) {
	Fraction fr;
	EXPECT_EQ(fr.get_components().first, 0);
	EXPECT_EQ(fr.get_components().second, 1);
}

TEST(Fraction, IntCtor) {
	std::vector<int64_t> test_vals{ 0, 1, -1, 12, -23, 1234567890, -9876543210};

	for (const auto& value : test_vals) {
		Fraction fr{ value };
		EXPECT_EQ(fr.get_components().first, value);
		EXPECT_EQ(fr.get_components().second, 1);
	}
}

TEST(Fraction, IntIntCtor) {
	std::vector<int64_t> test_nums{ 0, 1, -1, 2, 3, -4, 12, -24, 1234560, -987650 };
	std::vector<int64_t> test_dens{ 1, -1, -2, 3, 4, 12, -23, 123456, -98765 };

	for (const auto& num : test_nums) {
		for (const auto& den : test_dens) {
			std::cout << num << "\t" << den << std::endl;
			Fraction fr{ num, den };
			auto div = gcd(num, den);

			bool is_neg = (num < 0) ^ (den < 0);
			EXPECT_EQ(fr.get_components().first, is_neg ? -std::abs(num)/div : std::abs(num)/div);
			EXPECT_EQ(fr.get_components().second, (0 == num) ? 1 : std::abs(den)/div);
		}
	}
}

TEST(Fraction, Equals) {
}

TEST(Fraction, NotEquals) {
}

TEST(Fraction, LessThan) {
}

TEST(Fraction, LessThanOrEquals) {
}

TEST(Fraction, GreaterThan) {
}

TEST(Fraction, GreaterThanOrEquals) {
}

TEST(Fraction, PlusEquals0) {
	std::vector<int64_t> test_nums{ 0, 1, -1, 2, 3, -4, 12, -24, 123450, -987650 };
	std::vector<int64_t> test_dens{ 1, -1, -2, 3, 4, 12, -23, 123456, -98765 };

	const Fraction fr1;
	for (const auto& num : test_nums) {
		for (const auto& den : test_dens) {
			Fraction fr2{ num, den };
			std::cout << "\t" << num << "\t" << den << std::endl;

			fr2 += fr1;

			Fraction exp{ num, den };
			EXPECT_EQ(fr2.get_components().first, exp.get_components().first);
			EXPECT_EQ(fr2.get_components().second, exp.get_components().second);
		}
	}
}

TEST(Fraction, PlusEquals) {
	std::vector<int64_t> test_nums{ 1, -1, 2, 3, -4, 12, -24, 123450, -987650 };
	std::vector<int64_t> test_dens{ 1, -1, -2, 3, 4, 12, -23, 123456, -98765 };

	for (const auto& num1 : test_nums) {
		for (const auto& den1 : test_dens) {
			Fraction fr1{ num1, den1 };
			std::cout << num1 << "\t" << den1 << std::endl;
			for (const auto& num2 : test_nums) {
				for (const auto& den2 : test_dens) {
					Fraction fr2{ num2, den2 };
					std::cout << "\t" << num2 << "\t" << den2 << std::endl;

					fr2 += fr1;

					auto num = num1 * den2 + num2 * den1;
					auto den = den1 * den2;
					auto div = gcd(num, den);

					bool is_neg = (num < 0) ^ (den < 0);
					EXPECT_EQ(fr2.get_components().first, is_neg ? -std::abs(num) / div : std::abs(num) / div);
					EXPECT_EQ(fr2.get_components().second, (0 == num) ? 1 : std::abs(den) / div);
				}
			}
		}
	}
}

TEST(Fraction, MinusEquals0) {
	std::vector<int64_t> test_nums{ 0, 1, -1, 2, 3, -4, 12, -24, 123450, -987650 };
	std::vector<int64_t> test_dens{ 1, -1, -2, 3, 4, 12, -23, 123456, -98765 };

	const Fraction fr1;
	for (const auto& num : test_nums) {
		for (const auto& den : test_dens) {
			Fraction fr2{ num, den };
			std::cout << "\t" << num << "\t" << den << std::endl;

			fr2 -= fr1;

			Fraction exp{ num, den };
			EXPECT_EQ(fr2.get_components().first, exp.get_components().first);
			EXPECT_EQ(fr2.get_components().second, exp.get_components().second);
		}
	}
}

TEST(Fraction, MinusEquals) {
	std::vector<int64_t> test_nums{ 1, -1, 2, 3, -4, 12, -24, 123450, -987650 };
	std::vector<int64_t> test_dens{ 1, -1, -2, 3, 4, 12, -23, 123456, -98765 };

	for (const auto& num1 : test_nums) {
		for (const auto& den1 : test_dens) {
			Fraction fr1{ num1, den1 };
			std::cout << num1 << "\t" << den1 << std::endl;
			for (const auto& num2 : test_nums) {
				for (const auto& den2 : test_dens) {
					Fraction fr2{ num2, den2 };
					std::cout << "\t" << num2 << "\t" << den2 << std::endl;

					fr2 -= fr1;

					auto num = num2 * den1 - num1 * den2;
					auto den = den1 * den2;
					auto div = gcd(num, den);

					bool is_neg = (num < 0) ^ (den < 0);
					EXPECT_EQ(fr2.get_components().first, is_neg ? -std::abs(num) / div : std::abs(num) / div);
					EXPECT_EQ(fr2.get_components().second, (0 == num) ? 1 : std::abs(den) / div);
				}
			}
		}
	}
}

TEST(Fraction, TimesEquals) {
	std::vector<int64_t> test_nums{ 0, 1, -1, 2, 3, -4, 12, -24, 123450, -987650 };
	std::vector<int64_t> test_dens{ 1, -1, -2, 3, 4, 12, -23, 123456, -98765 };

	for (const auto& num1 : test_nums) {
		for (const auto& den1 : test_dens) {
			Fraction fr1{ num1, den1 };
			std::cout << num1 << "\t" << den1 << std::endl;
			for (const auto& num2 : test_nums) {
				for (const auto& den2 : test_dens) {
					Fraction fr2{ num2, den2 };
					std::cout << "\t" << num2 << "\t" << den2 << std::endl;

					fr2 *= fr1;

					auto num = num1 * num2;
					auto den = den1 * den2;
					auto div = gcd(num, den);

					bool is_neg = (num < 0) ^ (den < 0);
					EXPECT_EQ(fr2.get_components().first, is_neg ? -std::abs(num) / div : std::abs(num) / div);
					EXPECT_EQ(fr2.get_components().second, (0 == num) ? 1 : std::abs(den) / div);
				}
			}
		}
	}
}

TEST(Fraction, DivideEquals) {
	std::vector<int64_t> test_nums{ 0, 1, -1, 2, 3, -4, 12, -24, 123450, -987650 };
	std::vector<int64_t> test_dens{ 1, -1, -2, 3, 4, 12, -23, 123456, -98765 };

	for (const auto& num1 : test_nums) {
		if (0 == num1)
			continue;
		for (const auto& den1 : test_dens) {
			Fraction fr1{ num1, den1 };
			std::cout << num1 << "\t" << den1 << std::endl;
			for (const auto& num2 : test_nums) {
				for (const auto& den2 : test_dens) {
					Fraction fr2{ num2, den2 };
					std::cout << "\t" << num2 << "\t" << den2 << std::endl;

					fr2 /= fr1;

					auto num = num2 * den1;
					auto den = den2 * num1;
					auto div = gcd(num, den);

					bool is_neg = (num < 0) ^ (den < 0);
					EXPECT_EQ(fr2.get_components().first, is_neg ? -std::abs(num) / div : std::abs(num) / div);
					EXPECT_EQ(fr2.get_components().second, (0 == num) ? 1 : std::abs(den) / div);
				}
			}
		}
	}
}
