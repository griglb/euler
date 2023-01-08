#pragma once

#include <set>
#include <utility>
#include <vector>

#include "big_int.h"


using Repeat = std::vector<uint64_t>;
using Fraction = std::pair<uint64_t, Repeat>;

Fraction get_continued_fraction(uint64_t number);

std::pair<BigInt, BigInt> get_convergent(Fraction fraction);
