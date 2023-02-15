#pragma once


#include <cstdint>
#include <utility>


int64_t gcd(int64_t a, int64_t b);


class Fraction {
public:
	Fraction() = default;
	Fraction(int64_t value);
	Fraction(int64_t num, int64_t den);
	~Fraction() = default;

	std::pair<int64_t, int64_t> get_components() const;

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

private :
	int64_t num_ = 0;
	int64_t den_ = 1;
	bool is_negative_ = false;

	void canonicalize();
};