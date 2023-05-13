// NOTE: This is a more difficult version of Problem 114.
//
// A row measuring n units in length has red blocks with a minimum length of
// m units placed on it, such that any two red blocks (which are allowed to
// be different lengths) are separated by at least one black square.
//
// Let the fill-count function, F(m, n), represent the number of ways that a
// row can be filled.
//
// For example, F(3, 29) = 673135 and F(3, 30) = 1089155.
//
// That is, for m=3, it can be seen that n=30 is the smallest value for which
// the fill-count function first exceeds one million.
//
// In the same way, for m=10, it can be verified that F(10, 56) = 880711 and
// F(10, 57) = 1148904, so n=57 is the least value for which the fill-count
// function first exceeds one million.
//
// For m = 50, find the least value of n for which the fill-count function first exceeds one million.


#include <cstdint>
#include <iostream>
#include <map>
#include <tuple>


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
        for (const auto& [len, count] : std::get<1>(block_counts))
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


void calculate_counts_for_length(uint16_t min_block_len, uint16_t row_len) {
    if (min_block_len > row_len)
        return;

    // For N blocks of length k = min_block_len or longer, you need at least length
    //    L = kN + N-1 = (k+1)N - 1
    // So for length L, the max number of blocks is (L + 1) / 4.
    uint16_t max_blocks = (row_len + 1) / (min_block_len + 1);

    // Create new tuple of counts by number of blocks.
    AllBlockCounts counts;
    // Can always have the single 0 block solution, all Os
    std::get<0>(counts) = 1;
    // Can always have 1 block solutions
    for (uint16_t block_len = min_block_len; block_len <= row_len; ++block_len)
        std::get<1>(counts)[block_len] = row_len - block_len + 1;

    if (max_blocks >= 2) {
        uint16_t max_block_len = row_len - min_block_len - 1;  // Leave room for a min_block_len unit block and 1 space
        for (uint16_t b1 = min_block_len; b1 <= max_block_len; ++b1) {
            for (uint16_t b2 = min_block_len; b2 <= max_block_len; ++b2) {
                // Special case for 2 blocks - use triangle numbers
                int16_t num_spaces = row_len - b1 - b2;
                if (num_spaces <= 0)
                    continue;
                std::get<2>(counts)[{b1, b2}] = num_spaces * (num_spaces + 1) / 2;
            }
        }
    }

    if (max_blocks >= 3) {
        uint16_t max_block_len = row_len - 2 * (min_block_len + 1);  // Leave room for a 2x min_block_len unit block and 2 spaces
        for (uint16_t b1 = min_block_len; b1 <= max_block_len; ++b1) {
            for (uint16_t b2 = min_block_len; b2 <= max_block_len; ++b2) {
                if (row_len - b1 - b2 < min_block_len)
                    continue;
                for (uint16_t b3 = min_block_len; b3 <= max_block_len; ++b3) {
                    int16_t num_spaces = row_len - b1 - b2 - b3;
                    if (num_spaces < 2)
                        continue;
                    uint64_t sum{ 0 };
                    for (uint16_t head_len = b1 + 1; head_len <= max_block_len + 1; ++head_len) {
                        uint16_t tail_len = row_len - head_len;
                        if (b2 + b3 + 1 > tail_len)
                            continue;
                        const auto& lower = std::get<2>(g_by_length_counts.at(tail_len));
                        sum += lower.at({ b2, b3 });
                    }
                    std::get<3>(counts)[{b1, b2, b3}] = sum;
                }
            }
        }
    }

    if (max_blocks >= 4) {
        uint16_t max_block_len = row_len - 3 * (min_block_len + 1);  // Leave room for a 3x min_block_len unit block and 3 spaces
        for (uint16_t b1 = min_block_len; b1 <= max_block_len; ++b1) {
            for (uint16_t b2 = min_block_len; b2 <= max_block_len; ++b2) {
                if (row_len - b1 - b2 < 2*min_block_len + 1)
                    continue;
                for (uint16_t b3 = min_block_len; b3 <= max_block_len; ++b3) {
                    if (row_len - b1 - b2 - b3 < min_block_len)
                        continue;
                    for (uint16_t b4 = min_block_len; b4 <= max_block_len; ++b4) {
                        int16_t num_spaces = row_len - b1 - b2 - b3 - b4;
                        if (num_spaces < 3)
                            continue;
                        uint64_t sum{ 0 };
                        for (uint16_t head_len = b1 + 1; head_len <= max_block_len + 1; ++head_len) {
                            uint16_t tail_len = row_len - head_len;
                            if (b2 + b3 + b4 + 2 > tail_len)
                                continue;
                            const auto& lower = std::get<3>(g_by_length_counts.at(tail_len));
                            sum += lower.at({ b2, b3, b4 });
                        }
                        std::get<4>(counts)[{b1, b2, b3, b4}] = sum;
                    }
                }
            }
        }
    }

    if (max_blocks >= 5) {
        uint16_t max_block_len = row_len - 4 * (min_block_len + 1);  // Leave room for a 4x min_block_len unit block and 4 spaces
        for (uint16_t b1 = min_block_len; b1 <= max_block_len; ++b1) {
            for (uint16_t b2 = min_block_len; b2 <= max_block_len; ++b2) {
                if (row_len - b1 - b2 < 3*min_block_len + 2)
                    continue;
                for (uint16_t b3 = min_block_len; b3 <= max_block_len; ++b3) {
                    if (row_len - b1 - b2 - b3 < 2*min_block_len + 1)
                        continue;
                    for (uint16_t b4 = min_block_len; b4 <= max_block_len; ++b4) {
                        if (row_len - b1 - b2 - b3 < min_block_len)
                            continue;
                        for (uint16_t b5 = min_block_len; b5 <= max_block_len; ++b5) {
                            int16_t num_spaces = row_len - b1 - b2 - b3 - b4 - b5;
                            if (num_spaces < 4)
                                continue;
                            uint64_t sum{ 0 };
                            for (uint16_t head_len = b1 + 1; head_len <= max_block_len + 1; ++head_len) {
                                uint16_t tail_len = row_len - head_len;
                                if (b2 + b3 + b4 + b5 + 3 > tail_len)
                                    continue;
                                const auto& lower = std::get<4>(g_by_length_counts.at(tail_len));
                                sum += lower.at({ b2, b3, b4, b5 });
                            }
                            std::get<5>(counts)[{b1, b2, b3, b4, b5}] = sum;
                        }
                    }
                }
            }
        }
    }

    // Add all the counts to the global database.
    g_by_length_counts[row_len] = counts;
}


uint64_t solve_for(uint16_t min_block_len) {
    uint64_t total{ 0 };
    uint16_t row_len{ min_block_len };
    while (true) {
        calculate_counts_for_length(min_block_len, row_len);
        //    print_counts();

        const auto& block_counts = g_by_length_counts.at(row_len);
        total = std::get<0>(block_counts);
        for (const auto& [len, count] : std::get<1>(block_counts))
            total += count;
        for (const auto& [tup, count] : std::get<2>(block_counts))
            total += count;
        for (const auto& [tup, count] : std::get<3>(block_counts))
            total += count;
        for (const auto& [tup, count] : std::get<4>(block_counts))
            total += count;
        for (const auto& [tup, count] : std::get<5>(block_counts))
            total += count;
        for (const auto& [tup, count] : std::get<6>(block_counts))
            total += count;
        for (const auto& [tup, count] : std::get<7>(block_counts))
            total += count;
        for (const auto& [tup, count] : std::get<8>(block_counts))
            total += count;
        for (const auto& [tup, count] : std::get<9>(block_counts))
            total += count;
        for (const auto& [tup, count] : std::get<10>(block_counts))
            total += count;
        for (const auto& [tup, count] : std::get<11>(block_counts))
            total += count;
        for (const auto& [tup, count] : std::get<12>(block_counts))
            total += count;

        std::wcout << row_len << "\t" << total << std::endl;

        if (total >= 1'000'000)
            break;

        ++row_len;
    }

    return row_len;
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    calculate_counts_for_length(10, 10);
    //    calculate_counts_for_length(10, 11);
    //    calculate_counts_for_length(10, 12);
    //    calculate_counts_for_length(10, 13);
    //    calculate_counts_for_length(10, 14);
    //    calculate_counts_for_length(10, 15);
    //    calculate_counts_for_length(10, 16);
    //    calculate_counts_for_length(10, 17);
    //    calculate_counts_for_length(10, 18);
    //    calculate_counts_for_length(10, 19);
    //    calculate_counts_for_length(10, 20);
    //    calculate_counts_for_length(10, 21);
    //    calculate_counts_for_length(10, 22);
    //    print_counts();
    //}

    {
        auto row_len = solve_for(10);
        std::cout << "row_len = " << row_len << std::endl;
    }

    {
        auto row_len = solve_for(50);
        std::cout << "row_len = " << row_len << std::endl;
    }
}
