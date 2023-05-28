// The most naive way of computing n^15 requires fourteen multiplications:
//      n × n × ... × n = n^15
// But using a "binary" method you can compute it in six multiplications:
//      n × n = n^2
//      n^2 × n^2 = n^4
//      n^4 × n^4 = n^8
//      n^8 × n^4 = n^12
//      n^12 × n^2 = n^14
//      n^14 × n = n^15
// However it is yet possible to compute it in only five multiplications:
//      n × n = n^2
//      n^2 × n = n^3
//      n^3 × n^3 = n^6
//      n^6 × n^6 = n^12
//      n^12 × n^3 = n^15
// We shall define m(k) to be the minimum number of multiplications
// to compute n^k; for example m(15) = 5.
// For 1 <= k <= 200, find sum m(k).


#include <cstdint>
#include <iostream>
#include <set>
#include <utility>
#include <vector>


// n^2 - 1 mult
//     n x n
// n^3 - 2 mults
//     n x n = n^2
//     n^2 x n = n^3
// n^4 - 2 mults
//     n x n = n^2
//     n^2 x n^2 = n^4 
// n^5 - 3 mults
//     n x n = n^2
//     n^2 x n = n^3
//     n^3 x n^2 = n^5
// n^6 - 3 mults
//     n x n = n^2
//     n^2 x n^2 = n^4
//     n^4 x n^2 = n^6
// n^7 - 4 mults
//     n x n = n^2
//     n^2 x n^2 = n^4
//     n^4 x n^2 = n^6
//     n^6 x n = n^7
// n^8 - 3 mults
//     n x n = n^2
//     n^2 x n^2 = n^4
//     n^4 x n^4 = n^8
// n^9 - 4 mults
//     n x n = n^2
//     n^2 x n = n^3
//     n^3 x n^3 = n^6
//     n^6 x n^3 = n^9
// n^10 - 4 mults
//     n x n = n^2
//     n^2 x n^2 = n^4
//     n^4 x n^2 = n^6
//     n^6 x n^4 = n^10
// n^11 - 5 mults
//     n x n = n^2
//     n^2 x n = n^3
//     n^3 x n^2 = n^5
//     n^3 x n^3 = n^6
//     n^6 x n^5 = n^11
// n^12 - 4 mults
//     n x n = n^2
//     n^2 x n = n^3
//     n^3 x n^3 = n^6
//     n^6 x n^6 = n^12
// n^13 - 5 mults
//     n x n = n^2
//     n^2 x n^2 = n^4
//     n^4 x n^4 = n^8
//     n^8 x n^4 = n^12
//     n^12 x n = n^13
// n^14 - 5 mults
//     n x n = n^2
//     n^2 x n = n^3
//     n^3 x n^3 = n^6
//     n^6 x n = n^7
//     n^7 x n^7 = n^14
// n^15 - 5 mults
//     n x n = n^2
//     n^2 x n = n^3
//     n^3 x n^3 = n^6
//     n^6 x n^6 = n^12
//     n^12 x n^3 = n^15


using Power = uint16_t;
using Operands = std::pair<Power, Power>;  // Two Powers of n involved in a multiplication
using Path = std::set<Operands>;  // Sequence of Operands that lead to a Power
using CommonPaths = std::set<Path>;  // All Paths that lead to same Power
using AllPaths = std::vector<CommonPaths>;

// Use n^15 as an example, some CommonPaths include:
//   [ { (1, 1), (2, 1), (3, 3), (6, 6), (12, 3) },
//     { (1, 1), (2, 2), (4, 4), (8, 4), (12, 2), (14, 1) } ]
// The last Operands in any CommonPaths could be
//   (14, 1), (13, 2), (12, 3), (11, 4), (10, 5), (9, 6), (8, 7)
// Iterate over all these Operands,
//   then iterate over outer product of the CommonPaths for those 2 Powers,
//     merge the Paths in outer product, storing each unique one in CommonPaths


int16_t get_binary_upper_bound(Power power) {
    int16_t ret{ -1 };

    size_t max_bit_ind{ 0 };
    for (size_t bit_ind = 0; bit_ind <= 8; ++bit_ind) {
        if (power & (1 << bit_ind)) {
            max_bit_ind = bit_ind;
            ++ret;
            // std::cout << bit_ind << std::endl;
        }
    }
    // std::cout << "max_bit_ind = " << max_bit_ind << std::endl;
    ret += max_bit_ind;

    return ret;
}


CommonPaths calculate_common_paths(Power power, const AllPaths &all_paths) {
    if (all_paths.size() - power > 1)
        throw "all_paths not big enough";

    if (0 == power)
        return { };  // no multiplications for n^0
    if (1 == power)
        return { };  // no multiplications for n^1
    if (2 == power)
        return { { { 1, 1 } } };  // 1 multiplication n^2 = n x n

    auto upper = get_binary_upper_bound(power);

    CommonPaths ret;
    // Special case for (power - 1, 1), since all_paths[1] is empty.
    const auto &paths_minus_one = all_paths[power - 1];
    for (const auto &path : paths_minus_one) {
        Path product{ path };
        product.insert({ power - 1, 1 });
        if (product.size() > upper)
            continue;
        ret.insert(product);
    }
    // Now process all other pairs of addends
    for (uint16_t addend1 = 2; addend1 <= power / 2; ++addend1) {
        uint16_t addend2 = power - addend1;
        std::cout << "addends = " << addend1 << " + " << addend2 << std::endl;  
        const auto &common_paths1 = all_paths[addend1];
        const auto &common_paths2 = all_paths[addend2];
        for (const auto &path1 : common_paths1) {
            for (const auto &path2 : common_paths2) {
                Path product{ path1 };
                for (const auto &ops : path2)
                    product.insert(ops);
                product.insert({ addend2, addend1 });
                if (product.size() > upper)
                    continue;
                ret.insert(product);
            }
        }
    }
    return ret;
}


uint64_t m(Power power, AllPaths &all_paths) {
    while (all_paths.size() <= power)
        all_paths.emplace_back(calculate_common_paths(all_paths.size(), all_paths));

    uint64_t ret{ 1ULL << 63 };
    for (const auto &paths : all_paths[power]) {
        ret = std::min(ret, paths.size());
    }
    return ret;
}


// uint64_t solve_for(Power max_power) {
//     AllPaths memo;

// }


int main()
{
    std::cout << "Hello World!\n";

    {
        Power power = 15;
        std::cout << power << "\t" << get_binary_upper_bound(power) << std::endl;

        for (Power power = 0; power < 255; ++power)
            std::cout << power << "\t" << get_binary_upper_bound(power) << std::endl;
    }
//return 0;
    {
        constexpr uint16_t max_val{ 200 };
        AllPaths all_paths;
        auto val = m(max_val, all_paths);
        size_t index{ 0 };
        // for (const auto &common_paths : all_paths) {
        //     std::cout << index++ << " : [";
        //     for (const auto &path : common_paths) {
        //         std::cout << " {";
        //         for (const auto &ops : path) {
        //             std::cout << " ( " << ops.first << ", " << ops.second << " ),";
        //         }
        //         std::cout << " }," << std::endl;
        //     }
        //     std::cout << "]" << std::endl;
        // }

        uint64_t sum{ 0 };
        for (Power p = 2; p <= max_val; ++p) {
            auto val = m(p, all_paths);
            sum += val;
            std::cout << "m(" << p << ") = " << val << std::endl;
        }
        std::cout << "sum = " << sum << std::endl;
    }
}


// m(2) = 1
// m(3) = 2
// m(4) = 2
// m(5) = 3
// m(6) = 3
// m(7) = 4
// m(8) = 3
// m(9) = 4
// m(10) = 4
// m(11) = 5
// m(12) = 4
// m(13) = 5
// m(14) = 5
// m(15) = 5
// m(16) = 4
// m(17) = 5
// m(18) = 5
// m(19) = 6
// m(20) = 5
// m(21) = 6
// m(22) = 6
// m(23) = 6
// m(24) = 5
// m(25) = 6
// m(26) = 6
// m(27) = 6
// m(28) = 6
// m(29) = 7
// m(30) = 6
// m(31) = 7
// m(32) = 5
// m(33) = 6
// m(34) = 6
// m(35) = 7
// m(36) = 6
// m(37) = 7
// m(38) = 7
// m(39) = 7
// m(40) = 6
// m(41) = 7
// m(42) = 7
// m(43) = 7
// m(44) = 7
// m(45) = 7
// m(46) = 7
// m(47) = 8
// m(48) = 6
// m(49) = 7
// m(50) = 7
// m(51) = 7
// m(52) = 7
// m(53) = 8
// m(54) = 7
// m(55) = 8
// m(56) = 7
// m(57) = 8
// m(58) = 8
// m(59) = 8
// m(60) = 7
// m(61) = 8
// m(62) = 8
// m(63) = 8
// m(64) = 6
// m(65) = 7
// m(66) = 7
// m(67) = 8
// m(68) = 7
// m(69) = 8
// m(70) = 8
// m(71) = 9
// m(72) = 7
// m(73) = 8
// m(74) = 8
// m(75) = 8
// m(76) = 8
// m(77) = 8
// m(78) = 8
// m(79) = 9
// m(80) = 7
// m(81) = 8
// m(82) = 8
// m(83) = 8
// m(84) = 8
// m(85) = 8
// m(86) = 8
// m(87) = 9
// m(88) = 8
// m(89) = 9
// m(90) = 8
// m(91) = 9
// m(92) = 8
// m(93) = 9
// m(94) = 9
// m(95) = 9
// m(96) = 7
// m(97) = 8
// m(98) = 8
// m(99) = 8
// m(100) = 8
// m(101) = 9
// m(102) = 8
// m(103) = 9
// m(104) = 8
// m(105) = 9
// m(106) = 9
// m(107) = 9
// m(108) = 8
// m(109) = 9
// m(110) = 9
// m(111) = 9
// m(112) = 8
// m(113) = 9
// m(114) = 9
// m(115) = 9
// m(116) = 9
// m(117) = 9
// m(118) = 9
// m(119) = 9
// m(120) = 8
// m(121) = 9
// m(122) = 9
// m(123) = 9
// m(124) = 9
// m(125) = 9
// m(126) = 9
// m(127) = 10
// m(128) = 7
// m(129) = 8
// m(130) = 8
// m(131) = 9
// m(132) = 8
// m(133) = 9
// m(134) = 9
// m(135) = 9
// m(136) = 8
// m(137) = 9
// m(138) = 9
// m(139) = 10
// m(140) = 9
// m(141) = 10
// m(142) = 10
// m(143) = 10
// m(144) = 8
// m(145) = 9
// m(146) = 9
// m(147) = 9
// m(148) = 9
// m(149) = 9
// m(150) = 9
// m(151) = 10
// m(152) = 9
// m(153) = 9
// m(154) = 9
// m(155) = 10
// m(156) = 9
// m(157) = 10
// m(158) = 10
// m(159) = 10
// m(160) = 8
// m(161) = 9
// m(162) = 9
// m(163) = 9
// m(164) = 9
// m(165) = 9
// m(166) = 9
// m(167) = 10
// m(168) = 9
// m(169) = 10
// m(170) = 9
// m(171) = 10
// m(172) = 9
// m(173) = 10
// m(174) = 10
// m(175) = 10
// m(176) = 9
// m(177) = 10
// m(178) = 10
// m(179) = 10
// m(180) = 9
// m(181) = 10
// m(182) = 10
// m(183) = 10
// m(184) = 9
// m(185) = 10
// m(186) = 10
// m(187) = 10
// m(188) = 10
// m(189) = 10
// m(190) = 10
// m(191) = 11
// m(192) = 8
// m(193) = 9
// m(194) = 9
// m(195) = 9
// m(196) = 9
// m(197) = 10
// m(198) = 9
// m(199) = 10
// m(200) = 9
// sum = 1582
