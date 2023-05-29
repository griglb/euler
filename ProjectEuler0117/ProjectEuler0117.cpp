// Using a combination of grey square tiles and oblong tiles chosen from :
//   red tiles (measuring two units),
//   green tiles (measuring three units), and
//   blue tiles (measuring four units),
// it is possible to tile a row measuring five units in length in exactly fifteen different ways.
//      O O O O O       R R O O O
//      O R R O O       O O R R O
//      O O O R R       R R|R R O
//      R R O R R       O R R|R R
//      G G G O O       O G G G O
//      O O G G G       R R G G G
//      G G G R R       B B B B O
//      O B B B B
//
// How many ways can a row measuring fifty units in length be tiled?
//
// NOTE : This is related to Problem 116.

#include <array>
#include <cstdint>
#include <iostream>
#include <map>
#include <numeric>
#include <tuple>


// Length 0 - 1 total, 1 all blank option
// Length 1 - 1 total, 1 all blank option
// Length 2 - 2 total: 1 red options, 1 all blank option
//      RR  - 1 for L=0 with RR
// Length 3 - 4 total: 2 red options, 1 green option, 1 all blank option
//      RRO  - 1 for L=1 with RR
//      ORR  - 1 for L=0 with ORR
//      GGG  - 1 for L=0 with GGG
// Length 4 - 8 total: 4 red options, 2 green options, 1 blue option, 1 all blank option
//      RROO        RRRR  - 2 for L=2 with RR
//      ORRO  - 1 for L=1 with ORR
//      OORR  - 1 for L=0 with OORR
//      GGGO  - 1 for L=1 with GGG
//      OGGG  - 1 for L=0 with OGGG
//      BBBB  - 1 for L=0 with BBBB
// Length 5 - 15 total: 8 red options, 4 green options, 2 blue options, 1 all blank option
//      RROOO       RRRRO       RRORR       RRGGG  - 4 for L=3 with RR
//      ORROO       ORRRR  - 2 for L=2 with ORR
//      OORRO  - 1 for L=1 with OORR
//      OOORR  - 1 for L=0 with OOORR
//      GGGOO       GGGRR  - 2 for L=2 with GGG
//      OGGGO  - 1 for L=1 with OGGG
//      OOGGG  - 1 for L=0 with OOGGG
//      BBBBO  - 1 for L=1 with BBBB
//      OBBBB  - 1 for L=0 with OBBBB


// Memoize as
constexpr size_t MaxRowLen{ 50 };
using BlockLen = uint16_t;
using BlockCounts = uint64_t;
using LengthCounts = std::array<BlockCounts, MaxRowLen + 1>;

LengthCounts g_by_length_counts;


void print_counts() {
    for (size_t row_len = 1; row_len <= MaxRowLen; ++row_len) {
        if (g_by_length_counts[row_len] == 0)
            continue;
        std::cout << "row length = " << row_len
                  << "\tcount = " << g_by_length_counts[row_len] << std::endl;
    }
}


void calculate_counts_for_length(uint16_t block_len, uint16_t row_len) {
    if (block_len > row_len)
        return;

    // For N blocks of block_len, you need at least length L = N * block_len.
    // We don't need spaces in between the blocks, like in problems 114 & 115,
    // so for length L, the max number of blocks is L / block_len.
    uint16_t max_num_spaces = row_len - block_len;

    uint64_t &count{ g_by_length_counts[row_len] };

    // There is always a single 0 block solution.
    if (0 == count) {
       count = 1;
       std::cout << "added empty solution" << std::endl;
    }

    for (uint16_t num_spaces = 0; num_spaces <= max_num_spaces; ++num_spaces) {
        auto tail_len = row_len - block_len - num_spaces;
        count += g_by_length_counts[tail_len];
        std::cout << row_len << "  " << block_len << "  " << num_spaces << "  " << tail_len << "  " << g_by_length_counts[tail_len] << "  " << count << std::endl;
    }
}


uint64_t solve_for(uint16_t row_len) {
    for (uint16_t len = 2; len <= row_len; ++len) {
        calculate_counts_for_length(2, len);
        calculate_counts_for_length(3, len);
        calculate_counts_for_length(4, len);
    }
    print_counts();

    return std::accumulate(g_by_length_counts.begin(), g_by_length_counts.end(), 0ULL);
}


int main()
{
    std::cout << "Hello World!\n";

    g_by_length_counts[0] = g_by_length_counts[1] = 1;
    {
        auto total = solve_for(50);
    }

}
