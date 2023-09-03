// A composite number can be factored many different ways.  For instance, not including
// multiplication by one, 24 can be factored in 7 distinct ways:
//                      24 = 2 x 2 x 2 x 3
//                      24 = 2 x 3 x 4
//                      24 = 2 x 2 x 6
//                      24 = 4 x 6
//                      24 = 3 x 8
//                      24 = 2 x 12
//                      24 = 24
//
// Recall that the digital root of a number, in base 10, is found by adding together
// the digits of that number, and repeating that process until a number is arrived at
// that is less than 10.  Thus the digital root of 467 is 8.
//
// We shall call a Digital Root Sum(DRS) the sum of the digital roots of the individual
// factors of our number.
// The chart below demonstrates all of the DRS values for 24.
//                  Factorisation   Digital Root Sum
//                  2 x 2 x 2 x 3           9
//                    2 x 3 x 4             9
//                    2 x 2 x 6            10
//                      4 x 6              10
//                      3 x 8              11
//                      2 x 12              5
//                       24                 6
//
// The maximum Digital Root Sum of 24 is 11.
// The function mdrs(n) gives the maximum Digital Root Sum of n.
// So mdrs(24) = 11.
// Find sum(mdrs(n) for n < 1'000'000.


#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <utility>
#include <vector>

#include "combinatorics.h"
#include "partitions.h"
#include "prime_helper.h"


// The Digital Root of a number is the same as that number mod 9, so it is
// easy to calculate.  So the Digital Root Sum is just the sum of each factor
// mod 9, and the mdrs is just the max over those sums.
//
// Calculating the set of factorisations is the challenge.  I have PrimeHelper
// to get the prime factorisation, but that is only 1 of many possible factorisations.
// Once we have the prime factorization, we can use that to generate the others.
// This is a form of the parition problem.  You can see there are 4 prime factors
// for 24, so we can partition them by index, where multi-element partitions become
// products of the prime factors at those indices:
//      { 0 }, { 1 }, { 2 }, { 3 }  = 2 x 2 x 2 x 3
//      { 0, 1 }, { 2 }, { 3 }      = 4 x 2 x 3
//      { 0, 2 }, { 1 }, { 3 }      = 4 x 2 x 3
//      { 0, 3 }, { 1 }, { 2 }      = 6 x 2 x 2
//      { 1, 2 }, { 0 }, { 3 }      = 4 x 2 x 3
//      { 1, 3 }, { 0 }, { 2 }      = 6 x 2 x 2
//      { 2, 3 }, { 0 }, { 1 }      = 6 x 2 x 2
//      { 0, 1 }, { 2, 3 }          = 4 x 6
//      { 0, 2 }, { 1, 3 }          = 4 x 6
//      { 0, 3 }, { 1, 2 }          = 6 x 4
//      { 0, 1, 2 }, { 3 }          = 8 x 3
//      { 0, 1, 3 }, { 2 }          = 12 x 2
//      { 0, 2, 3 }, { 1 }          = 12 x 2
//      { 1, 2, 3 }, { 0 }          = 12 x 2
//      { 0, 1, 2, 3 }              = 24
//
// Because of the repeated factor 2, the 15 partitions reduce to the 7 unique factorisations.
//


// Histogram of prime factors in (1, 1'000'000), and the time to calculate partitions (ns):
// 0           0                     0
// 1       78498                   400
// 2      210035                   567
// 3      250853                 4'600
// 4      198062                33'500
// 5      124465                35'100
// 6       68963               192'900
// 7       35585               624'200
// 8       17572             2'858'300
// 9        8491            15'552'200
// 10       4016            95'204'800
// 11       1878           839'606'400
// 12        864         6'226'532'900
// 13        400        37'559'145'700
// 14        179           too big
// 15         79
// 16         35
// 17         14
// 18          7
// 19          2
//
// Only 316 out of 999'998 numbers have >13 factors, so treat them specially
// Sum = 14168506   (when missing these 316 numbers)


class MDRS_Helper {
public :
    using Factors = std::multiset<uint64_t>;
    using AllFactors = std::set<Factors>;

    MDRS_Helper(size_t max_num = 10'000) {
        helper_.get_primes(max_num);
        mdrs_.resize(max_num);
        // For single digit numbers, they are the largest digit root.
        for (size_t n = 2; n < 10; ++n)
            mdrs_[n] = n;

        partitioner_.get_partitions(13);
    }

    size_t get_max_partition_size() { return partitioner_.get_max_partition_size(); }

    inline int64_t dr(const int64_t num) {
        auto dr = num % 9;
        return (dr > 0) ? dr : 9;
    }

    std::vector<size_t> count_factors(size_t max_num) {
        std::vector<size_t> ret(20, 0);

        for (size_t num = 2; num < max_num; ++num) {
            const auto factors = helper_.get_factorization_fast(num);
            size_t count{ 0 };
            for (const auto& [prime, exp] : factors) {
                count += exp;
            }
            if (count >= ret.size())
                ret.resize(count + 1);
            ++ret[count];
        }

        return ret;
    }

    int64_t mdrs(const int64_t num) {
        if ((mdrs_.size() > num) && (mdrs_[num] > 0))
            return mdrs_[num];

        if (mdrs_.size() <= num)
            mdrs_.resize(num + 1);

        int64_t value{ 0 };

        const auto factorisations = get_factorisations(num);

        for (const auto& factors : factorisations) {
            int64_t score{ 0 };
            for (const auto& fact : factors) {
                score += dr(fact);
            }
            value = std::max(value, score);
        }

        mdrs_[num] = value;
        return mdrs_[num];
    }

    int64_t mdrs_recurse(const int64_t num) {
        if ((mdrs_.size() > num) && (mdrs_[num] > 0))
            return mdrs_[num];

        if (mdrs_.size() <= num)
            mdrs_.resize(num + 1);

        int64_t value{ 0 };

        auto factors = helper_.get_factorization_fast(num);

        if (1 == factors.size()) {
            const auto& [prime, exp] = *factors.begin();
            if (1 == exp) {
                // Prime numbers are their own digital root.
                value = dr(factors.begin()->first);
            }
            else {
                int64_t score1 = mdrs_[num / prime] + prime % 9;
                int64_t score2 = dr(num);

                value = std::max(score1, score2);

                if ((prime < 4) && (exp > 1)) {
                    int64_t p2 = prime * prime;
                    int64_t score = mdrs_[num / p2] + p2;
                    value = std::max(value, score);
                }
                if ((2 == prime) && (exp > 2)) {
                    int64_t score = mdrs_[num / 8] + 8;
                    value = std::max(value, score);
                }
            }
        }
        else {
            for (const auto& [prime, exp] : factors) {
                int64_t quot = num / prime;
                if (0 == mdrs_[quot])
                    mdrs(quot);

                int64_t score = mdrs_[quot] + prime % 9;
                value = std::max(value, score);
            }
            // Check the digital root of num, just in case
            value = std::max(value, dr(num));

            // Check the factors, if there are any <10, then they could create a higher MDRS
            uint64_t exp2{ 0 };
            uint64_t exp3{ 0 };
            if (factors.find(2) != factors.cend())
                exp2 = factors.at(2);
            if (factors.find(3) != factors.cend())
                exp3 = factors.at(3);

            if (exp2 > 1) {
                int64_t score = mdrs_[num / 4] + 4;
                value = std::max(value, score);
            }
            if (exp2 > 2) {
                int64_t score = mdrs_[num / 8] + 8;
                value = std::max(value, score);
            }

            if (exp3 > 1) {
                int64_t score = mdrs_[num / 9] + 9;
                value = std::max(value, score);
            }

            if ((exp2 > 0) && (exp3 > 0)) {
                int64_t score = mdrs_[num / 6] + 6;
                value = std::max(value, score);
            }
        }

        mdrs_[num] = value;
        return mdrs_[num];
    }

    int64_t mdrs_recurse_all(const int64_t num) {
        if ((mdrs_.size() > num) && (mdrs_[num] > 0))
            return mdrs_[num];

        if (mdrs_.size() <= num)
            mdrs_.resize(num + 1);

        int64_t value{ dr(num)};

        auto divisors = helper_.get_proper_divisors(num);

        if (1 < divisors.size()) {
             for (const auto& div : divisors) {
                 if (1 == div)
                     continue;
                 int64_t quot = num / div;
                 if (0 == mdrs_[quot])
                    mdrs(quot);

                int64_t score = mdrs_[quot] + dr(div);
                value = std::max(value, score);
            }
        }

        mdrs_[num] = value;
        return mdrs_[num];
    }


    AllFactors get_factorisations(const int64_t num) {
        AllFactors ret;

        const auto factorization = helper_.get_factorization_fast(num);
        std::vector<uint64_t> prime_factors;
        for (const auto& [prime, exp] : factorization) {
            for (size_t i = 0; i < exp; ++i)
                prime_factors.push_back(prime);
        }

        if (prime_factors.size() > 13)
            return ret;

        const auto &parts = partitioner_.get_partitions(prime_factors.size());

        for (const auto& part : parts) {
            Factors factors;
            for (const auto& prod : part) {
                uint64_t fact{ 1 };
                for (const auto& el : prod) {
                    fact *= prime_factors[el];
                }
                factors.insert(fact);
            }
            ret.insert(factors);
        }

        return ret;
    }


private :
    PrimeHelper helper_;
    Partitioner partitioner_;

    using Indices = std::vector<size_t>;
    using NumChoose = std::pair<int64_t, int64_t>;
    std::map<NumChoose, Indices> factor_lut_;

    std::vector<uint64_t> mdrs_;

};


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    Partitioner partitioner;

    //    size_t n = 4;

    //    auto partitions = partitioner.get_partitions(n);
    //    std::cout << "N = " << n << " has " << partitions.size() << " different partitions:" << std::endl;
    //    //for (const auto &part : partitions) {
    //    //    std::cout << "\t";
    //    //    for (const auto& prod : part) {
    //    //        std::cout << "{ ";
    //    //        for (const auto& el : prod)
    //    //            std::cout << el << ", ";
    //    //        std::cout << "}";
    //    //    }
    //    //    std::cout << std::endl;
    //    //}
    //}

    //{
    //    MDRS_Helper helper;

    //    auto factorisations = helper.get_factorisations(8192);
    //    for (const auto & factors : factorisations) {
    //        for (const auto& fact : factors) {
    //            std::cout << fact << " x ";
    //        }
    //        std::cout << std::endl;
    //    }
    //    std::cout << "Max partition size = " << helper.get_max_partition_size() << std::endl;
    //    return 0;
    //}

    //{
    //    MDRS_Helper helper;
    //    MDRS_Helper helper2;

    //    size_t wrong{ 0 };

    //    for (int64_t num = 2; num <= 100; ++num) {
    //        std::cout << "mdrs(" << num << ") = " << helper.mdrs(num) << "\t" << helper2.mdrs_recurse(num) << std::endl;
    //        if (helper.mdrs(num) != helper2.mdrs_recurse(num)) {
    //            std::cout << "  Number " << num << " doesn't match!!!" << std::endl;
    //            ++wrong;
    //        }
    //    }
    //    std::cout << "Max partition size = " << helper.get_max_partition_size() << std::endl;
    //    std::cout << "Number wrong = " << wrong << std::endl;
    //    return 0;
    //}

    //{
    //    constexpr size_t kMaxNum{ 1'000'000 };

    //    MDRS_Helper helper(kMaxNum);

    //    auto counts = helper.count_factors(kMaxNum);
    //    for (size_t n = 0; n < counts.size(); ++n)
    //        std::cout << n << "\t" << counts[n] << std::endl;
    //    return 0;
    //}

    //{
    //    Partitioner partitioner;

    //    for (size_t n = 1; n < 15; ++n) {
    //        auto start = std::chrono::steady_clock::now();

    //        partitioner.get_partitions(n);

    //        auto end = std::chrono::steady_clock::now();

    //        auto dur = end - start;

    //        std::cout << n << "\t" << dur.count() << std::endl;
    //    }
    //}

    {
        constexpr size_t kMaxNum{ 1'000'000 };

        MDRS_Helper helper(kMaxNum);
        int64_t sum{ 0 };

//        std::ofstream out_file{ "d:\\temp\\pe159_recurse_all.csv" };

        for (int64_t num = 2; num < kMaxNum; ++num) {
            const auto mdrs = helper.mdrs_recurse_all(num);
            std::cout << "mdrs(" << num << ") = " << mdrs << std::endl;
//            out_file << "mdrs(" << num << ") = " << mdrs << std::endl;
            sum += mdrs;
        }
        std::cout << "Sum = " << sum << std::endl;
        std::cout << "Max partition size = " << helper.get_max_partition_size() << std::endl;
    }
}


// 16384 = 2^14 = 8 x 8 x 8 x 8 x 4
