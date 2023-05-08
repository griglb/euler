// A row measuring seven units in length has red blocks with a minimum length of
// three units placed on it, such that any two red blocks (which are allowed to be
// different lengths) are separated by at least one grey square.
// There are exactly seventeen ways of doing this.
//      O O O O O O O       X X X O O O O
//      O X X X O O O       O O X X X O O
//      O O O X X X O       O O O O X X X
//      X X X O X X X       X X X X O O O
//      O X X X X O O       O O X X X X O
//      O O O X X X X       X X X X X O O
//      O X X X X X O       O O X X X X X
//      X X X X X X O       O X X X X X X
//      X X X X X X X
//
// How many ways can a row measuring fifty units in length be filled?
//
// NOTE : Although the example above does not lend itself to the possibility,
//        in general it is permitted to mix block sizes.For example, on a row
//        measuring eight units in length you could use red(3), grey(1), and red(4).


#include <cstdint>
#include <iostream>
#include <map>
#include <tuple>


// Length 3 - 2 options
//      OOO         XXX
// Length 4 - 4 options
//      OOOO
//      OXXX        XXXO
//      XXXX
// Length 5 - 7 options 
//      OOOOO
//      OOXXX       OXXXO       XXXOO
//      OXXXX       XXXXO
//      XXXXX
// Length 6 - 11 options
//      OOOOOO
//      OOOXXX      OOXXXO      OXXXOO      XXXOOO
//      OOXXXX      OXXXXO      XXXXOO
//      OXXXXX      XXXXXO
//      XXXXXX
// Length 7 - 17 options
//      OOOOOOO
//      OOOOXXX     OOOXXXO     OOXXXOO     OXXXOOO     XXXOOOO
//      OOOXXXX     OOXXXXO     OXXXXOO     XXXXOOO
//      OOXXXXX     OXXXXXO     XXXXXOO
//      OXXXXXX     XXXXXXO
//      XXXXXXX
//      XXXOXXX  - 1 for (3,3)
// Length 8 - 27 options
//      OOOOOOOO
//      OOOOOXXX    OOOOXXXO    OOOXXXOO    OOXXXOOO    OXXXOOOO    XXXOOOOO
//      OOOOXXXX    OOOXXXXO    OOXXXXOO    OXXXXOOO    XXXXOOOO
//      OOOXXXXX    OOXXXXXO    OXXXXXOO    XXXXXOOO
//      OOXXXXXX    OXXXXXXO    XXXXXXOO
//      OXXXXXXX    XXXXXXXO
//      XXXXXXXX
//      OXXXOXXX
//      XXXOOXXX    XXXOXXXO  - 3 for (3,3)
//      XXXOXXXX  - 1 for (3,4)
//      XXXXOXXX  - 1 for (4,3)
// Length 9 - 44 options
//      000000000
//      28 options with 1 red block
//      OOXXXOXXX
//      OXXXOOXXX       OXXXOXXXO
//      XXXOOOXXX       XXXOOXXXO       XXXOXXXOO  - 6 for (3,3)
//      OXXXOXXXX
//      XXXOOXXXX       XXXOXXXXO  - 3 for (3,4)
//      XXXOXXXXX  - 1 for (3,5)
//      OXXXXOXXX
//      XXXXOOXXX       XXXXOXXXO  - 3 for (4,3)
//      XXXXOXXXX  - 1 for (4,4)
//      XXXXXOXXX  - 1 for (5,3)
// Length 10 - 
//      OOOOOOOOOO
//      36 options with 1 red block
//      OOOXXXOXXX
//      OOXXXOOXXX      OOXXXOXXXO
//      OXXXOOOXXX      OXXXOOXXXO      OXXXOXXXOO
//      XXXOOOOXXX      XXXOOOXXXO      XXXOOXXXOO      XXXOXXXOOO  -  10 for (3,3)
//      OOXXXOXXXX
//      OXXXOOXXXX      OXXXOXXXXO
//      XXXOOOXXXX      XXXOOXXXXO      XXXOXXXXOO  - 6 for (3,4)
//      OXXXOXXXXX
//      XXXOOXXXXX      XXXOXXXXXO  - 3 for (3,5)
//      XXXOXXXXXX  - 1 for (3,6)
//      OOXXXXOXXX
//      OXXXXOOXXX      OXXXXOXXXO
//      XXXXOOOXXX      XXXXOOXXXO      XXXXOXXXOO - 6 for (4,3)
//      OXXXXOXXXX
//      XXXXOOXXXX      XXXXOXXXXO  - 3 for (4,4)
//      XXXXOXXXXX  - 1 for (4,5)
//      OXXXXXOXXX
//      XXXXXOOXXX      XXXXXOXXXO  - 3 for (5,3)
//      XXXXXOXXXX  - 1 for (5,4)
//      XXXXXXOXXX  - 1 for (6,3)
// Look at patterns of 3 blocks
// Length 11
//      XXXOXXXOXXX  - 1 for (3,3,3)  - L=7 (3,3)
// Length 12
//      OXXXOXXXOXXX  - 1 for (3,3,3) starting OXXXO - L=7 (3,3)
//      XXXOOXXXOXXX
//      XXXOXXXOOXXX    XXXOXXXOXXXO  - 3 for (3,3,3) starting XXXO - L=8 (3,3)
//      XXXXOXXXOXXX  - 1 for (4,3,3) - L=7 (3,3)
//      XXXOXXXXOXXX  - 1 for (3,4,3) - L=8 (4,3)
//      XXXOXXXOXXXX  - 1 for (3,3,4) - L=8 (3,4)
// Length 13
//      OOXXXOXXXOXXX  - 1 for (3,3,3) starting OOXXXO - L=7 (3,3)
//      OXXXOOXXXOXXX
//      OXXXOXXXOOXXX   OXXXOXXXOXXXO  - 3 for (3,3,3) starting OXXXO - L=8 (3,3)
//      XXXOOOXXXOXXX
//      XXXOOXXXOOXXX   XXXOOXXXOXXXO
//      XXXOXXXOOOXXX   XXXOXXXOOXXXO   XXXOXXXOXXXOO  - 6 for (3,3,3) starting XXXO - L=9 (3,3)
//      OXXXXOXXXOXXX  - 1 for (4,3,3) starting OXXXXO - L=7 (3,3)
//      XXXXOOXXXOXXX
//      XXXXOXXXOOXXX   XXXXOXXXOXXXO  - 3 for (4,3,3) starting XXXXO - L=8 (3,3)
//      OXXXOXXXXOXXX  - 1 for (3,4,3) starting OXXXO - L=8 (4,3)
//      XXXOOXXXXOXXX
//      XXXOXXXXOOXXX   XXXOXXXXOXXXO  - 3 for (3,4,4) starting XXXO - L=9 (4,3)
//      OXXXOXXXOXXXX  - 1 for (3,3,4) starting OXXXO - L=8 (3,4)
//      XXXOOXXXOXXXX
//      XXXOXXXOOXXXX   XXXOXXXOXXXXO  - 3 for (3,3,4) starting XXXO - L=9 (3,4)
//      XXXXOXXXXOXXX  - 1 for (4,4,3) - L=8 (4,3)
//      XXXXOXXXOXXXX  - 1 for (4,3,4) - L=8 (3,4)
//      XXXOXXXXOXXXX  - 1 for (3,4,4) - L=9 (4,4)
//      XXXXXOXXXOXXX  - 1 for (5,3,3) - L=7 (3,3)
//      XXXOXXXXXOXXX  - 1 for (3,5,3) - L=9 (5,3)
//      XXXOXXXOXXXXX  - 1 for (3,3,5) - L=9 (3,5)

// For Length L>2:
//  - 0 red blocks: 1 - all Os
//  - 1 red block:  (L-2)'th triangle number = (L-2)(L-1)/2
// For L>6:
//  - 2 red blocks:  sum of triangle numbers - sigma n=1..L-6 of (L-5-n)*n'th triangle number = (L-5-n)*n*(n+1)/2
// For L>11:
//  - 3 red blocks:
//
// For L = 50:
//  - 12 red blocks:


// Memoize as
//   map<length, tuple of block lengths>
using OneBlock = uint16_t;
using TwoBlocks = std::tuple<uint16_t, uint16_t>;
using ThreeBlocks = std::tuple<uint16_t, uint16_t, uint16_t>;
using FourBlocks = std::tuple<uint16_t, uint16_t, uint16_t, uint16_t>;
using FiveBlocks = std::tuple<uint16_t, uint16_t, uint16_t, uint16_t, uint16_t>;
using SixBlocks = std::tuple<uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t>;
using SevenBlocks = std::tuple<uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t>;
using EightBlocks = std::tuple<uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t>;
using NineBlocks = std::tuple<uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t>;
using TenBlocks = std::tuple<uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t>;
using ElevenBlocks = std::tuple<uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t>;
using TwelveBlocks = std::tuple<uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t>;

using ZeroBlockCounts = uint64_t;
using OneBlockCounts = std::map<OneBlock, uint64_t>;
using TwoBlockCounts = std::map<TwoBlocks, uint64_t>;
using ThreeBlockCounts = std::map<ThreeBlocks, uint64_t>;
using FourBlockCounts = std::map<FourBlocks, uint64_t>;
using FiveBlockCounts = std::map<FiveBlocks, uint64_t>;
using SixBlockCounts = std::map<SixBlocks, uint64_t>;
using SevenBlockCounts = std::map<SevenBlocks, uint64_t>;
using EightBlockCounts = std::map<EightBlocks, uint64_t>;
using NineBlockCounts = std::map<NineBlocks, uint64_t>;
using TenBlockCounts = std::map<TenBlocks, uint64_t>;
using ElevenBlockCounts = std::map<ElevenBlocks, uint64_t>;
using TwelveBlockCounts = std::map<TwelveBlocks, uint64_t>;

using AllBlockCounts = std::tuple<ZeroBlockCounts, OneBlockCounts, TwoBlockCounts, ThreeBlockCounts,
                                  FourBlockCounts, FiveBlockCounts, SixBlockCounts, SevenBlockCounts,
                                  EightBlockCounts, NineBlockCounts, TenBlockCounts, ElevenBlockCounts,
                                  TwelveBlockCounts >;
using LengthCounts = std::map<uint16_t, AllBlockCounts>;

LengthCounts g_by_length_counts;


template < std::size_t... Ns, typename... Ts >
auto tail_impl(std::index_sequence<Ns...>, std::tuple<Ts...> t) {
    return  std::make_tuple(std::get<Ns + 1u>(t)...);
}


template < typename... Ts >
auto tail(std::tuple<Ts...> t) {
    return  tail_impl(std::make_index_sequence<sizeof...(Ts) - 1u>(), t);
}


void calculate_counts_for_length(uint16_t row_len) {
    if (3 > row_len)
        return;

    // For N blocks, you need at least legnth L = 3N + N-1 = 4N - 1
    // So for length L, the max number of blocks is (L + 1) / 4.
    uint16_t max_blocks = (row_len + 1) / 4;

    // Create new tuple of counts by number of blocks.
    AllBlockCounts counts;
    // Can always have the single 0 block solution, all Os
    std::get<0>(counts) = 1;
    // Can always have 1 block solutions
    for (uint16_t block_len = 3; block_len <= row_len; ++block_len)
        std::get<1>(counts)[block_len] = row_len - block_len + 1;

    if (max_blocks >= 2) {
        uint16_t max_block_len = row_len - 4;  // Leave room for a 3 unit block and 1 space
        for (uint16_t b1 = 3; b1 <= max_block_len; ++b1) {
            for (uint16_t b2 = 3; b2 <= max_block_len; ++b2) {
                // Special case for 2 blocks - use triangle numbers
                int16_t num_spaces = row_len - b1 - b2;
                if (num_spaces <= 0)
                    continue;
                std::get<2>(counts)[{b1, b2}] = num_spaces * (num_spaces + 1) / 2;
            }
        }
    }

    if (max_blocks >= 3) {
        uint16_t max_block_len = row_len - 8;  // Leave room for a 2x 3 unit block and 2 spaces
        for (uint16_t b1 = 3; b1 <= max_block_len; ++b1) {
            for (uint16_t b2 = 3; b2 <= max_block_len; ++b2) {
                if (row_len - b1 - b2 < 3)
                    continue;
                for (uint16_t b3 = 3; b3 <= max_block_len; ++b3) {
                    int16_t num_spaces = row_len - b1 - b2 - b3;
                    if (num_spaces < 2)
                        continue;
                    uint64_t sum{ 0 };
                    for (uint16_t head_len = b1 + 1; head_len <= max_block_len + 1; ++head_len) {
                        uint16_t tail_len = row_len - head_len;
                        if (b2 + b3 + 1 > tail_len)
                            continue;
                        const auto &lower = std::get<2>(g_by_length_counts.at(tail_len));
                        sum += lower.at({ b2, b3 });
                    }
                    std::get<3>(counts)[{b1, b2, b3}] = sum;
                }
            }
        }
    }

    if (max_blocks >= 4) {
        uint16_t max_block_len = row_len - 12;  // Leave room for a 3x 3 unit block and 3 spaces
        for (uint16_t b1 = 3; b1 <= max_block_len; ++b1) {
            for (uint16_t b2 = 3; b2 <= max_block_len; ++b2) {
                if (row_len - b1 - b2 < 7)
                    continue;
                for (uint16_t b3 = 3; b3 <= max_block_len; ++b3) {
                    if (row_len - b1 - b2 - b3 < 3)
                        continue;
                    for (uint16_t b4 = 3; b4 <= max_block_len; ++b4) {
                        int16_t num_spaces = row_len - b1 - b2 - b3 - b4;
                        if (num_spaces < 3)
                            continue;
                        uint64_t sum{ 0 };
                        for (uint16_t head_len = b1 + 1; head_len <= max_block_len + 1; ++head_len) {
                            uint16_t tail_len = row_len - head_len;
                            if (b2 + b3 + b4 + 2 > tail_len)
                                continue;
                            const auto &lower = std::get<3>(g_by_length_counts.at(tail_len));
                            sum += lower.at({ b2, b3, b4 });
                        }
                        std::get<4>(counts)[{b1, b2, b3, b4}] = sum;
                    }
                }
            }
        }
    }

    if (max_blocks >= 5) {
        uint16_t max_block_len = row_len - 16;  // Leave room for a 4x 3 unit block and 4 spaces
        for (uint16_t b1 = 3; b1 <= max_block_len; ++b1) {
            for (uint16_t b2 = 3; b2 <= max_block_len; ++b2) {
                if (row_len - b1 - b2 < 11)
                    continue;
                for (uint16_t b3 = 3; b3 <= max_block_len; ++b3) {
                    if (row_len - b1 - b2 - b3 < 7)
                        continue;
                    for (uint16_t b4 = 3; b4 <= max_block_len; ++b4) {
                        if (row_len - b1 - b2 - b3 - b4 < 3)
                            continue;
                        for (uint16_t b5 = 3; b5 <= max_block_len; ++b5) {
                            int16_t num_spaces = row_len - b1 - b2 - b3 - b4 - b5;
                            if (num_spaces < 4)
                                continue;
                            uint64_t sum{ 0 };
                            for (uint16_t head_len = b1 + 1; head_len <= max_block_len + 1; ++head_len) {
                                uint16_t tail_len = row_len - head_len;
                                if (b2 + b3 + b4 + b5 + 3 > tail_len)
                                    continue;
                                const auto &lower = std::get<4>(g_by_length_counts.at(tail_len));
                                sum += lower.at({ b2, b3, b4, b5 });
                            }
                            std::get<5>(counts)[{b1, b2, b3, b4, b5}] = sum;
                        }
                    }
                }
            }
        }
    }

    if (max_blocks >= 6) {
        uint16_t max_block_len = row_len - 20;  // Leave room for a 5x 3 unit block and 5 spaces
        for (uint16_t b1 = 3; b1 <= max_block_len; ++b1) {
            for (uint16_t b2 = 3; b2 <= max_block_len; ++b2) {
                if (row_len - b1 - b2 < 15)
                    continue;
                for (uint16_t b3 = 3; b3 <= max_block_len; ++b3) {
                    if (row_len - b1 - b2 - b3 < 11)
                        continue;
                    for (uint16_t b4 = 3; b4 <= max_block_len; ++b4) {
                        if (row_len - b1 - b2 - b3 - b4 < 7)
                            continue;
                        for (uint16_t b5 = 3; b5 <= max_block_len; ++b5) {
                            if (row_len - b1 - b2 - b3 - b4 - b5 < 3)
                                continue;
                            for (uint16_t b6 = 3; b6 <= max_block_len; ++b6) {
                                int16_t num_spaces = row_len - b1 - b2 - b3 - b4 - b5 - b6;
                                if (num_spaces < 5)
                                    continue;
                                uint64_t sum{ 0 };
                                for (uint16_t head_len = b1 + 1; head_len <= max_block_len + 1; ++head_len) {
                                    uint16_t tail_len = row_len - head_len;
                                    if (b2 + b3 + b4 + b5 + b6 + 4 > tail_len)
                                        continue;
                                    const auto &lower = std::get<5>(g_by_length_counts.at(tail_len));
                                    sum += lower.at({ b2, b3, b4, b5, b6 });
                                }
                                std::get<6>(counts)[{b1, b2, b3, b4, b5, b6}] = sum;
                            }
                        }
                    }
                }
            }
        }
    }

    if (max_blocks >= 7) {
        uint16_t max_block_len = row_len - 24;  // Leave room for a 6x 3 unit block and 6 spaces
        for (uint16_t b1 = 3; b1 <= max_block_len; ++b1) {
            for (uint16_t b2 = 3; b2 <= max_block_len; ++b2) {
                if (row_len - b1 - b2 < 19)
                    continue;
                for (uint16_t b3 = 3; b3 <= max_block_len; ++b3) {
                    if (row_len - b1 - b2 - b3 < 15)
                        continue;
                    for (uint16_t b4 = 3; b4 <= max_block_len; ++b4) {
                        if (row_len - b1 - b2 - b3 - b4 < 11)
                            continue;
                        for (uint16_t b5 = 3; b5 <= max_block_len; ++b5) {
                            if (row_len - b1 - b2 - b3 - b4 - b5 < 7)
                                continue;
                            for (uint16_t b6 = 3; b6 <= max_block_len; ++b6) {
                                if (row_len - b1 - b2 - b3 - b4 - b5 - b6 < 3)
                                    continue;
                                for (uint16_t b7 = 3; b7 <= max_block_len; ++b7) {
                                    int16_t num_spaces = row_len - b1 - b2 - b3 - b4 - b5 - b6 - b7;
                                    if (num_spaces < 6)
                                        continue;
                                    uint64_t sum{ 0 };
                                    for (uint16_t head_len = b1 + 1; head_len <= max_block_len + 1; ++head_len) {
                                        uint16_t tail_len = row_len - head_len;
                                        if (b2 + b3 + b4 + b5 + b6 + b7 + 5 > tail_len)
                                            continue;
                                        const auto &lower = std::get<6>(g_by_length_counts.at(tail_len));
                                        sum += lower.at({ b2, b3, b4, b5, b6, b7 });
                                    }
                                    std::get<7>(counts)[{b1, b2, b3, b4, b5, b6, b7}] = sum;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (max_blocks >= 8) {
        uint16_t max_block_len = row_len - 28;  // Leave room for a 7x 3 unit block and 7 spaces
        for (uint16_t b1 = 3; b1 <= max_block_len; ++b1) {
            for (uint16_t b2 = 3; b2 <= max_block_len; ++b2) {
                if (row_len - b1 - b2 < 23)
                    continue;
                for (uint16_t b3 = 3; b3 <= max_block_len; ++b3) {
                    if (row_len - b1 - b2 - b3 < 19)
                        continue;
                    for (uint16_t b4 = 3; b4 <= max_block_len; ++b4) {
                        if (row_len - b1 - b2 - b3 - b4 < 15)
                            continue;
                        for (uint16_t b5 = 3; b5 <= max_block_len; ++b5) {
                            if (row_len - b1 - b2 - b3 - b4 - b5 < 11)
                                continue;
                            for (uint16_t b6 = 3; b6 <= max_block_len; ++b6) {
                                if (row_len - b1 - b2 - b3 - b4 - b5 - b6 < 7)
                                    continue;
                                for (uint16_t b7 = 3; b7 <= max_block_len; ++b7) {
                                    if (row_len - b1 - b2 - b3 - b4 - b5 - b6 - b7 < 3)
                                        continue;
                                    for (uint16_t b8 = 3; b8 <= max_block_len; ++b8) {
                                        int16_t num_spaces = row_len - b1 - b2 - b3 - b4 - b5 - b6 - b7 - b8;
                                        if (num_spaces < 7)
                                            continue;
                                        uint64_t sum{ 0 };
                                        for (uint16_t head_len = b1 + 1; head_len <= max_block_len + 1; ++head_len) {
                                            uint16_t tail_len = row_len - head_len;
                                            if (b2 + b3 + b4 + b5 + b6 + b7 + b8 + 6 > tail_len)
                                                continue;
                                            const auto &lower = std::get<7>(g_by_length_counts.at(tail_len));
                                            sum += lower.at({ b2, b3, b4, b5, b6, b7, b8 });
                                        }
                                        std::get<8>(counts)[{b1, b2, b3, b4, b5, b6, b7, b8}] = sum;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (max_blocks >= 9) {
        uint16_t max_block_len = row_len - 32;  // Leave room for a 8x 3 unit block and 8 spaces
        for (uint16_t b1 = 3; b1 <= max_block_len; ++b1) {
            for (uint16_t b2 = 3; b2 <= max_block_len; ++b2) {
                if (row_len - b1 - b2 < 27)
                    continue;
                for (uint16_t b3 = 3; b3 <= max_block_len; ++b3) {
                    if (row_len - b1 - b2 - b3 < 23)
                        continue;
                    for (uint16_t b4 = 3; b4 <= max_block_len; ++b4) {
                        if (row_len - b1 - b2 - b3 - b4 < 19)
                            continue;
                        for (uint16_t b5 = 3; b5 <= max_block_len; ++b5) {
                            if (row_len - b1 - b2 - b3 - b4 - b5 < 15)
                                continue;
                            for (uint16_t b6 = 3; b6 <= max_block_len; ++b6) {
                                if (row_len - b1 - b2 - b3 - b4 - b5 - b6 < 11)
                                    continue;
                                for (uint16_t b7 = 3; b7 <= max_block_len; ++b7) {
                                    if (row_len - b1 - b2 - b3 - b4 - b5 - b6 - b7 < 7)
                                        continue;
                                    for (uint16_t b8 = 3; b8 <= max_block_len; ++b8) {
                                        if (row_len - b1 - b2 - b3 - b4 - b5 - b6 - b7 - b8 < 3)
                                            continue;
                                        for (uint16_t b9 = 3; b9 <= max_block_len; ++b9) {
                                            int16_t num_spaces = row_len - b1 - b2 - b3 - b4 - b5 - b6 - b7 - b8 - b9;
                                            if (num_spaces < 8)
                                                continue;
                                            uint64_t sum{ 0 };
                                            for (uint16_t head_len = b1 + 1; head_len <= max_block_len + 1; ++head_len) {
                                                uint16_t tail_len = row_len - head_len;
                                                if (b2 + b3 + b4 + b5 + b6 + b7 + b8 + b9 + 7 > tail_len)
                                                    continue;
                                                const auto &lower = std::get<8>(g_by_length_counts.at(tail_len));
                                                sum += lower.at({ b2, b3, b4, b5, b6, b7, b8, b9 });
                                            }
                                            std::get<9>(counts)[{b1, b2, b3, b4, b5, b6, b7, b8, b9}] = sum;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (max_blocks >= 10) {
        uint16_t max_block_len = row_len - 36;  // Leave room for a 9x 3 unit block and 9 spaces
        for (uint16_t b1 = 3; b1 <= max_block_len; ++b1) {
            for (uint16_t b2 = 3; b2 <= max_block_len; ++b2) {
                if (row_len - b1 - b2 < 31)
                    continue;
                for (uint16_t b3 = 3; b3 <= max_block_len; ++b3) {
                    if (row_len - b1 - b2 - b3 < 27)
                        continue;
                    for (uint16_t b4 = 3; b4 <= max_block_len; ++b4) {
                        if (row_len - b1 - b2 - b3 - b4 < 23)
                            continue;
                        for (uint16_t b5 = 3; b5 <= max_block_len; ++b5) {
                            if (row_len - b1 - b2 - b3 - b4 - b5 < 19)
                                continue;
                            for (uint16_t b6 = 3; b6 <= max_block_len; ++b6) {
                                if (row_len - b1 - b2 - b3 - b4 - b5 - b6 < 15)
                                    continue;
                                for (uint16_t b7 = 3; b7 <= max_block_len; ++b7) {
                                    if (row_len - b1 - b2 - b3 - b4 - b5 - b6 - b7 < 11)
                                        continue;
                                    for (uint16_t b8 = 3; b8 <= max_block_len; ++b8) {
                                        if (row_len - b1 - b2 - b3 - b4 - b5 - b6 - b7 - b8 < 7)
                                            continue;
                                        for (uint16_t b9 = 3; b9 <= max_block_len; ++b9) {
                                            if (row_len - b1 - b2 - b3 - b4 - b5 - b6 - b7 - b8 - b9 < 3)
                                                continue;
                                            for (uint16_t b10 = 3; b10 <= max_block_len; ++b10) {
                                                int16_t num_spaces = row_len - b1 - b2 - b3 - b4 - b5 - b6 - b7 - b8 - b9 - b10;
                                                if (num_spaces < 9)
                                                    continue;
                                                uint64_t sum{ 0 };
                                                for (uint16_t head_len = b1 + 1; head_len <= max_block_len + 1; ++head_len) {
                                                    uint16_t tail_len = row_len - head_len;
                                                    if (b2 + b3 + b4 + b5 + b6 + b7 + b8 + b9 + b10 + 8 > tail_len)
                                                        continue;
                                                    const auto &lower = std::get<9>(g_by_length_counts.at(tail_len));
                                                    sum += lower.at({ b2, b3, b4, b5, b6, b7, b8, b9, b10 });
                                                }
                                                std::get<10>(counts)[{b1, b2, b3, b4, b5, b6, b7, b8, b9, b10}] = sum;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (max_blocks >= 11) {
        uint16_t max_block_len = row_len - 40;  // Leave room for a 10x 3 unit block and 10 spaces
        for (uint16_t b1 = 3; b1 <= max_block_len; ++b1) {
            for (uint16_t b2 = 3; b2 <= max_block_len; ++b2) {
                if (row_len - b1 - b2 < 35)
                    continue;
                for (uint16_t b3 = 3; b3 <= max_block_len; ++b3) {
                    if (row_len - b1 - b2 - b3 < 31)
                        continue;
                    for (uint16_t b4 = 3; b4 <= max_block_len; ++b4) {
                        if (row_len - b1 - b2 - b3 - b4 < 27)
                            continue;
                        for (uint16_t b5 = 3; b5 <= max_block_len; ++b5) {
                            if (row_len - b1 - b2 - b3 - b4 - b5 < 23)
                                continue;
                            for (uint16_t b6 = 3; b6 <= max_block_len; ++b6) {
                                if (row_len - b1 - b2 - b3 - b4 - b5 - b6 < 19)
                                    continue;
                                for (uint16_t b7 = 3; b7 <= max_block_len; ++b7) {
                                    if (row_len - b1 - b2 - b3 - b4 - b5 - b6 - b7 < 15)
                                        continue;
                                    for (uint16_t b8 = 3; b8 <= max_block_len; ++b8) {
                                        if (row_len - b1 - b2 - b3 - b4 - b5 - b6 - b7 - b8 < 11)
                                            continue;
                                        for (uint16_t b9 = 3; b9 <= max_block_len; ++b9) {
                                            if (row_len - b1 - b2 - b3 - b4 - b5 - b6 - b7 - b8 - b9 < 7)
                                                continue;
                                            for (uint16_t b10 = 3; b10 <= max_block_len; ++b10) {
                                                if (row_len - b1 - b2 - b3 - b4 - b5 - b6 - b7 - b8 - b9 - b10 < 3)
                                                    continue;
                                                for (uint16_t b11 = 3; b11 <= max_block_len; ++b11) {
                                                    int16_t num_spaces = row_len - b1 - b2 - b3 - b4 - b5 - b6 - b7 - b8 - b9 - b10 - b11;
                                                    if (num_spaces < 10)
                                                        continue;
                                                    uint64_t sum{ 0 };
                                                    for (uint16_t head_len = b1 + 1; head_len <= max_block_len + 1; ++head_len) {
                                                        uint16_t tail_len = row_len - head_len;
                                                        if (b2 + b3 + b4 + b5 + b6 + b7 + b8 + b9 + b10 + b11 + 9 > tail_len)
                                                            continue;
                                                        const auto &lower = std::get<10>(g_by_length_counts.at(tail_len));
                                                        sum += lower.at({ b2, b3, b4, b5, b6, b7, b8, b9, b10, b11 });
                                                    }
                                                    std::get<11>(counts)[{b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11}] = sum;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (max_blocks >= 12) {
        uint16_t max_block_len = row_len - 44;  // Leave room for a 10x 3 unit block and 10 spaces
        for (uint16_t b1 = 3; b1 <= max_block_len; ++b1) {
            for (uint16_t b2 = 3; b2 <= max_block_len; ++b2) {
                if (row_len - b1 - b2 < 39)
                    continue;
                for (uint16_t b3 = 3; b3 <= max_block_len; ++b3) {
                    if (row_len - b1 - b2 - b3 < 35)
                        continue;
                    for (uint16_t b4 = 3; b4 <= max_block_len; ++b4) {
                        if (row_len - b1 - b2 - b3 - b4 < 31)
                            continue;
                        for (uint16_t b5 = 3; b5 <= max_block_len; ++b5) {
                            if (row_len - b1 - b2 - b3 - b4 - b5 < 27)
                                continue;
                            for (uint16_t b6 = 3; b6 <= max_block_len; ++b6) {
                                if (row_len - b1 - b2 - b3 - b4 - b5 - b6 < 23)
                                    continue;
                                for (uint16_t b7 = 3; b7 <= max_block_len; ++b7) {
                                    if (row_len - b1 - b2 - b3 - b4 - b5 - b6 - b7 < 19)
                                        continue;
                                    for (uint16_t b8 = 3; b8 <= max_block_len; ++b8) {
                                        if (row_len - b1 - b2 - b3 - b4 - b5 - b6 - b7 - b8 < 15)
                                            continue;
                                        for (uint16_t b9 = 3; b9 <= max_block_len; ++b9) {
                                            if (row_len - b1 - b2 - b3 - b4 - b5 - b6 - b7 - b8 - b9 < 11)
                                                continue;
                                            for (uint16_t b10 = 3; b10 <= max_block_len; ++b10) {
                                                if (row_len - b1 - b2 - b3 - b4 - b5 - b6 - b7 - b8 - b9 - b10 < 7)
                                                    continue;
                                                for (uint16_t b11 = 3; b11 <= max_block_len; ++b11) {
                                                    if (row_len - b1 - b2 - b3 - b4 - b5 - b6 - b7 - b8 - b9 - b10 - b11 < 3)
                                                        continue;
                                                    for (uint16_t b12 = 3; b12 <= max_block_len; ++b12) {
                                                        int16_t num_spaces = row_len - b1 - b2 - b3 - b4 - b5 - b6 - b7 - b8 - b9 - b10 - b11 - b12;
                                                        if (num_spaces < 11)
                                                            continue;
                                                        uint64_t sum{ 0 };
                                                        for (uint16_t head_len = b1 + 1; head_len <= max_block_len + 1; ++head_len) {
                                                            uint16_t tail_len = row_len - head_len;
                                                            if (b2 + b3 + b4 + b5 + b6 + b7 + b8 + b9 + b10 + b11 + b12 + 10 > tail_len)
                                                                continue;
                                                            const auto &lower = std::get<11>(g_by_length_counts.at(tail_len));
                                                            sum += lower.at({ b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12 });
                                                        }
                                                        std::get<12>(counts)[{b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12}] = sum;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Add all the counts to the global database.
    g_by_length_counts[row_len] = counts;
}


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


void print_counts() {
    for (const auto& [len, block_counts] : g_by_length_counts) {
        std::cout << "row length = " << len << std::endl;
        std::cout << "  0 blocks:" << std::endl;
        std::cout << "    " << std::get<0>(block_counts) << std::endl;
        std::cout << "  1 block:" << std::endl;
        for (const auto &[len, count] : std::get<1>(block_counts))
            std::cout << "    " << len << " : " << count << std::endl;
        if (!std::get<2>(block_counts).empty()) {
            std::cout << "  2 blocks:" << std::endl;
            for (const auto& [tup, count] : std::get<2>(block_counts)) {
                std::cout << "    " << tup << " : " << count << std::endl;
            }
        }
        if (!std::get<3>(block_counts).empty()) {
            std::cout << "  3 blocks:" << std::endl;
            for (const auto& [tup, count] : std::get<3>(block_counts)) {
                std::cout << "    " << tup << " : " << count << std::endl;
            }
        }
        if (!std::get<4>(block_counts).empty()) {
            std::cout << "  4 blocks:" << std::endl;
            for (const auto& [tup, count] : std::get<4>(block_counts)) {
                std::cout << "    " << tup << " : " << count << std::endl;
            }
        }
        if (!std::get<5>(block_counts).empty()) {
            std::cout << "  5 blocks:" << std::endl;
            for (const auto& [tup, count] : std::get<5>(block_counts)) {
                std::cout << "    " << tup << " : " << count << std::endl;
            }
        }
        if (!std::get<6>(block_counts).empty()) {
            std::cout << "  6 blocks:" << std::endl;
            for (const auto& [tup, count] : std::get<6>(block_counts)) {
                std::cout << "    " << tup << " : " << count << std::endl;
            }
        }
        if (!std::get<7>(block_counts).empty()) {
            std::cout << "  7 blocks:" << std::endl;
            for (const auto& [tup, count] : std::get<7>(block_counts)) {
                std::cout << "    " << tup << " : " << count << std::endl;
            }
        }
        if (!std::get<8>(block_counts).empty()) {
            std::cout << "  8 blocks:" << std::endl;
            for (const auto& [tup, count] : std::get<8>(block_counts)) {
                std::cout << "    " << tup << " : " << count << std::endl;
            }
        }
        if (!std::get<9>(block_counts).empty()) {
            std::cout << "  9 blocks:" << std::endl;
            for (const auto& [tup, count] : std::get<9>(block_counts)) {
                std::cout << "    " << tup << " : " << count << std::endl;
            }
        }
        if (!std::get<10>(block_counts).empty()) {
            std::cout << "  10 blocks:" << std::endl;
            for (const auto& [tup, count] : std::get<10>(block_counts)) {
                std::cout << "    " << tup << " : " << count << std::endl;
            }
        }
        if (!std::get<11>(block_counts).empty()) {
            std::cout << "  11 blocks:" << std::endl;
            for (const auto& [tup, count] : std::get<11>(block_counts)) {
                std::cout << "    " << tup << " : " << count << std::endl;
            }
        }
        if (!std::get<12>(block_counts).empty()) {
            std::cout << "  12 blocks:" << std::endl;
            for (const auto& [tup, count] : std::get<12>(block_counts)) {
                std::cout << "    " << tup << " : " << count << std::endl;
            }
        }
    }
}


uint64_t solve_for(uint16_t row_len) {
    for (uint16_t len = 3; len <= row_len; ++len)
        calculate_counts_for_length(len);
//    print_counts();

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

    return sum;
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    for (int i = 0; i < 10; ++i)
    //        std::cout << i << "\t" << permutations_of_size(i) << std::endl;
    //}

    //{
    //    calculate_counts_for_length(3);
    //    calculate_counts_for_length(4);
    //    calculate_counts_for_length(5);
    //    calculate_counts_for_length(6);
    //    calculate_counts_for_length(7);
    //    calculate_counts_for_length(8);
    //    calculate_counts_for_length(9);
    //    calculate_counts_for_length(10);
    //    calculate_counts_for_length(11);
    //    calculate_counts_for_length(12);
    //    calculate_counts_for_length(13);
    //    calculate_counts_for_length(14);
    //    calculate_counts_for_length(15);
    //    calculate_counts_for_length(16);
    //    print_counts();
    //}

    auto total = solve_for(50);
    std::cout << "total = " << total << std::endl;
}
