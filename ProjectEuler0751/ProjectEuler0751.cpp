// A non-decreasing sequence of integers can be generated from any positive real value by the
// following procedure:
//          b_1 = theta
//          b_n = floor(b_n-1) * (b_n-1 - floor(b_n-1) + 1)
//          a_n = floor(b_2)
//
// For example, theta = 2.956938891377988... generates the Fibonacci sequence:
//      2, 3, 5, 8, 13, 21, 34, 55, 89, ...
//
// The concatenation of a sequence of positive integers a_n  is a real value denoted tau
// constructed by concatenating the elements of the sequence after the decimal point,
// starting at a_1: a_1.a_2 a_3 a_4...
//
// For example, the Fibonacci sequence constructed from theta = 2.956938891377988... yields
// the concatenation 2.3581321345589...
// Clearly, tau != theta for this value of theta.
//
// Find the only value of theta for which the generated sequence starts at a_1 = 2 and the
// concatenation of the generated sequence equals the original value : tau = theta.
// Give your answer rounded to 24 places after the decimal point.


#include <iomanip>
#include <iostream>
#include <vector>

#include "big_int.h"


BigInt get_sequence(double theta) {
    std::vector<double> b{ theta };
    for (size_t n = 2; n < 14; ++n) {
        b.push_back(std::floor(b.back()) * (b.back() - std::floor(b.back()) + 1));
    }

    BigInt::Digits digits;
    for (const auto& b_n : b) {
std::cout << std::setprecision(16) << b_n << std::endl;
        const BigInt::Digits& a_dig = BigInt{ b_n }.get_digits();
        std::copy(a_dig.cbegin(), a_dig.cend(), std::back_inserter(digits));
    }

    return BigInt{ digits };
}


//2.956938891377988
//3.913877782755976
//5.741633348267928
//8.708166741339639
//13.66533393071711
//21.64934109932244
//34.63616308577127
//55.62954491622327
//89.62497039227969
//144.6223649128926
//233.6205474565395
//377.587557373703
//598.5091298860389


BigInt get_sequence(const uint64_t theta) {
    std::vector<uint64_t> b{ theta };
    std::vector<uint64_t> a;

    uint64_t tmp{ theta };
    size_t exp10{ 0 };
    size_t pow10{ 1 };
    while (tmp > 10) {
        ++exp10;
        pow10 *= 10;
        tmp /= 10;
    }

    for (size_t n = 2; n < 14; ++n) {
        auto b_n = b.back();
        auto a_n = b_n / pow10;
        a.push_back(a_n);

        b_n %= pow10;
        b_n += pow10;

        auto b_n_1 = a_n * b_n;
        b.push_back(b_n_1);
        std::cout << b.back() << std::endl;
    }

    BigInt::Digits digits;
    for (const auto& a_n : a) {
        const BigInt::Digits& a_dig = BigInt{ a_n }.get_digits();
        std::copy(a_dig.cbegin(), a_dig.cend(), std::back_inserter(digits));
    }

    return BigInt{ digits };
}


BigInt get_sequence(const BigInt theta) {
    std::vector<BigInt> b{ theta };
    std::vector<BigInt> a;

    size_t exp10 = theta.get_digits().size() - 1;

    for (size_t n = 2; n <= exp10+1; ++n) {
        auto b_n_dig = b.back().get_digits();
        auto a_n_dig = b_n_dig;
        for (size_t e = 0; e < exp10; ++e)
            a_n_dig.pop_back();
        a.push_back(BigInt{ a_n_dig });

        std::reverse(b_n_dig.begin(), b_n_dig.end());
        while (b_n_dig.size() > exp10)
            b_n_dig.pop_back();
        b_n_dig.push_back(1);
        std::reverse(b_n_dig.begin(), b_n_dig.end());

        b.push_back(a.back() * BigInt { b_n_dig });
//        std::cout << b.back() << std::endl;
    }

    auto a_n_dig = b.back().get_digits();
    for (size_t e = 0; e < exp10; ++e)
        a_n_dig.pop_back();
    a.push_back(BigInt{ a_n_dig });

    // When the b_n = A.BCDEFGH then
    //      b_n+1 = A * (1.BCDEFGH)
    // This becomes slightly harder when A > 10.


    BigInt::Digits digits;
    for (const auto& a_n : a) {
        const BigInt::Digits& a_dig = BigInt{ a_n }.get_digits();
        std::copy(a_dig.cbegin(), a_dig.cend(), std::back_inserter(digits));
    }

    return BigInt{ digits };
}


void clamp_sequence(BigInt& seq) {
    auto dig = seq.get_digits();
    while (dig.size() > 25)
        dig.pop_back();
    seq.set_digits(dig);
}

BigInt solve_it() {
    const BigInt ten24{ BigInt{1'000'000'000'000}*BigInt{1'000'000'000'000} };

    BigInt lower{ 2'000'000'000'000 };
    BigInt upper{ 3'000'000'000'000 };

    lower *= 1'000'000'000'000;
    upper *= 1'000'000'000'000;

    for (int i = 0; i < 75; ++i) {
        BigInt lower_seq = get_sequence(lower);
        BigInt upper_seq = get_sequence(upper);

        BigInt mid = (upper + lower) / 2ull;
        BigInt mid_seq = get_sequence(mid);

        std::cout << lower_seq << "\t" << mid << "\t" << mid_seq << "\t" << upper_seq << std::endl;

        clamp_sequence(lower_seq);
        clamp_sequence(mid_seq);
        clamp_sequence(upper_seq);

        std::cout << lower_seq << "\t" << mid << "\t" << mid_seq << "\t" << upper_seq << std::endl;

        if (mid > mid_seq) {
            upper = mid;
        }
        else {
            lower = mid;
        }

        std::cout << lower << "\t" << upper << std::endl;
    }

    return {};
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    std::cout << get_sequence(2.956938891377988) << std::endl;
    //}

    //{
    //    std::cout << get_sequence(2956938891377988ull) << std::endl;
    //}

    //{
    //    std::cout << get_sequence(BigInt{ 2956938891377988 }) << std::endl;
    //}

    {
        std::cout << solve_it() << std::endl;
    }
}
