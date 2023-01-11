#pragma once

#include <cstdint>

#include "prime_helper.h"


class Phi {
public:
    Phi() = default;
    ~Phi() = default;

    uint64_t operator()(uint64_t n) const;

    uint64_t naive2(uint64_t n) const;

private:
    mutable PrimeHelper helper;

};
