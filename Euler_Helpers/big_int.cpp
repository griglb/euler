#include "big_int.h"

#include <math.h>

#include <algorithm>
#include <cstdint>
#include <list>


constexpr int64_t MinInt8{ INT8_MIN };
constexpr int64_t MaxInt8{ INT8_MAX };
constexpr int64_t MinInt16{ INT16_MIN };
constexpr int64_t MaxInt16{ INT16_MAX };
constexpr int64_t MinInt32{ INT32_MIN };
constexpr int64_t MaxInt32{ INT32_MAX };
constexpr int64_t MinInt64{ INT64_MIN };
constexpr int64_t MaxInt64{ INT64_MAX };
constexpr uint64_t MaxUInt64{ UINT64_MAX };

template <typename T>
void store_signed_digits(const T& value, BigInt::Digits& digits) {
	int64_t big_value{ value };
	// Need a special case for -2^63, as the negation will make it 0
	if (MinInt64 == big_value) {
		big_value--;
		digits.push_back((big_value % 10) + 1);
		big_value /= 10;
		while (big_value != 0) {
			digits.push_back(big_value % 10);
			big_value /= 10;
		}
	}
	else {
		if (big_value < 0)
			big_value *= -1;
		// Append the digits one at a time, which is more efficient than prepending.
		while (big_value != 0) {
			digits.push_back(big_value % 10);
			big_value /= 10;
		}
	}
	// Now flip the order of digits to be correct.
	std::reverse(digits.begin(), digits.end());
}


template <typename T>
void store_unsigned_digits(const T& value, BigInt::Digits& digits) {
	uint64_t big_value{ value };
	// Append the digits one at a time, which is more efficient than prepending.
	while (big_value > 0) {
		digits.push_back(big_value % 10);
		big_value /= 10;
	}
	// Now flip the order of digits to be correct.
	std::reverse(digits.begin(), digits.end());
}


BigInt::BigInt()
	: digits_(0)
	, is_negative_(false)
{ }


BigInt::BigInt(int8_t value) :	is_negative_(value < 0) {
	store_signed_digits(value, digits_);
}


BigInt::BigInt(int16_t value) : is_negative_(value < 0) {
	store_signed_digits(value, digits_);
}


BigInt::BigInt(int32_t value) : is_negative_(value < 0) {
	store_signed_digits(value, digits_);
}


BigInt::BigInt(int64_t value) : is_negative_(value < 0) {
	store_signed_digits(value, digits_);
}


BigInt::BigInt(uint8_t value) : is_negative_(false) {
	store_unsigned_digits(value, digits_);
}


BigInt::BigInt(uint16_t value) : is_negative_(false) {
	store_unsigned_digits(value, digits_);
}


BigInt::BigInt(uint32_t value) : is_negative_(false) {
	store_unsigned_digits(value, digits_);
}


BigInt::BigInt(uint64_t value) : is_negative_(false) {
	store_unsigned_digits(value, digits_);
}


BigInt::BigInt(double value) : is_negative_(value < 0) {
	if (is_negative_)
		value *= -1;
	// Append the digits one at a time, which is more efficient than prepending.
	while (value >= 1) {
		digits_.push_back(static_cast<uint8_t>(fmod(value, 10)));
		value /= 10.0;
	}
	// Now flip the order of digits to be correct.
	std::reverse(digits_.begin(), digits_.end());
}


BigInt::BigInt(const BigInt& other)
   : digits_(other.digits_)
   , is_negative_(other.is_negative_)
{ }


BigInt& BigInt::operator=(int64_t rhs) {
	is_negative_ = rhs < 0;

	// Need a special case for -2^63, as the negation will make it 0
	if (MinInt64 == rhs) {
		rhs--;
		digits_.push_back((rhs % 10) + 1);
		rhs /= 10;
		while (rhs != 0) {
			digits_.push_back(rhs % 10);
			rhs /= 10;
		}
	}
	else {
		if (is_negative_)
			rhs *= -1;
		// Reset my digits and append the digits one at a time, which is more efficient than prepending.
		digits_.clear();
		while (rhs > 0) {
			digits_.push_back(rhs % 10);
			rhs /= 10;
		}
	}
	// Now flip the order of digits to be correct.
	std::reverse(digits_.begin(), digits_.end());
	return *this;
}


BigInt& BigInt::operator=(uint64_t rhs) {
	is_negative_ = false;
	// Reset my digits and append the digits one at a time, which is more efficient than prepending.
	digits_.clear();
	while (rhs > 0) {
		digits_.push_back(rhs % 10);
		rhs /= 10;
	}
	// Now flip the order of digits to be correct.
	std::reverse(digits_.begin(), digits_.end());
	return *this;
}


BigInt& BigInt::operator=(double rhs) {
	is_negative_ = rhs < 0;
	if (is_negative_)
		rhs *= -1;
	// Reset my digits and append the digits one at a time, which is more efficient than prepending.
	digits_.clear();
	while (rhs > 0) {
		digits_.push_back(static_cast<uint8_t>(fmod(rhs, 10)));
		rhs /= 10.0;
	}
	// Now flip the order of digits to be correct.
	std::reverse(digits_.begin(), digits_.end());
	return *this;
}


BigInt& BigInt::operator=(const BigInt& rhs) {
	digits_ = rhs.digits_;
	is_negative_ = rhs.is_negative_;
	return *this;
}


BigInt::Digits BigInt::get_digits() const {
	return digits_;
}


void BigInt::set_digits(const Digits &digits, bool is_negative) {
	for (const auto &dig: digits) {
		if (dig < 0)
			throw "digits must be non-negative";
		if (dig > 9)
			throw "digits must be in [0, 9]";
	}
	is_negative_ = is_negative;
	digits_ = digits;
	if (digits_.empty())
		is_negative_ = false;
}


std::string BigInt::to_string() const {
	if (digits_.empty())
		return { "0" };

	std::string ret;
	if (is_negative_) {
		ret = "-";
	}

	for (const auto& digit : digits_) {
		ret += digit + '0';
	}
	return ret;
}


int64_t BigInt::to_int() const {
	if (digits_.empty())
		return 0;

	if (is_negative_) {
		int64_t ret{ -digits_.front() };
		for (size_t i = 1; i < digits_.size(); ++i) {
			ret *= 10;
			ret -= digits_[i];
		}
		return ret;
	}
	else {
		int64_t ret{ digits_.front() };
		for (size_t i = 1; i < digits_.size(); ++i) {
			ret *= 10;
			ret += digits_[i];
		}
		return ret;
	}
}


BigInt BigInt::sqrt() const {
	// Test for negative self.
	if (is_negative_)
		throw "Can't take square root of a negative number.";

	// Test for self equals 0, which causes divide by 0 errors.
	if (digits_.empty())
		return *this;

	// If a double can express my value exactly, then use built in sqrt().
	if (digits_.size() < 15) {
		double my_value{ 0 };
		for (const auto& dig : digits_) {
			my_value *= 10.0;
			my_value += dig;
		}
		return BigInt{ std::sqrt(my_value) };
	}

	// Use Newton's method for the square root.
	//    f(x) = x^2 - this = 0
	//    x_n+1 = x_n - f(x_n)/f'(x_n)
	//    x_n+1 = x_n - (x_n^2 - this) / (2 * x_n)
	//    x_n+1 = x_n - x_n / 2 + this / (2 * x_n)
	//    x_n+1 = (x_n + this / x_n) / 2
	BigInt x_n{ *this };
	for (size_t count = 0; count < 1000; ++count) {
		// We want x_n+1 = (x_n + this / x_n) / 2
		// Can do this with a sequence of in-place operations:
		//    x_n+1 = this
		//    x_n+1 /= x_n
		//    x_n+1 += x_n
		//    x_n+1 /= 2
		BigInt x_np1{ *this };
		x_np1 /= x_n;
		x_np1 += x_n;
		x_np1 /= 2;

//        std::cout << i << "\t" << x_np1 << std::endl;

		if (x_np1 == x_n)
			break;

		x_n = x_np1;
	}

	return x_n;
}


#if 1
bool BigInt::is_perfect_square() const {
//	std::cout << "is_perfect_square(" << *this << ")" << std::endl;
	BigInt root { sqrt() };
	BigInt tmp{ root };
	tmp *= root;
	return tmp == *this;
}

#else

bool BigInt::is_perfect_square() const {
//	std::cout << "is_perfect_square(" << *this << ")" << std::endl;
	// Test for negative self, which is never a perfect square.
	if (is_negative_)
		return false;

	// Test for self equals 0, which is 0^2.
	if (digits_.empty())
		return true;

	// If a double can express my value exactly, then use built in sqrt().
	if (digits_.size() < 15) {
		double my_value{ 0 };
		for (const auto& dig : digits_) {
			my_value *= 10.0;
			my_value += dig;
		}
		uint64_t root = std::sqrt(my_value);
		return root * root == my_value;
	}


	BigInt ten_me{ *this };
	size_t num_digits = digits_.size();
	for (size_t i = 0; i < num_digits; ++i) {
		ten_me.digits_.push_back(0);
		ten_me.digits_.push_back(0);
	}

	// Use Newton's method for the square root.
	//    f(x) = x^2 - this = 0
	//    x_n+1 = x_n - f(x_n)/f'(x_n)
	//    x_n+1 = x_n - (x_n^2 - this) / (2 * x_n)
	//    x_n+1 = x_n - x_n / 2 + this / (2 * x_n)
	//    x_n+1 = (x_n + this / x_n) / 2
	BigInt x_n{ ten_me };
	int i{ 0 };
	while (true) {
		// We want x_n+1 = (x_n + this / x_n) / 2
		// Can do this with a sequence of in-place operations:
		//    x_n+1 = this
		//    x_n+1 /= x_n
		//    x_n+1 += x_n
		//    x_n+1 /= 2
		BigInt x_np1{ ten_me };
		x_np1 /= x_n;
		x_np1 += x_n;
		x_np1 /= 2;

//        std::cout << "\t" << i++ << "\t" << x_np1 << std::endl;

		if (x_np1 == x_n)
			break;

		x_n = x_np1;
	}

	auto iter = x_n.digits_.crbegin();
	for (size_t i = 0; i < num_digits; ++i, ++iter) {
		if (*iter != 0)
			return false;
	}
	return true;
}
#endif

bool BigInt::operator==(int8_t rhs) const {
	BigInt tmp{ rhs };
	return operator==(tmp);
}


bool BigInt::operator==(int16_t rhs) const {
	BigInt tmp{ rhs };
	return operator==(tmp);
}


bool BigInt::operator==(int32_t rhs) const {
	BigInt tmp{ rhs };
	return operator==(tmp);
}


bool BigInt::operator==(int64_t rhs) const {
	BigInt tmp{ rhs };
	return operator==(tmp);
}


bool BigInt::operator==(uint8_t rhs) const {
	BigInt tmp{ rhs };
	return operator==(tmp);
}


bool BigInt::operator==(uint16_t rhs) const {
	BigInt tmp{ rhs };
	return operator==(tmp);
}


bool BigInt::operator==(uint32_t rhs) const {
	BigInt tmp{ rhs };
	return operator==(tmp);
}


bool BigInt::operator==(uint64_t rhs) const {
	BigInt tmp{ rhs };
	return operator==(tmp);
}


bool BigInt::operator==(double rhs) const {
	BigInt tmp{ rhs };
	return operator==(tmp);
}


bool BigInt::operator==(const BigInt& rhs) const {
	return (rhs.is_negative_ == is_negative_) && (rhs.digits_ == digits_);
}


bool BigInt::operator!=(int64_t rhs) const {
	return !operator==(rhs);
}


bool BigInt::operator!=(uint64_t rhs) const {
	return !operator==(rhs);
}


bool BigInt::operator!=(double rhs) const {
	return !operator==(rhs);
}


bool BigInt::operator!=(const BigInt& rhs) const {
	return !operator==(rhs);
}


bool BigInt::operator<(int64_t rhs) const {
	BigInt tmp{ rhs };
	return operator<(tmp);
}


bool BigInt::operator<(uint64_t rhs) const {
	BigInt tmp{ rhs };
	return operator<(tmp);
}


bool BigInt::operator<(double rhs) const {
	BigInt tmp{ rhs };
	return operator<(tmp);
}


bool BigInt::operator<(const BigInt& rhs) const {
	// If signs differ, then no need to compare digits
	if (is_negative_ && !rhs.is_negative_)
		return true;
	if (!is_negative_ && rhs.is_negative_)
		return false;

	if (is_negative_ && rhs.is_negative_) {
		// Both me and rhs are negative, need to flip comparison.

		// If different number of digits, then simple returns.
		if (digits_.size() != rhs.digits_.size())
			return digits_.size() > rhs.digits_.size();

		// Same number of digits.

		auto rhs_iter = rhs.digits_.cbegin();
		for (auto iter = digits_.cbegin(); iter != digits_.cend(); ++iter, ++rhs_iter) {
			if (*iter != *rhs_iter)
				return *iter > *rhs_iter;
		}
	}
	else {
		// Both me and rhs are positive.

		// If different number of digits, then simple return.
		if (digits_.size() != rhs.digits_.size())
			return digits_.size() < rhs.digits_.size();

		// Same number of digits.
		// Check for 0 < 0.
		if (digits_.empty())
			return false;
		
		auto rhs_iter = rhs.digits_.cbegin();
		for (auto iter = digits_.cbegin(); iter != digits_.cend(); ++iter, ++rhs_iter) {
			if (*iter != *rhs_iter)
				return *iter < *rhs_iter;
		}
	}
	// If we get here, then equal, which is not less than.
	return false;
}


bool BigInt::operator<=(int64_t rhs) const {
	BigInt tmp{ rhs };
	return operator<=(tmp);
}


bool BigInt::operator<=(uint64_t rhs) const {
	BigInt tmp{ rhs };
	return operator<=(tmp);
}


bool BigInt::operator<=(double rhs) const {
	BigInt tmp{ rhs };
	return operator<=(tmp);
}


bool BigInt::operator<=(const BigInt& rhs) const {
	return !operator>(rhs);
}


bool BigInt::operator>(int64_t rhs) const {
	BigInt tmp{ rhs };
	return operator>(tmp);
}


bool BigInt::operator>(uint64_t rhs) const {
	BigInt tmp{ rhs };
	return operator>(tmp);
}


bool BigInt::operator>(double rhs) const {
	BigInt tmp{ rhs };
	return operator>(tmp);
}


bool BigInt::operator>(const BigInt& rhs) const {
	// If signs differ, then no need to compare digits
	if (is_negative_ && !rhs.is_negative_)
		return false;
	if (!is_negative_ && rhs.is_negative_)
		return true;

	if (is_negative_ && rhs.is_negative_) {
		// Both me and rhs are negative, need to flip comparison.

		// If different number of digits, then simple returns.
		if (digits_.size() != rhs.digits_.size())
			return digits_.size() < rhs.digits_.size();

		// Same number of digits.

		auto rhs_iter = rhs.digits_.cbegin();
		for (auto iter = digits_.cbegin(); iter != digits_.cend(); ++iter, ++rhs_iter) {
			if (*iter != *rhs_iter)
				return *iter < *rhs_iter;
		}
	}
	else {
		// Both me and rhs are positive.

		// If different number of digits, then simple return.
		if (digits_.size() != rhs.digits_.size())
			return digits_.size() > rhs.digits_.size();

		// Same number of digits.
		// Check for 0 < 0.
		if (digits_.empty())
			return false;

		auto rhs_iter = rhs.digits_.cbegin();
		for (auto iter = digits_.cbegin(); iter != digits_.cend(); ++iter, ++rhs_iter) {
			if (*iter != *rhs_iter)
				return *iter > *rhs_iter;
		}
	}
	// If we get here, then equal, which is not less than.
	return false;
}


bool BigInt::operator>=(int64_t rhs) const {
	BigInt tmp{ rhs };
	return operator>=(tmp);
}


bool BigInt::operator>=(uint64_t rhs) const {
	BigInt tmp{ rhs };
	return operator>=(tmp);
}


bool BigInt::operator>=(double rhs) const {
	BigInt tmp{ rhs };
	return operator<=(tmp);
}


bool BigInt::operator>=(const BigInt& rhs) const {
	return !operator<(rhs);
}


BigInt& BigInt::operator++() {
	// Special case to go from 0 to 1.
	if (digits_.empty()) {
		digits_.push_back(1);
		is_negative_ = false;
		return *this;
	}

	if (is_negative_) {
		is_negative_ = false;
		operator--();
		is_negative_ = !digits_.empty();
	}
	else {
		digits_.back()++;

		int carry{ 0 };
		for (auto iter = digits_.rbegin(); iter != digits_.rend(); ++iter) {
			if (*iter > 9) {
				carry = *iter / 10;
				*iter %= 10;
				if (iter + 1 != digits_.rend()) {
					*(iter + 1) += carry;
					carry = 0;
				}
			}
		}
		if (carry > 0) {
			digits_.insert(digits_.begin(), carry);
		}
	}
	return *this;
}


BigInt BigInt::operator++(int) {
	BigInt old{ *this };
	operator++();
	return old;
}


BigInt& BigInt::operator--() {
	// Special case to go from 0 to -1.
	if (digits_.empty()) {
		digits_.push_back(1);
		is_negative_ = true;
		return *this;
	}

	if (is_negative_) {
		is_negative_ = false;
		operator++();
		is_negative_ = !digits_.empty();
	}
	else {
		digits_.back()--;

		int carry{ 0 };
		for (auto iter = digits_.rbegin(); iter != digits_.rend(); ++iter) {
			if (*iter < 0) {
				carry = -1;
				*iter += 10;
				if (iter + 1 != digits_.rend()) {
					*(iter + 1) += carry;
					carry = 0;
				}
			}
		}
		fix_digits();
	}
	return *this;
}


BigInt BigInt::operator--(int) {
	BigInt old{ *this };
	operator--();
	return old;
}


BigInt& BigInt::operator+=(int8_t rhs) {
	BigInt tmp{ rhs };
	return operator+=(tmp);
}


BigInt& BigInt::operator+=(int16_t rhs) {
	BigInt tmp{ rhs };
	return operator+=(tmp);
}


BigInt& BigInt::operator+=(int32_t rhs) {
	BigInt tmp{ rhs };
	return operator+=(tmp);
}


BigInt& BigInt::operator+=(int64_t rhs) {
	BigInt tmp{ rhs };
	return operator+=(tmp);
}


BigInt& BigInt::operator+=(uint8_t rhs) {
	BigInt tmp{ rhs };
	return operator+=(tmp);
}


BigInt& BigInt::operator+=(uint16_t rhs) {
	BigInt tmp{ rhs };
	return operator+=(tmp);
}


BigInt& BigInt::operator+=(uint32_t rhs) {
	BigInt tmp{ rhs };
	return operator+=(tmp);
}


BigInt& BigInt::operator+=(uint64_t rhs) {
	BigInt tmp{ rhs };
	return operator+=(tmp);
}


BigInt& BigInt::operator+=(const BigInt& rhs) {
	if (digits_.empty()) {
		is_negative_ = rhs.is_negative_;
		digits_ = rhs.digits_;
		return *this;
	}

	if (is_negative_ == rhs.is_negative_) {
		// Iterate over common digits and add them in-place.
		auto rhs_iter = rhs.digits_.rbegin();
		for (auto iter = digits_.rbegin(); iter != digits_.rend() && rhs_iter != rhs.digits_.rend(); ++iter, ++rhs_iter) {
			*iter += *rhs_iter;
		}
		// Prepend any extra digits from rhs.
		if (rhs_iter != rhs.digits_.rend()) {
			std::reverse(digits_.begin(), digits_.end());
			digits_.insert(digits_.end(), rhs_iter, rhs.digits_.rend());
			std::reverse(digits_.begin(), digits_.end());
		}
		// Iterate over digits right to left to handle any carries in addition.
		int carry{ 0 };
		for (auto iter = digits_.rbegin(); iter != digits_.rend(); ++iter) {
			if (*iter > 9) {
				carry = *iter / 10;
				*iter %= 10;
				if (iter + 1 != digits_.rend()) {
					*(iter + 1) += carry;
					carry = 0;
				}
			}
		}
		if (carry > 0) {
			digits_.insert(digits_.begin(), carry);
		}
	}
	else {
		// Opposite signs, check if same magnitude to sum to 0.
		is_negative_ = !is_negative_;
		if (rhs == *this) {
			is_negative_ = false;
			digits_.clear();
			return *this;
		}
		is_negative_ = !is_negative_;
		// Figure out which is smaller magnitude, to subtract from larger.
		bool rhs_smaller{ false };
		if (is_negative_) {
			is_negative_ = false;
			rhs_smaller = rhs < *this;
			is_negative_ = true;
		}
		else {
			rhs.is_negative_ = false;
			rhs_smaller = rhs < *this;
			rhs.is_negative_ = true;
		}
		if (rhs_smaller) {
			is_negative_ = !is_negative_;
			operator-=(rhs);
			is_negative_ = !is_negative_;
		}
		else {
			is_negative_ = !is_negative_;
			BigInt tmp = rhs - *this;
			is_negative_ = tmp.is_negative_;
			digits_ = tmp.digits_;
		}
	}
	return *this;
}


BigInt& BigInt::operator-=(int8_t rhs) {
	BigInt tmp{ rhs };
	return operator-=(tmp);
}


BigInt& BigInt::operator-=(int16_t rhs) {
	BigInt tmp{ rhs };
	return operator-=(tmp);
}


BigInt& BigInt::operator-=(int32_t rhs) {
	BigInt tmp{ rhs };
	return operator-=(tmp);
}


BigInt& BigInt::operator-=(int64_t rhs) {
	BigInt tmp{ rhs };
	return operator-=(tmp);
}


BigInt& BigInt::operator-=(uint8_t rhs) {
	BigInt tmp{ rhs };
	return operator-=(tmp);
}


BigInt& BigInt::operator-=(uint16_t rhs) {
	BigInt tmp{ rhs };
	return operator-=(tmp);
}


BigInt& BigInt::operator-=(uint32_t rhs) {
	BigInt tmp{ rhs };
	return operator-=(tmp);
}


BigInt& BigInt::operator-=(uint64_t rhs) {
	BigInt tmp{ rhs };
	return operator-=(tmp);
}


BigInt& BigInt::operator-=(const BigInt& rhs) {
	if (digits_.empty()) {
		is_negative_ = !rhs.is_negative_;
		digits_ = rhs.digits_;
		return *this;
	}

	if (is_negative_ == rhs.is_negative_) {
		// Same signs, check if same magnitude to sum to 0.
		if (rhs == *this) {
			is_negative_ = false;
			digits_.clear();
			return *this;
		}
		// Figure out which is smaller magnitude, to subtract from larger.
		bool rhs_smaller = is_negative_ ? (rhs > *this) : (rhs < *this);

		if (rhs_smaller) {
			// Iterate over common digits and subtract them in-place.
			auto rhs_iter = rhs.digits_.rbegin();
			for (auto iter = digits_.rbegin(); iter != digits_.rend() && rhs_iter != rhs.digits_.rend(); ++iter, ++rhs_iter) {
				*iter -= *rhs_iter;
			}
			// Iterate over digits right to left to handle any carries in addition.
			int carry{ 0 };
			for (auto iter = digits_.rbegin(); iter != digits_.rend(); ++iter) {
				if (*iter < 0) {
					carry = -1;
					*iter += 10;
					if (iter + 1 != digits_.rend()) {
						*(iter + 1) += carry;
					}
				}
			}
		}
		else {
			// rhs has a bigger magnitude than me, so my sign flips.
			is_negative_ = !is_negative_;
			// Iterate over common digits and subtract them in-place.
			auto rhs_iter = rhs.digits_.rbegin();
			for (auto iter = digits_.rbegin(); iter != digits_.rend() && rhs_iter != rhs.digits_.rend(); ++iter, ++rhs_iter) {
				*iter = *rhs_iter - *iter;
			}
			// Prepend any extra digits from rhs.
			if (rhs_iter != rhs.digits_.rend()) {
				std::reverse(digits_.begin(), digits_.end());
				digits_.insert(digits_.end(), rhs_iter, rhs.digits_.rend());
				std::reverse(digits_.begin(), digits_.end());
			}
			// Iterate over digits right to left to handle any carries in addition.
			int carry{ 0 };
			for (auto iter = digits_.rbegin(); iter != digits_.rend(); ++iter) {
				if (*iter < 0) {
					carry = -1;
					*iter += 10;
					if (iter + 1 != digits_.rend()) {
						*(iter + 1) += carry;
					}
				}
			}
		}
		fix_digits();
	}
	else {
		is_negative_ = !is_negative_;
		operator+=(rhs);
		is_negative_ = !is_negative_;
	}

	return *this;
}


BigInt& BigInt::operator*=(int8_t rhs) {
	BigInt tmp{ rhs };
	return operator*=(tmp);
}


BigInt& BigInt::operator*=(int16_t rhs) {
	BigInt tmp{ rhs };
	return operator*=(tmp);
}


BigInt& BigInt::operator*=(int32_t rhs) {
	BigInt tmp{ rhs };
	return operator*=(tmp);
}


BigInt& BigInt::operator*=(int64_t rhs) {
	BigInt tmp{ rhs };
	return operator*=(tmp);
}


BigInt& BigInt::operator*=(uint8_t rhs) {
	BigInt tmp{ rhs };
	return operator*=(tmp);
}


BigInt& BigInt::operator*=(uint16_t rhs) {
	BigInt tmp{ rhs };
	return operator*=(tmp);
}


BigInt& BigInt::operator*=(uint32_t rhs) {
	BigInt tmp{ rhs };
	return operator*=(tmp);
}


BigInt& BigInt::operator*=(uint64_t rhs) {
	BigInt tmp{ rhs };
	return operator*=(tmp);
}


BigInt::Digits multiply_helper(const BigInt::Digits &term1, const int8_t term2, bool do_carry=false) {
	if (0 == term2) {
		// When multiplying by 0, zero out term1.
		return BigInt::Digits(term1.size(), 0);
	}
	else if (1 == term2) {
		// Nothing to do when multiplying by 1, leave term1 unchanged.
		return term1;
	}

	BigInt::Digits product;

	// Multiply digit by digit left to right, leaving 2 digit values in the output.
	for (const auto& digit : term1) {
		product.push_back(term2 * digit);
	}
	// Process each digit right to left, carrying the 10s digits in products.
	if (do_carry) {
		int8_t carry{ 0 };
		for (auto iter = product.rbegin(); iter != product.rend(); ++iter) {
			if (*iter > 9) {
				carry = *iter / 10;
				*iter %= 10;
				if (iter + 1 != product.rend()) {
					*(iter + 1) += carry;
					carry = 0;
				}
			}
		}
		if (carry > 0) {
			product.insert(product.begin(), carry);
		}
	}

	return product;
}

BigInt& BigInt::operator*=(const BigInt& rhs) {
	// Multiplication by 0 is 0.
	if (digits_.empty()) {
		return *this;
	}
	if (rhs.digits_.empty()) {
		is_negative_ = false;
		digits_.clear();
		return *this;
	}

	// Product is negative only if exactly one term is negative, so XOR
	is_negative_ ^= rhs.is_negative_;

	if (1 == rhs.digits_.size()) {
		digits_ = multiply_helper(digits_, rhs.digits_.front(), true);
		return *this;
	}

	// Cache my original digits to be used in multiplication
	Digits my_orig_digits = digits_;

	// Zero out my digits for the summation to follow.
	for (auto &dig : digits_) {
		dig = 0;
	}

	// Iterate over digits in rhs and multiply by my digits, summing as we go.
	for (auto rhs_iter = rhs.digits_.crbegin(); rhs_iter != rhs.digits_.crend(); ++rhs_iter) {
		Digits tmp = multiply_helper(my_orig_digits, *rhs_iter);
		// Add trailing 0s for the position of the rhs digit being multiplied
		for (int i = 0; i < rhs_iter - rhs.digits_.crbegin(); ++i)
			tmp.push_back(0);

		auto tmp_iter = tmp.crbegin();
		for (auto iter = digits_.rbegin(); iter != digits_.rend(); ++iter, ++tmp_iter) {
			*iter += *tmp_iter;
		}
		// Prepend first digit from rhs product.
		if (tmp_iter != tmp.crend()) {
			digits_.insert(digits_.begin(), *tmp_iter);
		}
		// Iterate over digits right to left to handle any carries in addition.
		int carry{ 0 };
		for (auto iter = digits_.rbegin(); iter != digits_.rend(); ++iter) {
			if (*iter > 9) {
				carry = *iter / 10;
				*iter %= 10;
				if (iter + 1 != digits_.rend()) {
					*(iter + 1) += carry;
					carry = 0;
				}
			}
		}
		if (carry > 0) {
			digits_.insert(digits_.begin(), carry);
		}
	}

	return *this;
}


BigInt& BigInt::operator/=(int8_t rhs) {
	BigInt tmp{ rhs };
	return operator/=(tmp);
}


BigInt& BigInt::operator/=(int16_t rhs) {
	BigInt tmp{ rhs };
	return operator/=(tmp);
}


BigInt& BigInt::operator/=(int32_t rhs) {
	BigInt tmp{ rhs };
	return operator/=(tmp);
}


BigInt& BigInt::operator/=(int64_t rhs) {
	BigInt tmp{ rhs };
	return operator/=(tmp);
}


BigInt& BigInt::operator/=(uint8_t rhs) {
	BigInt tmp{ rhs };
	return operator/=(tmp);
}


BigInt& BigInt::operator/=(uint16_t rhs) {
	BigInt tmp{ rhs };
	return operator/=(tmp);
}


BigInt& BigInt::operator/=(uint32_t rhs) {
	BigInt tmp{ rhs };
	return operator/=(tmp);
}


BigInt& BigInt::operator/=(uint64_t rhs) {
	BigInt tmp{ rhs };
	return operator/=(tmp);
}


BigInt& BigInt::operator/=(const BigInt &rhs) {
	// Multiplication by 0 is 0.
	if (rhs.digits_.empty()) {
		throw "Division by 0";
	}
	if (digits_.empty()) {
		return *this;
	}

	// If denominator greater than numerator, then set to 0 and return.
	{
		BigInt tmp_num, tmp_den;
		tmp_num.set_digits(digits_);
		tmp_den.set_digits(rhs.digits_);
		if (tmp_den > tmp_num) {
			digits_.clear();
			is_negative_ = false;
			return *this;
		}
	}

	// Product is negative only if exactly one term is negative, so XOR.
	is_negative_ ^= rhs.is_negative_;

	// If denominator is +-1, then return, we've already set the sign and the digits don't change.
	if ((rhs.digits_.size() == 1) && (rhs.digits_[0] == 1)) {
		return *this;
	}

	// If numerator equals denominator, then set to 1 and return.
	if (digits_ == rhs.digits_) {
		digits_.clear();
		digits_.push_back(1);
		return *this;
	}

	// Cache my original digits as a list for faster removal from front.
	std::list<int8_t> orig_num_digits{ digits_.begin(), digits_.end() };

	// Clear out my digits, so they can store the quotient.
	digits_.clear();

	BigInt curr_num;
	// Move digits from original numerator to current numerator, same number as denominator.
	for (size_t i = 0; i < rhs.digits_.size(); ++i) {
		curr_num.digits_.push_back(orig_num_digits.front());
		orig_num_digits.pop_front();
	}
	// If current numerator is less than denominator, move one more digit.
	if (curr_num < rhs) {
		curr_num.digits_.push_back(orig_num_digits.front());
		orig_num_digits.pop_front();
	}

	// Iterate until we run out of digits in the numerator
	BigInt tmp_den;
	tmp_den.set_digits(rhs.digits_);
	while (true) {
		// Get the next digit in the overall quotient.
		int8_t next_digit{ 0 };
		while (curr_num >= tmp_den) {
			curr_num -= tmp_den;
			++next_digit;
		}
		// Add next digit to my digits.
		digits_.push_back(next_digit);

		if (orig_num_digits.empty())
			break;

		// Move next numerator digit and repeat
		curr_num.digits_.push_back(orig_num_digits.front());
		curr_num.fix_digits();
		orig_num_digits.pop_front();
	}

	return *this;
}


void BigInt::fix_digits() {
	// Make sure we don't have any leading 0's.
	if (digits_.empty())
		return;

	auto iter = digits_.cbegin();
	while ((iter != digits_.cend()) && (0 == *iter))
		++iter;
	digits_.erase(digits_.begin(), iter);
	if (digits_.empty())
		is_negative_ = false;
}


BigInt operator+(BigInt lhs, int64_t rhs) {
	BigInt ret{ lhs };
	ret += rhs;
	return ret;
}


BigInt operator+(BigInt lhs, uint64_t rhs) {
	BigInt ret{ lhs };
	ret += rhs;
	return ret;
}


BigInt operator+(BigInt lhs, const BigInt& rhs) {
	BigInt ret{ lhs };
	ret += rhs;
	return ret;
}


BigInt operator-(BigInt lhs, int64_t rhs) {
	BigInt ret{ lhs };
	ret -= rhs;
	return ret;
}


BigInt operator-(BigInt lhs, uint64_t rhs) {
	BigInt ret{ lhs };
	ret -= rhs;
	return ret;
}


BigInt operator-(BigInt lhs, const BigInt& rhs) {
	BigInt ret{ lhs };
	ret -= rhs;
	return ret;
}


std::ostream& operator<<(std::ostream& out, const BigInt& value) {
	out << value.to_string();
	return out;
}


std::istream& operator>>(std::istream& in, BigInt& value) {
	value.digits_.clear();
	value.is_negative_ = false;

	char character{ '\0' };
	in >> character;
	if (character == '-') {
		value.is_negative_ = true;
	}
	else {
		in.putback(character);
	}

	while (true) {
		in >> character;
		if ((character >= '0') && (character <= '9')) {
			value.digits_.push_back(character - '0');
		}
		else
			break;
	}
	return in;
}
