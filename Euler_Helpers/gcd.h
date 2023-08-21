#pragma once

#include <cstdint>

#include "big_int.h"


int64_t gcd(int64_t a, int64_t b);

BigInt gcd(const BigInt& a, const BigInt& b);

int64_t lcm(int64_t a, int64_t b);

BigInt lcm(const BigInt& a, const BigInt& b);
