// Leonhard Euler was born on 15 April 1707.
//
// Consider the sequence 1504170715041707n mod 4503599627370517.
//
// An element of this sequence is defined to be an Eulercoin if it is strictly
// smaller than all previously found Eulercoins.
//
// For example, the first term is 1504170715041707 which is the first Eulercoin.
// The second term is 3008341430083414 which is greater than 1504170715041707
// so is not an Eulercoin.
// However, the third term is 8912517754604 which is small enough to be a new Eulercoin.
//
// The sum of the first 2 Eulercoins is therefore 1513083232796311.
//
// Find the sum of all Eulercoins.


#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <utility>


// The modulus is on the order of 2 {^52, so we can use uint64_t here and don't need BigInt.
// 1504170715041707 = 17 x 1249 x 12043 x 5882353


using Coin = uint64_t;

std::pair<uint64_t, Coin> get_next_coin(uint64_t start) {
    constexpr Coin date{ 1504170715041707 };
    constexpr Coin modulus{ 4503599627370517 };

    Coin current{ start };
    Coin index{ 0 };
    while (true) {
        current += date;
        current %= modulus;
        ++index;

        if (current < start) {
            return { index, current };
        }
    }
}


int main(int argc, char *argv[])
{
    std::cout << "Hello World!\n";

    if (2 != argc) {
        std::cerr << "Usage: helper <starting_coin_value>" << std::endl;
        return -1;
    }

    uint64_t start{ 0 };
    std::string start_string{ argv[1] };
    std::stringstream str{ start_string };
    str >> start;
    std::cout << "Starting with coin value " << start << std::endl;

    auto [delta_n, coin] = get_next_coin(start);
    std::cout << "The next coin after " << start << " is " << coin
              << ", with an increase in n of " << delta_n << std::endl;
}




//                1 = 1504170715041707  *** not part of the return value!
//                3 = 8912517754604
//              506 = 2044785486369
//             2527 = 1311409677241
//             4548 = 578033868113
//            11117 = 422691927098
//            17686 = 267349986083
//            24255 = 112008045068
//            55079 = 68674149121
//            85903 = 25340253174
//           202630 = 7346610401
//           724617 = 4046188430
//          1246604 = 745766459
//          6755007 = 428410324
//         12263410 = 111054189
//         42298633 = 15806432
//        326125654 = 15397267
//        609952675 = 14988102
//        893779696 = 14578937
//       1177606717 = 14169772
//       1461433738 = 13760607
//       1745260759 = 13351442
//       2029087780 = 12942277
//       2312914801 = 12533112
//       2596741822 = 12123947
//       2880568843 = 11714782
//       3164395864 = 11305617
//       3448222885 = 10896452
//       3732049906 = 10487287
//       4015876927 = 10078122
//       4299703948 = 9668957
//       4583530969 = 9259792
//       4867357990 = 8850627
//       5151185011 = 8441462
//       5435012032 = 8032297
//       5718839053 = 7623132
//       6002666074 = 7213967
//       6286493095 = 6804802
//       6570320116 = 6395637
//       6854147137 = 5986472
//       7137974158 = 5577307
//       7421801179 = 5168142
//       7705628200 = 4758977
//       7989455221 = 4349812
//       8273282242 = 3940647
//       8557109263 = 3531482
//       8840936284 = 3122317
//       9124763305 = 2713152
//       9408590326 = 2303987
//       9692417347 = 1894822
//       9976244368 = 1485657
//      10260071389 = 1076492
//      10543898410 = 667327
//      10827725431 = 258162
//      21939277883 = 107159
//      54990108218 = 63315
//      88040938553 = 19471
//     297173645994 = 14569
//     506306353435 = 9667
//     715439060876 = 4765
//    1640010829193 = 4628
//    2564582597510 = 4491
//    3489154365827 = 4354
//    4413726134144 = 4217
//    5338297902461 = 4080
//    6262869670778 = 3943
//    7187441439095 = 3806
//    8112013207412 = 3669
//    9036584975729 = 3532
//    9961156744046 = 3395
//   10885728512363 = 3258
//   11810300280680 = 3121
//   12734872048997 = 2984
//   13659443817314 = 2847
//   14584015585631 = 2710
//   15508587353948 = 2573
//   16433159122265 = 2436
//   17357730890582 = 2299
//   18282302658899 = 2162
//   19206874427216 = 2025
//   20131446195533 = 1888
//   21056017963850 = 1751
//   21980589732167 = 1614
//   22905161500484 = 1477

// 4503599627370517 = 0
