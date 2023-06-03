// A hexagonal tile with number is surrounded by a ring of six hexagonal tiles,
// starting at "12 o'clock" and numbering the tiles 2 to 7 in an anti-clockwise
// direction.
//
// New rings are added in the same fashion, with the next rings being numbered
// 8 to 19, 20 to 37, 38 to 61, and so on.
// The diagram below shows the first three rings.
// 
//                  20
//              21      37
//          22       8      36
//      23       9      19      35
//          10       2      18
//      24       3       7      34
//          11       1      17
//      25       4       6      33
//          12       5      16
//      26      13      15      32
//          27      14      31
//              28      30
//                  29
// 
// By finding the difference between tile and each of its six neighbours we shall
// define PD(n) to be the number of those differences which are prime.
//
// For example, working clockwise around tile 8 the differences are 12, 29, 11,
// 6, 1, and 13.  So PD(8) = 3.
//
// In the same way, the differences around tile 17 are 1, 17, 16, 1, 11, and 10,
// hence PD(17) = 2.
//
//  It can be shown that the maximum value of PD(n) is 3.
//
// If all of the tiles for which PD(n) = 3 are listed in ascending order to form
// a sequence, the 10th tile would be 271.
//
// Find the 2000th tile in this sequence.


#include <array>
#include <cstdint>
#include <iostream>
#include <unordered_set>

#include "prime_helper.h"

// First step is to calculate which ring a given number is in.
// Look at highest number in each ring:
//      0       1
//      1       7
//      2      19
//      3      37
//      4      61
// Ring N has 6*N numbers in it, so the highest number in a ring can be calculated as
//      hi(N) = sum(6*i; i=1..N) + 1
//      hi(N) = 6 * N * (N+1) / 2 + 1
//      hi(N) = 3 * N * (N+1) + 1
// So for a number x, which ring?
// The highest value in each ring N is 3*N^2 + 3*N + 1
// The lowest value in each ring N is 3*(N-1)^2 + 3*(N-1) + 2
//      lo(N) = 3*N^2 - 6*n + 3 + 3*N - 3 + 2
//      lo(N) = 3*N^2 - 3*N + 2
// Use quadratic equation to solve for N given x in [lo(N), hi(N)]
//      3*N^2 - 3*N + 2 = x
//   A = 3, B = -3, C = (2-x)
//      x = (3 +- sqrt(9 - 12*(2-x)) / 6
//      x = (3 +- sqrt(12*x - 15)) / 6
//      x = 1/2 +- sqrt((12 * x - 15) / 36)
//      x = 1/2 +- sqrt(x/3 - 5/12)
// The difference term can be negative, the sum term cannot, so use that.
// This will yield a floating point number x, truncating it to its floor is the ring index:
//      ring(x) = floor(sqrt(x/3 - 5/12) + 1/2)

// PD(1) is a special case, being ring 0.  We can see the differences are 1, 2, 3, 4, 5, 6,
// so PD(1) = 3, making 1 the first tile in the sequence.

// For all other tiles, there are 2 possible sets of neighbors:
//   if the tile is at one of the 6 vertices of the ring, then it will
//     have 3 neighbors from ring n+1, 2 from ring n, and 1 from ring n-1
//   if the tile is not at one of the 6 vertices, then it will have
//     2 neighbors from each ring, n, n-1, n+1
// To identify if a tile is at a vertex or not:
//     n = ring(x)
//     pos = x - lo(n)
//     if (pos % n == 0) then vertex

// The 2 neighbors from ring n will always have a difference of 1, with the
// exception of when the tile is the first tile in that ring, lo(n).  In that
// case the intra-ring neighbors are (1, 6*n-1).


class RingCounter {
public :
    RingCounter(uint64_t num_rings) : num_rings_(num_rings) {
        auto prime_vec = helper_.get_primes(num_rings_ * 12 + 6);
        primes_ = std::unordered_set<uint64_t>{ prime_vec.begin(), prime_vec.end() };
    }
    ~RingCounter() = default;

    uint64_t get_ring(uint64_t x) {
        if (1 == x)
            return 0;
        double value = sqrt(x / 3.0 - 5.0 / 12.0) + 0.5;
        return static_cast<uint64_t>(value);
    }


    uint64_t get_lo(uint64_t ring) {
        return 3 * ring * (ring - 1) + 2;
    }


    uint64_t get_hi(uint64_t ring) {
        return 3 * ring * (ring + 1) + 1;
    }


    uint64_t PD(uint64_t x) {
        // The center tile and first ring are special cases,
        // so just hard-code the return values here
        switch (x) {
        case 1: return 3;  //  diffs = 1, 6, 5, 4, 3, 2
        case 2: return 3;  //  diffs = 6, 17, 5, 1, 1, 7
        case 3: return 2;  //  diffs = 6, 1, 2, 1, 8, 7
        case 4: return 2;  //  diffs = 1, 3, 1, 9, 8, 7
        case 5: return 0;  //  diffs = 4, 1, 10, 9, 8, 1
        case 6: return 2;  //  diffs = 1, 11, 10, 9, 1, 5
        case 7: return 2;  //  diffs = 12, 11, 10, 1, 6, 5
        }

        calc_diffs(x);
        uint64_t ret{ 0 };
        for (const auto& d : diffs_) {
            if (primes_.find(d) != primes_.end())
                ++ret;
        }
        return ret;
    }

    std::vector<uint64_t> get_sequence(uint64_t length) {
        std::vector<uint64_t> ret;

        uint64_t lo = get_lo(num_rings_);

        for (uint64_t x = 1; x < lo; ++x) {
            if (PD(x) == 3) {
                ret.push_back(x);
                std::cout << ret.size() << "\t" << get_ring(x) << "\t" << x << std::endl;
                //if (ret.size() > length)
                //    break;
            }
        }

        return ret;
    }

private :
    uint64_t num_rings_;
    PrimeHelper helper_;
    std::array<uint64_t, 6> diffs_;
    std::unordered_set<uint64_t> primes_;

    void calc_diffs(uint64_t x) {
        const uint64_t n = get_ring(x);
        const uint64_t lo = get_lo(n);
        const uint64_t pos = x - lo;
        uint64_t six_n = 6 * n;

        // Check special case of tile being first vertex in ring
        if (pos == 0) {
            // The neighbor on ring n-1 has a difference of 6*(n-1)
            diffs_[0] = six_n - 6;
            // The neighbors on ring n are (1, 6*n-1)
            diffs_[1] = 1;
            diffs_[2] = six_n - 1;
            // The neighbors on ring n+1 are (6*n, 6*n+1, 6*n+6*(n+1)-1)
            diffs_[3] = six_n;
            diffs_[4] = six_n + 1;
            diffs_[5] = 2 * six_n + 5;
            return;
        }

        // The differences between adjacent rows change by 1 at each vertex,
        // the side variable keeps track of this shift.
        const uint64_t side = pos / n;

        if ((pos % n) == 0) {
            // We have a vertex tile

            // The neighbor on ring n-1 has a difference of 6*(n-1) + side
            diffs_[0] = six_n - 6 + side;
            // The neighbors on ring n are both 1
            diffs_[1] = 1;
            diffs_[2] = 1;
            // The neighbors on ring n+1 are (6*n + side - 1, 6*n + side, 6*n + side + 1)
            diffs_[3] = six_n + side - 1;
            diffs_[4] = six_n + side;
            diffs_[5] = six_n + side + 1;
        }
        else {
            // We have an edge tile

            // The neighbors on ring n-1 are of (6*(n-1) + side + 1, 6*(n-1) + side)
            diffs_[0] = six_n - 6 + side + 1;
            diffs_[1] = six_n - 6 + side;
            // The neighbors on ring n are both 1, unless this is the last tile in ring n
            diffs_[2] = 1;
            diffs_[3] = x == get_hi(n) ? six_n - 1 : 1;
            // The neighbors on ring n+1 are (6*n + side - 1, 6*n + side, 6*n + side + 1)
            diffs_[4] = six_n + side;
            diffs_[5] = six_n + side + 1;
        }
    }
};


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    for (uint64_t num = 1; num < 150; ++num)
    //        std::cout << num << " is in ring " << get_ring(num) << std::endl;
    //}

    //{
    //    for (uint64_t ring = 0; ring <= 10; ++ring)
    //        std::cout << ring << "\t" << get_lo(ring) << std::endl;
    //}

    //{
    //    for (uint64_t ring = 0; ring <= 10; ++ring)
    //        std::cout << ring << "\t" << get_hi(ring) << std::endl;
    //}

    //{
    //    RingCounter counter{ 10 };
    //    std::cout << "PD(8) = " << counter.PD(8) << std::endl;
    //    std::cout << "PD(17) = " << counter.PD(17) << std::endl;
    //    std::cout << "PD(271) = " << counter.PD(271) << std::endl;
    //    auto seq = counter.get_sequence(12);
    //    for (const auto& x : seq)
    //        std::cout << x << "\t";
    //    std::cout << std::endl;
    //}

    {
        RingCounter counter{ 25000 };
        auto seq = counter.get_sequence(2000);
        //for (size_t i = 0; i < seq.size(); ++i)
        //    std::cout << i << "\t" << seq[i] << std::endl;
//        std::cout << seq.size() << std::endl;
        //for (const auto& x : seq)
        //    std::cout << x << "\t";
        //std::cout << std::endl;
    }
}
