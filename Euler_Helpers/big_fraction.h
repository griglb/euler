#pragma once


#include <cstdint>
#include <iostream>
#include <utility>

#include "big_int.h"


class BigFraction {
public:
	using MyType = BigInt;

	BigFraction() = default;
	BigFraction(int64_t value);
	BigFraction(int64_t num, int64_t den);
	BigFraction(const BigInt &num, const BigInt& den);
	~BigFraction() = default;

	std::pair<MyType, MyType> get_components() const;

	BigFraction abs() const;
	BigFraction inverse() const;

	// Comparison operators
	bool operator==(int8_t rhs) const;
	bool operator==(int16_t rhs) const;
	bool operator==(int32_t rhs) const;
	bool operator==(int64_t rhs) const;
	bool operator==(uint8_t rhs) const;
	bool operator==(uint16_t rhs) const;
	bool operator==(uint32_t rhs) const;
	bool operator==(uint64_t rhs) const;
	bool operator==(double rhs) const;
	bool operator==(const BigFraction& rhs) const;

	bool operator!=(int64_t rhs) const;
	bool operator!=(uint64_t rhs) const;
	bool operator!=(double rhs) const;
	bool operator!=(const BigFraction& rhs) const;

	bool operator<(int64_t rhs) const;
	bool operator<(uint64_t rhs) const;
	bool operator<(double rhs) const;
	bool operator<(const BigFraction& rhs) const;

	bool operator<=(int64_t rhs) const;
	bool operator<=(uint64_t rhs) const;
	bool operator<=(double rhs) const;
	bool operator<=(const BigFraction& rhs) const;

	bool operator>(int64_t rhs) const;
	bool operator>(uint64_t rhs) const;
	bool operator>(double rhs) const;
	bool operator>(const BigFraction& rhs) const;

	bool operator>=(int64_t rhs) const;
	bool operator>=(uint64_t rhs) const;
	bool operator>=(double rhs) const;
	bool operator>=(const BigFraction& rhs) const;

	// Addition operators
	BigFraction& operator+=(int8_t rhs);
	BigFraction& operator+=(int16_t rhs);
	BigFraction& operator+=(int32_t rhs);
	BigFraction& operator+=(int64_t rhs);
	BigFraction& operator+=(uint8_t rhs);
	BigFraction& operator+=(uint16_t rhs);
	BigFraction& operator+=(uint32_t rhs);
	BigFraction& operator+=(uint64_t rhs);
	BigFraction& operator+=(const BigFraction& rhs);

	// Subtraction operators
	BigFraction& operator-=(int8_t rhs);
	BigFraction& operator-=(int16_t rhs);
	BigFraction& operator-=(int32_t rhs);
	BigFraction& operator-=(int64_t rhs);
	BigFraction& operator-=(uint8_t rhs);
	BigFraction& operator-=(uint16_t rhs);
	BigFraction& operator-=(uint32_t rhs);
	BigFraction& operator-=(uint64_t rhs);
	BigFraction& operator-=(const BigFraction& rhs);

	// Multiplication operators
	BigFraction& operator*=(int8_t rhs);
	BigFraction& operator*=(int16_t rhs);
	BigFraction& operator*=(int32_t rhs);
	BigFraction& operator*=(int64_t rhs);
	BigFraction& operator*=(uint8_t rhs);
	BigFraction& operator*=(uint16_t rhs);
	BigFraction& operator*=(uint32_t rhs);
	BigFraction& operator*=(uint64_t rhs);
	BigFraction& operator*=(const BigFraction& rhs);

	// Division operators
	BigFraction& operator/=(int8_t rhs);
	BigFraction& operator/=(int16_t rhs);
	BigFraction& operator/=(int32_t rhs);
	BigFraction& operator/=(int64_t rhs);
	BigFraction& operator/=(uint8_t rhs);
	BigFraction& operator/=(uint16_t rhs);
	BigFraction& operator/=(uint32_t rhs);
	BigFraction& operator/=(uint64_t rhs);
	BigFraction& operator/=(const BigFraction& rhs);

	// Friend non-methods
	friend BigFraction operator+(BigFraction lhs, int64_t rhs);
	friend BigFraction operator+(BigFraction lhs, uint64_t rhs);
	friend BigFraction operator+(BigFraction lhs, const BigFraction& rhs);

	friend BigFraction operator-(BigFraction lhs, int64_t rhs);
	friend BigFraction operator-(BigFraction lhs, uint64_t rhs);
	friend BigFraction operator-(BigFraction lhs, const BigFraction& rhs);

	friend BigFraction operator*(BigFraction lhs, int64_t rhs);
	friend BigFraction operator*(BigFraction lhs, uint64_t rhs);
	friend BigFraction operator*(BigFraction lhs, const BigFraction& rhs);

	friend BigFraction operator/(BigFraction lhs, int64_t rhs);
	friend BigFraction operator/(BigFraction lhs, uint64_t rhs);
	friend BigFraction operator/(BigFraction lhs, const BigFraction& rhs);

	friend std::ostream& operator<<(std::ostream& out, const BigFraction& value);
	friend std::istream& operator>>(std::istream& in, BigFraction& value);

private :
	BigInt num_ = BigInt(0);
	BigInt den_ = BigInt(1);
	bool is_negative_ = false;

	void canonicalize();
};