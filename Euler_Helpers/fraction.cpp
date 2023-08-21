#include "fraction.h"

#include <cmath>
#include <utility>

#include "gcd.h"


Fraction::Fraction(int64_t value) : num_{ value }, den_{ 1 }, is_negative_{ value < 0 } {
    if (num_ < 0.0)
        num_ = -num_;
}


Fraction::Fraction(int64_t num, int64_t den) : num_{ num }, den_{ den }, is_negative_{ false } {
    is_negative_ = (num_ < 0.0) ^ (den_ < 0.0);
    if (num_ < 0.0)
        num_ = -num_;
    if (den_ < 0.0)
        den_ = -den_;
    canonicalize();
}


std::pair<int64_t, int64_t> Fraction::get_components() const {
    return { is_negative_ ? -num_ : num_, den_ };
}


Fraction Fraction::abs() const {
    return Fraction{ num_, den_ };
}


Fraction Fraction::inverse() const {
    return Fraction{ den_, num_ };
}


// Comparison operators
//bool operator==(int8_t rhs) const;
//bool operator==(int16_t rhs) const;
//bool operator==(int32_t rhs) const;
//bool operator==(int64_t rhs) const;
//bool operator==(uint8_t rhs) const;
//bool operator==(uint16_t rhs) const;
//bool operator==(uint32_t rhs) const;
//bool operator==(uint64_t rhs) const;
//bool operator==(double rhs) const;
bool Fraction::operator==(const Fraction& rhs) const {
    return (is_negative_ == rhs.is_negative_) && (num_ == rhs.num_) && (den_ == rhs.den_);
}


//bool operator!=(int64_t rhs) const;
//bool operator!=(uint64_t rhs) const;
//bool operator!=(double rhs) const;
bool Fraction::operator!=(const Fraction& rhs) const {
    return !operator==(rhs);
}


//bool operator<(int64_t rhs) const;
//bool operator<(uint64_t rhs) const;
//bool operator<(double rhs) const;
bool Fraction::operator<(const Fraction& rhs) const {
    if (is_negative_ != rhs.is_negative_) {
        return is_negative_;
    }

    auto left_prod = num_ * rhs.den_;
    auto right_prod = den_ * rhs.num_;

    return is_negative_ ? (left_prod > right_prod) : (left_prod < right_prod);
}


//bool operator<=(int64_t rhs) const;
//bool operator<=(uint64_t rhs) const;
//bool operator<=(double rhs) const;
//bool operator<=(const Fraction& rhs) const;

//bool operator>(int64_t rhs) const;
//bool operator>(uint64_t rhs) const;
//bool operator>(double rhs) const;
bool Fraction::operator>(const Fraction& rhs) const {
    if (is_negative_ != rhs.is_negative_) {
        return rhs.is_negative_;
    }

    auto left_prod = num_ * rhs.den_;
    auto right_prod = den_ * rhs.num_;

    return is_negative_ ? (left_prod < right_prod) : (left_prod > right_prod);
}



//bool operator>=(int64_t rhs) const;
//bool operator>=(uint64_t rhs) const;
//bool operator>=(double rhs) const;
//bool operator>=(const Fraction& rhs) const;


// Addition operators
//Fraction& operator+=(int8_t rhs);
//Fraction& operator+=(int16_t rhs);
//Fraction& operator+=(int32_t rhs);
//Fraction& operator+=(int64_t rhs);
//Fraction& operator+=(uint8_t rhs);
//Fraction& operator+=(uint16_t rhs);
//Fraction& operator+=(uint32_t rhs);
//Fraction& operator+=(uint64_t rhs);
Fraction& Fraction::operator+=(const Fraction& rhs) {
    // Easy out for adding 0.
    if (MyType{ 0 } == rhs.num_)
        return *this;

    if (is_negative_ == rhs.is_negative_) {
        // Same sign, so add the fractions.
        num_ = num_*rhs.den_ + rhs.num_*den_;
        den_ *= rhs.den_;
    }
    else {
        // Opposite signs, need to handle a potential sign change.
        MyType my_num = num_ * rhs.den_;
        MyType rhs_num = rhs.num_ * den_;
        if (my_num >= rhs_num)
            num_ = my_num - rhs_num;
        else {
            num_ = rhs_num - my_num;
            is_negative_ = !is_negative_;
        }
        den_ *= rhs.den_;
    }

    canonicalize();

    return *this;
}


// Subtraction operators
//Fraction& operator-=(int8_t rhs);
//Fraction& operator-=(int16_t rhs);
//Fraction& operator-=(int32_t rhs);
//Fraction& operator-=(int64_t rhs);
//Fraction& operator-=(uint8_t rhs);
//Fraction& operator-=(uint16_t rhs);
//Fraction& operator-=(uint32_t rhs);
//Fraction& operator-=(uint64_t rhs);
Fraction& Fraction::operator-=(const Fraction& rhs) {
    // Easy out for adding 0.
    if (MyType{ 0 } == rhs.num_)
        return *this;

    MyType my_num = num_ * rhs.den_;
    MyType rhs_num = rhs.num_ * den_;

    if (is_negative_)
        my_num = -my_num;
    if (rhs.is_negative_)
        rhs_num = -rhs_num;

    num_ = my_num - rhs_num;
    den_ *= rhs.den_;
    is_negative_ = num_ < MyType{ 0 };
    if (num_ < MyType{ 0 }) {
        num_ = -num_;
    }

    canonicalize();

    return *this;
}

// Multiplication operators
//Fraction& operator*=(int8_t rhs);
//Fraction& operator*=(int16_t rhs);
//Fraction& operator*=(int32_t rhs);
Fraction& Fraction::operator*=(int64_t rhs) {
    num_ *= rhs;

    canonicalize();

    return *this;
}


//Fraction& operator*=(uint8_t rhs);
//Fraction& operator*=(uint16_t rhs);
Fraction& Fraction::operator*=(uint32_t rhs) {
    num_ *= rhs;

    canonicalize();

    return *this;
}

//Fraction& operator*=(uint64_t rhs);
Fraction& Fraction::operator*=(const Fraction& rhs) {
    num_ *= rhs.num_;
    den_ *= rhs.den_;
    is_negative_ ^= rhs.is_negative_;

    canonicalize();

    return *this;
}


// Division operators
//Fraction& operator/=(int8_t rhs);
//Fraction& operator/=(int16_t rhs);
//Fraction& operator/=(int32_t rhs);
//Fraction& operator/=(int64_t rhs);
//Fraction& operator/=(uint8_t rhs);
//Fraction& operator/=(uint16_t rhs);
//Fraction& operator/=(uint32_t rhs);
//Fraction& operator/=(uint64_t rhs);
Fraction& Fraction::operator/=(const Fraction& rhs) {
    if (MyType{ 0 } == rhs.num_)
        throw "Divide by 0";

    num_ *= rhs.den_;
    den_ *= rhs.num_;
    is_negative_ ^= rhs.is_negative_;

    canonicalize();

    return *this;
}


void Fraction::canonicalize() {
    if (MyType{ 0 } == num_) {
        den_ = MyType{ 1 };
        is_negative_ = false;
        return;
    }

    auto div = gcd(num_, den_);
    if (div > 1.0) {
        num_ /= div;
        den_ /= div;
    }
}

Fraction operator+(Fraction lhs, int64_t rhs) {
    Fraction ret{ lhs };
    ret += Fraction{ rhs };
    return ret;
}

Fraction operator+(Fraction lhs, uint64_t rhs) {
    Fraction ret{ lhs };
    ret += Fraction(rhs);
    return ret;
}

Fraction operator+(Fraction lhs, const Fraction& rhs) {
    Fraction ret{ lhs };
    ret += rhs;
    return ret;
}

Fraction operator-(Fraction lhs, int64_t rhs) {
    Fraction ret{ lhs };
    ret -= Fraction{ rhs };
    return ret;
}

Fraction operator-(Fraction lhs, uint64_t rhs) {
    Fraction ret{ lhs };
    ret -= Fraction(rhs);
    return ret;
}

Fraction operator-(Fraction lhs, const Fraction& rhs) {
    Fraction ret{ lhs };
    ret -= rhs;
    return ret;
}

Fraction operator*(Fraction lhs, int64_t rhs) {
    Fraction ret{ lhs };
    ret *= Fraction{ rhs };
    return ret;
}

Fraction operator*(Fraction lhs, uint64_t rhs) {
    Fraction ret{ lhs };
    ret *= Fraction(rhs);
    return ret;
}

Fraction operator*(Fraction lhs, const Fraction& rhs) {
    Fraction ret{ lhs };
    ret *= rhs;
    return ret;
}

Fraction operator/(Fraction lhs, int64_t rhs) {
    Fraction ret{ lhs };
    ret /= Fraction{ rhs };
    return ret;
}

Fraction operator/(Fraction lhs, uint64_t rhs) {
    Fraction ret{ lhs };
    ret /= Fraction(rhs);
    return ret;
}

Fraction operator/(Fraction lhs, const Fraction& rhs) {
    Fraction ret{ lhs };
    ret /= rhs;
    return ret;
}

std::ostream& operator<<(std::ostream& out, const Fraction& value) {
    out << (value.is_negative_ ? -value.num_ : value.num_) << "/" << value.den_;
    return out;
}

//std::istream& operator>>(std::istream& in, Fraction& value);
