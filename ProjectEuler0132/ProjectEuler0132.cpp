// A number consisting entirely of ones is called a repunit.
// We shall define R(k) to be a repunit of length k.
//
// For example, R(10) = 1111111111 = 11 x 41 x 271 x 9091,
// and the sum of these prime factors is 9414.
//
// Find the sum of the first forty prime factors of R(10^9).


#include <iostream>
#include <map>
#include <numeric>
#include <optional>
#include <set>
#include <utility>

#include "big_int.h"
#include "prime_helper.h"


// For any R(k), calculate the proper divisors of k, call them
// {d1, d2, d3, ..., dn}.  Then each R(di) divides into R(k).
// This can be proven as such:
//   - R(k) is a number with k consecutive 1's
//   - For any di that is a factor of k, R(di) is a number with di consecutive 1's
//   - You can construct R(k) by concatenating k/di copies of R(di)
//   - R(k) / R(di) = {1 followed by di-1 0's} repeated k/di - 1 times, + 1
//     For example, R(6) / R(2) = 111111 / 11 = 10101

// So for R(10^9), we can look at the divisors of 10^9 = 2^9 x 5^9
// There are 99 total divisors, though we omit 1 since R(1) = 1 is not helpful:
// 2
// 4
// 5
// 8
// 10
// 16
// 20
// 25
// 32
// 40
// 50
// 64
// 80
// 100
// 125
// 128
// 160
// 200
// 250
// 256
// 320
// 400
// 500
// 512
// 625
// 640
// 800
// 1000
// 1250
// 1280
// 1600
// 2000
// 2500
// 2560
// 3125
// 3200
// 4000
// 5000
// 6250
// 6400
// 8000
// 10000
// 12500
// 12800
// 15625
// 16000
// 20000
// 25000
// 31250
// 32000
// 40000
// 50000
// 62500
// 64000
// 78125
// 80000
// 100000
// 125000
// 156250
// 160000
// 200000
// 250000
// 312500
// 320000
// 390625
// 400000
// 500000
// 625000
// 781250
// 800000
// 1000000
// 1250000
// 1562500
// 1600000
// 1953125
// 2000000
// 2500000
// 3125000
// 3906250
// 4000000
// 5000000
// 6250000
// 7812500
// 8000000
// 10000000
// 12500000
// 15625000
// 20000000
// 25000000
// 31250000
// 40000000
// 50000000
// 62500000
// 100000000
// 125000000
// 200000000
// 250000000
// 500000000

// Can a relationship between the factors of R(a) and R(b) be established with R(a*b)?
// Prime factorizations of the first few R(k) values:
//      k   factors
//      2   11
//      3   3, 37
//      4   11, 101
//      5   41, 241
//      6   3, 7, 11, 13, 37
//      7   239, 4649
//      8   11, 73, 101, 137
//      9   3, 3, 37, 333667
//     10   11, 41, 271, 9091
//     11   21649, 513239
//     12   3, 7, 11, 13, 37, 101, 9901


class RepunitSolver {
public :
    RepunitSolver() {
        helper_.get_primes(1'000'000);
        // special case for R(1), R(2)
        r_factors_[1] = {};
        r_factors_[2] = { { 11, 1 } };
    }

    std::set<uint64_t> solve() {
        std::set<uint64_t> ret;

        auto divisors = helper_.get_proper_divisors(1'000'000);
        for (const auto& div : divisors) {
            std::cout << div << std::endl;
            if (1 == div)
                continue;

            BigInt R;
            R.set_digits(BigInt::Digits(div, 1));
            auto sub_divisors = helper_.get_proper_divisors(div);

            // Divide R(div) by all the unique primes that factor into R(sub_div)
            std::set<uint64_t> factors;
            for (const auto& sub_div : sub_divisors) {
                if (1 == sub_div)
                    continue;
                auto& tmp = r_factors_.at(sub_div);
                for (const auto& [prime, exp] : tmp)
                    factors.insert(prime);
            }
            for (const auto& fact : factors) {
                R /= fact;
            }

            auto factorization = helper_.get_factorization_fast(R.to_int());
            for (const auto& [prime, exp] : factorization)
                ret.insert(prime);

            for (const auto& fact : factors) {
                if (factorization.find(fact) == factorization.end()) {
                    factorization[fact] = 1;
                }
                else {
                    ++factorization[fact];
                }
            }
            r_factors_[div] = factorization;

            if (ret.size() >= 40)
                break;
        }

        return ret;
    }

private :
    PrimeHelper helper_;
    std::map<uint64_t, Factorization> r_factors_;


};


void solve1() {
    constexpr uint64_t max_value{ 50'000 };

    std::vector<uint64_t> answer;

    std::map<uint64_t, BigInt> R_factors;

    uint64_t pow5{ 1 };
    for (uint8_t exp5 = 0; exp5 < 9; ++exp5, pow5 *= 5) {
        std::cout << pow5 << std::endl;
        BigInt R_5{ 1 };
        R_5.set_digits(BigInt::Digits(pow5, 1));
        BigInt R_5_1{ 1 };
        if (1 < pow5)
            R_5_1.set_digits(BigInt::Digits(pow5 / 5, 1));

        uint64_t pow2{ 1 };
        for (uint8_t exp2 = 0; exp2 < 10; ++exp2, pow2 *= 2) {
            BigInt R_2{ 1 };
            R_2.set_digits(BigInt::Digits(pow2, 1));
            BigInt R_2_1{ 1 };
            if (1 < pow2)
                R_2_1.set_digits(BigInt::Digits(pow2 / 2, 1));

            const uint64_t prod{ pow2 * pow5 };
            if (1 == prod)
                continue;
            BigInt R_prod;
            R_prod.set_digits(BigInt::Digits(prod, 1));
            if (pow2 > 1)
                R_prod /= pow5 > 1 ? R_5 : R_5_1;
            else
                R_prod /= R_5_1;
            if (pow5 > 1)
                R_prod /= pow2 > 1 ? R_2 : R_2_1;
            else
                R_prod /= R_2_1;

            R_factors[prod] = R_prod;
        }
    }

    //for (const auto& [key, value] : R_factors)
    //    std::cout << key << "\t" << value << std::endl;
    std::cout << std::endl << "There are " << R_factors.size() << " factors" << std::endl;

    PrimeHelper helper;
    auto primes = helper.get_primes(max_value);
    for (const auto& p : primes) {
        if ((2 == p) || (5 == p))
            continue;

        std::cout << p << "\t" << answer.size() << std::endl;

        for (const auto& [div, R] : R_factors) {
            BigInt quot = R / p;
            quot *= p;
            if (R == quot) {
                answer.push_back(p);
                break;
            }
        }

        if (answer.size() == 40)
            break;
    }
    std::cout << "The list of primes is" << std::endl;
    for (const auto& ans : answer)
        std::cout << ans << std::endl;
    std::cout << "Sum = " << std::accumulate(answer.begin(), answer.end(), 0ULL);
}


void solve2() {
    constexpr uint64_t max_value{ 200'000 };

    std::vector<uint64_t> answer;

    std::map<uint64_t, BigInt> R_factors;

    uint64_t pow5{ 1 };
    for (uint8_t exp5 = 0; exp5 < 10; ++exp5, pow5 *= 5) {
        std::cout << pow5 << std::endl;
        uint64_t pow2{ 1 };
        for (uint8_t exp2 = 0; exp2 < 10; ++exp2, pow2 *= 2) {
            const uint64_t prod{ pow2 * pow5 };
            if (1 == prod)
                continue;
            if (prod > max_value)
                continue;

            BigInt R_prod;
            R_prod.set_digits(BigInt::Digits(prod, 1));
            R_factors[prod] = R_prod;
        }
    }

    //for (const auto& [key, value] : R_factors)
    //    std::cout << key << "\t" << value << std::endl;
    std::cout << std::endl << "There are " << R_factors.size() << " factors" << std::endl;

    PrimeHelper helper;
    auto primes = helper.get_primes(max_value);
    for (const auto& p : primes) {
        if ((2 == p) || (5 == p))
            continue;

        BigInt p_big{ p };

        std::cout << p_big << "\t" << answer.size() << std::endl;

        std::optional<std::pair<uint64_t, uint64_t>> found;
        for (const auto& [div, R] : R_factors) {
            // From Problem 130, we know that p-1 % A(p) == 0, so skip any div > p and not holding this relationship.
            if (div > p)
                continue;
            if (((p - 1) % div) != 0)
                continue;

            BigInt quot = R / p;
            quot *= p;
            if (R == quot) {
                found = { p, div };
                std::cout << "\t" << p << " divides R(" << div << ")" << std::endl;
                break;
            }
        }

        if (found.has_value()) {
            answer.push_back(found->first);
            if (answer.size() >= 40)
                break;

            // Divide all mulitples of div by the new prime factor
            for (auto& [div, R] : R_factors) {
                if (0 == div % found->second)
                    R /= found->first;
            }
        }
    }

    //for (const auto& [key, value] : R_factors)
    //    std::cout << key << "\t" << value << std::endl;

    std::cout << "The list of primes is" << std::endl;
    for (const auto& ans : answer)
        std::cout << ans << std::endl;
    std::cout << "Sum = " << std::accumulate(answer.begin(), answer.end(), 0ULL);
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    PrimeHelper helper;
    //    auto divisors = helper.get_proper_divisors(1'000'000'000);
    //    for (const auto& d : divisors)
    //        std::cout << d << std::endl;
    //    std::cout << "There are " << divisors.size() << " proper divisors" << std::endl;
    //}

    {
        solve2();
    }

}
