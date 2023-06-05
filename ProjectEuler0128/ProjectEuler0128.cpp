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
#include <cmath>
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
        double value = std::sqrt(x / 3.0 - 5.0 / 12.0) + 0.5;
        return static_cast<uint64_t>(value);
    }


    uint64_t get_lo(uint64_t ring) {
        return 3 * ring * (ring - 1) + 2;
    }


    uint64_t get_hi(uint64_t ring) {
        return 3 * ring * (ring + 1) + 1;
    }


    std::vector<uint64_t> get_sequence(uint64_t length) {
        std::vector<uint64_t> ret;

        // Special case for first ring
        // PD(2) = 3  diffs = 6, 17, 5, 1, 1, 7
        // PD(3) = 2  diffs = 6, 1, 2, 1, 8, 7
        // PD(4) = 2  diffs = 1, 3, 1, 9, 8, 7
        // PD(5) = 0  diffs = 4, 1, 10, 9, 8, 1
        // PD(6) = 2  diffs = 1, 11, 10, 9, 1, 5
        // PD(7) = 2  diffs = 12, 11, 10, 1, 6, 5
        ret.push_back(1);
        std::cout << "1\t0\t1" << std::endl;
        ret.push_back(2);
        std::cout << "2\t1\t2" << std::endl;

        // Process each ring separately
        uint64_t x{ 8 };
        for (uint64_t n = 2; n < num_rings_; ++n) {
            uint64_t six_n = 6 * n;

            // Start with the vertex that starts this ring
            {
                // The neighbor on ring n-1 has a difference of 6*(n-1), which is even
                // The neighbors on ring n are (1, 6*n-1)
                // The neighbors on ring n+1 are (6*n, 6*n+1, 6*n+6*(n+1)-1)
                //   Skip the 6*n number, since it is even.
                if ((primes_.find(six_n - 1) != primes_.end()) &&
                    (primes_.find(six_n + 1) != primes_.end()) &&
                    (primes_.find(2 * six_n + 5) != primes_.end())) {
                    x = get_lo(n);
                    ret.push_back(x);
                    std::cout << ret.size() << "\t" << n << "\t" << x << std::endl;
                }
            }

            // None of the other 5 vertices can be in the sequence:
            //  - the neighbor on ring n-1 has a difference of 6*(n-1) + side
            //  - the neighbors on ring n are both 1
            //  - the neighbors on ring n+1 are (6*n + side - 1, 6*n + side, 6*n + side + 1)
            // If side is even, then there are only 2 odd differences > 1
            // If side is odd, then there are only 2 odd differences > 1

            // Look at the tiles along the edge:
            // The only edge tile that can possibly be in the sequence is the last
            // tile in a ring.  Consider the diffs to the neighboring tiles:
            //  - the neighbors on ring n-1 are of (6*(n-1) + side + 1, 6*(n-1) + side)
            //  - the neighbors on ring n are both 1, unless this is the last tile in the ring
            //  - the neighbors on ring n+1 are (6*n + side, 6*n + side + 1)
            // For edge tiles on all sides, you will have adjacent tiles in the adjacent rings,
            // so one even and one odd difference.  This means at most 2 of those neighbors can
            // have prime differences.  For edge tiles on the first 5 sides, the neighbors in
            // the same ring have a difference of 1.  The same holds for all but the last tile
            // on the 6th side.  So PD(n) <= 2 for all these edge tiles.
            // The lone exception to this rule is the last tile in the ring, where the neighbors
            // on the same ring jump over the discontinuity, and you have the possibility of
            // a third prime difference 6*n - 1.
            {
                // The neighbors on ring n-1 are of (6*n, 6*n+6*(n-1)-1)
                //   6*n is even, so skip
                // The neighbors on ring n are (1, 6*n - 1)
                // The neighbors on ring n+1 are (6*n + 5, 6*n + 5 + 1)
                //   6*n + 6 is even, so skip
                if ((primes_.find(2 * six_n - 7) != primes_.end()) &&
                    (primes_.find(six_n - 1) != primes_.end()) &&
                    (primes_.find(six_n + 5) != primes_.end())) {
                    x = get_hi(n);
                    ret.push_back(x);
                    std::cout << ret.size() << "\t" << n << "\t" << x << std::endl;
                }
            }

            if (ret.size() > length)
                break;
        }


        return ret;
    }


private :
    uint64_t num_rings_;
    PrimeHelper helper_;
    std::unordered_set<uint64_t> primes_;

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

//    {
//        RingCounter counter{ 11 };
////        std::cout << "PD(8) = " << counter.PD(8) << std::endl;
////        std::cout << "PD(17) = " << counter.PD(17) << std::endl;
////        std::cout << "PD(271) = " << counter.PD(271) << std::endl;
//        auto seq = counter.get_sequence(12);
//        for (const auto& x : seq)
//            std::cout << x << "\t";
//        std::cout << std::endl;
//    }
//return 0;
    {
        RingCounter counter{ 75000 };
        auto seq = counter.get_sequence(2000);
        //for (size_t i = 0; i < seq.size(); ++i)
        //    std::cout << i << "\t" << seq[i] << std::endl;
//        std::cout << seq.size() << std::endl;
        //for (const auto& x : seq)
        //    std::cout << x << "\t";
        //std::cout << std::endl;
    }
}
