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


#include <algorithm>
#include <array>
#include <chrono>
#include <ctime>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

#include "big_int.h"

#ifdef _WIN32
#pragma warning(disable : 4996)
#endif


using namespace std::chrono_literals;


// The modulus is on the order of 2^52, so we can use uint64_t here and don't need BigInt.
// 4503599627370517 is prime, so the modulus forms a unique cycle of that length.
// 1504170715041707 = 17 x 1249 x 12043 x 5882353

// The modulus is a little less than 3 times the increment, so the sequence of terms
// generally increases, with the mod being invoked every 3rd or 4th element.
// To go from n=X to n=Y, we don't need to iterate over every value, we can jump:
//    F(Y) = (F(X) + increment * (Y - X)) mod modulus

// There are very few Eulercoins, because they form a monotonically decreasing sequence.
// If we know that F(N) is an Eulercoin, then the next Eulercoin is at N' such that
//    F(N') < F(N)
// Using the above expression:
//    (F(N) + increment * (N' - N)) mod modulus < F(N)
// This implies that:
//    -F(N) <= ((increment * (N' - N)) mod modulus) - modulus < 0
//    (increment * (N' - N)) mod modulus >= modulus - F(N)
// There could by multiple values of n for which this is true, N' is the smallest value greater than N.

// There are times when (F(N') - F(N)) < F(N).  When this happens, the same jump in n will
// happen over and over, resulting in the same decrease in coin value.  This will repeat until
// we reach an F(k) < F(N') - F(N).

// The expression above tells us the change in F(n) due to a given change in n, but we can
// can also calculate the difference in n needed for a given change in F(n).  This requires
// calculating the modular multiplicative inverse of the increment.
//    1504170715041707^-1 mod 4503599627370517 = 3451657199285664
// So F(n + 3451657199285664) = F(n) + 1
// If we want to decrease F(k) by 1, we take the additive inverse of 3451657199285664, which
// is just modulus - 3451657199285664 = 1051942428084853
// So F(n + 1051942428084853) = F(n) - 1
// Repeated applications of this can be used to get us any change in F(n) value:
//    F(n + d * 1051942428084853) = F(n) - d
// Of course the F(n) function cycles on the modulus, so it can be reduced to:
//    F((n + d * 1051942428084853) mod modulus) = F(n) - d

// One way to attack the problem is for a given Eulercoin C = F(N) to minimize this:
//   min((n + d * 1051942428084853) mod modulus; d < C)
// We can't do this efficiently for large values of n, but once n is small enough, we can
// search through a list of delta_n values for all the values of d < C.  This is what the
// CoinFinder class does, calculating the values of delta_n for d in [0, 1000000] in the
// delta_n_for_delta_coin_ member.


using Coin = int64_t;
using Index = size_t;

constexpr Coin kIncrement{ 1504170715041707 };
constexpr Coin kModulus{ 4503599627370517 };


std::vector<Coin> get_euler_coins() {
    std::vector<Coin> ret;

    ret.push_back(kIncrement);
    Coin current{ kIncrement };
    Index index{ 1 };
    while (true) {
        current += kIncrement;
        current %= kModulus;
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

    std::vector<Coin> ret;

    Coin current{ kIncrement };
    Index index{ 1 };
    Index last_index{ index };

    // The first coin is the kIncrement value
    {
        ret.push_back(current);
        auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::cout << index << "\t= " << current << "\t@ " << ctime(&timenow);
    }

    // The second coin is at n = 3
    {
        current += kIncrement;
        current %= kModulus;
        ++index;

        current += kIncrement;
        current %= kModulus;
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
            BigInt jump{ kIncrement };
            jump *= delta_index;
            jump %= kModulus;

            current += jump.to_int();
            current %= kModulus;
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
            current += kIncrement;
            current %= kModulus;
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
    const Coin *start;
    const Index start_index;
    const Index count;
    const Coin last_coin;
    Coin *new_coin;
    Index *found_index;
};

void thread_func(const ThreadParams &params) {
    std::cout << "\tthread_func, thread id = " << std::this_thread::get_id()
              << ", start coin = " << *params.start
              << ", start_index = " << params.start_index
              << ", count = " << params.count
              << ", last_coin = " << params.last_coin
              << std::endl;

    Coin current{ *params.start };
    Index index{ 0 };

    while (index < params.count) {
        if (index % 50'000'000 == 0) {
            if (Coin(-1) == *params.start) {
//                std::cout << "\tThread " << std::this_thread::get_id() << " got poison pill, exiting thread..." << std::endl;
                return;
            }
//            std::cout << "\tThread " << std::this_thread::get_id() << ", index = " << index << ", current = " << current << std::endl;
            std::this_thread::sleep_for(50ms);
        }
        current += kIncrement;
        current %= kModulus;
        ++index;

        if (current < params.last_coin) {
            *params.new_coin = current;
            *params.found_index = params.start_index + index;
//            std::cout << "\tThread " << std::this_thread::get_id() << " found the next coin " << current << std::endl;
            return;
        }
    }

//    std::cout << "\tThread " << std::this_thread::get_id() << " unsuccessful, exiting..." << std::endl;
    *params.new_coin = -current;
}


std::vector<Coin> get_euler_coins_threaded() {
    std::vector<Coin> ret;
    std::vector<Index> n_values;

    Coin current{ kIncrement };
    Index index{ 1 };

    // The first coin is the kIncrement value
    {
        ret.push_back(current);
        n_values.push_back(1);
        auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::cout << index << "\t= " << current << "\t@ " << ctime(&timenow);
    }

    // The second coin is at n = 3
    {
        current += kIncrement;
        current %= kModulus;
        ++index;

        current += kIncrement;
        current %= kModulus;
        ++index;

        ret.push_back(current);
        n_values.push_back(3);
        auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::cout << index << "\t= " << current << "\t@ " << ctime(&timenow);
    }

    Coin last_coin{ ret.back() };
    Index delta_index{ index - n_values.back() };

    while (true) {
//    for (int i = 0; i < 60; ++i) {
        // First try jumping another delta_index, to see if it is an Eulercoin
        {
            BigInt jump{ kIncrement };
            jump *= delta_index;
            jump %= kModulus;

            current += jump.to_int();
            current %= kModulus;
            index += delta_index;

            if (current < last_coin) {
                last_coin = current;
                ret.push_back(last_coin);
                n_values.push_back(index);

                auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                std::cout << index << "\t== " << current << "\t@ " << ctime(&timenow);
                std::cout << "\tjumped by delta_n = " << delta_index << std::endl;

                if (0 == current)
                    break;

                continue;
            }
        }

        // For small delta_n values, the cost of spinning up threads outweighs the search.
        if (delta_index < 10'000'000) {
            Index found_index;
            ThreadParams params{ .start = &current,
                                 .start_index = index,
                                 .count = kModulus,
                                 .last_coin = last_coin,
                                 .new_coin = &current,
                                 .found_index = &found_index };
            thread_func(params);

            last_coin = current;
            index = found_index;
            delta_index = index - n_values.back();
            ret.push_back(last_coin);
            n_values.push_back(index);
            auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::cout << index << "\t= " << current << "\t@ " << ctime(&timenow);

            continue;
        }


        // If we got here, the delta_n increased, use threads to split up the searching.
        std::cout << "\tusing threads" << std::endl;

        // Each thread is responsible to search delta_index values.
        constexpr size_t kNumThread{ 10 };
        std::array<std::thread, kNumThread> threads;
        std::array<Coin, kNumThread> start_values;
        std::array<Coin, kNumThread> found_coin_values;
        std::array<Index, kNumThread> found_index_values;
        size_t group{ 0 };

        // Use a BigInt to calculate delta_n * kIncrement mod kModulus
        Coin start{ current };
        BigInt jump_big{ kIncrement };
        jump_big *= delta_index;
        jump_big %= kModulus;
        auto jump = jump_big.to_int();

        // Keep creating thread groups until we find the next coin.
        while (true) {
            // Did any thread find a coin
            bool found_coin{ false };

            // Start the threads at multiples of delta_n.
            for (size_t ind = 0; ind < kNumThread; ++ind) {
                start_values[ind] = start;
                found_coin_values[ind] = 0;
                found_index_values[ind] = 0;
                ThreadParams params{ .start = &start_values[ind],
                                     .start_index = index + ind * delta_index,
                                     .count = delta_index,
                                     .last_coin = last_coin,
                                     .new_coin = &found_coin_values[ind],
                                     .found_index = &found_index_values[ind] };
                threads[ind] = std::thread(thread_func, params);
                std::this_thread::sleep_for(5ms);

                start += jump;
                start %= kModulus;
            }

            // Let the threads run for a little while before inspecting them.
            std::this_thread::sleep_for(50ms);

            // Poll threads until one finds a coin or all terminate.
            while (true) {
                // Did any thread run to completion
                bool threads_all_done = std::all_of(found_coin_values.begin(), found_coin_values.end(),
                    [](const Coin& value) { return value != 0; });
                if (!threads_all_done) {
                    std::this_thread::sleep_for(100ms);
                    continue;
                }

                // 0-based index of the thread that found a coin
                size_t thread_ind{ 0 };
                found_coin = std::any_of(found_coin_values.begin(), found_coin_values.end(),
                    [](const Coin& value) { return value > 0; });
                if (found_coin) {
                    for (size_t ind = 0; ind < kNumThread; ++ind) {
                        if ((0 == thread_ind) && (0 < found_coin_values[ind])) {
                            thread_ind = ind;
                        }
                        if (0 == found_coin_values[ind]) {
                            start_values[ind] = -1;
                        }
                    }
                    for (auto& th : threads)
                        th.join();

                    current = found_coin_values[thread_ind];
                    last_coin = current;
                    index = found_index_values[thread_ind];
                    delta_index = index - n_values.back();
                    ret.push_back(last_coin);
                    n_values.push_back(index);

                    auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                    std::cout << index << "\t= " << current << " in thread " << kNumThread * group + thread_ind << "\t@ " << ctime(&timenow);
                    std::cout << "\tindex = " << index << ", last_index = " << n_values[n_values.size()-2] << ", delta_n = " << delta_index << std::endl;
                    break;
                }
                else {
                    for (auto& th : threads)
                        th.join();

                    // Update index and current for next group of threads
                    index += kNumThread * delta_index;
                    current = -found_coin_values[kNumThread - 1];

                    break;
                }
            }

            if (0 == current)
                break;

            if (found_coin)
                break;

            ++group;
        }
    }

    return ret;
}


class CoinFinder {
public:
    CoinFinder() : delta_n_for_delta_coin_(kTailSize + 1, 0) {
        // Populate delta_n_for_delta_coin_
        delta_n_for_delta_coin_[0] = kModulus;
        auto inv = get_inverse(kIncrement, kModulus);
        auto delta_n = kModulus - inv;
        for (size_t c = 1; c <= kTailSize; ++c)
            delta_n_for_delta_coin_[c] = (delta_n_for_delta_coin_[c - 1] + delta_n) % kModulus;
    }
    ~CoinFinder() = default;
       

    int64_t get_egcd(int64_t a, int64_t b) {
        int64_t x{ 0 };
        int64_t y{ 1 };
        int64_t u{ 1 };
        int64_t v{ 0 };

        while (a != 0) {
            int64_t q = b / a;
            int64_t r = b % a;

            int64_t m = x - u * q;
            int64_t n = y - v * q;

            b = a;
            a = r;
            x = u;
            y = v;
            u = m;
            v = n;
        }

        return x;
    }


    int64_t get_inverse(int64_t value, int64_t modulus) {
        int64_t x = get_egcd(value, modulus);

        int64_t ret{ x % modulus };
        if (ret < 0)
            ret += modulus;

        return ret;
    }


    Index get_delta_n(Coin delta_coin) {
        Coin value{ delta_coin };
        if (value < 0)
            value += kModulus;
        auto inv = get_inverse(kIncrement, kModulus);

        BigInt jump{ value };
        jump *= inv;
        jump %= kModulus;

        return jump.to_int();
    }


    std::pair<Index, Coin> get_next_delta_n_naive(Coin coin) {
        auto inv = get_inverse(kIncrement, kModulus);
        BigInt inv_big{ inv };

        Index min_delta_n{ kModulus };
        Coin coin_for_min{ -1 };

        for (Coin c = 1; c < coin; ++c) {
            BigInt jump{ inv_big };
            jump *= (kModulus - c);
            jump %= kModulus;
            Index tmp = jump.to_int();
            if (tmp < min_delta_n) {
                min_delta_n = tmp;
                coin_for_min = c;
            }
        }

        return { min_delta_n, coin_for_min };
    }


    std::pair<Index, Coin> get_next_delta_n(Coin coin) {
        if (coin > kTailSize)
            return get_next_delta_n_naive(coin);

        Index min_delta_n{ kModulus };
        Coin coin_for_min{ -1 };

        for (Coin c = 1; c < coin; ++c) {
            if (delta_n_for_delta_coin_[c] < min_delta_n) {
                min_delta_n = delta_n_for_delta_coin_[c];
                coin_for_min = c;
            }
        }

        return { min_delta_n, coin_for_min };
    }


    std::vector<Coin> get_euler_coins_jumps() {
        std::vector<Coin> ret;

        Coin current{ kIncrement };
        Index index{ 1 };
        Index last_index{ index };

        // The first coin is the kIncrement value
        {
            ret.push_back(current);
            auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::cout << index << "\t= " << current << "\t@ " << ctime(&timenow);
        }

        // The second coin is at n = 3
        {
            current += kIncrement;
            current %= kModulus;
            ++index;

            current += kIncrement;
            current %= kModulus;
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
                BigInt jump{ kIncrement };
                jump *= delta_index;
                jump %= kModulus;

                current += jump.to_int();
                current %= kModulus;
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

            // Is the last Eulercoin small enough to use the tail
            if (last_coin <= kTailSize) {
                auto [delta_n, delta_coin] = get_next_delta_n(last_coin);
                last_index = index - delta_index;
                index = last_index + delta_n;
                ret.push_back(last_coin - delta_coin);
                current = last_coin = ret.back();
                delta_index = delta_n;
                auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                std::cout << index << "\t== " << current << "\t@ " << ctime(&timenow);
                std::cout << "\t" << "tail jump by delta_n = " << delta_index << std::endl;

                continue;
            }

            // If we got here, the delta_n increased, but at least we skipped a lot of test numbers
            while (true) {
                current += kIncrement;
                current %= kModulus;
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

private:
    static constexpr Coin kTailSize{ 1'000'000 };
    std::vector<Index> delta_n_for_delta_coin_;

};


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    auto coins = get_euler_coins();
    //    std::cout << "There are " << coins.size() << " coins, summing to "
    //              << std::accumulate(coins.begin(), coins.end(), Coin{ 0 }) << std::endl;
    //}

    //{
    //    auto coins = get_euler_coins_jump();
    //    std::cout << "There are " << coins.size() << " coins, summing to "
    //              << std::accumulate(coins.begin(), coins.end(), Coin{ 0 }) << std::endl;
    //}

    //{
    //    auto coins = get_euler_coins_threaded();
    //    std::cout << "There are " << coins.size() << " coins, summing to "
    //              << std::accumulate(coins.begin(), coins.end(), Coin{ 0 }) << std::endl;
    //}

    // {
    //     CoinFinder finder;

    //     std::cout << "inverse of 5 mod 13 = " << finder.get_inverse(5, 13) << std::endl;
    //     std::cout << "inverse of -137 mod " << kModulus << " = " << finder.get_inverse(-137, kModulus) << std::endl;
    //     std::cout << "inverse of " << kIncrement << " mod " << kModulus << " = " << finder.get_inverse(kIncrement, kModulus) << std::endl;

    //     std::cout << "delta_n for -137 = " << finder.get_delta_n(-137) << std::endl;

    //     std::cout << "delta_n for Coin 107 = " << finder.get_next_delta_n(107).first << " -> " << 107 - finder.get_next_delta_n(107).second << std::endl;
    //     std::cout << "delta_n for Coin 1888 = " << finder.get_next_delta_n(1888).first << " -> " << 1888 - finder.get_next_delta_n(1888).second << std::endl;
    //     std::cout << "delta_n for Coin 63315 = " << finder.get_next_delta_n(63315).first << " -> " << 63315 - finder.get_next_delta_n(63315).second << std::endl;
    //     std::cout << "delta_n for Coin 107159 = " << finder.get_next_delta_n(107159).first << " -> " << 107159 - finder.get_next_delta_n(107159).second << std::endl;
    //     std::cout << "delta_n for Coin 258162 = " << finder.get_next_delta_n(258162).first << " -> " << 258162 - finder.get_next_delta_n(258162).second << std::endl;
    //     std::cout << "delta_n for Coin 667327 = " << finder.get_next_delta_n(667327).first << " -> " << 667327 - finder.get_next_delta_n(667327).second << std::endl;
    // }

    {
        CoinFinder finder;

        auto coins = finder.get_euler_coins_jumps();
        std::cout << "There are " << coins.size() << " coins, summing to "
                    << std::accumulate(coins.begin(), coins.end(), Coin{ 0 }) << std::endl;
    }

    return 0;
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
//   65226330135625 = 77               33075450951971
//   98301781087596 = 47               33075450951971
//  131377232039567 = 17               33075450951971
//
// 4503599627370517 = 0
