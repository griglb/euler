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


#include <chrono>
#include <ctime>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

#include "big_int.h"


using namespace std::chrono_literals;


// The modulus is on the order of 2^52, so we can use uint64_t here and don't need BigInt.
// 4503599627370517 is prime, so the modulus forms a unique cycle of that length.
// 1504170715041707 = 17 x 1249 x 12043 x 5882353


using Coin = int64_t;
using Index = size_t;

std::vector<Coin> get_euler_coins() {
    constexpr Coin date{ 1504170715041707 };
    constexpr Coin modulus{ 4503599627370517 };

    std::vector<Coin> ret;

    ret.push_back(date);
    Coin current{ date };
    Index index{ 1 };
    while (true) {
        current += date;
        current %= modulus;
        ++index;

        if (current < ret.back()) {
            ret.push_back(current);
            auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::cout << index << "\t= " << current << "\t@ " << ctime(&timenow) << std::endl;
        }

        if (0 == current)
            break;
    }

    return ret;
}


std::vector<Coin> get_euler_coins_jumps() {
    constexpr Coin increment{ 1504170715041707 };
    constexpr Coin modulus{ 4503599627370517 };

    std::vector<Coin> ret;

    Coin current{ increment };
    Index index{ 1 };
    Index last_index{ index };

    // The first coin is the increment value
    {
        ret.push_back(current);
        auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::cout << index << "\t= " << current << "\t@ " << ctime(&timenow);
    }

    // The second coin is at n = 3
    {
        current += increment;
        current %= modulus;
        ++index;

        current += increment;
        current %= modulus;
        ++index;

        ret.push_back(current);
        auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::cout << index << "\t= " << current << "\t@ " << ctime(&timenow);
    }

    Coin last_coin{ ret.back() };
    Index delta_index{ index - last_index };
    last_index = index;

    while (true) {
        // First try jumping another delta_index, to see if it is an Eulercoin
        {
            BigInt jump{ increment };
            jump *= delta_index;
            jump %= modulus;

            current += jump.to_int();
            current %= modulus;
            index += delta_index;

            if (current < last_coin) {
                last_coin = current;
                ret.push_back(last_coin);
                auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                std::cout << index << "\t== " << current << "\t@ " << ctime(&timenow);
                std::cout << "\t" << "jumped by delta_n = " << delta_index << std::endl;
                last_index = index;

                if (0 == current)
                    break;

                continue;
            }
        }

        // If we got here, the delta_n increased, but at least we skipped a lot of test numbers
        while (true) {
            current += increment;
            current %= modulus;
            ++index;

            if (current < last_coin) {
                last_coin = current;
                ret.push_back(last_coin);
                auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                std::cout << index << "\t= " << current << "\t@ " << ctime(&timenow);
                delta_index = index - last_index;
                last_index = index;
                break;
            }

            if (0 == current)
                break;
        }

        if (0 == current)
            break;
    }

    return ret;
}


struct ThreadParams {
    const Coin increment;
    const Coin modulus;
    const Coin *start;
    const Index count;
    const Coin last_coin;
    Coin *new_coin;
    Index *delta_n;
};

void thread_func(const ThreadParams &params) {
    std::cout << "\tthread_func, thread id = " << std::this_thread::get_id()
              << ", start = " << *params.start
              << ", count = " << params.count
              << ", last_coin = " << params.last_coin
              << std::endl;

    Coin current{ *params.start };
    Index index{ 0 };

    while (index < params.count) {
        if (index % 10'000 == 0) {
            if (Coin(-1) == *params.start) {
                std::cout << "Thread " << std::this_thread::get_id() << " got poison pill, exiting thread..." << std::endl;
                return;
            }
            std::cout << "Thread " << std::this_thread::get_id() << ", index = " << index << ", current = " << current << std::endl;
            std::this_thread::sleep_for(50ms);
        }
        current += params.increment;
        current %= params.modulus;
        ++index;

        if (current < params.last_coin) {
            *params.new_coin = current;
            *params.delta_n = index;
            std::cout << "Thread " << std::this_thread::get_id() << " found the next coin, returning..." << std::endl;
            return;
        }
    }

    *params.new_coin = -1;
}


std::vector<Coin> get_euler_coins_threaded() {
    constexpr Coin increment{ 1504170715041707 };
    constexpr Coin modulus{ 4503599627370517 };

    std::vector<Coin> ret;

    Coin current{ increment };
    Index index{ 1 };
    Index last_index{ index };

    // The first coin is the increment value
    {
        ret.push_back(current);
        auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::cout << index << "\t= " << current << "\t@ " << ctime(&timenow);
    }

    // The second coin is at n = 3
    {
        current += increment;
        current %= modulus;
        ++index;

        current += increment;
        current %= modulus;
        ++index;

        ret.push_back(current);
        auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::cout << index << "\t= " << current << "\t@ " << ctime(&timenow);
    }

    Coin last_coin{ ret.back() };
    Index delta_index{ index - last_index };
    last_index = index;

//    while (true) {
    for (int i = 0; i < 30; ++i) {
        // First try jumping another delta_index, to see if it is an Eulercoin
        {
            BigInt jump{ increment };
            jump *= delta_index;
            jump %= modulus;

            current += jump.to_int();
            current %= modulus;
            index += delta_index;

            if (current < last_coin) {
                last_coin = current;
                ret.push_back(last_coin);
                auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                std::cout << index << "\t== " << current << "\t@ " << ctime(&timenow);
                std::cout << "\t" << "jumped by delta_n = " << delta_index << std::endl;
                last_index = index;

                if (0 == current)
                    break;

                continue;
            }
        }

        if (delta_index < 10000) {
            Index delta_n;
            ThreadParams params{ .increment = increment,
                                 .modulus = modulus,
                                 .start = &current,
                                 .count = modulus,
                                 .last_coin = last_coin,
                                 .new_coin = &current,
                                 .delta_n = &delta_n};
            thread_func(params);

            last_coin = current;
            ret.push_back(last_coin);
            last_index = index;
            delta_index += delta_n;
            index += delta_n;
            auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::cout << index << "\t= " << current << "\t@ " << ctime(&timenow);

            continue;
        }

        // If we got here, the delta_n increased, use threads to split up the searching.
        // Each thread is responsible to search delta_index values.
        constexpr size_t kNumThread{ 10 };
        std::array<std::thread, kNumThread> threads;
        std::array<Coin, kNumThread> start_values;
        std::array<Coin, kNumThread> found_values;
        std::array<Index, kNumThread> delta_n_values;

        std::cout << "using threads" << std::endl;


        Coin start{ current };
        BigInt jump{ increment };
        jump *= delta_index;
        jump %= modulus;

        for (size_t ind = 0 ; ind < kNumThread; ++ind) {
            start += jump.to_int();
            start %= modulus;
            start_values[ind] = start;
            found_values[ind] = 0;
            ThreadParams params{ .increment = increment,
                                 .modulus = modulus,
                                 .start = &start_values[ind],
                                 .count = delta_index,
                                 .last_coin = last_coin,
                                 .new_coin = &found_values[ind],
                                 .delta_n = &delta_n_values[ind]};
            threads[ind] = std::thread(thread_func, params);
            std::this_thread::sleep_for(5ms);
        }

        std::this_thread::sleep_for(50ms);

        while (true) {
            bool found_coin{ false };
            bool thread_done{ false };
            size_t thread_ind{ 0 };
            for (const auto &value : found_values) {
                if (value > 0) {
                    found_coin = true;
                    break;
                }
                if (value < 0) {
                    thread_done = true;
                }
                ++thread_ind;
            }
            if (found_coin) {
                for (size_t ind = 0 ; ind < kNumThread; ++ind) {
                    if (0 == found_values[ind]) {
                        start_values[ind] = -1;
                    }
                }
                for (auto& th : threads)
                    th.join();

                current = found_values[thread_ind];
                last_coin = current;
                ret.push_back(last_coin);
                delta_index = delta_n_values[thread_ind];
                last_index = index;
                index += delta_index;

                auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                std::cout << index << "\t= " << current << " in thread " << thread_ind << "\t@ " << ctime(&timenow);
                break;
            }
            else if (thread_done) {
                for (auto& th : threads)
                    th.join();
                continue;
            }

            std::this_thread::sleep_for(500ms);
        }

        if (0 == current)
            break;
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    {
        auto coins = get_euler_coins_threaded();
        std::cout << "There are " << coins.size() << " coins, summing to "
                  << std::accumulate(coins.begin(), coins.end(), Coin{ 0 }) << std::endl;
    }
}



//          n               coin              delta_n
//                1 = 1504170715041707
//                3 = 8912517754604                 2
//              506 = 2044785486369               503
//             2527 = 1311409677241              2021
//             4548 = 578033868113               2021
//            11117 = 422691927098               6569
//            17686 = 267349986083               6569
//            24255 = 112008045068               6569
//            55079 = 68674149121               30824
//            85903 = 25340253174               30824
//           202630 = 7346610401               116727
//           724617 = 4046188430               521987
//          1246604 = 745766459                521987
//          6755007 = 428410324               5508403
//         12263410 = 111054189               5508403
//         42298633 = 15806432               30035223
//        326125654 = 15397267              283827021
//        609952675 = 14988102              283827021
//        893779696 = 14578937              283827021
//       1177606717 = 14169772              283827021
//       1461433738 = 13760607              283827021
//       1745260759 = 13351442              283827021
//       2029087780 = 12942277              283827021
//       2312914801 = 12533112              283827021
//       2596741822 = 12123947              283827021
//       2880568843 = 11714782              283827021
//       3164395864 = 11305617              283827021
//       3448222885 = 10896452              283827021
//       3732049906 = 10487287              283827021
//       4015876927 = 10078122              283827021
//       4299703948 = 9668957               283827021
//       4583530969 = 9259792               283827021
//       4867357990 = 8850627               283827021
//       5151185011 = 8441462               283827021
//       5435012032 = 8032297               283827021
//       5718839053 = 7623132               283827021
//       6002666074 = 7213967               283827021
//       6286493095 = 6804802               283827021
//       6570320116 = 6395637               283827021
//       6854147137 = 5986472               283827021
//       7137974158 = 5577307               283827021
//       7421801179 = 5168142               283827021
//       7705628200 = 4758977               283827021
//       7989455221 = 4349812               283827021
//       8273282242 = 3940647               283827021
//       8557109263 = 3531482               283827021
//       8840936284 = 3122317               283827021
//       9124763305 = 2713152               283827021
//       9408590326 = 2303987               283827021
//       9692417347 = 1894822               283827021
//       9976244368 = 1485657               283827021
//      10260071389 = 1076492               283827021
//      10543898410 = 667327                283827021
//      10827725431 = 258162                283827021
//      21939277883 = 107159              11111552452
//      54990108218 = 63315               33050830335
//      88040938553 = 19471               33050830335
//     297173645994 = 14569              209132707441
//     506306353435 = 9667               209132707441
//     715439060876 = 4765               209132707441
//    1640010829193 = 4628               924571768317
//    2564582597510 = 4491               924571768317
//    3489154365827 = 4354               924571768317
//    4413726134144 = 4217               924571768317
//    5338297902461 = 4080               924571768317
//    6262869670778 = 3943               924571768317
//    7187441439095 = 3806               924571768317
//    8112013207412 = 3669               924571768317
//    9036584975729 = 3532               924571768317
//    9961156744046 = 3395               924571768317
//   10885728512363 = 3258               924571768317
//   11810300280680 = 3121               924571768317
//   12734872048997 = 2984               924571768317
//   13659443817314 = 2847               924571768317
//   14584015585631 = 2710               924571768317
//   15508587353948 = 2573               924571768317
//   16433159122265 = 2436               924571768317
//   17357730890582 = 2299               924571768317
//   18282302658899 = 2162               924571768317
//   19206874427216 = 2025               924571768317
//   20131446195533 = 1888               924571768317
//   21056017963850 = 1751               924571768317
//   21980589732167 = 1614               924571768317
//   22905161500484 = 1477               924571768317
//   23829733268801 = 1340               924571768317
//   24754305037118 = 1203               924571768317
//   25678876805435 = 1066               924571768317
//   26603448573752 = 929                924571768317
//   27528020342069 = 792                924571768317
//   28452592110386 = 655                924571768317
//   29377163878703 = 518                924571768317
//   30301735647020 = 381                924571768317
//   31226307415337 = 244                924571768317
//   32150879183654 = 107                924571768317
// 4503599627370517 = 0
