// The minimum number of cubes to cover every visible face on a cuboid measuring
// 3 x 2 x 1 is twenty-two.
//
// If we then add a second layer to this solid it would require forty-six cubes to
// cover every visible face, the third layer would require seventy-eight cubes,
// and the fourth layer would require one-hundred and eighteen cubes to cover every
// visible face.
//
// However, the first layer on a cuboid measuring 5 x 1 x 1 also requires twenty-two
// cubes; similarly the first layer on cuboids measuring 5 x 3 x 1, 7 x 2 x 1, and
// 11 x 1 x 1 all contain forty-six cubes.
//
// We shall define C(n) to represent the number of cuboids that contain cubes in one
// of its layers. So C(22) = 2, C(46) = 4, C(78) = 5, and C(118) = 6.
//
// It turns out that 154 is the least value of for which C(n) = 10.
//
// Find the least value of n for which C(n) = 1000.


#include <array>
#include <iostream>
#include <map>
#include <tuple>
#include <unordered_map>
#include <vector>


// Look at surface area of original cuboid:
//   2x 3*2 + 2x 3*1 + 2x 2*1 = 22
// This is number of cubes in first layer.
// Evaluate other listed cuboids:
//   5 x 1 x 1 -> 2 * (5*1 + 5*1 + 1*1) = 2 * (5 + 5 + 1) = 2 * 11 = 22
//   5 x 3 x 1 -> 2 * (5*3 + 5*1 + 3*1) = 2 * (15 + 5 + 3) = 2 * 23 = 46
//   7 x 2 x 1 -> 2 * (7*2 + 7*1 + 2*1) = 2 * (14 + 7 + 2) = 2 * 23 = 46
//   11 x 1 x 1 -> 2 * (11*1 + 11*1 + 1*1) = 2 * (11 + 11 + 1) = 2 * 23 = 46

// For second layer, there are Nx1x1 strips that cover the sides of 2 cubes
// from first layer.
// Enumerate the second layer around 3 x 2 x 1 from top to bottom (or vice versa):
//        3*2           = 6
//    2x 2*1 + 2x 3*1   = 4 + 6 = 10
//  2x 2*1 + 2x 3*1 + 4 = 4 * 6 * 4 = 14
//    2x 2*1 + 2x 3*1   = 4 + 6 = 10
//        3*2           = 6
//    total = 6 + 10 + 14 + 10 + 6 = 46
// Generic case:
//        l*w
//    2x l*1 + 2x w*1
//  2x l*1 + 2x w*1 + 4  - h times this sum
//    2x l*1 + 2x w*1
//        l*w
//    total = 2*l*w + 4*l + 4*w + 2*l*h + 2*w*h + 4*h
//    total = 2*(l*w + l*h + w*h) + 4*(l + w + h)
// Note the rotational symmetry, where l, w, h can be swapped and yield the same total

// For third layer, there are Nx1x1 strips that cover the sides of 2 cubes
// from second layer.
// Enumerate the third layer around 3 x 2 x 1 from top to bottom (or vice versa):
//        3*2           = 6
//    2x 2*1 + 2x 3*1   = 4 + 6 = 10
//  2x 2*1 + 2x 3*1 + 4 = 4 * 6 * 4 = 14
//  2x 2*1 + 2x 3*1 + 8 = 4 * 6 * 8 = 18
//  2x 2*1 + 2x 3*1 + 4 = 4 * 6 * 4 = 14
//    2x 2*1 + 2x 3*1   = 4 + 6 = 10
//        3*2           = 6
//    total = 6 + 10 + 14 + 18 + 14 + 10 + 6 = 78
// Generic case:
//        l*w
//    2x l*1 + 2x w*1
//  2x l*1 + 2x w*1 + 4
//  2x l*1 + 2x w*1 + 8  - h times this sum
//  2x l*1 + 2x w*1 + 4
//    2x l*1 + 2x w*1
//        l*w
//    total = 2*l*w + 8*l + 8*w + 8 + 2*l*h + 2*w*h + 8*h
//    total = 2*(l*w + l*h + w*h) + 8*(l + w + h) + 8
// Note the rotational symmetry, where l, w, h can be swapped and yield the same total

// For fourth layer:
// Generic case:
//        l*w
//    2x l*1 + 2x w*1
//  2x l*1 + 2x w*1 + 4
//  2x l*1 + 2x w*1 + 8
//  2x l*1 + 2x w*1 + 12  - h times this sum
//  2x l*1 + 2x w*1 + 8
//  2x l*1 + 2x w*1 + 4
//    2x l*1 + 2x w*1
//        l*w
//    total = 2*l*w + 12*l + 12*w + 24 + 2*l*h + 2*w*h + 12*h
//    total = 2*(l*w + l*h + w*h) + 12*(l + w + h) + 24
// Note the rotational symmetry, where l, w, h can be swapped and yield the same total

// Extrapolate to N'th layer:
//    total = 2*(l*w + l*h + w*h) + 4*(N-1)*(l + w + h) + 8*(N-2)*(N-1)/2


using Cuboid = std::tuple<uint64_t, uint64_t, uint64_t>;    //  length, width, height
using Layer = std::tuple<Cuboid, uint64_t>;                 //  {l,w,h}, layer index
using Layers = std::vector<Layer>;
using Counts = std::map<uint64_t, Layers>;                  //  key on number of blocks in a layer
using Counts2 = std::unordered_map<uint64_t, uint64_t>;     //  key on number of blocks in a layer, value is number of ways to get that count


uint64_t first_layer(Cuboid cuboid) {
    auto &[l, w, h] = cuboid;
    return 2 * (l*w + l*h + w*h);
}


uint64_t second_layer(Cuboid cuboid) {
    auto& [l, w, h] = cuboid;
    return 2 * (l*w + l*h + w*h) + 4 * (l + w + h);
}


uint64_t third_layer(Cuboid cuboid) {
    auto& [l, w, h] = cuboid;
    return 2 * (l*w + l*h + w*h) + 8 * (l + w + h) + 8;
}


uint64_t fourth_layer(Cuboid cuboid) {
    auto& [l, w, h] = cuboid;
    return 2 * (l*w + l*h + w*h) + 12 * (l + w + h) + 24;
}


uint64_t nth_layer(Cuboid cuboid, uint64_t layer) {
    auto& [l, w, h] = cuboid;
    return 2 * (l * w + l * h + w * h)
         + 4 * (layer - 1) * (l + w + h)
         + 8 * (layer - 2) * (layer - 1) / 2;

    // 2 * (1 + 1 + 1) + 4*(n-1)*(1 + 1 + 1) + 8*(n-2)*(n-1)/2
    // 6 + 12*(n-1) + 4*n^2 - 12*n + 8
    // 4*n^2 + 2 = 25000
    // n^2 = 6250
    // n = 80
    // 6 + 12*(79) + 4*78*79

    //switch (layer) {
    //case 1:  return 2 * (l * w + l * h + w * h);
    //case 2:  return 2 * (l * w + l * h + w * h) + 4 * (l + w + h);
    //default: return 2 * (l * w + l * h + w * h) + 4 * (layer - 1) * (l + w + h) + 8 * (layer - 2) * (layer - 1) / 2
    //}
}


Counts get_counts(uint64_t max_dim, uint64_t max_layer) {
    Counts ret;

    for (uint64_t l = 1; l <= max_dim; ++l) {
        for (uint64_t w = 1; w <= l; ++w) {
            for (uint64_t h = 1; h <= w; ++h) {
                Cuboid cuboid{ l, w, h };

                for (uint64_t layer = 1; layer <= max_layer; ++layer) {
                    auto count = nth_layer(cuboid, layer);
                    if (ret.find(count) == ret.end())
                        ret[count] = Layers{ };
                    ret[count].push_back({ cuboid, layer });
                }

                //auto c1 = first_layer(cuboid);
                //if (ret.find(c1) == ret.end())
                //    ret[c1] = Layers{ };
                //ret[c1].push_back({ cuboid, 1 });

                //auto c2 = second_layer(cuboid);
                //if (ret.find(c2) == ret.end())
                //    ret[c2] = Layers{ };
                //ret[c2].push_back({ cuboid, 2 });

                //auto c3 = third_layer(cuboid);
                //if (ret.find(c3) == ret.end())
                //    ret[c3] = Layers{ };
                //ret[c3].push_back({ cuboid, 3 });

                //auto c4 = fourth_layer(cuboid);
                //if (ret.find(c4) == ret.end())
                //    ret[c4] = Layers{ };
                //ret[c4].push_back({ cuboid, 4 });
            }
        }
    }

    return ret;
}


uint64_t solve_for(uint64_t min_c) {
    auto counts = get_counts(500, 30);

    for (const auto& [n, layers] : counts) {
        if (layers.size() == min_c)
            return n;
    }

    return 0;
}


uint64_t solve_by_counting(uint64_t min_c) {
    constexpr size_t kMaxCount{ 25000 };

    std::array<uint64_t, kMaxCount> counts;

    for (uint64_t l = 1; l <= kMaxCount / 2; ++l) {
        for (uint64_t w = 1; w <= l; ++w) {
            for (uint64_t h = 1; h <= w; ++h) {
                Cuboid cuboid{ l, w, h };

                for (uint64_t layer = 1; layer <= 80; ++layer) {
                    auto count = nth_layer(cuboid, layer);
                    if (count >= kMaxCount)
                        break;

                    counts[count]++;
                }
            }
        }
    }

    for (size_t n = 0; n < kMaxCount; ++n) {
        if (counts[n] == min_c)
            return n;
    }
    return 0;
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    for (uint64_t l = 1; l < 5; ++l) {
    //        for (uint64_t w = 1; w <= l; ++w) {
    //            for (uint64_t h = 1; h <= w; ++h) {
    //                std::cout << "(" << l << ", " << w << ", " << h << ") -> "
    //                          << first_layer({ l, w, h }) << ",\t"
    //                          << second_layer({ l, w, h }) << ",\t"
    //                          << third_layer({ l, w, h }) << ",\t"
    //                          << fourth_layer({ l, w, h })
    //                          << std::endl;
    //            }
    //        }
    //    }
    //}

    //{
    //    for (uint64_t l = 1; l < 5; ++l) {
    //        for (uint64_t w = 1; w <= l; ++w) {
    //            for (uint64_t h = 1; h <= w; ++h) {
    //                std::cout << "(" << l << ", " << w << ", " << h << ") -> "
    //                    << nth_layer({ l, w, h }, 1) << ",\t"
    //                    << nth_layer({ l, w, h }, 2) << ",\t"
    //                    << nth_layer({ l, w, h }, 3) << ",\t"
    //                    << nth_layer({ l, w, h }, 4)
    //                    << std::endl;
    //            }
    //        }
    //    }
    //}
//    return 0;
    //{
    //    auto counts = get_counts(38, 5);
    //    //for (const auto& [count, layers] : counts) {
    //    //    std::cout << count << " :\t";
    //    //    for (const auto& layer : layers) {
    //    //        auto &[cuboid, index] = layer;
    //    //        auto& [l, w, h] = cuboid;
    //    //        std::cout << "(" << l << ", " << w << ", " << h << ") @ " << index << ",\t";
    //    //    }
    //    //    std::cout << std::endl;
    //    //}
    //    std::cout << "C(22) = " << counts[22].size() << std::endl;
    //    std::cout << "C(46) = " << counts[46].size() << std::endl;
    //    std::cout << "C(78) = " << counts[78].size() << std::endl;
    //    std::cout << "C(118) = " << counts[118].size() << std::endl;
    //    std::cout << "C(154) = " << counts[154].size() << std::endl;
    //}

//    std::cout << solve_for(10) << std::endl;
//    std::cout << solve_for(1000) << std::endl;

//    std::cout << solve_by_counting(10) << std::endl;
    std::cout << solve_by_counting(1000) << std::endl;
}
