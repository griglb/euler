// A sequence a_n (n>=0) starts with a_0 = 3 and for each n >= 0:
//      - if a_n is a triangle number, then a_n+1 = a_n + 1
//      - otherwise, a_n+1 = 2a_n - a_n-1 + 1
//
// The sequence begins:
//      3, 4, 6, 7, 9, 12, 16, 21, 22, 24, 27, 31, 36, 37, 39, 42, ...
//      -     -                --                  --
// where triangle numbers are underlined.
//
// The 10th triangle number in the sequence is a_2964 = 1439056.
// Find the index n such that a_n is the 70th triangle number in the sequence.


#include <cmath>
#include <iostream>
#include <unordered_set>
#include <vector>

#include "big_int.h"


// When a_T is a triangle number, then we know a_T+1 = a_T + 1.
// Look at the next subsequence before the next triangle number:
//      a_T+2 = 2 a_T+1 - a_T + 1 = 2 (a_T + 1) - a_T + 1 = 2 a_T + 2 - a_T + 1 = a_T + 3 = a_T+1 + 2
//      a_T+3 = 2 a_T+2 - a_T+1 + 1 = 2 (a_T + 3) - (a_T + 1) + 1 = 2 a_T + 6 - a_T - 1 + 1 = a_T + 6 = a_T+2 + 3
//      a_T+4 = 2 a_T+3 - a_T+2 + 1 = 2 (a_T + 6) - (a_T + 3) + 1 = 2 a_T + 12 - a_T - 3 + 1 = a_T + 10 = a_T+3 + 4
//
// It looks like a_T+k = a_T + T(k) where T(k) is the k'th triangle number.  Let's prove it by induction:
//      Let a_T be a triangle number.
//      Assume a_T+k = a_T + T(k).
// Prove a_T+k+1 = a_T + T(k+1).
//      a_T+k+1 = 2 a_T+k - a_T+k-1 + 1
//              = 2 a_T + 2 T(k) - a_T - T(k-1) + 1
//              = a_T + 2 k(k+1)/2 - (k-1)k/2 + 1
//              = a_T + k^2 + k - 1/2 k^2 + k/2 + 1
//              = a_T + 1/2 k^2 + 3k/2 + 1
//              = a_T + 1/2 (k^2 + 3k + 2)
//              = a_T + 1/2 (k+1)(k+2)
//              = a_T + T(k+1) .QED

// This means the sequence is piecewise parabolic in shape, incrementing by traingle numbers
// between a_n values that are themselves triangle numbers.
// So how do we quickly determine if a random number is a triangle number?
// The test for N is that if (8N+1) is a perfect square, then it is trianguler.
//      T(k) = k(k+1)/2
//      8 T(k) + 1 = 4k(k+1) + 1
//                 = 4k^2 + 4k + 1
//                 = (2k + 1)^2
// So if it's a perfect square, then we also know the argument n for the function T(n).

// This approach works, but it doesn't scale because the 47th triangle number in this
// sequence is the last that fits in 64-bit integers.
// It also isn't super efficient, since it starts with T(1), T(2), ... for each parabola.
// Instead of calculating a_T+k for each k = 1,2,3,... let's instead calculate the sequence
// of triangle numbers and see which T(T+k) = A_T + T(n) for some n. We don't need to know
// the value of every a_k, only the indices of the triangular numbers in this sequence,
// or technically only the 70th.
//


class Solver1 {
public:
    Solver1() {
        //a_n_ = BigInt{ 3 };
        a_n_ = 3;
        tri_indices_.push_back(0);
        //tri_values_.push_back(BigInt{ 3 });
        tri_values_.push_back(3);
    }
    ~Solver1() = default;

    // This function returns the index n where a_n is a triangle number.
    uint64_t solve_for(size_t tri_index) {
        static constexpr size_t kBiggest64BitIndex = 30;
        if (tri_index <= kBiggest64BitIndex) {
            return solve_64_bits(tri_index);
        }

        big_a_n_ = solve_64_bits(kBiggest64BitIndex);
        for (const auto& index : tri_indices_)
            big_tri_indices_.push_back(BigInt{ index });
        for (const auto& value : tri_values_)
            big_tri_values_.push_back(BigInt{ value });

        return solve_big_int(tri_index);
    }

    uint64_t a_n(uint64_t n) {
        return a_n_;
    }

private:
    bool is_triangular(uint64_t n) {
        uint64_t scaled = 8 * n + 1;
        uint64_t root = std::sqrt(scaled);
        return root * root == scaled;
    }

    bool is_triangular(BigInt n) {
        static BigInt kScale{8};
        static BigInt kOffset{1};
        BigInt scaled = n * kScale + kOffset;
        return scaled.is_perfect_square();
    }

    uint64_t solve_64_bits(size_t tri_index) {
        while (tri_index > tri_indices_.size()) {
            for (uint64_t k = 1; ; ++k) {
                a_n_ += k;
                if (is_triangular(a_n_)) {
                    tri_indices_.push_back(tri_indices_.back() + k);
                    tri_values_.push_back(a_n_);
                    std::cout << "\t" << tri_indices_.size() << " - a_" << tri_indices_.back() << " = " << a_n_ << std::endl;
                    break;
                }
            }
        }
        return tri_indices_[tri_index - 1];
    }

    uint64_t solve_big_int(size_t tri_index) {
        while (tri_index > big_tri_indices_.size()) {
            for (BigInt k = BigInt{ 1 }; ; ++k) {
                big_a_n_ += k;
                if (is_triangular(a_n_)) {
                    big_tri_indices_.push_back(big_tri_values_.back() + k);
                    big_tri_values_.push_back(big_a_n_);
                    std::cout << "\t" << big_tri_indices_.size() << " - a_" << big_tri_indices_.back() << " = " << big_a_n_ << std::endl;
                    break;
                }
            }
        }
        return big_tri_indices_[tri_index - 1].to_int();
    }

    uint64_t a_n_;
    std::vector<uint64_t> tri_indices_;
    std::vector<uint64_t> tri_values_;
    BigInt big_a_n_;
    std::vector<BigInt> big_tri_indices_;
    std::vector<BigInt> big_tri_values_;

};

class Solver2 {
public:
    static constexpr size_t kBiggest64BitIndex = 34;

    Solver2() { }
    ~Solver2() = default;

    // This function returns the index n where a_n is a triangle number.
    std::pair<uint64_t, BigInt> solve_for(size_t tri_index) {
        if (tri_index <= kBiggest64BitIndex) {
            return solve_64_bits(tri_index);
        }

        return solve_big_int(tri_index);
    }

private:
    bool is_triangular(uint64_t n) {
        if (0 == n) return false;
        // All triangular numbers end with one of { 0, 1, 3, 5, 6, 8 },
        // so if it ends with one of { 2, 4, 7, 9 } then we know it can't be.
        const int8_t last_digit = n % 10;
        if ((2 == last_digit) || (4 == last_digit) || (7 == last_digit) || (9 == last_digit))
            return false;
        uint64_t scaled = 8 * n + 1;
        uint64_t root = std::sqrt(scaled);
        return root * root == scaled;
    }

    bool is_triangular(const BigInt& n) {
        static BigInt kScale{8};
        static BigInt kOffset{1};

        // All triangular numbers end with one of { 0, 1, 3, 5, 6, 8 },
        // so if it ends with one of { 2, 4, 7, 9 } then we know it can't be.
        const int8_t last_digit = n.get_digits().back();
        if ((2 == last_digit) || (4 == last_digit) || (7 == last_digit) || (9 == last_digit))
            return false;
        BigInt scaled = n * kScale + kOffset;
        return scaled.is_perfect_square();
    }

    uint64_t get_index_of_triangle(uint64_t t_n) {
        uint64_t scaled = 8 * t_n + 1;
        uint64_t root = std::sqrt(scaled);
        return (root - 1) / 2;
    }

    uint64_t get_index_of_triangle(const BigInt& t_n) {
        static BigInt kScale{8};
        static BigInt kOffset{1};
        BigInt scaled = t_n * kScale + kOffset;
        BigInt root = scaled.sqrt();
        return (root.to_int() - 1) / 2;
    }

    std::pair<uint64_t, BigInt> solve_64_bits(size_t a_tri_count) {
        uint64_t a_index = 0;       // a_0 is first triangle number
        uint64_t a_count = 1;       // a_0 is first triangle number
        uint64_t tri_index = 2;     // a_0 = T(2) = 3
        uint64_t last_tri = 3;      // a_0 = 3
        while (a_tri_count > a_count) {
            for (uint64_t next_n = tri_index + 1; ; ++next_n) {
                // Calculate next triangle number.
                uint64_t t_n = next_n * (next_n + 1) / 2;
                // Check if the difference between latest and last known triangle is triangle.
                const uint64_t diff = t_n - last_tri;
                if (is_triangular(diff)) {
                    // Get the index n of the difference in the triangle sequence T(n).
                    uint64_t k = get_index_of_triangle(diff);
                    a_index += k;
                    ++a_count;
                    tri_index = next_n;
                    last_tri = t_n;
                    std::cout << "\t" << a_count << " - a_" << a_index << " = " << last_tri << std::endl;
                    break;
                }
            }
        }
        return { a_index, BigInt{last_tri} };
    }

    std::pair<uint64_t, BigInt> solve_big_int(size_t a_tri_count) {
        static BigInt kIncrement{1};
        static BigInt kScaleFactor{2};

        auto [small_a_index, small_last_tri] = solve_64_bits(kBiggest64BitIndex);

        uint64_t a_index = small_a_index;       // a_0 is first triangle number
        uint64_t a_count = kBiggest64BitIndex;       // a_0 is first triangle number
        BigInt last_tri{ small_last_tri };       // a_0 = 3
        BigInt tri_index{ get_index_of_triangle(last_tri) };      // a_0 = T(2) = 3
        while (a_tri_count > a_count) {
            for (BigInt next_n = tri_index + kIncrement; ; ++next_n) {
                // Calculate next triangle number.
                BigInt t_n = next_n * (next_n + kIncrement) / kScaleFactor;
                // Check if the difference between latest and last known triangle is triangle.
                const BigInt diff = t_n - last_tri;
                if (is_triangular(diff)) {
                    // Get the index n of the difference in the triangle sequence T(n).
                    uint64_t k = get_index_of_triangle(diff);
                    a_index += k;
                    ++a_count;
                    tri_index = next_n;
                    last_tri = t_n;
                    std::cout << "\t" << a_count << " - a_" << a_index << " = " << last_tri << std::endl;
                    break;
                }
            }
        }
        return { a_index, last_tri };
    }

};


int main()
{
    std::cout << "Hello World!\n";

    {
        Solver1 solver;
        auto index = solver.solve_for(10);
        auto a_n = solver.a_n(index);
        std::cout << "The 10th triangle number in the sequence is a_" << index << " = " << a_n << std::endl;
    }

    //{
    //    Solver1 solver;
    //    auto index = solver.solve_for(45);
    //    auto a_n = solver.a_n(index);
    //    std::cout << "The 70th triangle number in the sequence is a_" << index << " = " << a_n << std::endl;
    //}

    {
        Solver2 solver;
        auto [index, a_n] = solver.solve_for(40);
        std::cout << "The 10th triangle number in the sequence is a_" << index << " = " << a_n << std::endl;
    } 
}


// 2 - a_2 = 6
// 3 - a_7 = 21
// 4 - a_12 = 36
// 5 - a_22 = 91
// 6 - a_31 = 136
// 7 - a_166 = 9316
// 8 - a_705 = 154846
// 9 - a_1749 = 700336
//10 - a_2964 = 1439056
//11 - a_30089 = 369335431
//12 - a_50473 = 577099351
//13 - a_75952 = 901701811
//14 - a_107802 = 1408928986
//15 - a_116846 = 1449830476
//16 - a_328876 = 23928296941
//17 - a_492946 = 37387861426
//18 - a_1005666 = 168829016986
//19 - a_1768901 = 460093231216
//20 - a_16622005 = 110767449875176
//21 - a_19192440 = 114071019205006
//22 - a_47513135 = 515101916006866
//23 - a_62328944 = 624856021577011
//24 - a_63337538 = 625364653009726
//25 - a_67970152 = 636095211562531
//26 - a_85573521 = 791034520439296
//27 - a_221945741 = 10089725782489606
//28 - a_274706935 = 11481597605023021
//29 - a_388359024 = 17939996328881026
//30 - a_743521803 = 81010296299963836
//31 - a_898256292 = 92981677420376641
//32 - a_973293477 = 95796967024257346
//33 - a_1794008057 = 432583178346902836
//34 - a_2518531591 = 695050354369089181
//35 - a_3402800581 = 1086016178149033726
//36 - a_4978097567 = 2326796475987024317
//37 - a_4999706038 = 2327029939007307473
//38 - a_5085476503 = 2330708225383350818
//39 - a_5109345282 = 2330993084700760628
//40 - a_5116638191 = 2331019677965248223
//41 - a_5151366065 = 2331622690598892098
//42 - a_5166271719 = 2331733779866928783
//43 - a_5336938403 = 2346297338466040053
//44 - a_5550271757 = 2369052898537151388
//45 - a_6769319497 = 3112091595346229058
//46 - a_8284471162 = 4259933880079941003
//47 - a_8852283942 = 4421139556929511593
 

//30 - a_743521803 = 81010296299963836
//31 - a_898256292 = 92981677420376641
//32 - a_973293477 = 95796967024257346
//33 - a_1794008057 = 432583178346902836
//34 - a_2518531591 = 695050354369089181
//35 - a_3402800581 = 1086016178149033726
//36 - a_4929156493 = 6862583382388275458
//37 - a_6772360963 = 1643755714782786287
//38 - a_8270985531 = 7378379531866079787
//39 - a_9625859933 = 5990378845927251838
//40 - a_11709604526 = 8161374611397491959
