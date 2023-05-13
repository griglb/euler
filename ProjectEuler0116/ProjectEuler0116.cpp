// A row of five grey square tiles is to have a number of its tiles replaced with
// coloured oblong tiles chosen from red(length two), green(length three),
// or blue(length four).
//
// If red tiles are chosen there are exactly seven ways this can be done.
//      R R O O O       O R R O O
//      O O R R O       O O O R R
//      R R|R R O       R R O R R
//      O R R|R R
//
// If green tiles are chosen there are three ways.
//      G G G O O       O G G G O
//      O O G G G
//
// And if blue tiles are chosen there are two ways.
//      B B B B O       O B B B B
//
// Assuming that colours cannot be mixed there are 7 + 3 + 2 = 12 ways of
// replacing the grey tiles in a row measuring five units in length.
//
// How many different ways can the grey tiles in a row measuring fifty units in
// length be replaced if colours cannot be mixed and at least one coloured tile
// must be used ?
//
// NOTE : This is related to Problem 117.

#include <cstdint>
#include <iostream>
#include <map>
#include <tuple>


// Length 2 - 1 red options
//      RR  - 2-2+1 = 1 single
// Length 3 - 2 red options, 1 green option
//      RRO         ORR  - 3-2+1 = 2 singles
//      GGG  - 3-3+1 = 1 single
// Length 4 - 4 red options, 2 green options, 1 blue option
//      OORR        ORRO        RROO  - 4-2+1 = 3 singles
//      RRRR  - 1 double for L=2 with RR
//      OGGG        GGGO  - 4-3+1 = 2 singles
//      BBBB  - 4-4+1 = 1 single
// Length 5 - 7 red options, 3 green options, 2 blue options
//      OOORR       OORRO       ORROO       RROOO  - 5-2+1 = 4 singles
//      ORRRR  - 1 double for L=2 with ORR
//      RRORR       RRRRO  - 2 doubles for L=3 with RR
//      OOGGG       OGGGO       GGGOO  - 5-3+1 = 3 singles
//      OBBBB       BBBBO  - 5-4+1 = 2 single
// Length 6 - 12 red options, 5 green options, 3 blue options
//      OOOORR      OOORRO      OORROO      ORROOO      RROOOO  - 6-2+1 = 5 singles
//      OORRRR  - 1 double for L=2 with OORR
//      ORRORR      ORRRRO  - 2 doubles for L=3 with ORR
//      RROORR      RRORRO      RRRROO  - 3 doubles for L=4 with RR
//      RRRRRR  - 1 triple for L=2 with RRRR
//      OOOGGG      OOGGGO      OGGGOO      GGGOOO  - 6-3+1 = 4 singles
//      GGGGGG  - 1 triple for L=3 with GGG
//      OOBBBB      OBBBBO      BBBBOO  - 6-4+1 = 3 singles
// Length 7 - 20 red options, 8 green options, 4 blue options
//      OOOOORR     OOOORRO     OOORROO     OORROOO     ORROOOO     RROOOOO  - 7-2+1 = 6 singles
//      OOORRRR  - 1 double for L=2 with OOORR
//      OORRORR     OORRRRO  - 2 doubles for L=3 with OORR
//      ORROORR     ORRORRO     ORRRROO  - 3 doubles for L=4 with ORR
//      RROOORR     RROORRO     RRORROO     RRRROOO  - 4 doubles for L=5 with RR
//      ORRRRRR  - 1 triple for L=4 with ORR
//      RRORRRR     RRRRORR     RRRRRRO  - 3 triples for L=5 with RR
//      OOOOGGG     OOOGGGO     OOGGGOO     OGGGOOO     GGGOOOO  - 7-3+1 = 5 singles
//      OGGGGGG  - 1 double for L=3 with OGGG
//      GGGOGGG     GGGGGGO  - 2 doubles for L=4 with GGG
//      OOOBBBB     OOBBBBO     OBBBBOO     BBBBOOO  - 7-4+1 = 4 singles

// Memoize as
//   map<length, tuple of block lengths>
using BlockLen = uint16_t;
using ZeroBlockCounts = uint64_t;
using BlockCounts = std::map<BlockLen, uint64_t>;
using AllBlockCounts = std::tuple<ZeroBlockCounts, BlockCounts, BlockCounts, BlockCounts,
                                  BlockCounts, BlockCounts, BlockCounts, BlockCounts,
                                  BlockCounts, BlockCounts, BlockCounts, BlockCounts,
                                  BlockCounts, BlockCounts, BlockCounts, BlockCounts,
                                  BlockCounts, BlockCounts, BlockCounts, BlockCounts,
                                  BlockCounts, BlockCounts, BlockCounts, BlockCounts,
                                  BlockCounts, BlockCounts>;

using RowLen = uint16_t;
using LengthCounts = std::map<RowLen, AllBlockCounts>;

LengthCounts g_by_length_counts;


template <typename TupleT, std::size_t... Is>
std::ostream& printTupleImp(std::ostream& os, const TupleT& tp, std::index_sequence<Is...>) {
    auto printElem = [&os](const auto& x, size_t id) {
        if (id > 0)
            os << ", ";
        os << x;
    };

    (printElem(std::get<Is>(tp), Is), ...);
    return os;
}


template <typename TupleT, std::size_t TupSize = std::tuple_size<TupleT>::value>
std::ostream& operator <<(std::ostream& os, const TupleT& tp) {
    return printTupleImp(os, tp, std::make_index_sequence<TupSize>{});
}


template <uint16_t NumBlocks>
void print_block_counts(const AllBlockCounts &block_counts) {
    if (!std::get<NumBlocks>(block_counts).empty()) {
        std::cout << "  " << NumBlocks << " blocks:" << std::endl;
        for (const auto& [tup, count] : std::get<NumBlocks>(block_counts)) {
            std::cout << "    " << tup << " : " << count << std::endl;
        }
    }
}

void print_counts() {
    for (const auto& [len, block_counts] : g_by_length_counts) {
        std::cout << "row length = " << len << std::endl;
        std::cout << "  0 blocks:" << std::endl;
        std::cout << "    " << std::get<0>(block_counts) << std::endl;
        std::cout << "  1 block:" << std::endl;
        for (const auto& [len, count] : std::get<1>(block_counts))
            std::cout << "    " << len << " : " << count << std::endl;
    
        print_block_counts<2>(block_counts);
        print_block_counts<3>(block_counts);
        print_block_counts<4>(block_counts);
        print_block_counts<5>(block_counts);
        print_block_counts<6>(block_counts);
        print_block_counts<7>(block_counts);
        print_block_counts<8>(block_counts);
        print_block_counts<9>(block_counts);
        print_block_counts<10>(block_counts);
        print_block_counts<11>(block_counts);
        print_block_counts<12>(block_counts);
        print_block_counts<13>(block_counts);
        print_block_counts<14>(block_counts);
        print_block_counts<15>(block_counts);
        print_block_counts<16>(block_counts);
        print_block_counts<17>(block_counts);
        print_block_counts<18>(block_counts);
        print_block_counts<19>(block_counts);
        print_block_counts<20>(block_counts);
        print_block_counts<21>(block_counts);
        print_block_counts<22>(block_counts);
        print_block_counts<23>(block_counts);
        print_block_counts<24>(block_counts);
        print_block_counts<25>(block_counts);
    }
}


template <uint16_t NumBlocks>
void calculate_counts_for_num_blocks(uint16_t block_len, uint16_t row_len, AllBlockCounts &counts) {
    int16_t num_spaces = row_len - NumBlocks * block_len;
    if (num_spaces < 0)
        return;

    uint64_t sum{ 0 };
    uint16_t max_head_len = row_len - (NumBlocks - 1) * block_len;
    for (uint16_t head_len = block_len; head_len <= max_head_len + 1; ++head_len) {
        uint16_t tail_len = row_len - head_len;
        if ((NumBlocks - 1) * block_len > tail_len)
            continue;
        const auto& lower = std::get<NumBlocks - 1>(g_by_length_counts.at(tail_len));
        sum += lower.at(block_len);
    }
    std::get<NumBlocks>(counts)[block_len] = sum;
}


void calculate_counts_for_length(uint16_t block_len, uint16_t row_len) {
    if (block_len > row_len)
        return;

    // For N blocks of block_len, you need at least length L = N * block_len.
    // We don't need spaces in between the blocks, like in problems 114 & 115,
    // so for length L, the max number of blocks is L / block_len.
    uint16_t max_blocks = row_len / block_len;

    // Create new tuple of counts by number of blocks.
    AllBlockCounts counts;
    if (g_by_length_counts.find(row_len) != g_by_length_counts.end())
        counts = g_by_length_counts.at(row_len);

    // This time, 0 block solutions are not allowed, at least one block must be used.
    std::get<0>(counts) = 0;
    // Can always have 1 block solutions, which is number of empty slots plus 1
    std::get<1>(counts)[block_len] = row_len - block_len + 1;

    if (max_blocks >= 2) {
        // Special case for 2 blocks - use triangle numbers
        int16_t num_spaces = row_len - 2 * block_len;
        if (num_spaces >= 0)
            std::get<2>(counts)[block_len] = (num_spaces + 1) * (num_spaces + 2) / 2;
    }

    calculate_counts_for_num_blocks<3>(block_len, row_len, counts);
    calculate_counts_for_num_blocks<4>(block_len, row_len, counts);
    calculate_counts_for_num_blocks<5>(block_len, row_len, counts);
    calculate_counts_for_num_blocks<6>(block_len, row_len, counts);
    calculate_counts_for_num_blocks<7>(block_len, row_len, counts);
    calculate_counts_for_num_blocks<8>(block_len, row_len, counts);
    calculate_counts_for_num_blocks<9>(block_len, row_len, counts);
    calculate_counts_for_num_blocks<10>(block_len, row_len, counts);
    calculate_counts_for_num_blocks<11>(block_len, row_len, counts);
    calculate_counts_for_num_blocks<12>(block_len, row_len, counts);
    calculate_counts_for_num_blocks<13>(block_len, row_len, counts);
    calculate_counts_for_num_blocks<14>(block_len, row_len, counts);
    calculate_counts_for_num_blocks<15>(block_len, row_len, counts);
    calculate_counts_for_num_blocks<16>(block_len, row_len, counts);
    calculate_counts_for_num_blocks<17>(block_len, row_len, counts);
    calculate_counts_for_num_blocks<18>(block_len, row_len, counts);
    calculate_counts_for_num_blocks<19>(block_len, row_len, counts);
    calculate_counts_for_num_blocks<20>(block_len, row_len, counts);
    calculate_counts_for_num_blocks<21>(block_len, row_len, counts);
    calculate_counts_for_num_blocks<22>(block_len, row_len, counts);
    calculate_counts_for_num_blocks<23>(block_len, row_len, counts);
    calculate_counts_for_num_blocks<24>(block_len, row_len, counts);
    calculate_counts_for_num_blocks<25>(block_len, row_len, counts);

    // Add all the counts to the global database.
    g_by_length_counts[row_len] = counts;
}


uint64_t solve_for(uint16_t row_len) {
    for (uint16_t len = 2; len <= row_len; ++len) {
        calculate_counts_for_length(2, len);
        calculate_counts_for_length(3, len);
        calculate_counts_for_length(4, len);
    }
    print_counts();

    uint64_t sum{ 0 };

    const auto& block_counts = g_by_length_counts.at(row_len);
    sum += std::get<0>(block_counts);
    for (const auto& [len, count] : std::get<1>(block_counts))
        sum += count;
    for (const auto& [tup, count] : std::get<2>(block_counts))
        sum += count;
    for (const auto& [tup, count] : std::get<3>(block_counts))
        sum += count;
    for (const auto& [tup, count] : std::get<4>(block_counts))
        sum += count;
    for (const auto& [tup, count] : std::get<5>(block_counts))
        sum += count;
    for (const auto& [tup, count] : std::get<6>(block_counts))
        sum += count;
    for (const auto& [tup, count] : std::get<7>(block_counts))
        sum += count;
    for (const auto& [tup, count] : std::get<8>(block_counts))
        sum += count;
    for (const auto& [tup, count] : std::get<9>(block_counts))
        sum += count;
    for (const auto& [tup, count] : std::get<10>(block_counts))
        sum += count;
    for (const auto& [tup, count] : std::get<11>(block_counts))
        sum += count;
    for (const auto& [tup, count] : std::get<12>(block_counts))
        sum += count;
    for (const auto& [tup, count] : std::get<13>(block_counts))
        sum += count;
    for (const auto& [tup, count] : std::get<14>(block_counts))
        sum += count;
    for (const auto& [tup, count] : std::get<15>(block_counts))
        sum += count;
    for (const auto& [tup, count] : std::get<16>(block_counts))
        sum += count;
    for (const auto& [tup, count] : std::get<17>(block_counts))
        sum += count;
    for (const auto& [tup, count] : std::get<18>(block_counts))
        sum += count;
    for (const auto& [tup, count] : std::get<19>(block_counts))
        sum += count;
    for (const auto& [tup, count] : std::get<20>(block_counts))
        sum += count;
    for (const auto& [tup, count] : std::get<21>(block_counts))
        sum += count;
    for (const auto& [tup, count] : std::get<22>(block_counts))
        sum += count;
    for (const auto& [tup, count] : std::get<23>(block_counts))
        sum += count;
    for (const auto& [tup, count] : std::get<24>(block_counts))
        sum += count;
    for (const auto& [tup, count] : std::get<25>(block_counts))
        sum += count;

    return sum;
}


int main()
{
    std::cout << "Hello World!\n";

    {
        auto total = solve_for(50);
        std::cout << "total = " << total << std::endl;
    }

}
