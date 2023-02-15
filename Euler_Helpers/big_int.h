#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>


class BigInt {
public:
	using Digits = std::vector<int8_t>;

	BigInt();
	~BigInt() = default;
	explicit BigInt(int8_t value);
	explicit BigInt(int16_t value);
	explicit BigInt(int32_t value);
	explicit BigInt(int64_t value);
	explicit BigInt(uint8_t value);
	explicit BigInt(uint16_t value);
	explicit BigInt(uint32_t value);
	explicit BigInt(uint64_t value);
	explicit BigInt(double value);
	BigInt(const BigInt& other);

	// Assignment operators
	BigInt& operator=(int64_t rhs);
	BigInt& operator=(uint64_t rhs);
	BigInt& operator=(double rhs);
	BigInt& operator=(const BigInt& rhs);

	Digits get_digits() const;
	void set_digits(const Digits &digits, bool is_negative=false);

	// Conversion operations
	std::string to_string() const;
	int64_t to_int() const;

	BigInt sqrt() const;
	bool is_perfect_square() const;

	BigInt operator-() const;
	BigInt abs() const;

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
	bool operator==(const BigInt& rhs) const;

	bool operator!=(int64_t rhs) const;
	bool operator!=(uint64_t rhs) const;
	bool operator!=(double rhs) const;
	bool operator!=(const BigInt& rhs) const;

	bool operator<(int64_t rhs) const;
	bool operator<(uint64_t rhs) const;
	bool operator<(double rhs) const;
	bool operator<(const BigInt& rhs) const;

	bool operator<=(int64_t rhs) const;
	bool operator<=(uint64_t rhs) const;
	bool operator<=(double rhs) const;
	bool operator<=(const BigInt& rhs) const;

	bool operator>(int64_t rhs) const;
	bool operator>(uint64_t rhs) const;
	bool operator>(double rhs) const;
	bool operator>(const BigInt& rhs) const;

	bool operator>=(int64_t rhs) const;
	bool operator>=(uint64_t rhs) const;
	bool operator>=(double rhs) const;
	bool operator>=(const BigInt& rhs) const;

	// Increment and decrement operators
	BigInt& operator++();
	BigInt operator++(int);
	BigInt& operator--();
	BigInt operator--(int);

	// Addition operators
	BigInt& operator+=(int8_t rhs);
	BigInt& operator+=(int16_t rhs);
	BigInt& operator+=(int32_t rhs);
	BigInt& operator+=(int64_t rhs);
	BigInt& operator+=(uint8_t rhs);
	BigInt& operator+=(uint16_t rhs);
	BigInt& operator+=(uint32_t rhs);
	BigInt& operator+=(uint64_t rhs);
	BigInt& operator+=(const BigInt &rhs);

	// Subtraction operators
	BigInt& operator-=(int8_t rhs);
	BigInt& operator-=(int16_t rhs);
	BigInt& operator-=(int32_t rhs);
	BigInt& operator-=(int64_t rhs);
	BigInt& operator-=(uint8_t rhs);
	BigInt& operator-=(uint16_t rhs);
	BigInt& operator-=(uint32_t rhs);
	BigInt& operator-=(uint64_t rhs);
	BigInt& operator-=(const BigInt& rhs);

	// Multiplication operators
	BigInt& operator*=(int8_t rhs);
	BigInt& operator*=(int16_t rhs);
	BigInt& operator*=(int32_t rhs);
	BigInt& operator*=(int64_t rhs);
	BigInt& operator*=(uint8_t rhs);
	BigInt& operator*=(uint16_t rhs);
	BigInt& operator*=(uint32_t rhs);
	BigInt& operator*=(uint64_t rhs);
	BigInt& operator*=(const BigInt& rhs);

	// Division operators
	BigInt& operator/=(int8_t rhs);
	BigInt& operator/=(int16_t rhs);
	BigInt& operator/=(int32_t rhs);
	BigInt& operator/=(int64_t rhs);
	BigInt& operator/=(uint8_t rhs);
	BigInt& operator/=(uint16_t rhs);
	BigInt& operator/=(uint32_t rhs);
	BigInt& operator/=(uint64_t rhs);
	BigInt& operator/=(const BigInt & rhs);

	// Friend non-methods
	friend BigInt operator+(BigInt lhs, int64_t rhs);
	friend BigInt operator+(BigInt lhs, uint64_t rhs);
	friend BigInt operator+(BigInt lhs, const BigInt& rhs);

	friend BigInt operator-(BigInt lhs, int64_t rhs);
	friend BigInt operator-(BigInt lhs, uint64_t rhs);
	friend BigInt operator-(BigInt lhs, const BigInt& rhs);

	friend BigInt operator*(BigInt lhs, int64_t rhs);
	friend BigInt operator*(BigInt lhs, uint64_t rhs);
	friend BigInt operator*(BigInt lhs, const BigInt& rhs);

	friend BigInt operator/(BigInt lhs, int64_t rhs);
	friend BigInt operator/(BigInt lhs, uint64_t rhs);
	friend BigInt operator/(BigInt lhs, const BigInt& rhs);

	friend std::ostream& operator<<(std::ostream& out, const BigInt& value);
	friend std::istream& operator>>(std::istream& in, BigInt& value);

private:
	Digits digits_;
	mutable bool is_negative_ = false;  // We need to flip signs in some const operators

	void fix_digits();
};

