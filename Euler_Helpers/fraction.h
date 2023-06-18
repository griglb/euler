#pragma once


#include <cstdint>
#include <iostream>
#include <utility>

#include "big_int.h"


int64_t gcd(int64_t a, int64_t b);
BigInt gcd(const BigInt& a, const BigInt& b);

int64_t lcm(int64_t a, int64_t b);


class Fraction {
public:
	Fraction() = default;
	Fraction(int64_t value);
	Fraction(int64_t num, int64_t den);
	Fraction(const BigInt &num, const BigInt& den);
	~Fraction() = default;

	std::pair<BigInt, BigInt> get_components() const;

	Fraction abs() const;

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
	bool operator==(const Fraction& rhs) const;

	bool operator!=(int64_t rhs) const;
	bool operator!=(uint64_t rhs) const;
	bool operator!=(double rhs) const;
	bool operator!=(const Fraction& rhs) const;

	bool operator<(int64_t rhs) const;
	bool operator<(uint64_t rhs) const;
	bool operator<(double rhs) const;
	bool operator<(const Fraction& rhs) const;

	bool operator<=(int64_t rhs) const;
	bool operator<=(uint64_t rhs) const;
	bool operator<=(double rhs) const;
	bool operator<=(const Fraction& rhs) const;

	bool operator>(int64_t rhs) const;
	bool operator>(uint64_t rhs) const;
	bool operator>(double rhs) const;
	bool operator>(const Fraction& rhs) const;

	bool operator>=(int64_t rhs) const;
	bool operator>=(uint64_t rhs) const;
	bool operator>=(double rhs) const;
	bool operator>=(const Fraction& rhs) const;

	// Addition operators
	Fraction& operator+=(int8_t rhs);
	Fraction& operator+=(int16_t rhs);
	Fraction& operator+=(int32_t rhs);
	Fraction& operator+=(int64_t rhs);
	Fraction& operator+=(uint8_t rhs);
	Fraction& operator+=(uint16_t rhs);
	Fraction& operator+=(uint32_t rhs);
	Fraction& operator+=(uint64_t rhs);
	Fraction& operator+=(const Fraction& rhs);

	// Subtraction operators
	Fraction& operator-=(int8_t rhs);
	Fraction& operator-=(int16_t rhs);
	Fraction& operator-=(int32_t rhs);
	Fraction& operator-=(int64_t rhs);
	Fraction& operator-=(uint8_t rhs);
	Fraction& operator-=(uint16_t rhs);
	Fraction& operator-=(uint32_t rhs);
	Fraction& operator-=(uint64_t rhs);
	Fraction& operator-=(const Fraction& rhs);

	// Multiplication operators
	Fraction& operator*=(int8_t rhs);
	Fraction& operator*=(int16_t rhs);
	Fraction& operator*=(int32_t rhs);
	Fraction& operator*=(int64_t rhs);
	Fraction& operator*=(uint8_t rhs);
	Fraction& operator*=(uint16_t rhs);
	Fraction& operator*=(uint32_t rhs);
	Fraction& operator*=(uint64_t rhs);
	Fraction& operator*=(const Fraction& rhs);

	// Division operators
	Fraction& operator/=(int8_t rhs);
	Fraction& operator/=(int16_t rhs);
	Fraction& operator/=(int32_t rhs);
	Fraction& operator/=(int64_t rhs);
	Fraction& operator/=(uint8_t rhs);
	Fraction& operator/=(uint16_t rhs);
	Fraction& operator/=(uint32_t rhs);
	Fraction& operator/=(uint64_t rhs);
	Fraction& operator/=(const Fraction& rhs);

	// Friend non-methods
	friend Fraction operator+(Fraction lhs, int64_t rhs);
	friend Fraction operator+(Fraction lhs, uint64_t rhs);
	friend Fraction operator+(Fraction lhs, const Fraction& rhs);

	friend Fraction operator-(Fraction lhs, int64_t rhs);
	friend Fraction operator-(Fraction lhs, uint64_t rhs);
	friend Fraction operator-(Fraction lhs, const Fraction& rhs);

	friend Fraction operator*(Fraction lhs, int64_t rhs);
	friend Fraction operator*(Fraction lhs, uint64_t rhs);
	friend Fraction operator*(Fraction lhs, const Fraction& rhs);

	friend Fraction operator/(Fraction lhs, int64_t rhs);
	friend Fraction operator/(Fraction lhs, uint64_t rhs);
	friend Fraction operator/(Fraction lhs, const Fraction& rhs);

	friend std::ostream& operator<<(std::ostream& out, const Fraction& value);
	friend std::istream& operator>>(std::istream& in, Fraction& value);

private :
	BigInt num_ = BigInt(0);
	BigInt den_ = BigInt(1);
	bool is_negative_ = false;

	void canonicalize();
};