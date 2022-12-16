#include "big_int.h"

#include <sstream>
#include <vector>

#include "gtest/gtest.h"


constexpr int8_t MaxInt8{ INT8_MAX };
constexpr int8_t MinInt8{ INT8_MIN };
constexpr uint8_t MaxUInt8{ UINT8_MAX };

constexpr int16_t MaxInt16{ INT16_MAX };
constexpr int16_t MinInt16{ INT16_MIN };
constexpr uint16_t MaxUInt16{ UINT16_MAX };

constexpr int32_t MaxInt32{ INT32_MAX };
constexpr int32_t MinInt32{ INT32_MIN };
constexpr uint32_t MaxUInt32{ UINT32_MAX };

constexpr int64_t MaxInt64{ 0x7FFFFFFFFFFFFFFFLL };
constexpr int64_t MinInt64{ 0x8000000000000000LL };
constexpr uint64_t MaxUInt64{ 0xFFFFFFFFFFFFFFFFULL };


TEST(BigInt, DefaultCtor) {
	BigInt bi;
	EXPECT_EQ(bi.to_int(), 0);
}

TEST(BigInt, Int8Ctor) {
	std::vector<int8_t> test_vals{ 0, 1, -1, 12, -23, MaxInt8, MinInt8 };

	for (const auto& value : test_vals) {
		BigInt bi{ value };
		EXPECT_EQ(bi.to_int(), value);
	}
}

TEST(BigInt, Int16Ctor) {
	std::vector<int16_t> test_vals{ 0, 1, -1, 12, -23, 12345, -23456, MaxInt16, MinInt16 };

	for (const auto& value : test_vals) {
		BigInt bi{ value };
		EXPECT_EQ(bi.to_int(), value);
	}
}

TEST(BigInt, Int32Ctor) {
	std::vector<int32_t> test_vals{ 0, 1, -1, 12, -23, 123456789, -234567890, MaxInt32, MinInt32 };

	for (const auto& value : test_vals) {
		BigInt bi{ value };
		EXPECT_EQ(bi.to_int(), value);
	}
}

TEST(BigInt, Int64Ctor) {
	std::vector<int64_t> test_vals{ 0, 1, -1, 12, -23, 123456789, -234567890, MaxInt64, MinInt64 };

	for (const auto& value : test_vals) {
		BigInt bi{ value };
		EXPECT_EQ(bi.to_int(), value);
	}
}

TEST(BigInt, UInt8Ctor) {
	std::vector<uint8_t> test_vals{ 0, 1, 12, 123, MaxUInt8 };

	for (const auto& value : test_vals) {
		BigInt bi{ value };
		EXPECT_EQ(bi.to_int(), value);
	}
}

TEST(BigInt, UInt16Ctor) {
	std::vector<uint16_t> test_vals{ 0, 1, 12, 54321, MaxUInt16 };

	for (const auto& value : test_vals) {
		BigInt bi{ value };
		EXPECT_EQ(bi.to_int(), value);
	}
}

TEST(BigInt, UInt32Ctor) {
	std::vector<uint32_t> test_vals{ 0, 1, 12, 123456789, MaxUInt32 };

	for (const auto& value : test_vals) {
		BigInt bi{ value };
		EXPECT_EQ(bi.to_int(), value);
	}
}

TEST(BigInt, UInt64Ctor) {
	std::vector<uint64_t> test_vals{ 0, 1, 12, 123456789, MaxUInt64 };

	for (const auto& value : test_vals) {
		BigInt bi{ value };
		EXPECT_EQ(bi.to_int(), value);
	}
}

TEST(BigInt, CopyCtor) {
	std::vector<int64_t> test_vals{ 0, 1, -1, 12, -23, 123456789, -234567890, MaxInt64, MinInt64 };

	for (const auto& value : test_vals) {
		BigInt bi{ value };
		BigInt bi2{ bi };
		EXPECT_EQ(bi2.to_int(), value);
	}
}

TEST(BigInt, Int64Assign) {
	std::vector<int64_t> test_vals{ 0, 1, -1, 12, -23, 123456789, -234567890, MaxInt64, MinInt64 };

	for (const auto& value : test_vals) {
		BigInt bi;
		EXPECT_EQ(bi.to_int(), 0);
		bi = value;
		EXPECT_EQ(bi.to_int(), value);
	}
}

TEST(BigInt, UInt64Assign) {
	std::vector<uint64_t> test_vals{ 0, 1, 12, 123456789, MaxUInt64 };

	for (const auto& value : test_vals) {
		BigInt bi;
		EXPECT_EQ(bi.to_int(), 0);
		bi = value;
		EXPECT_EQ(bi.to_int(), value);
	}
}

TEST(BigInt, CopyOper) {
	std::vector<int64_t> test_vals{ 0, 1, -1, 12, -23, 123456789, -234567890, MaxInt64, MinInt64 };

	for (const auto& value : test_vals) {
		BigInt bi{ value };
		BigInt bi2;
		EXPECT_EQ(bi2.to_int(), 0);
		bi2 = bi;
		EXPECT_EQ(bi2.to_int(), value);
	}
}

TEST(BigInt, Int64Equals) {
	std::vector<int64_t> test_vals{ 0, 1, -1, 12, -23, 123456789, -234567890, MaxInt64, MinInt64 };

	for (const auto& value : test_vals) {
		BigInt bi{ value };
		EXPECT_TRUE(bi == value);
		EXPECT_FALSE(bi == (value + 1));
		EXPECT_FALSE(bi == (value - 1));
	}
}

TEST(BigInt, UInt64Equals) {
	std::vector<uint64_t> test_vals{ 0, 1, 12, 123456789, MaxUInt64 };

	for (const auto& value : test_vals) {
		BigInt bi{ value };
		EXPECT_TRUE(bi == value);
		EXPECT_FALSE(bi == (value + 1));
		EXPECT_FALSE(bi == (value - 1));
	}
}

TEST(BigInt, BigIntEquals) {
	std::vector<int64_t> test_vals{ 0, 1, -1, 12, -23, 123456789, -234567890, MaxInt64, MinInt64 };

	for (const auto& value : test_vals) {
		BigInt bi{ value };
		BigInt bi2{ value };
		EXPECT_TRUE(bi == bi2);
		BigInt bi3{ value + 1};
		EXPECT_FALSE(bi == bi3);
		BigInt bi4{ value - 1};
		EXPECT_FALSE(bi == bi4);
	}
}

TEST(BigInt, Int64NotEquals) {
	std::vector<int64_t> test_vals{ 0, 1, -1, 12, -23, 123456789, -234567890, MaxInt64, MinInt64 };

	for (const auto& value : test_vals) {
		BigInt bi{ value };
		EXPECT_FALSE(bi != value);
		EXPECT_TRUE(bi != (value + 1));
		EXPECT_TRUE(bi != (value - 1));
	}
}

TEST(BigInt, UInt64NotEquals) {
	std::vector<uint64_t> test_vals{ 0, 1, 12, 123456789, MaxUInt64 };

	for (const auto& value : test_vals) {
		BigInt bi{ value };
		EXPECT_FALSE(bi != value);
		EXPECT_TRUE(bi != (value + 1));
		EXPECT_TRUE(bi != (value - 1));
	}
}

TEST(BigInt, BigIntNotEquals) {
	std::vector<int64_t> test_vals{ 0, 1, -1, 12, -23, 123456789, -234567890, MaxInt64, MinInt64 };

	for (const auto& value : test_vals) {
		BigInt bi{ value };
		BigInt bi2{ value };
		EXPECT_FALSE(bi != bi2);
		BigInt bi3{ value + 1 };
		EXPECT_TRUE(bi != bi3);
		BigInt bi4{ value - 1 };
		EXPECT_TRUE(bi != bi4);
	}
}

TEST(BigInt, BigIntLessThan) {
	std::vector<int64_t> test_vals{ 0, 1, -1, 12, -23, 1111, 1211, 123456789, 234567890, -123456789, -234567890, MaxInt64, MinInt64 };

	for (const auto& value1 : test_vals) {
		BigInt bi1{ value1 };
		for (const auto& value2 : test_vals) {
			BigInt bi2{ value2 };

			EXPECT_EQ(bi1 < bi2, bi1.to_int() < bi2.to_int());
		}
	}
}

TEST(BigInt, BigIntLessThanEquals) {
	std::vector<int64_t> test_vals{ 0, 1, -1, 12, -23, 1111, 1211, 123456789, 234567890, -123456789, -234567890, MaxInt64, MinInt64 };

	for (const auto& value1 : test_vals) {
		BigInt bi1{ value1 };
		for (const auto& value2 : test_vals) {
			BigInt bi2{ value2 };

			EXPECT_EQ(bi1 <= bi2, bi1.to_int() <= bi2.to_int());
		}
	}
}

TEST(BigInt, BigIntGreaterThan) {
	std::vector<int64_t> test_vals{ 0, 1, -1, 12, -23, 1111, 1211, 123456789, 234567890, -123456789, -234567890, MaxInt64, MinInt64 };

	for (const auto& value1 : test_vals) {
		BigInt bi1{ value1 };
		for (const auto& value2 : test_vals) {
			BigInt bi2{ value2 };

			EXPECT_EQ(bi1 > bi2, bi1.to_int() > bi2.to_int());
		}
	}
}

TEST(BigInt, BigIntGreaterThanEquals) {
	std::vector<int64_t> test_vals{ 0, 1, -1, 12, -23, 1111, 1211, 123456789, 234567890, -123456789, -234567890, MaxInt64, MinInt64 };

	for (const auto& value1 : test_vals) {
		BigInt bi1{ value1 };
		for (const auto& value2 : test_vals) {
			BigInt bi2{ value2 };

			EXPECT_EQ(bi1 >= bi2, bi1.to_int() >= bi2.to_int());
		}
	}
}

TEST(BigInt, ToString) {
	std::vector<int64_t> test_vals{ 0, 1, -1, 12, -23, 123456789, -234567890, MaxInt64, MinInt64 };
	std::vector<std::string> expect_vals{ "0", "1", "-1", "12", "-23", "123456789", "-234567890", "9223372036854775807", "-9223372036854775808" };

	auto& expect_iter = expect_vals.cbegin();
	for (const auto& value : test_vals) {
		BigInt bi{ value };
		EXPECT_EQ(bi.to_string(), *expect_iter++);
	}
}

TEST(BigInt, PreIncrement) {
	{
		std::vector<int64_t> test_vals{ 0, 1, 2, 3, 9, 99, 999, 123456789, 999999999, MaxInt64 };
		std::vector<std::string> expect_vals{ "1", "2", "3", "4", "10", "100", "1000", "123456790", "1000000000", "9223372036854775808" };

		auto& expect_iter = expect_vals.cbegin();
		for (auto& value : test_vals) {
			BigInt bi{ value };
			++bi;
			EXPECT_EQ(bi.to_int(), value+1);
			EXPECT_EQ(bi.to_string(), *expect_iter++);
		}
	}

	{
		std::vector<int64_t> test_vals{ -1, -2, -3, -10, -100, -1000, -234567890, -1000000000, MinInt64 };
		std::vector<std::string> expect_vals{ "0", "-1", "-2", "-9", "-99", "-999", "-234567889", "-999999999", "-9223372036854775807" };

		auto& expect_iter = expect_vals.cbegin();
		for (auto& value : test_vals) {
			BigInt bi{ value };
			++bi;
			EXPECT_EQ(bi.to_int(), value+1);
			EXPECT_EQ(bi.to_string(), *expect_iter++);
		}
	}
}

TEST(BigInt, PostIncrement) {
	{
		std::vector<int64_t> test_vals{ 0, 1, 2, 3, 9, 99, 999, 123456789, 999999999, MaxInt64 };
		std::vector<std::string> expect_vals{ "1", "2", "3", "4", "10", "100", "1000", "123456790", "1000000000", "9223372036854775808" };

		auto& expect_iter = expect_vals.cbegin();
		for (auto& value : test_vals) {
			BigInt bi{ value };
			BigInt bi2 = bi++;
			EXPECT_EQ(bi.to_int(), value + 1);
			EXPECT_EQ(bi.to_string(), *expect_iter++);
			EXPECT_EQ(bi2.to_int(), value);
		}
	}

	{
		std::vector<int64_t> test_vals{ -1, -2, -3, -10, -100, -1000, -234567890, -1000000000, MinInt64 };
		std::vector<std::string> expect_vals{ "0", "-1", "-2", "-9", "-99", "-999", "-234567889", "-999999999", "-9223372036854775807" };

		auto& expect_iter = expect_vals.cbegin();
		for (auto& value : test_vals) {
			BigInt bi{ value };
			BigInt bi2 = bi++;
			EXPECT_EQ(bi.to_int(), value + 1);
			EXPECT_EQ(bi.to_string(), *expect_iter++);
			EXPECT_EQ(bi2.to_int(), value);
		}
	}
}

TEST(BigInt, PreDecrement) {
	{
		std::vector<int64_t> test_vals{ 1, 2, 3, 10, 100, 1000, 234567890, 1000000000, MaxInt64 };
		std::vector<std::string> expect_vals{ "0", "1", "2", "9", "99", "999", "234567889", "999999999", "9223372036854775806" };

		auto& expect_iter = expect_vals.cbegin();
		for (auto& value : test_vals) {
			BigInt bi{ value };
			--bi;
			EXPECT_EQ(bi.to_int(), value - 1);
			EXPECT_EQ(bi.to_string(), *expect_iter++);
		}
	}

	{
		std::vector<int64_t> test_vals{ 0, -1, -2, -3, -9, -99, -999, -123456789, -999999999, MinInt64 };
		std::vector<std::string> expect_vals{ "-1", "-2", "-3", "-4", "-10", "-100", "-1000", "-123456790", "-1000000000", "-9223372036854775809" };

		auto& expect_iter = expect_vals.cbegin();
		for (auto& value : test_vals) {
			BigInt bi{ value };
			--bi;
			EXPECT_EQ(bi.to_int(), value - 1);
			EXPECT_EQ(bi.to_string(), *expect_iter++);
		}
	}
}

TEST(BigInt, PostDecrement) {
	{
		std::vector<int64_t> test_vals{ 1, 2, 3, 10, 100, 1000, 234567890, 1000000000, MaxInt64 };
		std::vector<std::string> expect_vals{ "0", "1", "2", "9", "99", "999", "234567889", "999999999", "9223372036854775806" };

		auto& expect_iter = expect_vals.cbegin();
		for (auto& value : test_vals) {
			BigInt bi{ value };
			BigInt bi2 = bi--;
			EXPECT_EQ(bi.to_int(), value - 1);
			EXPECT_EQ(bi.to_string(), *expect_iter++);
			EXPECT_EQ(bi2.to_int(), value);
		}
	}

	{
		std::vector<int64_t> test_vals{ 0, -1, -2, -3, -9, -99, -999, -123456789, -999999999, MinInt64 };
		std::vector<std::string> expect_vals{ "-1", "-2", "-3", "-4", "-10", "-100", "-1000", "-123456790", "-1000000000", "-9223372036854775809" };

		auto& expect_iter = expect_vals.cbegin();
		for (auto& value : test_vals) {
			BigInt bi{ value };
			BigInt bi2 = bi--;
			EXPECT_EQ(bi.to_int(), value - 1);
			EXPECT_EQ(bi.to_string(), *expect_iter++);
			EXPECT_EQ(bi2.to_int(), value);
		}
	}
}

TEST(BigInt, InplaceAdd0) {
	BigInt zero;

	{
		std::vector<int64_t> test_vals{ 0, 1, 2, 3, 9, 99, 999, 123456789, 999999999, MaxInt64 };

		for (auto& value : test_vals) {
			BigInt bi{ value };
			bi += zero;
			EXPECT_EQ(bi.to_int(), value);
			EXPECT_EQ(zero.to_int(), 0);
		}
	}

	{
		std::vector<int64_t> test_vals{ -1, -2, -3, -10, -100, -1000, -234567890, -1000000000, MinInt64 };

		for (auto& value : test_vals) {
			BigInt bi{ value };
			bi += zero;
			EXPECT_EQ(bi.to_int(), value);
			EXPECT_EQ(zero.to_int(), 0);
		}
	}
}

TEST(BigInt, InplaceAdd1) {
	BigInt one{ 1LL };

	{
		std::vector<int64_t> test_vals{ 0, 1, 2, 3, 9, 99, 999, 123456789, 999999999, MaxInt64 };
		std::vector<std::string> expect_vals{ "1", "2", "3", "4", "10", "100", "1000", "123456790", "1000000000", "9223372036854775808" };

		auto& expect_iter = expect_vals.cbegin();
		for (auto& value : test_vals) {
			BigInt bi{ value };
			bi += one;
			EXPECT_EQ(bi.to_int(), value + 1);
			EXPECT_EQ(bi.to_string(), *expect_iter++);
			EXPECT_EQ(one.to_int(), 1);
		}
	}

	{
		std::vector<int64_t> test_vals{ -1, -2, -3, -10, -100, -1000, -234567890, -1000000000, MinInt64 };
		std::vector<std::string> expect_vals{ "0", "-1", "-2", "-9", "-99", "-999", "-234567889", "-999999999", "-9223372036854775807" };

		auto& expect_iter = expect_vals.cbegin();
		for (auto& value : test_vals) {
			BigInt bi{ value };
			bi += one;
			EXPECT_EQ(bi.to_int(), value + 1);
			EXPECT_EQ(bi.to_string(), *expect_iter++);
			EXPECT_EQ(one.to_int(), 1);
		}
	}
}

TEST(BigInt, InplaceAddNeg1) {
	BigInt neg_one{ -1LL };

	{
		std::vector<int64_t> test_vals{ 1, 2, 3, 10, 100, 1000, 234567890, 1000000000, MaxInt64 };
		std::vector<std::string> expect_vals{ "0", "1", "2", "9", "99", "999", "234567889", "999999999", "9223372036854775806" };

		auto& expect_iter = expect_vals.cbegin();
		for (auto& value : test_vals) {
			BigInt bi{ value };
			bi += neg_one;
			EXPECT_EQ(bi.to_int(), value - 1);
			EXPECT_EQ(bi.to_string(), *expect_iter++);
			EXPECT_EQ(neg_one.to_int(), -1);
		}
	}

	{
		std::vector<int64_t> test_vals{ 0, -1, -2, -3, -9, -99, -999, -123456789, -999999999, MinInt64 };
		std::vector<std::string> expect_vals{ "-1", "-2", "-3", "-4", "-10", "-100", "-1000", "-123456790", "-1000000000", "-9223372036854775809" };

		auto& expect_iter = expect_vals.cbegin();
		for (auto& value : test_vals) {
			BigInt bi{ value };
			bi += neg_one;
			EXPECT_EQ(bi.to_int(), value - 1);
			EXPECT_EQ(bi.to_string(), *expect_iter++);
			EXPECT_EQ(neg_one.to_int(), -1);
		}
	}
}

TEST(BigInt, InplaceAddN) {
	std::vector<int64_t> test_vals{ 0, 1, 2, 3, 9, 99, 999, 123456789, 999999999,
								   -1, -2, -3, -10, -100, -1000, -234567890, -1000000000 };

	for (const auto& value1 : test_vals) {
		BigInt bi1{ value1 };
		for (const auto& value2 : test_vals) {
			BigInt bi2{ value2 };

			bi2 += bi1;

			EXPECT_EQ(bi1.to_int(), value1);
			EXPECT_EQ(bi2.to_int(), value2 + value1);
		}
	}
}

TEST(BigInt, InplaceSubtract0) {
	BigInt zero;

	{
		std::vector<int64_t> test_vals{ 0, 1, 2, 3, 9, 99, 999, 123456789, 999999999, MaxInt64 };

		for (auto& value : test_vals) {
			BigInt bi{ value };
			bi -= zero;
			EXPECT_EQ(bi.to_int(), value);
			EXPECT_EQ(zero.to_int(), 0);
		}
	}

	{
		std::vector<int64_t> test_vals{ -1, -2, -3, -10, -100, -1000, -234567890, -1000000000, MinInt64 };

		for (auto& value : test_vals) {
			BigInt bi{ value };
			bi -= zero;
			EXPECT_EQ(bi.to_int(), value);
			EXPECT_EQ(zero.to_int(), 0);
		}
	}
}

TEST(BigInt, InplaceSubtract1) {
	BigInt one{ 1LL };

	{
		std::vector<int64_t> test_vals{ 1, 2, 3, 10, 100, 1000, 234567890, 1000000000, MaxInt64 };
		std::vector<std::string> expect_vals{ "0", "1", "2", "9", "99", "999", "234567889", "999999999", "9223372036854775806" };

		auto& expect_iter = expect_vals.cbegin();
		for (auto& value : test_vals) {
			BigInt bi{ value };
			bi -= one;
			EXPECT_EQ(bi.to_int(), value - 1);
			EXPECT_EQ(bi.to_string(), *expect_iter++);
			EXPECT_EQ(one.to_int(), 1);
		}
	}

	{
		std::vector<int64_t> test_vals{ 0, -1, -2, -3, -9, -99, -999, -123456789, -999999999, MinInt64 };
		std::vector<std::string> expect_vals{ "-1", "-2", "-3", "-4", "-10", "-100", "-1000", "-123456790", "-1000000000", "-9223372036854775809" };

		auto& expect_iter = expect_vals.cbegin();
		for (auto& value : test_vals) {
			BigInt bi{ value };
			bi -= one;
			EXPECT_EQ(bi.to_int(), value - 1);
			EXPECT_EQ(bi.to_string(), *expect_iter++);
			EXPECT_EQ(one.to_int(), 1);
		}
	}
}

TEST(BigInt, InplaceSubtractNeg1) {
	BigInt neg_one{ -1LL };

	{
		std::vector<int64_t> test_vals{ 0, 1, 2, 3, 9, 99, 999, 123456789, 999999999, MaxInt64 };
		std::vector<std::string> expect_vals{ "1", "2", "3", "4", "10", "100", "1000", "123456790", "1000000000", "9223372036854775808" };

		auto& expect_iter = expect_vals.cbegin();
		for (auto& value : test_vals) {
			BigInt bi{ value };
			bi -= neg_one;
			EXPECT_EQ(bi.to_int(), value + 1);
			EXPECT_EQ(bi.to_string(), *expect_iter++);
			EXPECT_EQ(neg_one.to_int(), -1);
		}
	}

	{
		std::vector<int64_t> test_vals{ -1, -2, -3, -10, -100, -1000, -234567890, -1000000000, MinInt64 };
		std::vector<std::string> expect_vals{ "0", "-1", "-2", "-9", "-99", "-999", "-234567889", "-999999999", "-9223372036854775807" };

		auto& expect_iter = expect_vals.cbegin();
		for (auto& value : test_vals) {
			BigInt bi{ value };
			bi -= neg_one;
			EXPECT_EQ(bi.to_int(), value + 1);
			EXPECT_EQ(bi.to_string(), *expect_iter++);
			EXPECT_EQ(neg_one.to_int(), -1);
		}
	}
}

TEST(BigInt, InplaceSubtractN) {
	std::vector<int64_t> test_vals{ 0, 1, 2, 3, 9, 99, 999, 123456789, 999999999,
								   -1, -2, -3, -10, -100, -1000, -234567890, -1000000000 };

	for (const auto& value1 : test_vals) {
		BigInt bi1{ value1 };
		for (const auto& value2 : test_vals) {
			BigInt bi2{ value2 };

			bi2 -= bi1;

			EXPECT_EQ(bi1.to_int(), value1);
			EXPECT_EQ(bi2.to_int(), value2 - value1);
		}
	}
}

TEST(BigInt, InplaceMultiply0) {
	BigInt zero;

	{
		std::vector<int64_t> test_vals{ 0, 1, 2, 3, 9, 99, 999, 123456789, 999999999, MaxInt64 };

		for (auto& value : test_vals) {
			BigInt bi{ value };
			bi *= zero;
			EXPECT_EQ(bi.to_int(), 0);
			EXPECT_EQ(zero.to_int(), 0);
		}
	}

	{
		std::vector<int64_t> test_vals{ -1, -2, -3, -10, -100, -1000, -234567890, -1000000000, MinInt64 };

		for (auto& value : test_vals) {
			BigInt bi{ value };
			bi *= zero;
			EXPECT_EQ(bi.to_int(), 0);
			EXPECT_EQ(zero.to_int(), 0);
		}
	}
}

TEST(BigInt, InplaceMultiply1) {
	BigInt one{ 1LL };

	{
		std::vector<int64_t> test_vals{ 0, 1, 2, 3, 9, 99, 999, 123456789, 999999999, MaxInt64 };

		for (auto& value : test_vals) {
			BigInt bi{ value };
			bi *= one;
			EXPECT_EQ(bi.to_int(), value);
			EXPECT_EQ(one.to_int(), 1);
		}
	}

	{
		std::vector<int64_t> test_vals{ -1, -2, -3, -10, -100, -1000, -234567890, -1000000000, MinInt64 };

		for (auto& value : test_vals) {
			BigInt bi{ value };
			bi *= one;
			EXPECT_EQ(bi.to_int(), value);
			EXPECT_EQ(one.to_int(), 1);
		}
	}
}

TEST(BigInt, InplaceMultiplyNeg1) {
	BigInt neg_one{ -1LL };

	{
		std::vector<int64_t> test_vals{ 1, 2, 3, 10, 100, 1000, 234567890, 1000000000, MaxInt64 };

		for (auto& value : test_vals) {
			BigInt bi{ value };
			bi *= neg_one;
			EXPECT_EQ(bi.to_int(), -value);
			EXPECT_EQ(neg_one.to_int(), -1);
		}
	}

	{
		std::vector<int64_t> test_vals{ 0, -1, -2, -3, -9, -99, -999, -123456789, -999999999, MinInt64 };

		for (auto& value : test_vals) {
			BigInt bi{ value };
			bi *= neg_one;
			EXPECT_EQ(bi.to_int(), -value);
			EXPECT_EQ(neg_one.to_int(), -1);
		}
	}
}

TEST(BigInt, InplaceMultiplyN) {
	std::vector<int64_t> test_vals{ 0, 1, 2, 3, 9, 99, 999, 123456789, 999999999,
								   -1, -2, -3, -10, -100, -1000, -234567890, -1000000000 };

	for (const auto& value1 : test_vals) {
		BigInt bi1{ value1 };
		for (const auto& value2 : test_vals) {
			BigInt bi2{ value2 };
			bi2 *= bi1;

			EXPECT_EQ(bi1.to_int(), value1);
			EXPECT_EQ(bi2.to_int(), value2 * value1);
		}
	}
}

TEST(BigInt, StreamOut) {
	std::vector<int64_t> test_vals{ 0, 1, -1, 12, -23, 123456789, -234567890, MaxInt64, MinInt64 };
	std::vector<std::string> expect_vals{ "0", "1", "-1", "12", "-23", "123456789", "-234567890", "9223372036854775807", "-9223372036854775808" };

	auto& expect_iter = expect_vals.cbegin();
	for (const auto& value : test_vals) {
		BigInt bi{ value };
		std::stringstream str;
		str << bi;
		EXPECT_EQ(str.str(), *expect_iter++);
	}
}